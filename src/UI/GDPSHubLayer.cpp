#include "GDPSHubLayer.hpp"

#include <Geode/utils/cocos.hpp>

std::string current;

bool GDPSHubLayer::init()
{
    if (!CCLayer::init())
        return false;

    this->setID("GDPSHubLayer");

    this->setKeypadEnabled(true);

    auto background = createLayerBG();
    background->setID("background");
    this->addChild(background);

    auto winSize = CCDirector::get()->getWinSize();

    auto title = CCLabelBMFont::create("Basic Layer", "goldFont.fnt");
    title->setPosition(winSize.width / 2, winSize.height - 22.5f);
    title->setID("title-text");
    this->addChild(title);

    auto menu = CCMenu::create();
    menu->setID("menu");
    menu->setZOrder(10);

    auto backBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png"),
        this, menu_selector(GDPSHubLayer::onGoBack));
    backBtn->setPosition(-winSize.width / 2 + 25.f, winSize.height / 2 - 25.f);
    backBtn->setID("back-button");
    menu->addChild(backBtn);
    this->addChild(menu);
    
    return true;
}

void GDPSHubLayer::keyBackClicked()
{
    this->onGoBack(nullptr);
}

void GDPSHubLayer::onGoBack(CCObject *)
{
    CCDirector::get()->replaceScene(CCTransitionFade::create(0.5, MenuLayer::scene(false)));
}

GDPSHubLayer *GDPSHubLayer::create()
{
    auto ret = new GDPSHubLayer;
    if (ret->init())
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

void updateList() {
    
}