#include "GDPSHubLayer.hpp"
#include "../utils/GDPSHub.hpp"
#include "../utils/Structs.hpp"
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
  CCMenuItemSpriteExtra *allSel;
  CCMenuItemSpriteExtra *searchSel;
  bool changed = false;
  bool setup(GDPSHubLayer *layer) override {
    setTitle("Query Options");
    setID("search-popup"_spr);
    m_title->setID("title");
    m_title->setPositionY(135);
    m_buttonMenu->setID("back-menu");
    m_closeBtn->setID("close-btn");
    m_mainLayer->setID("main-layer");
    m_bgSprite->setID("background");
    m_layer = layer;

    auto menu = CCMenu::create();
    menu->setContentSize({250, 150});
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
    menu->addChildAtPosition(topSel, Anchor::Left, {25, 35});
    auto topLab = CCLabelBMFont::create("Top Servers","bigFont.fnt");
    topLab->setAnchorPoint({ 0, 0.5 });
    topLab->setScale(0.575);
    topLab->setID("top-label");
    menu->addChildAtPosition(topLab, Anchor::Left, {40, 35});

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
    menu->addChildAtPosition(recentSel, Anchor::Left, {25, 8});
    auto recentLab = CCLabelBMFont::create("Recently Added","bigFont.fnt");
    recentLab->setAnchorPoint({ 0, 0.5 });
    recentLab->setScale(0.575);
    recentLab->setID("recent-label");
    menu->addChildAtPosition(recentLab, Anchor::Left, {40, 8});

    // All Selection
    CCSprite *allOn = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
    allOn->setVisible(m_layer->m_queryType == "all");
    allOn->setID("on-sprite");
    CCSprite *allOff = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
    allOff->setVisible(m_layer->m_queryType != "all");
    allOff->setID("off-sprite");
    allSel = CCMenuItemSpriteExtra::create(allOn, this, menu_selector(PSSearchPopup::changeQueryType));
    allSel->addChildAtPosition(allOff, Anchor::Center);
    allSel->setID("all");
    allSel->m_baseScale = 0.75f;
    allSel->setScale(0.75f);
    menu->addChildAtPosition(allSel, Anchor::Left, {25, -19});
    auto allLab = CCLabelBMFont::create("Search","bigFont.fnt");
    allLab->setAnchorPoint({ 0, 0.5 });
    allLab->setScale(0.575);
    allLab->setID("all-label");
    menu->addChildAtPosition(allLab, Anchor::Left, {40, -19});

    // Search Box
    m_query = TextInput::create(250.f, "Search");
    m_query->setScale(.85); 
    m_query->setString(m_layer->m_search);
    m_query->setID("search-box");
    menu->addChildAtPosition(m_query, Anchor::Bottom, {0, 25});
    m_query->getInputNode()->setAllowedChars("`1234567890-=qwertyuiop[]\\asdfghjkl;'cxzvbnm,./~!@#$%^&*()_+QWERTYUIOP{}|ASDFGHJKL:\"ZXCVBNM<>? ");
    m_query->setCallback([this](const std::string& str){
      m_layer->m_search = str;
      changed = true;
    });
    m_query->setVisible(m_layer->m_queryType == "all");

    m_mainLayer->addChildAtPosition(menu, Anchor::Center, {0, 0});
    return true;
  }

  void changeQueryType(CCObject * sender) {
    std::string id = static_cast<CCNode *>(sender)->getID();
    if (id == "search") return;
    if (id == "all") m_layer->m_queryType = "all";
    else if (id == "top") m_layer->m_queryType = "top";
    else if (id == "recent") m_layer->m_queryType = "recent";

    allSel->getChildByID("on-sprite")->setVisible(m_layer->m_queryType == "all");
    allSel->getChildByID("off-sprite")->setVisible(m_layer->m_queryType != "all");
    topSel->getChildByID("on-sprite")->setVisible(m_layer->m_queryType == "top");
    topSel->getChildByID("off-sprite")->setVisible(m_layer->m_queryType != "top");
    recentSel->getChildByID("on-sprite")->setVisible(m_layer->m_queryType == "recent");
    recentSel->getChildByID("off-sprite")->setVisible(m_layer->m_queryType != "recent");
    m_query->setVisible(m_layer->m_queryType == "all");

    changed = true;
  }

  void onClose(CCObject * obj) override {
    if (changed) m_layer->fetchServers();
    Popup::onClose(obj);
  }

