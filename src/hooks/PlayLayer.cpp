#include <Geode/modify/PlayLayer.hpp>
#include "../utils/GDPSHub.hpp"

using namespace geode::prelude;

class $modify (PlayLayer) {

    void levelComplete() {
        if (GDPSHub::get()->isPreviewing()) {
            bool was = m_isTestMode;
            m_isTestMode = true;
            PlayLayer::levelComplete();
            m_isTestMode = was;
        } else 
            PlayLayer::levelComplete();
    }

    void destroyPlayer(PlayerObject* player, GameObject* object) {
        if (GDPSHub::get()->isPreviewing()) {
            bool was = m_isTestMode;
            m_isTestMode = true;
            PlayLayer::destroyPlayer(player, object);
            m_isTestMode = was;
        } else 
            PlayLayer::destroyPlayer(player, object);
    }

    void resetLevel() {
        if (GDPSHub::get()->isPreviewing()) {
            bool was = m_isTestMode;
            m_isTestMode = true;
            PlayLayer::resetLevel();
            m_isTestMode = was;
        } else 
            PlayLayer::resetLevel();
    }
};