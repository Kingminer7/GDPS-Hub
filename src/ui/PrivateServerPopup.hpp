#include <Geode/ui/Popup.hpp>
#include "../utils/Structs.hpp"

using namespace geode::prelude;

class PrivateServerPopup : public Popup<Server> {
protected:
    bool setup(Server server) override {
        this->setTitle(server.title);

        auto desc = MDTextArea::create(server.description, ccp(230, 200));
        desc->setPosition(120, 107);
        this->m_mainLayer->addChild(desc);

        return true;
    }

public:
    static PrivateServerPopup* create(Server server) {
        auto ret = new PrivateServerPopup();
        if (ret->initAnchored(440.f, 280.f, server)) {
            ret->autorelease();
            return ret;
        }

        delete ret;
        return nullptr;
    }
};
