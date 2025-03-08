#include "PSCreatorLayer.hpp"
#include "../utils/GDPSHub.hpp"
#include "Geode/binding/GameLevelManager.hpp"
#include "Geode/cocos/CCDirector.h"
#include "Geode/ui/SceneManager.hpp"
#include <Geode/binding/GauntletSelectLayer.hpp>
#include <Geode/binding/LeaderboardsLayer.hpp>
#include <Geode/binding/LevelSearchLayer.hpp>

#include <Geode/utils/cocos.hpp>

using namespace geode::prelude;

bool PSCreatorLayer::init() {
  if (!CCLayer::init())
    return false;

  setID("PSCreatorLayer"_spr);

  setKeypadEnabled(true);

  auto background = createLayerBG();
  background->setID("background");
  addChild(background);

  auto winSize = CCDirector::get()->getWinSize();

  auto menu = CCMenu::create();
  menu->setID("exit-menu");
  menu->setZOrder(10);

  auto backBtn = CCMenuItemSpriteExtra::create(
      CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png"), this,
      menu_selector(PSCreatorLayer::onGoBack));
  backBtn->setPosition(-winSize.width / 2 + 25.f, winSize.height / 2 - 25.f);
  backBtn->setID("back-button");
  menu->addChild(backBtn);
  addChild(menu);

  auto trCorner = CCSprite::createWithSpriteFrameName("GJ_sideArt_001.png");
  trCorner->setAnchorPoint({1, 1});
  trCorner->setFlipX(true);
  trCorner->setFlipY(true);
  trCorner->setID("top-right-corner");
  trCorner->setPosition({winSize.width + 1, winSize.height + 1});
  addChild(trCorner);

  auto brCorner = CCSprite::createWithSpriteFrameName("GJ_sideArt_001.png");
  brCorner->setAnchorPoint({1, 0});
  brCorner->setID("bottom-right-corner");
  brCorner->setFlipX(true);
  brCorner->setPosition({winSize.width + 1, -1});
  addChild(brCorner);

  auto tlCorner = CCSprite::createWithSpriteFrameName("GJ_sideArt_001.png");
  tlCorner->setAnchorPoint({0, 1});
  tlCorner->setFlipY(true);
  tlCorner->setID("top-left-corner");
  tlCorner->setPosition({-1, winSize.height + 1});
  addChild(tlCorner);

  auto blCorner = CCSprite::createWithSpriteFrameName("GJ_sideArt_001.png");
  blCorner->setAnchorPoint({0, 0});
  blCorner->setID("bottom-left-corner");
  blCorner->setPosition({-1, -1});
  addChild(blCorner);

  auto buttonMenu = CCMenu::create();
  buttonMenu->setID("button-menu");
  buttonMenu->setZOrder(10);
  buttonMenu->setContentSize({270, 270});

  auto scoresSpr =
      CCSprite::createWithSpriteFrameName("GJ_highscoreBtn_001.png");
  scoresSpr->setScale(0.75);
  auto scoresBtn = CCMenuItemSpriteExtra::create(
      scoresSpr, this, menu_selector(PSCreatorLayer::onScores));
  scoresBtn->setID("scores-button");
  buttonMenu->addChild(scoresBtn);

  auto gauntletsSpr =
      CCSprite::createWithSpriteFrameName("GJ_gauntletsBtn_001.png");
  gauntletsSpr->setScale(0.75);
  auto gauntletsBtn = CCMenuItemSpriteExtra::create(
      gauntletsSpr, this, menu_selector(PSCreatorLayer::onGauntlets));
  gauntletsBtn->setID("gauntlets-button");
  buttonMenu->addChild(gauntletsBtn);

  auto mapPacksSpr =
      CCSprite::createWithSpriteFrameName("GJ_mapPacksBtn_001.png");
  mapPacksSpr->setScale(0.75);
  auto mapPacksBtn = CCMenuItemSpriteExtra::create(
      mapPacksSpr, this, menu_selector(PSCreatorLayer::onMapPacks));
  mapPacksBtn->setID("map-packs-button");
  buttonMenu->addChild(mapPacksBtn);

  auto dailySpr = CCSprite::createWithSpriteFrameName("GJ_dailyBtn_001.png");
  dailySpr->setScale(0.75);
  auto dailyBtn = CCMenuItemSpriteExtra::create(
      dailySpr, this, menu_selector(PSCreatorLayer::onDaily));
  dailyBtn->setID("daily-button");
  buttonMenu->addChild(dailyBtn);

  auto weeklySpr = CCSprite::createWithSpriteFrameName("GJ_weeklyBtn_001.png");
  weeklySpr->setScale(0.75);
  auto weeklyBtn = CCMenuItemSpriteExtra::create(
      weeklySpr, this, menu_selector(PSCreatorLayer::onWeekly));
  weeklyBtn->setID("weekly-button");
  buttonMenu->addChild(weeklyBtn);

  auto eventSpr = CCSprite::createWithSpriteFrameName("GJ_eventBtn_001.png");
  eventSpr->setScale(0.75);
  auto eventBtn = CCMenuItemSpriteExtra::create(
      eventSpr, this, menu_selector(PSCreatorLayer::onEvent));
  eventBtn->setID("event-button");
  buttonMenu->addChild(eventBtn);

  auto featuredSpr =
      CCSprite::createWithSpriteFrameName("GJ_featuredBtn_001.png");
  featuredSpr->setScale(0.75);
  auto featuredBtn = CCMenuItemSpriteExtra::create(
      featuredSpr, this, menu_selector(PSCreatorLayer::onFeatured));
  featuredBtn->setID("featured-button");
  buttonMenu->addChild(featuredBtn);

  auto listsSpr = CCSprite::createWithSpriteFrameName("GJ_listsBtn_001.png");
  listsSpr->setScale(0.75);
  auto listsBtn = CCMenuItemSpriteExtra::create(
      listsSpr, this, menu_selector(PSCreatorLayer::onLists));
  listsBtn->setID("lists-button");
  buttonMenu->addChild(listsBtn);

  auto searchSpr = CCSprite::createWithSpriteFrameName("GJ_searchBtn_001.png");
  searchSpr->setScale(0.75);
  auto searchBtn = CCMenuItemSpriteExtra::create(
      searchSpr, this, menu_selector(PSCreatorLayer::onSearch));
  searchBtn->setID("search-button");
  buttonMenu->addChild(searchBtn);

  auto layout = AxisLayout::create(Axis::Row);
  layout->setGap(6.f);
  layout->setAxisAlignment(AxisAlignment::Center);
  layout->setGrowCrossAxis(true);
  layout->setAutoScale(false);
  buttonMenu->setLayout(layout);

  addChild(buttonMenu);

  GameLevelManager::get()->m_dailyID = -1;
  GameLevelManager::get()->m_weeklyID = -1;
  GameLevelManager::get()->m_eventID = -1;
  GameLevelManager::get()->m_dailyIDUnk = -1;
  GameLevelManager::get()->m_weeklyIDUnk = -1;
  GameLevelManager::get()->m_eventIDUnk = -1;
  GameLevelManager::get()->m_dailyTimeLeft = -1;
  GameLevelManager::get()->m_weeklyTimeLeft = -1;
  GameLevelManager::get()->m_eventTimeLeft = -1;

  if (!Mod::get()->getSettingValue<bool>("hide-previewing")) {
    infoLabel = CCLabelBMFont::create(
        std::string("Previewing " + m_server.title).c_str(), "bigFont.fnt");
    infoLabel->setID("server-info-label"_spr);
    infoLabel->setPosition({winSize.width / 2, 20});
    infoLabel->setScale(0.5);
    infoLabel->setZOrder(10);
    infoLabel->setOpacity(155);
    addChild(infoLabel);
    SceneManager::get()->keepAcrossScenes(infoLabel);
  }

  return true;
}

