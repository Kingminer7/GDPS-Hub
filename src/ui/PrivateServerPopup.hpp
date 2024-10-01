#include "../utils/Structs.hpp"
#include <Geode/binding/LoadingCircle.hpp>
#include <Geode/loader/Event.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/utils/web.hpp>

using namespace geode::prelude;

class PrivateServerPopup : public Popup<Server> {
protected:
  Ref<LoadingCircle> m_loadingIndicator;
  EventListener<web::WebTask> m_downloadListener;
  Ref<CCImage> m_image;
  Server server;
  std::mutex m;

  bool setup(Server server) override {
    this->server = server;
    this->setTitle(server.title);
    this->m_title->setPosition(60, 237);
    this->m_title->setAnchorPoint({0, 0.5});
    this->m_title->setScale(0.9);

    auto desc = MDTextArea::create(server.description, ccp(230, 200));
    desc->setPosition(120, 107);
    this->m_mainLayer->addChild(desc);

    m_loadingIndicator = LoadingCircle::create();
    m_loadingIndicator->setParentLayer(this->m_mainLayer);
    m_loadingIndicator->setPosition({30, 234});
    m_loadingIndicator->ignoreAnchorPointForPosition(false);
    m_loadingIndicator->setScale(0.75f);
    m_loadingIndicator->show();

    auto discordBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("gj_discordIcon_001.png"),
        this, menu_selector(PrivateServerPopup::onDiscord));
    discordBtn->setPosition(200, 30);
    auto menu = CCMenu::create(discordBtn, nullptr);
    menu->setPosition(0, 0);
    this->m_mainLayer->addChild(menu);

    retain();

    startDownload();

    return true;
  }

  void startDownload() {
    std::string URL = server.pfp;

    auto req = web::WebRequest();
    m_downloadListener.bind([this](web::WebTask::Event *e) {
      if (auto res = e->getValue()) {
        if (!res->ok()) {
          onDownloadFailed();
        } else {
          auto data = res->data();

          std::thread imageThread = std::thread([data, this]() {
            m.lock();
            m_image = new CCImage();
            m_image->initWithImageData(const_cast<uint8_t *>(data.data()),
                                       data.size());
            geode::Loader::get()->queueInMainThread([data, this]() {
              m_image->release();
              imageCreationFinished(m_image);
            });
            m.unlock();
          });
          imageThread.detach();
        }
      } else if (web::WebProgress *progress = e->getProgress()) {
        if (!progress->downloadProgress().has_value()) {
          return;
        }
      } else if (e->isCancelled()) {
        geode::log::warn("Exited before finishing");
      }
    });
    auto downloadTask = req.get(URL);
    m_downloadListener.setFilter(downloadTask);
  }

  void imageCreationFinished(CCImage *image) {

    CCTexture2D *texture = new CCTexture2D();
    texture->initWithImage(image);
    onDownloadFinished(CCSprite::createWithTexture(texture));
    texture->release();
  }

  void onDownloadFailed() {
    auto label = CCLabelBMFont::create("N/A", "bigFont.fnt");
    label->setPosition({30, 234});
    label->setScale(0.82f);
    label->setAnchorPoint({0.5, 0.5});    
    label->setOpacity(150);
    this->m_mainLayer->addChild(label);
    handleFinish();
  }

  void handleFinish() {
    m_loadingIndicator->fadeAndRemove();
    release();
  }

  void onDownloadFinished(CCSprite *image) {

    float imgScale = 50 / image->getContentSize().height;
    image->setScale(imgScale);
    image->setPosition({30, 234});
    this->m_mainLayer->addChild(image);
    handleFinish();
  }

  void onDiscord(CCObject *sender) {
    if (server.dcUrl == "") {
      return;
    }
    CCApplication::sharedApplication()->openURL(server.dcUrl.c_str());
  }

public:
  static PrivateServerPopup *create(Server server) {
    auto ret = new PrivateServerPopup();
    if (ret->initAnchored(440.f, 280.f, server)) {
      ret->autorelease();
      return ret;
    }

    delete ret;
    return nullptr;
  }
};
