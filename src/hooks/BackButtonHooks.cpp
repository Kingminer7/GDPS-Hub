#include <Geode/modify/GauntletSelectLayer.hpp>
#include <Geode/modify/LevelSearchLayer.hpp>
#include <Geode/modify/LeaderboardsLayer.hpp>
#include <Geode/modify/LevelBrowserLayer.hpp>
#include "../utils/GDPSHub.hpp"
#include "../ui/PSCreatorLayer.hpp"

using namespace geode::prelude;

class $modify(GauntletSelectLayer) {
    void onBack(CCObject* sender) {
        if (GDPSHub::get()->isPreviewing() == true) {
            CCDirector::get()->replaceScene(CCTransitionFade::create(0.5, PSCreatorLayer::scene()));
            return;
        }
        GauntletSelectLayer::onBack(sender);
        return;
    }
};

class $modify(LevelBrowserLayer) {
    void onBack(CCObject* sender) {
        if (GDPSHub::get()->isPreviewing() == true && GDPSHub::get()->fromLSL == false) {
            CCDirector::get()->replaceScene(CCTransitionFade::create(0.5, PSCreatorLayer::scene()));
            return;
        }
        LevelBrowserLayer::onBack(sender);
        return;
    }
};

class $modify(LeaderboardsLayer) {
    void onBack(CCObject* sender) {
        if (GDPSHub::get()->isPreviewing() == true) {
            // GameManager::get()->safePopScene(); // maybe some day ill figure it out
            CCDirector::get()->replaceScene(CCTransitionFade::create(0.5, PSCreatorLayer::scene()));
            return;
        }
        LeaderboardsLayer::onBack(sender);
        return;
    }
};

class $modify(LevelSearchLayer) {
    void onBack(CCObject* sender) {
        if (GDPSHub::get()->isPreviewing() == true) {
            CCDirector::get()->replaceScene(CCTransitionFade::create(0.5, PSCreatorLayer::scene()));
            return;
        }
        LevelSearchLayer::onBack(sender);
        return;
    }
};

#ifdef GEODE_IS_MACOS

class $modify(LevelSearchLayer) {
    void keyBackClicked() {
        if (GDPSHub::get()->isPreviewing() == true) {
            CCDirector::get()->replaceScene(CCTransitionFade::create(0.5, PSCreatorLayer::scene()));
            return;
        }
        LevelSearchLayer::keyBackClicked();
        return;
    }
};

#endif
