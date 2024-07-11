#include "../utils/Structs.hpp"
#include <Geode/utils/web.hpp>
#include <Geode/loader/Event.hpp>

#pragma once

using namespace geode::prelude;

class GDPSHubLayer : public CCLayer
{
protected:
    bool init() override;
    void keyBackClicked() override;
    std::vector<Server> servers = {};
    EventListener<web::WebTask> m_listener;
public:
    static GDPSHubLayer *create();
    static cocos2d::CCLayer *scene();
    void onGoBack(CCObject *);
    void onApply(CCObject *);
    void updateList();
};