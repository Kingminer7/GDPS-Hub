class GDPSHubInstance {
    protected:
        std::string currentServer;
        bool isPreviewing;
        std::string apiUrl = "https://gdps.hub/api/";
    public:
        GDPSHubInstance() {}

        void beginPreview(std::string server) {
            this->currentServer = server;
            this->isPreviewing = true;
        }
        
        void endPreview() {
            this->isPreviewing = false;
        }

        void switchPreview(std::string server) {
            this->currentServer = server;
        }

        std::string getCurrentServer() {
            return this->currentServer;
        }

        bool isPreviewing() {
            return this->isPreviewing;
        }
};