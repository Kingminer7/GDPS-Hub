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
  bool setup(GDPSHubLayer *layer) override {
    this->setTitle("Search");

    m_layer = layer;

    m_query = TextInput::create(220.f, "Search...");
    m_query->setPosition({0, 0});
    this->m_mainLayer->addChild(m_query);

    return true;
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
      CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png"), this,
      menu_selector(GDPSHubLayer::onGoBack));
  backBtn->setPosition(-winSize.width / 2 + 25.f, winSize.height / 2 - 25.f);
  backBtn->setID("back-button");
  menu->addChild(backBtn);
  this->addChild(menu);

  auto scrollBg = CCScale9Sprite::create("square02b_001.png", {0, 0, 80, 80});
  scrollBg->setColor({0, 0, 0});
  scrollBg->setOpacity(90);
  scrollBg->setContentSize({winSize.width - 100, winSize.height - 60});
  scrollBg->setPosition(winSize / 2);
  scrollBg->ignoreAnchorPointForPosition(false);
  scrollBg->setID("server-scroll-bg");
  this->addChild(scrollBg);

  scroll = ScrollLayer::create({winSize.width - 100, winSize.height - 60});
  scroll->setPosition((winSize - scroll->getContentSize()) / 2);
  scroll->setID("server-scroll");
  this->addChild(scroll);

  m_infoLabel = CCLabelBMFont::create("", "bigFont.fnt");
  m_infoLabel->setPosition(winSize / 2);
  m_infoLabel->setID("info-label");
  m_infoLabel->setAnchorPoint({0.5, 0.5});
  m_infoLabel->setScale(0.5);
  this->addChild(m_infoLabel);

  auto arrowMenu = CCMenu::create();
  arrowMenu->setID("nav-menu");
  arrowMenu->setZOrder(10);
  this->addChild(arrowMenu);

  auto searchBtn = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("gj_findBtn_001.png"), this, menu_selector(GDPSHubLayer::onSearch));
  searchBtn->setPosition(winSize.width / 2 - 25.f, 70);
  arrowMenu->addChild(searchBtn);

  auto leftArrowSpr =
      CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
  leftArrow = CCMenuItemSpriteExtra::create(
      leftArrowSpr, this, menu_selector(GDPSHubLayer::onLeftArrow));
  leftArrow->setPosition(-winSize.width / 2 + 25.f, 0);
  leftArrow->setVisible(false);
  arrowMenu->addChild(leftArrow);

  auto rightArrowSpr =
      CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
  rightArrowSpr->setFlipX(true);
  rightArrow = CCMenuItemSpriteExtra::create(
      rightArrowSpr, this, menu_selector(GDPSHubLayer::onRightArrow));
  rightArrow->setPosition(winSize.width / 2 - 25.f, 0);
  rightArrow->setVisible(false);
  arrowMenu->addChild(rightArrow);

  if (GDPSHub::get()->servers.size() == 0) {
    this->fetchServers();
  } else {
    this->updateList();
  }

  retain();

  return true;
}

void GDPSHubLayer::keyBackClicked() { this->onGoBack(nullptr); }

void GDPSHubLayer::onGoBack(CCObject *) {
  release();
  GDPSHub::get()->hubScene = nullptr;
  GDPSHub::get()->servers = {};
  CCDirector::get()->replaceScene(
      CCTransitionFade::create(0.5, MenuLayer::scene(false)));
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
  if (GDPSHub::get()->hubScene != nullptr)
    return GDPSHub::get()->hubScene;
  auto scene = CCScene::create();
  scene->addChild(GDPSHubLayer::create());
  GDPSHub::get()->hubScene = scene;
  scene->retain();
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
  GDPSHub::get()->servers.clear();
  scroll->m_contentLayer->removeAllChildren();
  if (m_loadingCircle == nullptr) {
    m_loadingCircle = LoadingCircle::create();
  }
  m_loadingCircle->setParentLayer(this);
  m_loadingCircle->setPosition({0, 0});
  m_loadingCircle->setScale(1.f);
  m_loadingCircle->show();
  fetching = true;

  m_listener.bind([this](web::WebTask::Event *e) {
    if (web::WebResponse *res = e->getValue()) {
      std::string err;
      auto opt = res->json();
      auto data = opt.value_or("err");
      if (data == "err") {
        log::info("{}", err);
        this->m_infoLabel->setString("Failed to fetch servers.");
        this->pages = 0;
        fetching = false;
        m_loadingCircle->fadeAndRemove();
        m_loadingCircle = nullptr;
        return;
      }
      if (!data.contains("success") || data["success"].as_bool() == false ||
          !data.contains("data")) {
        if (data.contains("message"))
          this->m_infoLabel->setString(data["message"].as_string().c_str());
        else
          this->m_infoLabel->setString("Failed to fetch servers.");
        this->pages = 0;
        fetching = false;
        m_loadingCircle->fadeAndRemove();
        m_loadingCircle = nullptr;
        return;
      }
      auto servers = data["data"].as_array();

      for (matjson::Value val : servers) {
        auto server = val.as<Server>();
        GDPSHub::get()->servers.push_back(server);
      }
      updateList();
      if (data.contains("totalPages")) {
        pages = data["totalPages"].as_int();
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
  PSSearchPopup::create(this)->show();
}