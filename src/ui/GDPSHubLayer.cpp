#include "GDPSHubLayer.hpp"
#include "PrivateServerNode.hpp"
#include "../utils/Structs.hpp"

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

    auto scrollBg = CCScale9Sprite::create("square02b_001.png", {0, 0, 80, 80});
    scrollBg->setColor({0, 0, 0});
    scrollBg->setOpacity(90);
    scrollBg->setContentSize({winSize.width - 85, winSize.height - 60});
    scrollBg->setPosition(winSize / 2);
    scrollBg->ignoreAnchorPointForPosition(false);
    scrollBg->setID("server-scroll-bg");
    this->addChild(scrollBg);

    scroll = ScrollLayer::create({winSize.width - 85, winSize.height - 60});
    scroll->setPosition((winSize - scroll->getContentSize()) / 2);
    scroll->setID("server-scroll");
    this->addChild(scroll);

    updateList();

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

void GDPSHubLayer::updateList() {
    scroll->m_contentLayer->removeAllChildren();
    m_loadingCircle->setParentLayer(this);
    m_loadingCircle->setPosition({ 0, 0 });
    m_loadingCircle->setScale(1.f);
    m_loadingCircle->show();
    scroll->moveToTop();

    m_listener.bind([this] (web::WebTask::Event* e) {
        if (web::WebResponse* res = e->getValue()) {
            std::string err;
            auto opt = res->json();
            auto data = opt.value_or("err");
            if (data == "err") {
                log::info("{}", err);
                FLAlertLayer::create(
                    "Error",
                    "Could not connect to GDPS Hub Servers.",
                    "Ok")
                    ->show();
                return;
            }
            auto servers = data.as_array();

            float totalHeight = 0.f;
            std::vector<PrivateServerNode *> rendered;

            for (matjson::Value val : servers) {
                auto server = val.as<Server>();
                
                auto node = PrivateServerNode::create(this, server, ccp(scroll->getContentWidth(), 80));
                node->setPosition(0, totalHeight);
                scroll->m_contentLayer->addChild(node);
                totalHeight += 80;
                rendered.push_back(node);
            }

            totalHeight -= 5;

            if (totalHeight < scroll->m_contentLayer->getContentSize().height)
            {
                totalHeight = scroll->m_contentLayer->getContentSize().height;
            }

            for (auto &node : rendered)
            {
                node->setPositionY(totalHeight - node->getPositionY() - 80);
            }

            scroll->m_contentLayer->setContentSize({scroll->m_contentLayer->getContentWidth(), totalHeight});
            scroll->moveToTop();
            m_loadingCircle->fadeAndRemove();
        }
    });

    if (Mod::get()->getSavedValue<bool>("devMode")) {
        std::string err;
        std::string fallback = R"(
        [{
        "id":1,
        "title":"Could not connect.fsdadfaadsdffsdadfafdsaafd",
        "description":"This is an example server for development without server access. Message km7dev if you see this. This is not intended to be built for release. Super long text for testing reasons :) gd is fun hello 1234567890:):):):P:):).",
        "gdpsdb":"https://km7dev.ps.fhgdps.com",
        "pfp":"https://raw.githubusercontent.com/geode-sdk/geode/main/loader/resources/logos/geode-logo.png",
        "discord_url":"https://google.com",
        "views":5819531923,
        "created_at":"1725541895"
        }]
        )";
        // timestamp should be Thu Sep 05 2024 09:11:35 GMT-0400 (Eastern Daylight Time) (current time as of creating the test)
        auto dat = matjson::parse(fallback, err);
        auto data = dat.value_or("err2");
        if (data == "err2") {
        log::info("{}", err);
            FLAlertLayer::create(
            "Error",
            "Could not connect to GDPS Hub Servers.",
            "Ok")
            ->show();
            return;
        }
        auto servers = data.as_array();

            float totalHeight = 0.f;
            std::vector<PrivateServerNode *> rendered;

            for (matjson::Value val : servers) {
                auto server = val.as<Server>();
                
                auto node = PrivateServerNode::create(this, server, ccp(scroll->getContentWidth(), 80));
                node->setPosition(0, totalHeight);
                scroll->m_contentLayer->addChild(node);
                totalHeight += 80;
                rendered.push_back(node);
            }

            totalHeight -= 5;

            if (totalHeight < scroll->m_contentLayer->getContentSize().height)
            {
                totalHeight = scroll->m_contentLayer->getContentSize().height;
            }

            for (auto &node : rendered)
            {
                node->setPositionY(totalHeight - node->getPositionY() - 80);
            }

            scroll->m_contentLayer->setContentSize({scroll->m_contentLayer->getContentWidth(), totalHeight});
            scroll->moveToTop();
            m_loadingCircle->fadeAndRemove();
    } else {
        auto req = web::WebRequest();
        m_listener.setFilter(req.get("https://api.gdpshub.com/gdps/geode"));
    }
}