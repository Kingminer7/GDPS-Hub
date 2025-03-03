#pragma once
#include <Geode/Geode.hpp>

#include "./Structs.hpp"
#include <km7dev.server_api/include/ServerAPIEvents.hpp>

using namespace geode::prelude;

class GDPSHub {
    protected:
        GDPSHub() {}
        static inline GDPSHub *instance = nullptr;
        int serverId = -1;
        Server *server;
        bool overriding = false;
    public:
        std::vector<Server> servers;

        // Real account data
        int accId;
        std::string gjp;
        CCDictionary *onlineLevels = nullptr;
        CCDictionary *dailyLevels = nullptr;
        CCDictionary *downloadedLevels = nullptr;
        CCDictionary *gauntletLevels = nullptr;
        CCDictionary *savedGauntlets = nullptr;
        CCDictionary *likedLevels = nullptr;
        CCDictionary *ratedLevels = nullptr;
        CCDictionary *reportedLevels = nullptr;
        CCDictionary *storedLevels = nullptr;

        // Dummy data
        CCDictionary *onlineLevelsDummy = nullptr;
        CCDictionary *dailyLevelsDummy = nullptr;
        CCDictionary *downloadedLevelsDummy = nullptr;
        CCDictionary *gauntletLevelsDummy = nullptr;
        CCDictionary *savedGauntletsDummy = nullptr;
        CCDictionary *likedLevelsDummy = nullptr;
        CCDictionary *ratedLevelsDummy = nullptr;
        CCDictionary *reportedLevelsDummy = nullptr;
        CCDictionary *storedLevelsDummy = nullptr;

        static GDPSHub *get();

        void storeInfo(bool useExistingDummyInfo = false);
        void restoreInfo(bool saveDummyInfo = false);
        void beginPreview(Server server);
        void endPreview();
        void switchPreview(Server server);
        Server *getCurrentServer();
        bool isPreviewing();
        static std::string stampToDateTime(int stamp);
};