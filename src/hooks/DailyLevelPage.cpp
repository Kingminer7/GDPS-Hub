#include "../utils/GDPSHub.hpp"
#include "Geode/cocos/CCDirector.h"

using namespace geode::prelude;

#include <Geode/modify/DailyLevelPage.hpp>
class $modify(dLPage, DailyLevelPage) {
	void onTheSafe(CCObject* h) {
		if (DailyLevelPage::m_type != GJTimedLevelType::Event) {
			DailyLevelPage::onTheSafe(h);
		} else {
			auto lbl = LevelBrowserLayer::scene(GJSearchObject::create(SearchType::EventSafe));
			CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5, lbl));
		}
	}

	void show() {
		if (DailyLevelPage::m_type == GJTimedLevelType::Event) {
			if (GDPSHub::get()->isPreviewing()) {
				auto label = CCLabelBMFont::create("If this doesn't load, there may not be an event level.", "bigFont.fnt");
				label->setScale(0.5);
				label->setID("warning-label"_spr);
				label->setPosition({CCDirector::get()->getWinSize().width / 2, label->getScaledContentHeight() / 2});
				DailyLevelPage::m_mainLayer->addChild(label);
			}
		}
		DailyLevelPage::show();
	}
};