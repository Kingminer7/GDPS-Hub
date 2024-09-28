#include "PSCreatorLayer.hpp"
#include "../utils/GDPSHub.hpp"
#include "GDPSHubLayer.hpp"
#include "../utils/Structs.hpp"
#include <Geode/binding/GauntletSelectLayer.hpp>
#include <Geode/binding/LeaderboardsLayer.hpp>
#include <Geode/binding/LevelSearchLayer.hpp>

#include <Geode/utils/cocos.hpp>

using namespace geode::prelude;

bool PSCreatorLayer::init()
{
    if (!CCLayer::init())
        return false;

    this->setID("PSCreatorLayer");

    this->setKeypadEnabled(true);

    auto background = createLayerBG();
    background->setID("background");
    this->addChild(background);

    auto winSize = CCDirector::get()->getWinSize();

    auto menu = CCMenu::create();
    menu->setID("exit-menu");
    menu->setZOrder(10);

    auto backBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png"),
        this, menu_selector(PSCreatorLayer::onGoBack));
    backBtn->setPosition(-winSize.width / 2 + 25.f, winSize.height / 2 - 25.f);
    backBtn->setID("back-button");
    menu->addChild(backBtn);
    this->addChild(menu);

    auto debugBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("accountBtn_settings_001.png"),
        this, menu_selector(PSCreatorLayer::onDebug));
    debugBtn->setPosition(-winSize.width / 2 + 25.f, winSize.height / 2 - 60.f);
    debugBtn->setID("debug-button");
    menu->addChild(debugBtn);
    this->addChild(menu);

    auto trCorner = CCSprite::createWithSpriteFrameName("GJ_sideArt_001.png");
    trCorner->setAnchorPoint({1, 1});
    trCorner->setFlipX(true);
    trCorner->setFlipY(true);
    trCorner->setID("top-right-corner");
    trCorner->setPosition({winSize.width + 1, winSize.height + 1});
    this->addChild(trCorner);

    auto brCorner = CCSprite::createWithSpriteFrameName("GJ_sideArt_001.png");
    brCorner->setAnchorPoint({1, 0});
    brCorner->setID("bottom-right-corner");
    brCorner->setFlipX(true);
    brCorner->setPosition({winSize.width + 1, -1});
    this->addChild(brCorner);

    auto tlCorner = CCSprite::createWithSpriteFrameName("GJ_sideArt_001.png");
    tlCorner->setAnchorPoint({0, 1});
    tlCorner->setFlipY(true);
    tlCorner->setID("top-left-corner");
    tlCorner->setPosition({-1, winSize.height + 1});
    this->addChild(tlCorner);

    auto blCorner = CCSprite::createWithSpriteFrameName("GJ_sideArt_001.png");
    blCorner->setAnchorPoint({0, 0});
    blCorner->setID("bottom-left-corner");
    blCorner->setPosition({-1, -1});
    this->addChild(blCorner);

    auto buttonMenu = CCMenu::create();
    buttonMenu->setID("button-menu");
    buttonMenu->setZOrder(10);
    buttonMenu->setContentSize({270, 270});

    auto scoresSpr = CCSprite::createWithSpriteFrameName("GJ_highscoreBtn_001.png");
    scoresSpr->setScale(0.75);
    auto scoresBtn = CCMenuItemSpriteExtra::create(scoresSpr, this, menu_selector(PSCreatorLayer::onScores));
    scoresBtn->setID("scores-button");
    buttonMenu->addChild(scoresBtn);

    auto gauntletsSpr = CCSprite::createWithSpriteFrameName("GJ_gauntletsBtn_001.png");
    gauntletsSpr->setScale(0.75);
    auto gauntletsBtn = CCMenuItemSpriteExtra::create(gauntletsSpr, this, menu_selector(PSCreatorLayer::onGauntlets));
    gauntletsBtn->setID("gauntlets-button");
    buttonMenu->addChild(gauntletsBtn);

    auto mapPacksSpr = CCSprite::createWithSpriteFrameName("GJ_mapPacksBtn_001.png");
    mapPacksSpr->setScale(0.75);
    auto mapPacksBtn = CCMenuItemSpriteExtra::create(mapPacksSpr, this, menu_selector(PSCreatorLayer::onMapPacks));
    mapPacksBtn->setID("map-packs-button");
    buttonMenu->addChild(mapPacksBtn);

    auto dailySpr = CCSprite::createWithSpriteFrameName("GJ_dailyBtn_001.png");
    dailySpr->setScale(0.75);
    auto dailyBtn = CCMenuItemSpriteExtra::create(dailySpr, this, menu_selector(PSCreatorLayer::onDaily));
    dailyBtn->setID("daily-button");
    buttonMenu->addChild(dailyBtn);

    auto weeklySpr = CCSprite::createWithSpriteFrameName("GJ_weeklyBtn_001.png");
    weeklySpr->setScale(0.75);
    auto weeklyBtn = CCMenuItemSpriteExtra::create(weeklySpr, this, menu_selector(PSCreatorLayer::onWeekly));
    weeklyBtn->setID("weekly-button");
    buttonMenu->addChild(weeklyBtn);

    auto eventSpr = CCSprite::createWithSpriteFrameName("GJ_eventBtn_001.png");
    eventSpr->setScale(0.75);
    auto eventBtn = CCMenuItemSpriteExtra::create(eventSpr, this, menu_selector(PSCreatorLayer::onEvent));
    eventBtn->setID("event-button");
    buttonMenu->addChild(eventBtn);

    auto featuredSpr = CCSprite::createWithSpriteFrameName("GJ_featuredBtn_001.png");
    featuredSpr->setScale(0.75);
    auto featuredBtn = CCMenuItemSpriteExtra::create(featuredSpr, this, menu_selector(PSCreatorLayer::onFeatured));
    featuredBtn->setID("featured-button");
    buttonMenu->addChild(featuredBtn);

    auto listsSpr = CCSprite::createWithSpriteFrameName("GJ_listsBtn_001.png");
    listsSpr->setScale(0.75);
    auto listsBtn = CCMenuItemSpriteExtra::create(listsSpr, this, menu_selector(PSCreatorLayer::onLists));
    listsBtn->setID("lists-button");
    buttonMenu->addChild(listsBtn);

    auto searchSpr = CCSprite::createWithSpriteFrameName("GJ_searchBtn_001.png");
    searchSpr->setScale(0.75);
    auto searchBtn = CCMenuItemSpriteExtra::create(searchSpr, this, menu_selector(PSCreatorLayer::onSearch));
    searchBtn->setID("search-button");
    buttonMenu->addChild(searchBtn);

    auto layout = AxisLayout::create(Axis::Row);
    layout->setGap(6.f);
    layout->setAxisAlignment(AxisAlignment::Center);
    layout->setGrowCrossAxis(true);
    layout->setAutoScale(false);
    buttonMenu->setLayout(layout);

    this->addChild(buttonMenu);

    return true;
}

