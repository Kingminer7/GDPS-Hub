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

class $modify(LeaderboardsLayer) {
    void onBack(CCObject* sender) {
        if (GDPSHub::get()->isPreviewing() == true) {
            // GameManager::get()->safePopScene(); // maybe some day ill figure it out
            auto scene = CCScene::create();
            scene->addChild(PSCreatorLayer::create());
            CCDirector::get()->replaceScene(CCTransitionFade::create(0.5, scene));
            return;
        }
        LeaderboardsLayer::onBack(sender);
        return;
    }
};

// class $modify(LevelSearchLayer) {
//     void onBack(CCObject* sender) {
//         if (GDPSHub::get()->isPreviewing() == true) {
//             auto scene = CCScene::create();
//             scene->addChild(PSCreatorLayer::create());
//             CCDirector::get()->replaceScene(CCTransitionFade::create(0.5, scene));
//             return;
//         }
//         LevelSearchLayer::onBack(sender);
//         return;
//     }
// };