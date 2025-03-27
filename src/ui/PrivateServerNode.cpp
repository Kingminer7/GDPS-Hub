#include "PrivateServerNode.hpp"
#include "PSCreatorLayer.hpp"
#include "PrivateServerPopup.hpp"
#include "../utils/GDPSHub.hpp"
#include "../utils/Structs.hpp"
#include "../utils/Images.hpp"

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

    auto bg = CCScale9Sprite::create("square02b_001.png", {0, 0, 80, 80});
    bg->setColor({0,0,0});
    bg->setOpacity(90);
    bg->setContentSize(size);
    bg->setID("server-node-bg");
    addChildAtPosition(bg, Anchor::Center);
	if (server.banner != ""){
    	auto banner = IconNode::create(fmt::format("server-banner-{}",server.id),server.banner,true);
    	banner->setAnchorPoint({0.5,0.5});
    	//banner->setScale(size.width/banner->getContentWidth());
    	//banner->setPosition(size.width/2,size.height/2);
		//banner->setOpacity(90);
		auto bannerClip = CCClippingNode::create();
		bannerClip->setStencil(bg);
		bannerClip->setAlphaThreshold(0.05f);
		bannerClip->setContentSize(size);
		bannerClip->setAnchorPoint({0.5,0.5});
		bannerClip->addChildAtPosition(banner,Anchor::Center);
		addChildAtPosition(bannerClip,Anchor::Center);
	}
	
    auto icon = IconNode::create(fmt::format("server-icon-{}", server.id), server.pfp);
    icon->setScale(1.125f);
    icon->setID("server-icon");
    addChildAtPosition(icon, Anchor::Left, {40, 0});

    auto name = CCLabelBMFont::create(entry.title.c_str(), "bigFont.fnt");
    name->setAnchorPoint({0, 0.5});
    name->limitLabelWidth(150, .5f, 0.1f);
    name->setID("name");
    addChildAtPosition(name, Anchor::Left, {75,25});

    auto author = CCLabelBMFont::create(fmt::format("By {}",entry.owner).c_str(), "goldFont.fnt");
    author->setAnchorPoint({0, 0.5});
    author->limitLabelWidth(150, .4f, 0.1f);
    author->setID("author");
    addChildAtPosition(author, Anchor::Left, {80+name->getScaledContentWidth(),25});

    auto idLab = CCLabelBMFont::create(fmt::format("#{}", entry.id).c_str(), "chatFont.fnt");
    idLab->setPosition(ccp(bg->getContentWidth()-5.f, 5.f));
    idLab->setAnchorPoint({1, 0.5});
    idLab->setOpacity(100);
    idLab->limitLabelWidth(64, 0.5f, 0.1f);
    idLab->setID("id");
    idLab->setAlignment(CCTextAlignment::kCCTextAlignmentRight);
    addChild(idLab);


    auto descStr = entry.description.size() > 200 ? entry.description.substr(0, 200) + "..." : entry.description;
	std::transform(descStr.begin(), descStr.end(), descStr.begin(), [&](char c) {
        return (c<0) ? ' ' : c;
    });

    // transparent black
    auto descBg = CCScale9Sprite::create("square02b_001.png", {0, 0, 80, 80});
    descBg->setColor({0, 0, 0});
    descBg->setOpacity(90);
    descBg->setScale(0.5f);
    descBg->setContentSize({410, 90});
    descBg->setID("description-bg");
    descBg->setAnchorPoint({0, 1});
    addChildAtPosition(descBg, Anchor::Left, {75, 12.5});

    auto desc = TextArea::create(descStr, "chatFont.fnt", .6, 180, {0, 1}, 10, false);
    desc->setContentSize({207, desc->getContentHeight()});
    desc->setAnchorPoint({0, 1});
    desc->setID("description");

    CCLayerColor* mask = CCLayerColor::create({255, 255, 255});
    mask->setContentSize({200, 40});
    mask->setPosition({0, 0});

    auto clip = CCClippingNode::create();
    clip->setID("description-clip");
    clip->setContentSize({200,  40});
    clip->setStencil(mask);
    clip->setZOrder(1);
    clip->setAnchorPoint({0, 1});
    clip->addChildAtPosition(desc, Anchor::TopLeft, {0, 0});

    addChildAtPosition(clip, Anchor::Left, {77.5, 10});

    auto menu = CCMenu::create();
    menu->setContentSize(ccp(size.width * .2 - 16, size.height));
    menu->setPosition(size.width * .8 + 8, 0);
    menu->setID("menu");

    auto spr = ButtonSprite::create("View","bigFont.fnt","geode.loader/GE_button_05.png",.8f);
    //spr->updateBGImage("geode.loader/GE_button_05.png");
    spr->setScale(0.8f);
    auto viewBtn = CCMenuItemSpriteExtra::create(spr,
        this,
        menu_selector(PrivateServerNode::viewServer));
    viewBtn->setPosition(ccp(menu->getContentWidth()-36.f, menu->getContentHeight() / 2));
    if (server.url == "No URL provided.") {
        viewBtn->setEnabled(false);
        viewBtn->setColor({100, 100, 100});
        viewBtn->setOpacity(100);
    }
    viewBtn->setID("view-btn");
    menu->addChild(viewBtn);

    auto infoSpr = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
    infoSpr->setScale(0.7f);
    auto infoBtn = CCMenuItemSpriteExtra::create(infoSpr,
        this,
        menu_selector(PrivateServerNode::openPopup));
    infoBtn->setPosition(ccp(menu->getContentWidth()-5.f, menu->getContentHeight()-13.f));
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
