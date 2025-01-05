#pragma once

#include "./Structs.hpp"
#include "Geode/binding/GameLevelManager.hpp"
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
        CCDictionary *onlineLevels;
        CCDictionary *dailyLevels;
        CCDictionary *downloadedLevels;
        CCDictionary *gauntletLevels;
        CCDictionary *likedLevels;
        CCDictionary *ratedLevels;
        CCDictionary *reportedLevels;
        CCDictionary *storedLevels;

        // Dummy data
        CCDictionary *onlineLevelsDummy;
        CCDictionary *dailyLevelsDummy;
        CCDictionary *downloadedLevelsDummy;
        CCDictionary *gauntletLevelsDummy;
        CCDictionary *likedLevelsDummy;
        CCDictionary *ratedLevelsDummy;
        CCDictionary *reportedLevelsDummy;
        CCDictionary *storedLevelsDummy;

        static GDPSHub *get() {
            if (!instance) instance = new GDPSHub();
            return instance;
        }

        void storeInfo(bool useExistingDummyInfo = false) {
            if (!useExistingDummyInfo) {
                onlineLevelsDummy = new CCDictionary();
                onlineLevelsDummy->retain();
                dailyLevelsDummy = new CCDictionary();
                dailyLevelsDummy->retain();
                downloadedLevelsDummy = new CCDictionary();
                downloadedLevelsDummy->retain();
                gauntletLevelsDummy = new CCDictionary();
                gauntletLevelsDummy->retain();
                likedLevelsDummy = new CCDictionary();
                likedLevelsDummy->retain();
                ratedLevelsDummy = new CCDictionary();
                ratedLevelsDummy->retain();
                reportedLevelsDummy = new CCDictionary();
                reportedLevelsDummy->retain();
                storedLevelsDummy = new CCDictionary();
                storedLevelsDummy->retain();
            }
            
            accId = GJAccountManager::get()->m_accountID;
            GJAccountManager::get()->m_accountID = 0;

            gjp = GJAccountManager::get()->m_GJP2;
            GJAccountManager::get()->m_GJP2 = "";

            onlineLevels = GameLevelManager::get()->m_onlineLevels;
            GameLevelManager::get()->m_onlineLevels = onlineLevelsDummy;
            
            dailyLevels = GameLevelManager::get()->m_dailyLevels;
            GameLevelManager::get()->m_dailyLevels = dailyLevelsDummy;

            downloadedLevels = GameLevelManager::get()->m_downloadedLevels;
            GameLevelManager::get()->m_downloadedLevels = downloadedLevelsDummy;

            gauntletLevels = GameLevelManager::get()->m_gauntletLevels;
            GameLevelManager::get()->m_gauntletLevels = gauntletLevelsDummy;

            likedLevels = GameLevelManager::get()->m_likedLevels;
            GameLevelManager::get()->m_likedLevels = likedLevelsDummy;

            ratedLevels = GameLevelManager::get()->m_ratedLevels;
            GameLevelManager::get()->m_ratedLevels = ratedLevelsDummy;

            reportedLevels = GameLevelManager::get()->m_reportedLevels;
            GameLevelManager::get()->m_reportedLevels = reportedLevelsDummy;

            storedLevels = GameLevelManager::get()->m_storedLevels;
            GameLevelManager::get()->m_storedLevels = storedLevelsDummy;
        }

        void restoreInfo(bool saveDummyInfo = false) {
            GJAccountManager::get()->m_accountID = accId;
            GJAccountManager::get()->m_GJP2 = gjp;
            GameLevelManager::get()->m_onlineLevels = onlineLevels;
            GameLevelManager::get()->m_dailyLevels = dailyLevels;
            GameLevelManager::get()->m_downloadedLevels = downloadedLevels;
            GameLevelManager::get()->m_gauntletLevels = gauntletLevels;
            GameLevelManager::get()->m_likedLevels = likedLevels;
            GameLevelManager::get()->m_ratedLevels = ratedLevels;
            GameLevelManager::get()->m_reportedLevels = reportedLevels;
            GameLevelManager::get()->m_storedLevels = storedLevels;

            if (!saveDummyInfo) {
                onlineLevelsDummy->release();
                dailyLevelsDummy->release();
                downloadedLevelsDummy->release();
                gauntletLevelsDummy->release();
                likedLevelsDummy->release();
                ratedLevelsDummy->release();
                reportedLevelsDummy->release();
                storedLevelsDummy->release();
            }
        }

        void beginPreview(Server server) {
            if (this->serverId != -1) return;
            this->server = &server;
            this->serverId = ServerAPIEvents::registerServer(server.url, 5).id;
            if (!this->overriding) {
                overriding = true;
                storeInfo();
            }
        }
        
        void endPreview() {
            if (this->serverId == -1) return;
            ServerAPIEvents::removeServer(this->serverId);
            this->serverId = -1;
            this->server = nullptr;
            if (this->overriding) {
                overriding = false;
                restoreInfo();
            }
        }

        void switchPreview(Server server) {
            if (this->serverId == -1) return;
            this->server = &server;
            ServerAPIEvents::updateServer(this->serverId, server.url);
        }

        Server *getCurrentServer() {
            if (this->serverId == -1) return nullptr;
            
            return this->server;
        }

        bool isPreviewing() {
            return this->serverId != -1;
        }
        
        static std::string stampToDateTime(int stamp)
        {
            time_t t = stamp;
            tm *tm = localtime(&t);
            char buf[80];
            strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tm);
            return buf;
        }
};