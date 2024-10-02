#include <Geode/ui/BasedButtonSprite.hpp>
#include "../ui/GDPSHubLayer.hpp"

using namespace geode::prelude;

#include <Geode/modify/MenuLayer.hpp>
class $modify(menuLayer, MenuLayer) {
	bool init() {
		if (!MenuLayer::init()) {
			return false;
		}

		auto gdpsHubBtn = CCMenuItemSpriteExtra::create(
			CircleButtonSprite::createWithSpriteFrameName("gdps_hub.png"_spr, .8, CircleBaseColor::Blue, CircleBaseSize::MediumAlt),
			this,
			menu_selector(menuLayer::onGdpsHub)
		);

		auto menu = this->getChildByID("bottom-menu");
		menu->addChild(gdpsHubBtn);

		gdpsHubBtn->setID("gdps-hub-btn"_spr);

		menu->updateLayout();

		return true;
	}

	void onGdpsHub(CCObject*) {
		CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5, GDPSHubLayer::scene()));
	}
};
