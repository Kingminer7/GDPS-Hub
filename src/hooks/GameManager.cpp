#include "../utils/GDPSHub.hpp"

using namespace geode::prelude;

#include <Geode/modify/GManager.hpp>
class $modify(GManager) {
	void save() {
        if (!typeinfo_cast<GameManager*>(this)) {
            GManager::save();
            return;
        }
        if (GDPSHub::get()->isPreviewing()) {
            GDPSHub::get()->restoreInfo(true);
        }
		GManager::save();
        if (GDPSHub::get()->isPreviewing()) {
            GDPSHub::get()->storeInfo(true);
        }
	}
};
