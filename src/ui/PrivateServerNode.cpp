#include "PrivateServerNode.hpp"
#include "PSCreatorLayer.hpp"
#include "PrivateServerPopup.hpp"
#include "../utils/GDPSHub.hpp"
#include "../utils/Structs.hpp"
#include <regex>

std::string removeMarkdown(const std::string& str) {
    static const std::vector<std::pair<std::regex, std::string>> patterns = {
        {std::regex(R"(\*\*(.+?)\*\*)"), "$1"},
        {std::regex(R"(__(.+?)__)"), "$1"},
        {std::regex(R"(_(.+?)_)"), "$1"},
        {std::regex(R"(\*(.+?)\*)"), "$1"},
        {std::regex(R"(~~(.+?)~~)"), "$1"},
        {std::regex(R"(`(.+?)`)"), "$1"},
        {std::regex(R"(```[\s\S]*?```)"), "$1"},
        {std::regex(R"(\[(.+?)\]\((.+?)\))"), "$1"},
        {std::regex(R"(!\[(.+?)\]\((.+?)\))"), "$1"},
        {std::regex(R"(^#+\s+(.+?)\s*$)"), "$1"},
        {std::regex(R"(^\s*=+\s*$)"), "$1"},
        {std::regex(R"(^\s*-+\s*$)"), "$1"},
        {std::regex(R"(^\s*>\s+(.+?)\s*$)"), "$1"},
        {std::regex(R"(^\s*[\*\+-]\s+(.+?)\s*$)"), "$1"},
        {std::regex(R"(^\s*\d+\.\s+(.+?)\s*$)"), "$1"},
        {std::regex(R"(^\s*[-*_]{3,}\s*$)"), "$1"}
    };

    std::string result = str;
    for (const auto& pattern : patterns) {
        result = std::regex_replace(result, pattern.first, pattern.second);
    }
    return result;
}

PrivateServerNode *PrivateServerNode::create(GDPSHubLayer *layer, Server entry, CCSize size)
{
    auto ret = new PrivateServerNode;
    if (ret && ret->init(layer, entry, size))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool PrivateServerNode::init(GDPSHubLayer *layer, Server entry, CCSize size)
{
    if (!CCNode::init())
        return false;
    setContentSize(size);
    m_layer = layer;
    server = entry;

    setID(fmt::format("gdps-{}", server.id));

    auto bg = CCScale9Sprite::create("GJ_square01.png", {0, 0, 80, 80});
    bg->setContentSize(size);
    bg->setPosition(size / 2);
    bg->setID("server-node-bg");
    addChild(bg);

    auto name = CCLabelBMFont::create(entry.title.c_str(), "bigFont.fnt");
    name->setScale(0.8f);
    name->setPosition(8, getContentHeight() - 15.f);
    name->setAnchorPoint({0, 0.5});
    name->limitLabelWidth(size.width - 124, 0.8f, 0.1f);
    name->setID("name");
    addChild(name);

    auto idLab = CCLabelBMFont::create(fmt::format("id {}", entry.id).c_str(), "chatFont.fnt");
    idLab->setPosition(12 + name->getContentWidth() * name->getScaleX(), getContentHeight() - 15.f);
    idLab->setAnchorPoint({0, 0.5});
    idLab->limitLabelWidth(64, 0.5f, 0.1f);
    idLab->setID("id");
    addChild(idLab);

    // auto desc = SimpleTextArea::create(entry.description, "chatFont.fnt", .7);
    // desc->setContentSize(ccp(size.width * .65, size.height - 30.f));
    // desc->setWrappingMode(WrappingMode::WORD_WRAP);
    // desc->setPosition(ccp(8, size.height - 30));
    // desc->setAnchorPoint({0, 1});
    // desc->setMaxLines(3);
    // desc->setWidth(size.width * .8);
    // desc->setID("description");
    // addChild(desc);

    auto desc = TextArea::create(removeMarkdown(entry.description), "chatFont.fnt", .7, size.width * .65, {0, 1}, 10, false);
    desc->setPosition({0, 40});
    desc->setContentSize({size.width * .65f, desc->getContentHeight()});
    desc->setAnchorPoint({0, 1});
    desc->setID("description");

    CCLayerColor* mask = CCLayerColor::create({255, 255, 255});
    mask->setContentSize({size.width * .65f, size.height - 40});
    mask->setPosition({0, 0});

    auto clip = CCClippingNode::create();
    clip->setID("description-clip");
    clip->setContentSize({size.width * .65f, size.height - 40});
    clip->setStencil(mask);
    clip->setZOrder(1);
    clip->setPosition(ccp(8, size.height - 30));
    clip->setAnchorPoint({0, 1});
    clip->addChild(desc);

    addChild(clip);

    auto menu = CCMenu::create();
    menu->setContentSize(ccp(size.width * .2 - 16, size.height));
    menu->setPosition(size.width * .8 + 8, 0);
    menu->setID("menu");

    auto spr = CCSprite::createWithSpriteFrameName("GJ_playBtn2_001.png");
    spr->setScale(.7f);
    auto viewBtn = CCMenuItemSpriteExtra::create(spr,
        this,
        menu_selector(PrivateServerNode::viewServer));
    viewBtn->setPosition(ccp(menu->getContentWidth() - 28.f, menu->getContentHeight() / 2));
    if (server.url == "No URL provided.") {
        viewBtn->setEnabled(false);
        viewBtn->setColor({100, 100, 100});
        viewBtn->setOpacity(100);
    }
    viewBtn->setID("view-btn");
    menu->addChild(viewBtn);

    spr = CircleButtonSprite::createWithSpriteFrameName("info.png"_spr, 1.5, CircleBaseColor::Green, CircleBaseSize::Large);
    spr->setScale(0.7f);
    auto infoBtn = CCMenuItemSpriteExtra::create(spr,
        this,
        menu_selector(PrivateServerNode::openPopup));
    infoBtn->setPosition(ccp(menu->getContentWidth() - 88.f, menu->getContentHeight() / 2));
    infoBtn->setID("info-btn");
    menu->addChild(infoBtn);

    addChild(menu);

    return true;
}

void PrivateServerNode::viewServer(CCObject *) {
	CCDirector::sharedDirector()->pushScene(CCTransitionFade::create(0.5, PSCreatorLayer::scene()));
    GDPSHub::get()->beginPreview(server);
}

void PrivateServerNode::openPopup(CCObject *) {
    auto popup = PrivateServerPopup::create(server);
    popup->show();
}

Server PrivateServerNode::getServer()
{
    return server;
}
