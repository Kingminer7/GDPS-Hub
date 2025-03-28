#include "GDPSHubLayer.hpp"
#include "../utils/GDPSHub.hpp"
#include "../utils/Structs.hpp"
#include "Geode/cocos/misc_nodes/CCClippingNode.h"
#include "PrivateServerNode.hpp"

#include <Geode/utils/cocos.hpp>
#include <Geode/ui/Popup.hpp>

using namespace geode::prelude;

class PSSearchPopup : public Popup<GDPSHubLayer *> {
protected:
  TextInput *m_query;
  GDPSHubLayer *m_layer;
  CCMenuItemSpriteExtra *topSel;
  CCMenuItemSpriteExtra *recentSel;
  bool changed = false;
  bool setup(GDPSHubLayer *layer) override {
    setTitle("Query Options");
    auto contentSize = m_bgSprite->getContentSize();
    m_bgSprite->setSpriteFrame(CCSprite::create("geode.loader/GE_square03.png")->displayFrame());
    m_bgSprite->setContentSize(contentSize);
    setID("search-popup"_spr);
    m_title->setID("title");
    m_buttonMenu->setID("back-menu");
    m_closeBtn->setID("close-btn");
    m_mainLayer->setID("main-layer");
    m_bgSprite->setID("background");
    m_layer = layer;

    auto menu = CCMenu::create();
    menu->setContentSize({250, 125});
    menu->setID("button-layer");

    // Top Selection
    CCSprite *topOn = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
    topOn->setVisible(m_layer->m_queryType == "top");
    topOn->setID("on-sprite");
    CCSprite *topOff = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
    topOff->setVisible(m_layer->m_queryType != "top");
    topOff->setID("off-sprite");
    topSel = CCMenuItemSpriteExtra::create(topOn, this, menu_selector(PSSearchPopup::changeQueryType));
    topSel->addChildAtPosition(topOff, Anchor::Center);
    topSel->setID("top");
    topSel->m_baseScale = 0.75f;
    topSel->setScale(0.75f);
    menu->addChildAtPosition(topSel, Anchor::Left, {25, 20});
    auto topLab = CCLabelBMFont::create("Top Servers","bigFont.fnt");
    topLab->setAnchorPoint({ 0, 0.5 });
    topLab->setScale(0.575);
    topLab->setID("top-label");
    menu->addChildAtPosition(topLab, Anchor::Left, {40, 20});

    // Recent Selection
    CCSprite *recentOn = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
    recentOn->setVisible(m_layer->m_queryType == "recent");
    recentOn->setID("on-sprite");
    CCSprite *recentOff = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
    recentOff->setVisible(m_layer->m_queryType != "recent");
    recentOff->setID("off-sprite");
    recentSel = CCMenuItemSpriteExtra::create(recentOn, this, menu_selector(PSSearchPopup::changeQueryType));
    recentSel->addChildAtPosition(recentOff, Anchor::Center);
    recentSel->setID("recent");
    recentSel->m_baseScale = 0.75f;
    recentSel->setScale(0.75f);
    menu->addChildAtPosition(recentSel, Anchor::Left, {25, -8});
    auto recentLab = CCLabelBMFont::create("Recently Added","bigFont.fnt");
    recentLab->setAnchorPoint({ 0, 0.5 });
    recentLab->setScale(0.575);
    recentLab->setID("recent-label");
    menu->addChildAtPosition(recentLab, Anchor::Left, {40, -8});

    // Search Box
    m_query = TextInput::create(205.f, "Search");
    m_query->setScale(.85); 
    m_query->setString(m_layer->m_search);
    m_query->setID("search-box");
    menu->addChildAtPosition(m_query, Anchor::BottomLeft, {102.5 , 25});
    m_query->getInputNode()->setAllowedChars("`1234567890-=qwertyuiop[]\\asdfghjkl;'cxzvbnm,./~!@#$%^&*()_+QWERTYUIOP{}|ASDFGHJKL:\"ZXCVBNM<>? ");
    m_query->setCallback([this](const std::string& str){
      m_layer->m_search = str;
      m_layer->m_page = 1;
      changed = true;
    });

    auto clearBtn = CCMenuItemSpriteExtra::create(
      CCSprite::createWithSpriteFrameName("GJ_longBtn07_001.png"), this, menu_selector(PSSearchPopup::resetSearch)
    );
    clearBtn->setID("clear-btn");
    
    menu->addChildAtPosition(clearBtn, Anchor::BottomRight, {-35, 25});
    m_mainLayer->addChildAtPosition(menu, Anchor::Center, {0, 0});
    return true;
  }

