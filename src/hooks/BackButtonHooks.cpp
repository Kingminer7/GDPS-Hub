#include "../ui/PSCreatorLayer.hpp"
#include "../utils/GDPSHub.hpp"
#include <Geode/modify/GauntletSelectLayer.hpp>
#include <Geode/modify/LeaderboardsLayer.hpp>
#include <Geode/modify/LevelBrowserLayer.hpp>
#include <Geode/modify/LevelSearchLayer.hpp>

using namespace geode::prelude;

class $modify(GauntletSelectLayer){void onBack(CCObject * sender){
    if (GDPSHub::get()->isPreviewing() == true){CCDirector::get()->replaceScene(
        CCTransitionFade::create(0.5, PSCreatorLayer::scene()));
return;
}
GauntletSelectLayer::onBack(sender);
return;
}
}
;

class $modify(LevelBrowserLayer){void onBack(CCObject * sender){
    log::info("{}", static_cast<int>(this->m_searchObject->m_searchType));
if (GDPSHub::get()->isPreviewing() == true &&
    // for whatever reason this doesnt work
    // this->m_searchObject->m_searchType != SearchType::Search &&

    GDPSHub::get()->fromLSL == false &&
    this->m_searchObject->m_searchType != SearchType::MapPackOnClick) {
  CCDirector::get()->replaceScene(
      CCTransitionFade::create(0.5, PSCreatorLayer::scene()));
  return;
}
LevelBrowserLayer::onBack(sender);
return;
}
}
;

class $modify(LeaderboardsLayer){void onBack(
    CCObject * sender){if (GDPSHub::get()->isPreviewing() == true){
    // GameManager::get()->safePopScene(); // maybe some day ill figure it out
    CCDirector::get()->replaceScene(
        CCTransitionFade::create(0.5, PSCreatorLayer::scene()));
return;
}
LeaderboardsLayer::onBack(sender);
return;
}
}
;

class $modify(LevelSearchLayer){void onBack(CCObject * sender){
    if (GDPSHub::get()->isPreviewing() == true){GDPSHub::get()->fromLSL = false;
        CCDirector::get()->replaceScene(
        CCTransitionFade::create(0.5, PSCreatorLayer::scene()));
return;
}
LevelSearchLayer::onBack(sender);
return;
}
}
;

#ifdef GEODE_IS_MACOS

class $modify(LevelSearchLayer){void keyBackClicked(){
    if (GDPSHub::get()->isPreviewing() == true){CCDirector::get()->replaceScene(
        CCTransitionFade::create(0.5, PSCreatorLayer::scene()));
return;
}
LevelSearchLayer::keyBackClicked();
return;
}
}
;

#endif
