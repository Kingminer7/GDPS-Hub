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
    topOn->setVisible(m_layer->queryType == "top");
    topOn->setID("on-sprite");
    CCSprite *topOff = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
    topOff->setVisible(m_layer->queryType != "top");
    topOff->setID("off-sprite");
    topSel = CCMenuItemSpriteExtra::create(topOn, this, menu_selector(PSSearchPopup::changeQueryType));
    topSel->addChildAtPosition(topOff, Anchor::Center);
    topSel->setID("top");
    menu->addChildAtPosition(topSel, Anchor::Left, {25, 40});
    auto topLab = CCLabelBMFont::create("Top Servers","bigFont.fnt");
    topLab->setAnchorPoint({ 0, 0.5 });
    topLab->setScale(0.7);
    topLab->setID("top-label");
    menu->addChildAtPosition(topLab, Anchor::Left, {52, 40});

    // Recent Selection
    CCSprite *recentOn = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
    recentOn->setVisible(m_layer->queryType == "recent");
    recentOn->setID("on-sprite");
    CCSprite *recentOff = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
    recentOff->setVisible(m_layer->queryType != "recent");
    recentOff->setID("off-sprite");
    recentSel = CCMenuItemSpriteExtra::create(recentOn, this, menu_selector(PSSearchPopup::changeQueryType));
    recentSel->addChildAtPosition(recentOff, Anchor::Center);
    recentSel->setID("recent");
    menu->addChildAtPosition(recentSel, Anchor::Left, {25, 5});
    auto recentLab = CCLabelBMFont::create("Recently Added","bigFont.fnt");
    recentLab->setAnchorPoint({ 0, 0.5 });
    recentLab->setScale(0.7);
    recentLab->setID("recent-label");
    menu->addChildAtPosition(recentLab, Anchor::Left, {52, 5});

    // All Selection
    CCSprite *allOn = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
    allOn->setVisible(m_layer->queryType == "all");
    allOn->setID("on-sprite");
    CCSprite *allOff = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
    allOff->setVisible(m_layer->queryType != "all");
    allOff->setID("off-sprite");
    allSel = CCMenuItemSpriteExtra::create(allOn, this, menu_selector(PSSearchPopup::changeQueryType));
    allSel->addChildAtPosition(allOff, Anchor::Center);
    allSel->setID("all");
    menu->addChildAtPosition(allSel, Anchor::Left, {25, -30});
    auto allLab = CCLabelBMFont::create("All Versions (BETA)","bigFont.fnt");
    allLab->setAnchorPoint({ 0, 0.5 });
    allLab->setScale(0.7);
    allLab->setID("all-label");
    menu->addChildAtPosition(allLab, Anchor::Left, {52, -30});

    // Search Box
    m_query = TextInput::create(190.f, "Search");
    m_query->setWidth(250.f); // Configurable width for scalability
    m_query->setString(m_layer->search);
    m_query->setID("search-box");
    menu->addChildAtPosition(m_query, Anchor::Left, {144, -65});
    std::function<void(const std::string&)> func = [this](const std::string& str){
      m_layer->search = str;
      changed = true;
    };
    m_query->setCallback(func);
    m_query->setEnabled(true);

    m_mainLayer->addChildAtPosition(menu, Anchor::Center, {0, 0});
    return true;
  }

  void changeQueryType(CCObject * sender) {
    std::string id = static_cast<CCNode *>(sender)->getID();
    if (id == "search") return;
    if (id == "all") m_layer->queryType = "all";
    else if (id == "top") m_layer->queryType = "top";
    else if (id == "recent") m_layer->queryType = "recent";

    allSel->getChildByID("on-sprite")->setVisible(m_layer->queryType == "all");
    allSel->getChildByID("off-sprite")->setVisible(m_layer->queryType != "all");
    topSel->getChildByID("on-sprite")->setVisible(m_layer->queryType == "top");
    topSel->getChildByID("off-sprite")->setVisible(m_layer->queryType != "top");
    recentSel->getChildByID("on-sprite")->setVisible(m_layer->queryType == "recent");
    recentSel->getChildByID("off-sprite")->setVisible(m_layer->queryType != "recent");

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

  setID("gdps-hub-layer");

  setKeypadEnabled(true);

  auto background = CCSprite::create("bg.png"_spr);

  background->setScale(0.7f);
  background->setPosition({258,148});
  background->setZOrder(-1);
  background->setID("background");
  addChild(background);

  auto winSize = CCDirector::get()->getWinSize();

  auto menu = CCMenu::create();
  menu->setID("back-menu");
  menu->setZOrder(10);

  auto backBtn = CCMenuItemSpriteExtra::create(
      CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png"), this,
      menu_selector(GDPSHubLayer::onGoBack));
  backBtn->setPosition(-winSize.width / 2 + 25.f, winSize.height / 2 - 25.f);
  backBtn->setID("back-button");
  menu->addChild(backBtn);
  addChild(menu);

  auto scrollBg = CCScale9Sprite::create("square02b_001.png", {0, 0, 80, 80});
  scrollBg->setColor({0, 0, 0});
  scrollBg->setOpacity(90);
  scrollBg->setContentSize({winSize.width - 100, winSize.height - 60});
  scrollBg->setPosition(winSize / 2);
  scrollBg->ignoreAnchorPointForPosition(false);
  scrollBg->setID("server-scroll-bg");
  addChild(scrollBg);

  scroll = ScrollLayer::create({winSize.width - 100, winSize.height - 60});
  scroll->setPosition((winSize - scroll->getContentSize()) / 2);
  scroll->setID("server-scroll");
  addChild(scroll);

  m_infoLabel = CCLabelBMFont::create("", "bigFont.fnt");
  m_infoLabel->setPosition(winSize / 2);
  m_infoLabel->setID("info-label");
  m_infoLabel->setAnchorPoint({0.5, 0.5});
  m_infoLabel->setScale(0.5);
  addChild(m_infoLabel);

  auto arrowMenu = CCMenu::create();
  arrowMenu->setID("nav-menu");
  arrowMenu->setZOrder(10);
  addChild(arrowMenu);

  auto searchBtn = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("gj_findBtn_001.png"), this, menu_selector(GDPSHubLayer::onSearch));
  searchBtn->setPosition(winSize.width / 2 - 25.f, 70);
  searchBtn->setID("search-btn");
  arrowMenu->addChild(searchBtn);

  auto leftArrowSpr =
      CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
  leftArrow = CCMenuItemSpriteExtra::create(
      leftArrowSpr, this, menu_selector(GDPSHubLayer::onLeftArrow));
  leftArrow->setPosition(-winSize.width / 2 + 25.f, 0);
  leftArrow->setVisible(false);
  leftArrow->setID("left-btn");
  arrowMenu->addChild(leftArrow);

  auto rightArrowSpr =
      CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
  rightArrowSpr->setFlipX(true);
  rightArrow = CCMenuItemSpriteExtra::create(
      rightArrowSpr, this, menu_selector(GDPSHubLayer::onRightArrow));
  rightArrow->setPosition(winSize.width / 2 - 25.f, 0);
  rightArrow->setVisible(false);
  rightArrow->setID("right-btn");
  arrowMenu->addChild(rightArrow);

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
  scroll->m_contentLayer->removeAllChildren();
  scroll->moveToTop();

  float totalHeight = 0.f;
  std::vector<PrivateServerNode *> rendered;
  for (Server server : GDPSHub::get()->servers) {
    auto node = PrivateServerNode::create(this, server,
                                          ccp(scroll->getContentWidth(), 80));
    node->setPosition(0, totalHeight);
    scroll->m_contentLayer->addChild(node);
    totalHeight += 80;
    rendered.push_back(node);
  }

  if (totalHeight < scroll->getContentSize().height) {
    totalHeight = scroll->getContentSize().height;
  }

  for (auto &node : rendered) {
    node->setPositionY(totalHeight - node->getPositionY() - 80);
  }

  scroll->m_contentLayer->setContentSize(
      {scroll->m_contentLayer->getContentWidth(), totalHeight});
  scroll->moveToTop();
}

