#include <Geode/modify/PlayLayer.hpp>
#include "../utils/GDPSHub.hpp"

using namespace geode::prelude;

class $modify (PlayLayer) {
    static void onModify(auto& self) {
        // i dont want your stupid warning so have an "auto hook = "
        auto hook = self.setHookPriority("PlayLayer::levelComplete", 10000);
    }

    void levelComplete() {
        if (GDPSHub::get()->isPreviewing()) {
            m_isTestMode = true;
            // Notification::create("You are previewing this server. Data won't save.", NotificationIcon::Info, 1.5f)->show();
        }
        PlayLayer::levelComplete();
    }

    void destroyPlayer(PlayerObject* player, GameObject* object) {
        if (GDPSHub::get()->isPreviewing()) {
            m_isTestMode = true;
            // Notification::create("You are previewing this server. Data won't save.", NotificationIcon::Info, 1.5f)->show();
        }
        PlayLayer::destroyPlayer(player, object);
    }

    void resetLevel() {
        if (GDPSHub::get()->isPreviewing()) {
            m_isTestMode = true;
          //  Notification::create("You are previewing this server. Data won't save.", NotificationIcon::Info, 1.5f)->show();
        }
        PlayLayer::resetLevel();
    }
};