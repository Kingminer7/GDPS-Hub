#pragma once

#include <Geode/binding/CCMenuItemSpriteExtra.hpp>
#include "GDPSHubLayer.hpp"
#include "../utils/Structs.hpp"

using namespace geode::prelude;

class GDPSHubLayer;

class PrivateServerNode : public CCNode {
protected:
    GDPSHubLayer* m_layer;
    Server server;

    bool init(
        GDPSHubLayer* layer,
        Server entry,
        CCSize size
    );

public:
    static PrivateServerNode* create(
        GDPSHubLayer* layer,
        Server entry,
        CCSize size
    );
    Server getServer();
    void viewServer(CCObject *);
};