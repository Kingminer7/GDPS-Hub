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
    m_title->setPosition(80, 254);
    m_title->setAnchorPoint({0, 0.5});
    m_title->limitLabelWidth(173, 0.9, 0.1);
    m_title->setID("server-name");
    setColor({14, 55, 190});

    auto author = CCLabelBMFont::create(
        fmt::format("by {}", server.owner).c_str(), "bigFont.fnt");
    author->setPosition(80, 235);
    author->limitLabelWidth(173, 0.5, 0.05);
    author->setAnchorPoint({0, 0.5});
    author->setID("server-owner");
    m_mainLayer->addChild(author);

    auto desc = MDTextArea::create(server.description, ccp(420, 168));
    desc->setPosition(220, 131);
    desc->setID("server-description");
    m_mainLayer->addChild(desc);

    m_loadingIndicator = LoadingCircle::create();
    m_loadingIndicator->setParentLayer(m_mainLayer);
    m_loadingIndicator->setPosition({47, 245});
    m_loadingIndicator->ignoreAnchorPointForPosition(false);
    m_loadingIndicator->setScale(0.75f);
    m_loadingIndicator->show();
    m_loadingIndicator->setID("loading-circle");

    auto menu = CCMenu::create();
    menu->setPosition(0, 0);
    menu->setID("interaction-menu");

    auto infoBox = CCScale9Sprite::create("square02_small.png", {0, 0, 40, 40});
    infoBox->setPosition(ccp(333, 245));
    infoBox->setContentSize(ccp(144, 51.2));
    infoBox->setColor({0, 0, 0});
    infoBox->setOpacity(75);
    infoBox->setID("server-info");
    m_mainLayer->addChild(infoBox);

    auto likeicon = CCSprite::createWithSpriteFrameName("GJ_likesIcon_001.png");
    likeicon->setScale(0.7f);
    likeicon->setPosition({13, 38});
    likeicon->setID("like-icon");
    infoBox->addChild(likeicon);

    auto likesn = CCLabelBMFont::create(fmt::format("{}", server.likes).c_str(), "bigFont.fnt");
    likesn->setAnchorPoint({0, 0.5f});
    likesn->setPosition({likeicon->getPositionX() + 12, 38});
    likesn->setID("likes-label");
    likesn->limitLabelWidth(87, .5, .1);
    infoBox->addChild(likesn);

    auto dislikeicon =
        CCSprite::createWithSpriteFrameName("GJ_dislikesIcon_001.png");
    dislikeicon->setScale(0.7f);
    dislikeicon->setPosition(
        {81, 36});
        dislikeicon->setContentSize(likeicon->getContentSize());
    dislikeicon->setID("dislike-icon");
    infoBox->addChild(dislikeicon);

    auto dislikesn = CCLabelBMFont::create(
        fmt::format("{}", server.dislikes).c_str(), "bigFont.fnt");
    dislikesn->limitLabelWidth(87, .5, .1); 
    dislikesn->setAnchorPoint({0, 0.5f});
    dislikesn->setPosition({dislikeicon->getPositionX() + 12, 38});
    dislikesn->setID("dislikes-label");
    infoBox->addChild(dislikesn);

    auto downloadicon =
        CCSprite::createWithSpriteFrameName("GJ_downloadsIcon_001.png");
    downloadicon->setScale(0.7f);
    downloadicon->setPosition({13, 14});
    downloadicon->setID("download-icon");
    infoBox->addChild(downloadicon);

    auto downloadsn = CCLabelBMFont::create(
        fmt::format("{}", server.views).c_str(), "bigFont.fnt");
    downloadsn->limitLabelWidth(87, .5, .1);
    downloadsn->setAnchorPoint({0, 0.5f});
    downloadsn->setPosition({downloadicon->getPositionX() + 12, downloadicon->getPositionY()});
    downloadsn->setID("downloads-label");
    infoBox->addChild(downloadsn);

    auto ratingicon =
        CCSprite::createWithSpriteFrameName("GJ_starsIcon_001.png");
    ratingicon->setScale(0.7f);
    ratingicon->setPosition({81,  14});
    ratingicon->setID("rating-icon");
    infoBox->addChild(ratingicon);

    auto ratingn = CCLabelBMFont::create(
        fmt::format("{}/5", server.rating).c_str(), "bigFont.fnt");
    ratingn->limitLabelWidth(87, .5, .1);
    ratingn->setAnchorPoint({0, 0.5f});
    ratingn->setPosition({ratingicon->getPositionX() + 12, ratingicon->getPositionY()});
    ratingn->setID("rating-label");
    infoBox->addChild(ratingn);

    auto discordBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("gj_discordIcon_001.png"), this,
        menu_selector(PrivateServerPopup::onDiscord));
    discordBtn->setPosition(420, 22);
    discordBtn->setID("discord-button");
    menu->addChild(discordBtn);

    auto webBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("gdpsHubBtn.png"_spr), this,
        menu_selector(PrivateServerPopup::onWeb));
    webBtn->setPosition(385, 22);
    webBtn->setID("web-button");
    menu->addChild(webBtn);

    auto toolBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("gdpsToolsBtn.png"_spr), this,
        menu_selector(PrivateServerPopup::onTools));
    toolBtn->setPosition(350, 22);
    toolBtn->setID("tools-button");
    menu->addChild(toolBtn);

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

    auto infoBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png"), this,
        menu_selector(PrivateServerPopup::onInfo));
    infoBtn->setPosition(420, 260);
    infoBtn->setID("info-button");
    menu->addChild(infoBtn);
    m_mainLayer->addChild(menu);
    m_mainLayer->setID("main-layer");

    retain();

    startDownload();

    return true;
  }

  void viewServer(CCObject *) {
    CCDirector::sharedDirector()->pushScene(
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
    // temp'ly remove this
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

  void onTools(CCObject *sender) {
    if (server.toolsUrl == "") {
      return;
    }
    web::openLinkInBrowser(server.toolsUrl);
  }

  void onWeb(CCObject *sender) {
    web::openLinkInBrowser(
        fmt::format("https://gdpshub.com/gdps/{}", server.id));
  }

  void onInfo(CCObject *) {
    log::info("{}", server.created_at);
    FLAlertLayer::create(
        "Server Info",
        fmt::format("<cl>ID: {}</c>\n<cy>Created: {}</c>\n<cg>Recommended Version: {}</c>", server.id,
                    GDPSHub::stampToDateTime(server.created_at), server.version),
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

          // std::thread imageThread = std::thread([data, this]() {
            m.lock();
            m_image = new CCImage();
            m_image->initWithImageData(const_cast<uint8_t *>(data.data()),
                                       data.size());
            geode::Loader::get()->queueInMainThread([data, this]() {
              m_image->release();
              imageCreationFinished(m_image);
            });
            m.unlock();
          // });
          // imageThread.detach();
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
    label->setPosition({47, 245});
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
    image->setPosition({47, 245});
    image->setID("ps-logo");
    m_mainLayer->addChild(image);
    handleFinish();
  }

public:
  static PrivateServerPopup *create(Server server) {
    auto ret = new PrivateServerPopup();
    ret->setColor({14, 55, 190});
    if (ret->initAnchored(440.f, 280.f, server)) {
      ret->autorelease();
      return ret;
    }

    delete ret;
    return nullptr;
  }
};
