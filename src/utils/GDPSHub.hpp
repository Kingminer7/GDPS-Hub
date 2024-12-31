#pragma once

#include "./Structs.hpp"
#include <km7dev.server_api/include/ServerAPIEvents.hpp>

using namespace geode::prelude;

class GDPSHub {
    protected:
        GDPSHub() {}
        static inline GDPSHub *instance = nullptr;
        int serverId = -1;
        Server *server;
    public:
        std::vector<Server> servers;
        bool fromLSL = false;
        static GDPSHub *get() {
            if (!instance) instance = new GDPSHub();
            return instance;
        }

        void beginPreview(Server server) {
            log::info("{}", server.url);
            if (this->serverId != -1) return;
            this->server = &server;
            this->serverId = ServerAPIEvents::registerServer(server.url, 5).id;
        }
        
        void endPreview() {
            if (this->serverId == -1) return;
            ServerAPIEvents::removeServer(this->serverId);
            this->serverId = -1;
            this->server = nullptr;
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