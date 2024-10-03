#include "../utils/GDPSHub.hpp"
#include "../utils/Structs.hpp"
#include "Geode/binding/ButtonSprite.hpp"
#include "PSCreatorLayer.hpp"
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
    this->m_title->setPosition(245 - (m_title->getScaledContentWidth() / 2),
                               248);
    this->m_title->setAnchorPoint({0, 0.5});
    this->m_title->setScale(0.9);
    this->m_title->setID("server-name");

    auto desc = MDTextArea::create(server.description, ccp(420, 168));
    desc->setPosition(220, 131);
    desc->setID("server-description");
    for (auto c : server.description) {
      // 1234567890-=qwertyuiop[]\asdfghjkl;'/z.x,cmvnb!@#$%^&*()_+|}{POIUYTREWQASDFGHJKL:"?><MNBVCXZ
      //  TODO: Use the suggested popup layout
    }
    this->m_mainLayer->addChild(desc);

    m_loadingIndicator = LoadingCircle::create();
    m_loadingIndicator->setParentLayer(this->m_mainLayer);
    m_loadingIndicator->setPosition(
        {220 - (m_title->getScaledContentWidth() / 2), 245});
    m_loadingIndicator->ignoreAnchorPointForPosition(false);
    m_loadingIndicator->setScale(0.75f);
    m_loadingIndicator->show();
    m_loadingIndicator->setID("loading-circle");

    // auto idLab = CCLabelBMFont::create(fmt::format("{}", server.id).c_str(),
    // "chatFont.fnt"); idLab->setAnchorPoint({ 0, 0.5 }); idLab->setPosition({
    // m_title->getContentWidth() * m_title->getScaleX() +
    // m_title->getPositionX(), 235 }); idLab->setID("id-label");
    // idLab->setScale(0.6f);
    // idLab->setOpacity(155);
    // this->m_mainLayer->addChild(idLab);

    // auto viewCtr = CCLabelBMFont::create(fmt::format("{} views",
    // server.views).c_str(), "chatFont.fnt"); viewCtr->setAnchorPoint({ 0, 0.5
    // }); viewCtr->setPosition({ 0, 0 }); viewCtr->setScale(0.8f);
    // viewCtr->setOpacity(155);
    // viewCtr->setID("view-label");
    // viewCtr->setAnchorPoint({ 1, 0.5 });
    // viewCtr->setPosition({ 434, 253 });
    // this->m_mainLayer->addChild(viewCtr);

    // auto creationLab = CCLabelBMFont::create(fmt::format("Created {}",
    // GDPSHub::stampToDateTime(server.created_at)).c_str(), "chatFont.fnt");
    // creationLab->setAnchorPoint({ 1, 0.5 });
    // creationLab->setPosition({ 434, 267 });
    // creationLab->setScale(0.8f);
    // creationLab->setOpacity(155);
    // creationLab->setID("timestamp-label");
    // this->m_mainLayer->addChild(creationLab);

    auto menu = CCMenu::create();
    menu->setPosition(0, 0);
    menu->setID("interaction-menu");

    auto discordBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("gj_discordIcon_001.png"), this,
        menu_selector(PrivateServerPopup::onDiscord));
    discordBtn->setPosition(420, 22);
    // discordBtn->m_baseScale = 0.7;
    // discordBtn->setScale(0.7);
    discordBtn->setID("discord-button");
    menu->addChild(discordBtn);

    auto viewBtn = CCMenuItemSpriteExtra::create(
        ButtonSprite::create("Play"), this,
        menu_selector(PrivateServerPopup::viewServer));
    viewBtn->setPosition({175, 26});
    viewBtn->setID("view-button");
    menu->addChild(viewBtn);

    auto saveBtn = CCMenuItemSpriteExtra::create(
        ButtonSprite::create("Save"), this,
        menu_selector(PrivateServerPopup::saveServer));
    saveBtn->setPosition({265, 26});
    saveBtn->setID("save-button");
    menu->addChild(saveBtn);

    auto trMenu = CCMenu::create();
    trMenu->setPosition(0, 0);
    trMenu->setID("interaction-menu");

    auto infoBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png"), this,
        menu_selector(PrivateServerPopup::onInfo));
    infoBtn->setPosition(420, 260);
    infoBtn->setID("info-button");
    menu->addChild(infoBtn);

    this->m_mainLayer->addChild(menu);

    retain();

    startDownload();

    return true;
  }

  void viewServer(CCObject *) {
    CCDirector::sharedDirector()->replaceScene(
        CCTransitionFade::create(0.5, PSCreatorLayer::scene()));
    GDPSHub::get()->beginPreview(this->server);
  }

  void saveServer(CCObject *) {
    FLAlertLayer::create("Error", "Not implemented.", "Close")->show();
  }

  void onDiscord(CCObject *sender) {
    if (server.dcUrl == "") {
      return;
    }
    CCApplication::sharedApplication()->openURL(server.dcUrl.c_str());
  }

  void onInfo(CCObject *) {
    FLAlertLayer::create(
        "Server Info",
        fmt::format("<cl>ID: {}</c>\n<cg>Views: {}</c>\n<cy>Created: {}</c>",
                    server.id, server.views,
                    GDPSHub::stampToDateTime(server.created_at)),
        "Close")
        ->show();
  }

  // Icon stuff

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
    label->setPosition({220 - (m_title->getScaledContentWidth() / 2), 245});
    label->setScale(0.82f);
    label->setAnchorPoint({0.5, 0.5});
    label->setOpacity(150);
    label->setID("no-icon");
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
    image->setPosition({220 - (m_title->getScaledContentWidth() / 2), 245});
    image->setID("ps-logo");
    this->m_mainLayer->addChild(image);
    handleFinish();
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