  void changeQueryType(CCObject * sender) {
    std::string id = static_cast<CCNode *>(sender)->getID();
    if (id == "top") m_layer->m_queryType = "top";
    else if (id == "recent") m_layer->m_queryType = "recent";

    topSel->getChildByID("on-sprite")->setVisible(m_layer->m_queryType == "top");
    topSel->getChildByID("off-sprite")->setVisible(m_layer->m_queryType != "top");
    recentSel->getChildByID("on-sprite")->setVisible(m_layer->m_queryType == "recent");
    recentSel->getChildByID("off-sprite")->setVisible(m_layer->m_queryType != "recent");

    changed = true;
  }

  void onClose(CCObject * obj) override {
    if (changed) m_layer->fetchServers();
    Popup::onClose(obj);
  }

public:
  static PSSearchPopup *create(GDPSHubLayer *layer) {
    auto ret = new PSSearchPopup();
    if (ret->initAnchored(250.f, 125.f, layer)) {
      ret->autorelease();
      return ret;
    }

    delete ret;
    return nullptr;
  }

  void resetSearch(CCObject *sender) {
    m_query->setString("");
    m_layer->m_search = "";
    m_layer->m_page = 1;
    changed = true;
  }
};

bool GDPSHubLayer::init() {
  if (!CCLayer::init())
    return false;

  setAnchorPoint({0, 0});

  setID("gdps-hub-layer"_spr);

  setKeypadEnabled(true);

  auto winSize = CCDirector::get()->getWinSize();

  auto background = CCLayerColor::create({25,35,47});
  background->setOpacity(255);
  background->setID("background");
  this->addChildAtPosition(background,Anchor::BottomLeft);

  auto swelvyNode = CCNode::create();
  swelvyNode->setID("swelvy-node");
  addChildAtPosition(swelvyNode, Anchor::Center);

  auto bottomGlow = CCSprite::createWithSpriteFrameName("chest_glow_bg_001.png");
  auto topGlow = CCSprite::createWithSpriteFrameName("chest_glow_bg_001.png");
  swelvyNode->addChild(bottomGlow);
  swelvyNode->addChild(topGlow);

  auto layers = std::initializer_list<std::pair<ccColor4B, const char*>> {
    // @geode-ignore(unknown-resource)
    { ccc4(255, 255, 255,255), "geode.loader/swelve-layer0.png" },
    // @geode-ignore(unknown-resource)
    { ccc4(255, 255, 255,100), "geode.loader/swelve-layer1.png" },
    // @geode-ignore(unknown-resource)
    { ccc4(255, 255, 255,50), "geode.loader/swelve-layer2.png" },
    // @geode-ignore(unknown-resource)
    { ccc4(43,127,252,255), "geode.loader/swelve-layer0.png" },
    // @geode-ignore(unknown-resource)
    { ccc4(46, 92, 247,255), "geode.loader/swelve-layer0.png" },
    // @geode-ignore(unknown-resource)
    { ccc4(43,127,252,255), "geode.loader/swelve-layer0.png" },
    // @geode-ignore(unknown-resource)
    { ccc4(46, 92, 247,255), "geode.loader/swelve-layer0.png" },
};
  int layerIndex = 1;
  for (auto layer : layers){
    auto swelveLayer = CCSprite::create(layer.second);
    ccTexParams params = {GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT};
    swelveLayer->getTexture()->setTexParameters(&params);
    auto rect = swelveLayer->getTextureRect();
    rect.size.width = winSize.width*1.75f;
    rect.origin.y = rect.origin.y + 1;
    swelveLayer->setTextureRect(rect);
    swelveLayer->setAnchorPoint({0.5,0});
    swelveLayer->setColor({layer.first.r,layer.first.g,layer.first.b});
    swelveLayer->setOpacity(layer.first.a);
    swelveLayer->setID(fmt::format("swelve-layer-{}",layerIndex-1));
    if (layerIndex < 4){
      swelveLayer->setScale(0.675f);
      swelveLayer->setPosition({0, -(winSize.height/2+60-2*(layerIndex-1))});
      float moveBy = rand() % 50 - 20;
      float moveTime = rand() % 5 + 2;
      auto action = CCSequence::create(
        CCEaseSineInOut::create(CCMoveBy::create(moveTime, {moveBy, 0})),
        CCEaseSineInOut::create(CCMoveBy::create(moveTime, {-moveBy, 0})),
        nullptr
      );
      swelveLayer->runAction(CCRepeatForever::create(action));
    }
    else {
    switch (layerIndex){
      case 4: {
        swelveLayer->setPosition({0, -(winSize.height/2+110)});  
        swelveLayer->setScale(1.25f);
        swelveLayer->setRotation(5.f);
        swelveLayer->setZOrder(-1);
        auto action = CCSequence::create(
          CCEaseInOut::create(CCMoveBy::create(4, {-5, -7}),2),
          CCEaseInOut::create(CCMoveBy::create(4, {5, 7}),2),
          nullptr
        );
        swelveLayer->runAction(CCRepeatForever::create(action));
        break;
      }
      case 5: {
        swelveLayer->setPosition({0, -(winSize.height/2+95)});  
        swelveLayer->setScale(1.25f);
        swelveLayer->setFlipX(true);
        swelveLayer->setRotation(-5.f);
        swelveLayer->setZOrder(-2);
        auto action = CCSequence::create(
          CCEaseInOut::create(CCMoveBy::create(4, {5, -10}),2),
          CCEaseInOut::create(CCMoveBy::create(4, {-5, 10}),2),
          nullptr
        );
        swelveLayer->runAction(CCRepeatForever::create(action));
        break;
      }
      case 6: {
        swelveLayer->setPosition({0, winSize.height/2-25});  
        swelveLayer->setScale(1.25f);
        swelveLayer->setFlipY(true);
        swelveLayer->setFlipX(true);
        swelveLayer->setRotation(5.f);
        swelveLayer->setZOrder(-1);
        auto action = CCSequence::create(
          CCEaseInOut::create(CCMoveBy::create(4, {5, 7}),2),
          CCEaseInOut::create(CCMoveBy::create(4, {-5, -7}),2),
          nullptr
        );
        swelveLayer->runAction(CCRepeatForever::create(action));
        break;
      }
      case 7: {
        swelveLayer->setPosition({0, winSize.height/2-45});  
        swelveLayer->setScale(1.25f);
        swelveLayer->setFlipY(true);
        swelveLayer->setRotation(-5.f);
        swelveLayer->setZOrder(-2);
        auto action = CCSequence::create(
          CCEaseInOut::create(CCMoveBy::create(4, {-5, 10}),2),
          CCEaseInOut::create(CCMoveBy::create(4, {5, -10}),2),
          nullptr
        );
        swelveLayer->runAction(CCRepeatForever::create(action));
        break;
        }
      }
    }
    swelvyNode->addChild(swelveLayer);
    layerIndex++;
  }

  bottomGlow->setColor({43,127,252});
  bottomGlow->setAnchorPoint({0,0});
  bottomGlow->setPosition({(-winSize.width/2)-15.f, -winSize.height/2});
  bottomGlow->setScaleX((winSize.width/bottomGlow->getContentSize().width)*4);
  bottomGlow->setScaleY(5.f);
  bottomGlow->setOpacity(50);

  topGlow->setColor({43,127,252});
  topGlow->setAnchorPoint({0,0});
  topGlow->setPosition({(-winSize.width/2)-15.f, (winSize.height/2)+20});
  topGlow->setScaleX((winSize.width/bottomGlow->getContentSize().width)*4);
  topGlow->setScaleY(-5.f);
  topGlow->setOpacity(50);

  auto action = CCSequence::create(
    CCEaseSineInOut::create(CCMoveBy::create(4,{0,-20.f})),
    CCEaseSineInOut::create(CCMoveBy::create(4,{0,20.f})),
    nullptr
  );
  auto actionInverted = CCSequence::create(
    CCEaseSineInOut::create(CCMoveBy::create(4,{0,20.f})),
    CCEaseSineInOut::create(CCMoveBy::create(4,{0,-20.f})),
    nullptr
  );

  bottomGlow->runAction(CCRepeatForever::create(action));
  topGlow->runAction(CCRepeatForever::create(actionInverted));
  bottomGlow->setID("bottom-glow");
  topGlow->setID("top-glow");

  auto menu = CCMenu::create();
  menu->setID("main-menu");
  menu->setZOrder(10);
  addChildAtPosition(menu, Anchor::Center);

  auto backBtn = CCMenuItemSpriteExtra::create(
      CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png"), this,
      menu_selector(GDPSHubLayer::onGoBack));
  backBtn->setID("back-button");
  menu->addChildAtPosition(backBtn, Anchor::TopLeft, {25.f, -25.f});

  m_header = CCSprite::create("gdpshub-header.png"_spr);
  m_header->setScale(0.5f);
  m_header->setID("header"_spr);
  m_header->setAnchorPoint({0.5f,0.f});
  addChildAtPosition(m_header, Anchor::Top,{0,-(m_header->getScaledContentHeight()+10.f)});

  auto scrollBg = CCScale9Sprite::create("square02b_001.png", {0, 0, 80, 80});
  scrollBg->setColor({0, 0, 0});
  scrollBg->setOpacity(90);
  scrollBg->setZOrder(-1);
  scrollBg->setContentSize({380, 240});
  scrollBg->ignoreAnchorPointForPosition(false);
  scrollBg->setID("server-scroll-bg");

  auto scrollBg2 = CCScale9Sprite::create("GH_square01.png"_spr, {0, 0, 80, 80});
  scrollBg2->setContentSize({400, 260});
  scrollBg2->ignoreAnchorPointForPosition(false);
  scrollBg2->setID("server-scroll-bg");
  addChildAtPosition(scrollBg2, Anchor::Center,{0,-(m_header->getScaledContentHeight()-10.f)});

  m_serverList = CCClippingNode::create();
  m_serverList->setID("server-list");
  m_serverList->setContentSize({380, 240});
  m_serverList->setAnchorPoint({0.5, 0.5});
  m_serverList->setStencil(scrollBg);
  m_serverList->setAlphaThreshold(0.05f);
  m_scroll = ScrollLayer::create({365, 240});
  m_scroll->setID("server-scroll");
  m_scroll->ignoreAnchorPointForPosition(false);
  m_serverList->addChildAtPosition(m_scroll, Anchor::Center, {-5, 0});
  m_serverList->addChildAtPosition(scrollBg, Anchor::Center);
  addChildAtPosition(m_serverList, Anchor::Center, {0, -(m_header->getScaledContentHeight()-10.f)});

  m_scrollbar = Scrollbar::create(m_scroll);
  m_scrollbar->setID("scrollbar");
  m_scrollbar->setScaleY(.95f);
  m_serverList->addChildAtPosition(m_scrollbar, Anchor::Center, {184, 0});

  m_infoLabel = CCLabelBMFont::create("", "bigFont.fnt");
  m_infoLabel->setID("info-label");
  m_infoLabel->setAnchorPoint({0.5, 0.5});
  m_infoLabel->setScale(0.5);
  addChildAtPosition(m_infoLabel, Anchor::Center,{0,-(m_header->getScaledContentHeight()-10.f)});

  auto searchBtn = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("gj_findBtn_001.png"), this, menu_selector(GDPSHubLayer::onSearch));
  searchBtn->setID("search-btn");
  menu->addChildAtPosition(searchBtn, Anchor::Right, {-25, 70});

  auto leftArrowSpr =
      CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
  m_leftArrow = CCMenuItemSpriteExtra::create(
      leftArrowSpr, this, menu_selector(GDPSHubLayer::onLeftArrow));
  m_leftArrow->setVisible(false);
  m_leftArrow->setID("left-btn");
  menu->addChildAtPosition(m_leftArrow, Anchor::Left, {25.f, -(m_header->getScaledContentHeight()-10.f)});

  auto rightArrowSpr =
      CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
  rightArrowSpr->setFlipX(true);
  m_rightArrow = CCMenuItemSpriteExtra::create(
      rightArrowSpr, this, menu_selector(GDPSHubLayer::onRightArrow));
  m_rightArrow->setVisible(false);
  m_rightArrow->setID("right-btn");
  menu->addChildAtPosition(m_rightArrow, Anchor::Right, {-25.f, -(m_header->getScaledContentHeight()-10.f)});

  if (GDPSHub::get()->servers.size() == 0) {
    fetchServers();
  } else {
    updateList();
  }

  return true;
}