void PSCreatorLayer::keyBackClicked() { onGoBack(nullptr); }

void PSCreatorLayer::onGoBack(CCObject *) {
  GDPSHub::get()->endPreview();
  SceneManager::get()->forget(infoLabel);
  CCDirector::sharedDirector()->popSceneWithTransition(.5, PopTransition::kPopTransitionFade);

  GameLevelManager::get()->m_dailyID = -1;
  GameLevelManager::get()->m_weeklyID = -1;
  GameLevelManager::get()->m_eventID = -1;
  GameLevelManager::get()->m_dailyIDUnk = -1;
  GameLevelManager::get()->m_weeklyIDUnk = -1;
  GameLevelManager::get()->m_eventIDUnk = -1;
  GameLevelManager::get()->m_dailyTimeLeft = -1;
  GameLevelManager::get()->m_weeklyTimeLeft = -1;
  GameLevelManager::get()->m_eventTimeLeft = -1;
}

PSCreatorLayer *PSCreatorLayer::create() {
  auto ret = new PSCreatorLayer;
  if (ret->init()) {
    ret->autorelease();
    return ret;
  }
  CC_SAFE_DELETE(ret);
  return nullptr;
}

void PSCreatorLayer::onScores(CCObject *) {
  auto scene = LeaderboardsLayer::scene(LeaderboardState::Default);
  CCDirector::sharedDirector()->pushScene(
      CCTransitionFade::create(0.5, scene));
}

