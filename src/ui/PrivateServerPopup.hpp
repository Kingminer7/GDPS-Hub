#include "../utils/GDPSHub.hpp"
#include "../utils/Structs.hpp"
#include "Geode/binding/ButtonSprite.hpp"
#include "Geode/binding/FLAlertLayer.hpp"
#include "PSCreatorLayer.hpp"
#include <Geode/binding/LoadingCircle.hpp>
#include <Geode/loader/Event.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/utils/web.hpp>
#include <matjson.hpp>

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
    setTitle(server.title);
    m_title->setPosition(245 - (m_title->getScaledContentWidth() / 2), 248);
    m_title->setAnchorPoint({0, 0.5});
    m_title->setScale(0.9);
    m_title->setID("server-name");

    auto desc = MDTextArea::create(server.description, ccp(420, 168));
    desc->setPosition(220, 131);
    desc->setID("server-description");
    m_mainLayer->addChild(desc);

    m_loadingIndicator = LoadingCircle::create();
    m_loadingIndicator->setParentLayer(m_mainLayer);
    m_loadingIndicator->setPosition(
        {220 - (m_title->getScaledContentWidth() / 2), 245});
    m_loadingIndicator->ignoreAnchorPointForPosition(false);
    m_loadingIndicator->setScale(0.75f);
    m_loadingIndicator->show();
    m_loadingIndicator->setID("loading-circle");

    auto menu = CCMenu::create();
    menu->setPosition(0, 0);
    menu->setID("interaction-menu");

    auto discordBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("gj_discordIcon_001.png"), this,
        menu_selector(PrivateServerPopup::onDiscord));
    discordBtn->setPosition(420, 22);
    discordBtn->setID("discord-button");
    menu->addChild(discordBtn);

    auto webBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("gdpsHubBtn.png"_spr), this,
        menu_selector(PrivateServerPopup::onWeb));
    webBtn->setPosition(387, 22);
    webBtn->setID("web-button");
    menu->addChild(webBtn);

    auto viewBtn = CCMenuItemSpriteExtra::create(
        ButtonSprite::create("Play"), this,
        menu_selector(PrivateServerPopup::viewServer));
    viewBtn->setPosition({175, 26});
    viewBtn->setID("view-button");
    if (server.url == "No URL provided.") {
      viewBtn->setEnabled(false);
      viewBtn->setColor({100, 100, 100});
      viewBtn->setOpacity(100);
    }
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

    m_mainLayer->addChild(menu);

    retain();

    startDownload();

    return true;
  }

  void viewServer(CCObject *) {
    CCDirector::sharedDirector()->replaceScene(
        CCTransitionFade::create(0.5, PSCreatorLayer::scene()));
    GDPSHub::get()->beginPreview(server);
  }

  void saveServer(CCObject *) {
    if (!Loader::get()->isModLoaded("km7dev.gdps-switcher")) {
      FLAlertLayer::create(
          "Error",
          "GDPS Switcher needs to be installed and enabled to save servers.",
          "OK")
          ->show();
      return;
    }
    auto gdpsSwitcher = Loader::get()->getLoadedMod("km7dev.gdps-switcher");

    auto servers =
        gdpsSwitcher->getSavedValue<std::vector<ServerEntry>>("saved-servers");
    for (auto &s : servers) {
      if (s.url == server.url) {
        FLAlertLayer::create(
            "Error", fmt::format("This server is already saved ({}).", s.name),
            "OK")
            ->show();
        return;
      }
    }
    servers.push_back({server.title, server.url});
    gdpsSwitcher->setSavedValue("saved-servers", servers);
    FLAlertLayer::create("Success", "Server saved to GDPS Switcher.", "OK")
        ->show();
  }

  void onDiscord(CCObject *sender) {
    if (server.dcUrl == "") {
      return;
    }
    web::openLinkInBrowser(server.dcUrl);
  }

  void onWeb(CCObject *sender) {
    web::openLinkInBrowser(
        fmt::format("https://gdpshub.com/gdps/{}", server.id));
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
    log::info("{}", URL);

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
    m_mainLayer->addChild(label);
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
    m_mainLayer->addChild(image);
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