void GDPSHubLayer::fetchServers() {
  retain();
  GDPSHub::get()->servers.clear();
  scroll->m_contentLayer->removeAllChildren();
  if (m_loadingCircle == nullptr) {
    m_loadingCircle = LoadingCircle::create();
  }
  m_loadingCircle->setParentLayer(this);
  m_loadingCircle->setPosition({0, 0});
  m_loadingCircle->setScale(1.f);
  m_loadingCircle->show();
  m_infoLabel->setString("");
  fetching = true;

  m_listener.bind([this](web::WebTask::Event *e) {
    if (web::WebResponse *res = e->getValue()) {
      std::string err;
      auto opt = res->json();
      auto data = opt.unwrapOr("err");
      if (data == "err") {
        log::info("{}", err);
        m_infoLabel->setString("Failed to fetch servers.");
        pages = 0;
        fetching = false;
        m_loadingCircle->fadeAndRemove();
        m_loadingCircle = nullptr;
        return;
      }
      if (!data.contains("success") || data["success"].asBool().unwrapOrDefault() == false ||
          !data.contains("data")) {
        if (data.contains("message"))
          m_infoLabel->setString(data["message"].asString().unwrapOrDefault().c_str());
        else
          m_infoLabel->setString("Failed to fetch servers.");
        pages = 0;
        fetching = false;
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
        pages = data["totalPages"].asInt().unwrapOrDefault();
      }

      leftArrow->setVisible(page > 1);
      rightArrow->setVisible(page < pages);

      m_loadingCircle->fadeAndRemove();
      m_loadingCircle = nullptr;
      fetching = false;
    }
  });
  auto req = web::WebRequest();
  m_listener.setFilter(req.get(
      fmt::format("https://api.gdpshub.com/geode/get?page={}&type={}&search={}",
                  page, queryType, search)));
}

void GDPSHubLayer::onLeftArrow(CCObject *) {
  if (fetching || page == 1)
    return;
  page--;
  fetchServers();
}

void GDPSHubLayer::onRightArrow(CCObject *) {
  if (fetching || page == pages)
    return;
  page++;
  fetchServers();
}

void GDPSHubLayer::onSearch(CCObject *) {
  if (fetching)
    return;
  PSSearchPopup::create(this)->show();
}