void PSCreatorLayer::onGauntlets(CCObject *) {
  auto scene = GauntletSelectLayer::scene(0);
  CCDirector::sharedDirector()->pushScene(
      CCTransitionFade::create(0.5, scene));
}

void PSCreatorLayer::onMapPacks(CCObject *) {
  auto gjso = GJSearchObject::create(SearchType::MapPack);
  auto scene = LevelBrowserLayer::scene(gjso);
  CCDirector::get()->pushScene(CCTransitionFade::create(0.5, scene));
}

void PSCreatorLayer::onDaily(CCObject *) {
  auto dlp = DailyLevelPage::create(GJTimedLevelType::Daily);
  dlp->show();
}

void PSCreatorLayer::onWeekly(CCObject *) {
  DailyLevelPage::create(GJTimedLevelType::Weekly)->show();
}

void PSCreatorLayer::onEvent(CCObject *) {
  DailyLevelPage::create(GJTimedLevelType::Event)->show();
}

void PSCreatorLayer::onFeatured(CCObject *) {
  auto gjso = GJSearchObject::create(SearchType::Featured);
  auto scene = LevelBrowserLayer::scene(gjso);
  CCDirector::get()->pushScene(CCTransitionFade::create(0.5, scene));
}

void PSCreatorLayer::onLists(CCObject *) {
  auto gjso = GJSearchObject::create(SearchType::Featured);
  gjso->m_searchMode = 1;
  auto scene = LevelBrowserLayer::scene(gjso);
  CCDirector::get()->pushScene(CCTransitionFade::create(0.5, scene));
}

void PSCreatorLayer::onSearch(CCObject *) {
  auto scene = LevelSearchLayer::scene(0);
  CCDirector::sharedDirector()->pushScene(
      CCTransitionFade::create(0.5, scene));
}

CCScene *PSCreatorLayer::scene() {
  auto scene = CCScene::create();
  scene->addChild(PSCreatorLayer::create());
  if (!Mod::get()->getSavedValue<bool>("has-seen", false)) {
    auto alert = FLAlertLayer::create("GDPS Hub", "You are previewing a server. Any progress you make on levels will not save.", "Ok");
    alert->m_scene = scene;
    alert->show();
    Mod::get()->setSavedValue<bool>("has-seen", true);
  }
  return scene;
}
