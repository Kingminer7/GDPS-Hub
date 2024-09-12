#include <Geode/modify/GauntletSelectLayer.hpp>
#include <Geode/modify/LevelBrowserLayer.hpp>
#include "../utils/GDPSHub.hpp"
#include "../ui/PSCreatorLayer.hpp"

using namespace geode::prelude;

class $modify(GauntletSelectLayer) {
    void onBack(CCObject* sender) {
        if (GDPSHub::get()->isPreviewing() == true) {
            auto scene = CCScene::create();
            scene->addChild(PSCreatorLayer::create());
            CCDirector::get()->replaceScene(CCTransitionFade::create(0.5, scene));
            return;
        }
        GauntletSelectLayer::onBack(sender);
        return;
    }
};

class $modify(LevelBrowserLayer) {
    void onBack(CCObject* sender) {
        if (GDPSHub::get()->isPreviewing() == true) {
            auto scene = CCScene::create();
            scene->addChild(PSCreatorLayer::create());
            CCDirector::get()->replaceScene(CCTransitionFade::create(0.5, scene));
            return;
        }
        LevelBrowserLayer::onBack(sender);
        return;
    }
};