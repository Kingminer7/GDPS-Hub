#include "PrivateServerNode.hpp"
#include "../utils/Structs.hpp"

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
    this->setContentSize(size);
    m_layer = layer;
    server = entry;

    this->setID(fmt::format("gdps-{}", server.id));

    auto bg = CCScale9Sprite::create("GJ_square01.png", {0, 0, 80, 80});
    bg->setContentSize(size);
    bg->setPosition(size / 2);
    bg->setID("server-node-bg");
    this->addChild(bg);

    auto name = CCLabelBMFont::create(entry.title.c_str(), "bigFont.fnt");
    name->setScale(0.8f);
    name->setPosition(8, this->getContentHeight() - 15.f);
    name->setAnchorPoint({0, 0.5});
    name->limitLabelWidth(size.width - 124, 0.8f, 0.1f);
    this->addChild(name);
    
    log::info("{}", std::to_string(entry.id));
    auto idLab = CCLabelBMFont::create(fmt::format("id {}", entry.id).c_str(), "chatFont.fnt");
    idLab->setPosition(4 + name->getContentWidth() * name->getScaleX(), this->getContentHeight() - 15.f);
    idLab->setAnchorPoint({0, 0.5});
    idLab->limitLabelWidth(116, 0.3f, 0.1f);
    this->addChild(idLab);

    auto desc = SimpleTextArea::create(entry.description, "chatFont.fnt", .7);
    desc->setContentSize(ccp(size.width * .65, size.height - 30.f));
    desc->setWrappingMode(WrappingMode::WORD_WRAP);
    desc->setPosition(ccp(8, size.height / 2 - 10));
    desc->setAnchorPoint({0, 0.5});
    desc->setMaxLines(3);
    desc->setWidth(size.width * .8);
    this->addChild(desc);

    auto menu = CCMenu::create();
    menu->setContentSize(ccp(size.width * .2 - 16, size.height));
    menu->setPosition(size.width * .8 + 8, 0);
    auto spr = CCSprite::createWithSpriteFrameName("GJ_playBtn2_001.png");
    spr->setScale(.7f);
    auto viewBtn = CCMenuItemSpriteExtra::create(spr,
        this,
        menu_selector(PrivateServerNode::viewServer));
    viewBtn->setPosition(ccp(menu->getContentWidth() - 28.f, menu->getContentHeight() / 2));
    menu->addChild(viewBtn);

    this->addChild(menu);

    return true;
}

void PrivateServerNode::viewServer(CCObject *) {
    FLAlertLayer::create("Error", "Not implemented.", "Ok")->show();
}

Server PrivateServerNode::getServer()
{
    return server;
}