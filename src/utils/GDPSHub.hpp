#pragma once

#include "./Structs.hpp"
#include <km7dev.server_api/include/ServerAPI.hpp>

using namespace geode::prelude;

class GDPSHub {
    protected:
        GDPSHub() {}
        static inline GDPSHub *instance = nullptr;
        int serverId = -1;
        Server *server;
        std::string apiUrl = "https://api.gdpshub.com/gdps/geode";
    public:
        std::vector<Server> servers;
        static GDPSHub *get() {
            if (!instance) instance = new GDPSHub();
            return instance;
        }

        void beginPreview(Server server) {
            log::info("{}", server.url);
            if (this->serverId != -1) return;
            this->server = &server;
            this->serverId = ServerAPI::get()->registerURL(server.url, 5);
        }
        
        void endPreview() {
            if (this->serverId == -1) return;
            ServerAPI::get()->removeURL(this->serverId);
            this->serverId = -1;
            this->server = nullptr;
        }

        void switchPreview(Server server) {
            if (this->serverId == -1) return;
            this->server = &server;
            ServerAPI::get()->updateURL(this->serverId, server.url);
        }

        Server *getCurrentServer() {
            if (this->serverId == -1) return nullptr;
            
            return this->server;
        }

        bool isPreviewing() {
            return this->serverId != -1;
        }
};