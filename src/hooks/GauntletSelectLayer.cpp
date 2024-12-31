#include "../utils/GDPSHub.hpp"
#include <Geode/modify/GauntletSelectLayer.hpp>

using namespace geode::prelude;

// robtop why do you not use popscene for this specific button >:(
class $modify(GauntletSelectLayer){
    void onBack(CCObject * sender){
        if (GDPSHub::get()->isPreviewing() == true){
            CCDirector::sharedDirector()->popSceneWithTransition(.5, PopTransition::kPopTransitionFade);
            return;
        }
        GauntletSelectLayer::onBack(sender);
    }
};