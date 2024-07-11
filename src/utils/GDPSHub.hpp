#include "./Structs.hpp"

using namespace geode::prelude;

class GDPSHub {
    protected:
        GDPSHub() {}
        static inline GDPSHub *instance = nullptr;
        Server currentServer;
        bool isPreviewing;
        std::string apiUrl = "https://api.gdpshub.com/gdps/geode";
    public:
        static GDPSHub *get() {
            if (!instance) instance = new GDPSHub();
            return instance;
        }

        void beginPreview(Server server) {
            this->currentServer = server;
            this->isPreviewing = true;
        }
        
        void endPreview() {
            this->isPreviewing = false;
        }

        void switchPreview(Server server) {
            this->currentServer = server;
        }

        Server getCurrentServer() {
            return this->currentServer;
        }

        bool isPreviewing() {
            return this->isPreviewing;
        }
};