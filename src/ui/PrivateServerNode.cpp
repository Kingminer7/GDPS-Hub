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

    auto desc = SimpleTextArea::create(entry.description, "chatFont.fnt", .7);
    desc->setContentSize(ccp(size.width * .65, size.height - 30.f));
    desc->setWrappingMode(WrappingMode::WORD_WRAP);
    desc->setPosition(ccp(8, size.height / 2 - 10));
    desc->setAnchorPoint({0, 0.5});
    desc->setMaxLines(4);
    desc->setWidth(size.width * .65);
    this->addChild(desc);

    auto menu = CCMenu::create();
    menu->setContentSize(ccp(size.width * .35, size.height));
    menu->setPosition(size.width * .65, 0);
    this->addChild(menu);

    return true;
}

Server PrivateServerNode::getServer()
{
    return server;
}