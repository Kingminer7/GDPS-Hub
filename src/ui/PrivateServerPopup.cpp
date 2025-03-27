#include "PrivateServerPopup.hpp"
#include "PSCreatorLayer.hpp"
#include "../utils/GDPSHub.hpp"
#include "../utils/Images.hpp"

bool PrivateServerPopup::setup(Server server) {
  setID("ps-popup"_spr);
  this->m_bgSprite->setID("background");
  auto contentSize = this->m_bgSprite->getContentSize();
  this->m_bgSprite->setSpriteFrame(CCSprite::create("geode.loader/GE_square03.png")->displayFrame());
  this->m_bgSprite->setContentSize(contentSize);
  this->m_buttonMenu->setID("back-menu");
  this->m_closeBtn->setID("back-button");
  this->m_server = server;
  setTitle(server.title);
  m_title->setFntFile("bigFont.fnt");
  m_title->setPosition(80, 254);
  m_title->setAnchorPoint({0, 0.5});
  m_title->limitLabelWidth(150, 0.7, 0.1);
  m_title->setID("server-name");
  setColor({ 0, 0, 0 });

  auto author = CCLabelBMFont::create(
      fmt::format("By {}", server.owner).c_str(), "goldFont.fnt");
  author->setPosition(80, 235);
  author->limitLabelWidth(173, 0.5, 0.05);
  author->setAnchorPoint({0, 0.5});
  author->setID("server-owner");
  m_mainLayer->addChild(author);

  auto desc = MDTextArea::create(server.fullDesc, ccp(420, 168));
  desc->setPosition(220, 131);
  desc->setID("server-description");
  m_mainLayer->addChild(desc);

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
  if (server.dcUrl == "") {
    discordBtn->setEnabled(false);
    discordBtn->setColor({100, 100, 100});
    discordBtn->setOpacity(100);
  }
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
  if (server.toolsUrl == "") {
    toolBtn->setEnabled(false);
    toolBtn->setColor({100, 100, 100});
    toolBtn->setOpacity(100);
  }
  menu->addChild(toolBtn);
  
  auto viewBtn = CCMenuItemSpriteExtra::create(
      ButtonSprite::create("Play"), this,
      menu_selector(PrivateServerPopup::viewServer));
  static_cast<ButtonSprite*>(viewBtn->getNormalImage())->updateBGImage("geode.loader/GE_button_05.png");
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
  static_cast<ButtonSprite*>(saveBtn->getNormalImage())->updateBGImage("geode.loader/GE_button_05.png");
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

  auto icon = IconNode::create(fmt::format("server-icon-{}", server.id), server.pfp);
  icon->setPosition({47, 245});
  icon->setID("server-icon");
  m_mainLayer->addChild(icon);

  log::info("{} {}", server.toolsUrl, server.dcUrl);

  return true;
}

void PrivateServerPopup::viewServer(CCObject *) {
  CCDirector::sharedDirector()->pushScene(
      CCTransitionFade::create(0.5, PSCreatorLayer::scene()));
  GDPSHub::get()->beginPreview(m_server);
}

void PrivateServerPopup::saveServer(CCObject *) {
  if (!Loader::get()->isModLoaded("km7dev.gdps-switcher")) {
    FLAlertLayer::create(
        "Error",
        "GDPS Switcher needs to be installed and enabled to save servers.",
        "OK")
        ->show();
    return;
  }

  if (GDPSUtils::createServer(m_server.title, m_server.url))
    FLAlertLayer::create("Success", "Server saved to GDPS Switcher.", "OK")->show();
  else
    FLAlertLayer::create("Error", "This server is already saved.","OK")->show();
}

void PrivateServerPopup::onDiscord(CCObject *sender) {
  if (m_server.dcUrl == "") {
    return;
  }
  web::openLinkInBrowser(m_server.dcUrl);
}

void PrivateServerPopup::onTools(CCObject *sender) {
  if (m_server.toolsUrl == "") {
    return;
  }
  web::openLinkInBrowser(m_server.toolsUrl);
}

void PrivateServerPopup::onWeb(CCObject *sender) {
  web::openLinkInBrowser(
      fmt::format("https://gdpshub.com/gdps/{}", m_server.id));
}

void PrivateServerPopup::onInfo(CCObject *) {
  log::info("{}", m_server.created_at);
  FLAlertLayer::create(
      "Server Info",
      fmt::format("<cl>ID: {}</c>\n<cy>Created: {}</c>\n<cg>Recommended Version: {}</c>", m_server.id,
                  GDPSHub::stampToDateTime(m_server.created_at), m_server.version),
      "Close")
      ->show();
}

PrivateServerPopup *PrivateServerPopup::create(Server server) {
  auto ret = new PrivateServerPopup();
  ret->setColor({14, 55, 190});
  if (ret->initAnchored(440.f, 280.f, server)) {
    ret->autorelease();
    return ret;
  }

  delete ret;
  return nullptr;
}
