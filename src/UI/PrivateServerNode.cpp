#include "PrivateServerNode.hpp"
#include "../utils/Structs.hpp"

PrivateServerNode *PrivateServerNode::create(GDPSHubLayer *layer, Server entry, CCSize size, bool selected)
{
    auto ret = new PrivateServerNode;
    if (ret && ret->init(layer, entry, size, selected))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool PrivateServerNode::init(GDPSHubLayer *layer, Server entry, CCSize size, bool selected)
{
    if (!CCNode::init())
        return false;
    this->setContentSize(size);
    m_layer = layer;
    server = entry;

    auto bg = CCScale9Sprite::create("GJ_square01.png", {0, 0, 80, 80});
    bg->setContentSize(size);
    bg->setPosition(size / 2);
    bg->setID("server-node-bg");
    this->addChild(bg);

    auto label = CCLabelBMFont::create(entry.title.c_str(), "bigFont.fnt");
    label->setScale(0.8f);
    label->setPosition(8, size.height / 2 + 2);
    label->setAnchorPoint({0, 0.5});
    label->limitLabelWidth(size.width - 124, 0.8f, 0.1f);
    this->addChild(label);

    return true;
}

Server PrivateServerNode::getServer()
{
    return server;
}