void GDPSHubLayer::keyBackClicked() { onGoBack(nullptr); }

void GDPSHubLayer::onGoBack(CCObject *) {
  GDPSHub::get()->servers = {};
  CCDirector::sharedDirector()->popSceneWithTransition(.5, PopTransition::kPopTransitionFade);
}

GDPSHubLayer *GDPSHubLayer::create() {
  auto ret = new GDPSHubLayer;
  if (ret->init()) {
    ret->autorelease();
    return ret;
  }
  CC_SAFE_DELETE(ret);
  return nullptr;
}

CCScene *GDPSHubLayer::scene() {
  auto scene = CCScene::create();
  scene->addChild(GDPSHubLayer::create());
  return scene;
}

void GDPSHubLayer::updateList() {
  m_scroll->m_contentLayer->removeAllChildren();

  // For neat little pad
  float totalHeight = 3.f;
  std::vector<PrivateServerNode *> rendered;
  for (Server server : GDPSHub::get()->servers) {
    auto node = PrivateServerNode::create(this, server,
                                          ccp(m_scroll->getContentWidth(), 80));
    node->setPosition(0, totalHeight);
    m_scroll->m_contentLayer->addChild(node);
    totalHeight += 83;
    rendered.push_back(node);
  }

  if (totalHeight < m_scroll->getContentSize().height) {
    totalHeight = m_scroll->getContentSize().height;
  }

  for (auto &node : rendered) {
    node->setPositionY(totalHeight - node->getPositionY() - 80);
  }

  m_scroll->m_contentLayer->setContentSize(
      {m_scroll->m_contentLayer->getContentWidth(), totalHeight});
  m_scroll->moveToTop();
  m_scrollbar->setVisible(true);
}

