#include <Geode/ui/Popup.hpp>
#include "../utils/Structs.hpp"

using namespace geode::prelude;

class PrivateServerPopup : public Popup<Server> {
protected:
    bool setup(Server server) override {
        this->setTitle(server.title);

        auto desc = MDTextArea::create(server.description, ccp(200, 175));
        desc->setPosition(200, 150);
        this->addChild(desc);

        return true;
    }

public:
    static PrivateServerPopup* create(Server server) {
        auto ret = new PrivateServerPopup();
        if (ret->initAnchored(400.f, 300.f, server)) {
            ret->autorelease();
            return ret;
        }

        delete ret;
        return nullptr;
    }
};