public:
  static PSSearchPopup *create(GDPSHubLayer *layer) {
    auto ret = new PSSearchPopup();
    if (ret->initAnchored(250.f, 150.f, layer)) {
      ret->autorelease();
      return ret;
    }

    delete ret;
    return nullptr;
  }
};


bool GDPSHubLayer::init() {
  if (!CCLayer::init())
    return false;

  setAnchorPoint({0, 0});

  setID("gdps-hub-layer"_spr);

  setKeypadEnabled(true);

  auto winSize = CCDirector::get()->getWinSize();

  auto background = CCSprite::create("bg.png"_spr);
  background->setScale(std::clamp(winSize.width / background->getContentWidth(), .75f, FLT_MAX));
  background->setZOrder(-1);
  background->setID("background");
  addChildAtPosition(background, Anchor::Center);

  auto menu = CCMenu::create();
  menu->setID("main-menu");
  menu->setZOrder(10);
  addChildAtPosition(menu, Anchor::Center);

  auto backBtn = CCMenuItemSpriteExtra::create(
      CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png"), this,
      menu_selector(GDPSHubLayer::onGoBack));
  backBtn->setID("back-button");
  menu->addChildAtPosition(backBtn, Anchor::TopLeft, {25.f, -25.f});

  auto scrollBg = CCScale9Sprite::create("square02b_001.png", {0, 0, 80, 80});
  scrollBg->setColor({0, 0, 0});
  scrollBg->setOpacity(90);
  scrollBg->setContentSize({380, 240});
  scrollBg->ignoreAnchorPointForPosition(false);
  scrollBg->setID("server-scroll-bg");
  addChildAtPosition(scrollBg, Anchor::Center);

  m_scroll = ScrollLayer::create({340, 240});
  m_scroll->setID("server-scroll");
  addChildAtPosition(m_scroll, Anchor::Center, -m_scroll->getContentSize() / 2);

  m_scrollbar = Scrollbar::create(m_scroll);
  m_scrollbar->setID("scrollbar");
  m_scrollbar->setScaleY(.95f);
  addChildAtPosition(m_scrollbar, Anchor::Center, {180, 0});

  m_infoLabel = CCLabelBMFont::create("", "bigFont.fnt");
  m_infoLabel->setID("info-label");
  m_infoLabel->setAnchorPoint({0.5, 0.5});
  m_infoLabel->setScale(0.5);
  addChildAtPosition(m_infoLabel, Anchor::Center);

  auto searchBtn = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("gj_findBtn_001.png"), this, menu_selector(GDPSHubLayer::onSearch));
  searchBtn->setID("search-btn");
  menu->addChildAtPosition(searchBtn, Anchor::Right, {-25, 70});

  auto leftArrowSpr =
      CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
  m_leftArrow = CCMenuItemSpriteExtra::create(
      leftArrowSpr, this, menu_selector(GDPSHubLayer::onLeftArrow));
  m_leftArrow->setVisible(false);
  m_leftArrow->setID("left-btn");
  menu->addChildAtPosition(m_leftArrow, Anchor::Left, {25.f, 0});

  auto rightArrowSpr =
      CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
  rightArrowSpr->setFlipX(true);
  m_rightArrow = CCMenuItemSpriteExtra::create(
      rightArrowSpr, this, menu_selector(GDPSHubLayer::onRightArrow));
  m_rightArrow->setVisible(false);
  m_rightArrow->setID("right-btn");
  menu->addChildAtPosition(m_rightArrow, Anchor::Right, {-25.f, 0});

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
  m_loadingCircle->setPosition({0, 0});
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
        log::info("{}", err);
        m_infoLabel->setString("Failed to fetch servers.");
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
          m_infoLabel->setString("Failed to fetch servers.");
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