void PSCreatorLayer::keyBackClicked()
{
    this->onGoBack(nullptr);
}

void PSCreatorLayer::onGoBack(CCObject *)
{
    GDPSHub::get()->endPreview();
    // CCDirector::get()->replaceScene(CCTransitionFade::create(0.5, MenuLayer::scene(false)));
    auto scene = CCScene::create();
    scene->addChild(GDPSHubLayer::create());
    // scene->addChild(PSCreatorLayer::create());
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5, scene));
}

PSCreatorLayer *PSCreatorLayer::create()
{
    auto ret = new PSCreatorLayer;
    if (ret->init())
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

void PSCreatorLayer::onScores(CCObject *)
{
    auto scene = LeaderboardsLayer::scene(LeaderboardState::Default);
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5, scene));
}

void PSCreatorLayer::onGauntlets(CCObject *)
{
    auto scene = GauntletSelectLayer::scene(0);
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5, scene));
}

void PSCreatorLayer::onMapPacks(CCObject *)
{
    auto gjso = GJSearchObject::create(SearchType::MapPack);
    auto scene = LevelBrowserLayer::scene(gjso);
    CCDirector::get()->replaceScene(CCTransitionFade::create(0.5, scene));
}

void PSCreatorLayer::onDaily(CCObject *)
{
    auto dlp = DailyLevelPage::create(GJTimedLevelType::Daily);
    dlp->show();
}

void PSCreatorLayer::onWeekly(CCObject *)
{
    DailyLevelPage::create(GJTimedLevelType::Weekly)->show();
}

void PSCreatorLayer::onEvent(CCObject *)
{
    DailyLevelPage::create(GJTimedLevelType::Event)->show();
}

void PSCreatorLayer::onFeatured(CCObject *)
{
    auto gjso = GJSearchObject::create(SearchType::Featured);
    auto scene = LevelBrowserLayer::scene(gjso);
    CCDirector::get()->replaceScene(CCTransitionFade::create(0.5, scene));
}

void PSCreatorLayer::onLists(CCObject *)
{
    auto gjso = GJSearchObject::create(SearchType::Featured);
    gjso->m_searchMode = 1;
    auto scene = LevelBrowserLayer::scene(gjso);
    CCDirector::get()->replaceScene(CCTransitionFade::create(0.5, scene));
}

void PSCreatorLayer::onSearch(CCObject *)
{
    auto scene = LevelSearchLayer::scene(0);
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5, scene));
}

void PSCreatorLayer::onDebug(CCObject *)
{
    std::string str = "";
    for (const auto &[uid, urlPrio] : ServerAPI::get()->getAllServers())
    {
        str += fmt::format("{}: {}\n", urlPrio.first, urlPrio.second);
    }
    geode::MDPopup::create("Server API Debug", str, "Close")->show();
}