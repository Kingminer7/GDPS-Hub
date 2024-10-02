#include "GDPSHubLayer.hpp"
#include "../utils/GDPSHub.hpp"
#include "PrivateServerNode.hpp"
#include "../utils/Structs.hpp"


#include <Geode/utils/cocos.hpp>

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
        CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png"),
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

    m_infoLabel = CCLabelBMFont::create("", "bigFont.fnt");
    m_infoLabel->setPosition(winSize / 2);
    m_infoLabel->setID("info-label");
    m_infoLabel->setAnchorPoint({0.5, 0.5});
    m_infoLabel->setScale(0.5);
    this->addChild(m_infoLabel);

    if (GDPSHub::get()->servers.size() == 0)
    {
        this->fetchServers();
    }
    else
    {
        this->updateList();
    }

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

void GDPSHubLayer::updateList()
{
    scroll->m_contentLayer->removeAllChildren();
    scroll->moveToTop();

    float totalHeight = 0.f;
    std::vector<PrivateServerNode *> rendered;
    for (Server server : GDPSHub::get()->servers)
    {
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
}

void GDPSHubLayer::fetchServers()
{
    m_loadingCircle->setParentLayer(this);
    m_loadingCircle->setPosition({0, 0});
    m_loadingCircle->setScale(1.f);
    m_loadingCircle->show();

    m_listener.bind([this](web::WebTask::Event *e)
                    {
        if (web::WebResponse* res = e->getValue()) {
            std::string err;
            auto opt = res->json();
            auto data = opt.value_or("err");
            if (data == "err") {
                log::info("{}", err);
                this->m_infoLabel->setString("Failed to fetch servers.");
                m_loadingCircle->fadeAndRemove();
                return;
            }
            if (!data.contains("success") || data["success"].as_bool() == false || !data.contains("data"))
            {
                if (data.contains("message")) this->m_infoLabel->setString(data["message"].as_string().c_str());
                else this->m_infoLabel->setString("Failed to fetch servers.");
                m_loadingCircle->fadeAndRemove();
                return;
            }
            auto servers = data["data"].as_array();

            for (matjson::Value val : servers) {
                auto server = val.as<Server>();
                GDPSHub::get()->servers.push_back(server);
            }
            updateList();
            m_loadingCircle->fadeAndRemove();
        } });
    auto req = web::WebRequest();
    m_listener.setFilter(req.get("https://api.gdpshub.com/geode/get"));
}