void GDPSHubLayer::fetchServers() {
  retain();
  m_scrollbar->setVisible(false);
  GDPSHub::get()->servers.clear();
  m_scroll->m_contentLayer->removeAllChildren();
  if (m_loadingCircle == nullptr) {
    m_loadingCircle = LoadingCircle::create();
  }
  m_loadingCircle->setParentLayer(this);
  m_loadingCircle->setPosition({0, -(m_header->getScaledContentHeight()-10.f)});
  m_loadingCircle->setScale(1.f);
  m_loadingCircle->show();
  m_infoLabel->setString("");
  m_fetching = true;

  m_listener.bind([this](web::WebTask::Event *e) {
    if (web::WebResponse *res = e->getValue()) {
      std::string err;
      auto opt = res->json();
      auto data = opt.unwrapOr("err");
      if (data == "err") {
        log::error("Failed to load servers: {}", err);
        m_infoLabel->setString("Something went wrong.");
        m_pages = 0;
        m_fetching = false;
        m_loadingCircle->fadeAndRemove();
        m_loadingCircle = nullptr;
        return;
      }
      if (!data.contains("success") || data["success"].asBool().unwrapOrDefault() == false ||
          !data.contains("data")) {
        if (data.contains("message")) {
          if (data["message"].asString().unwrapOrDefault() == "Invalid filter option." && m_queryType == "all" && m_search.empty()) {
            m_infoLabel->setString("No search specified.");
          } else {
            m_infoLabel->setString(data["message"].asString().unwrapOrDefault().c_str());
          }
        }
        else
          m_infoLabel->setString("Something went wrong.");
        m_pages = 0;
        m_fetching = false;
        m_loadingCircle->fadeAndRemove();
        m_loadingCircle = nullptr;
        return;
      }

      for (auto val : data["data"]) {
        auto server = val.as<Server>().unwrapOrDefault();
        GDPSHub::get()->servers.push_back(server);
      }
      updateList();
      if (data.contains("totalPages")) {
        m_pages = data["totalPages"].asInt().unwrapOrDefault();
      }

      m_leftArrow->setVisible(m_page > 1);
      m_rightArrow->setVisible(m_page < m_pages);

      m_loadingCircle->fadeAndRemove();
      m_loadingCircle = nullptr;
      m_fetching = false;
    }
  });
  auto urlEncode = [](const std::string &value) -> std::string {
    std::ostringstream escaped;
    escaped.fill('0');
    escaped << std::hex;

    for (char c : value) {
      if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
        escaped << c;
      } else {
        escaped << '%' << std::setw(2) << int((unsigned char)c);
      }
    }

    return escaped.str();
  };

  std::string safeSearch = urlEncode(m_search);
  auto req = web::WebRequest();
  m_listener.setFilter(req.get(
      fmt::format("https://api.gdpshub.com/geode/get?page={}&type={}&search={}",
                  m_page, m_queryType, safeSearch)));
}

void GDPSHubLayer::onLeftArrow(CCObject *) {
  if (m_fetching || m_page == 1)
    return;
  m_page--;
  fetchServers();
}

void GDPSHubLayer::onRightArrow(CCObject *) {
  if (m_fetching || m_page == m_pages)
    return;
  m_page++;
  fetchServers();
}

void GDPSHubLayer::onSearch(CCObject *) {
  if (m_fetching)
    return;
  PSSearchPopup::create(this)->show();
}
