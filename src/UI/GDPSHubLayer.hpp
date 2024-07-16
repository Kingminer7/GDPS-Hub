#pragma once

#include <Geode/Geode.hpp>
#include <Geode/binding/CCMenuItemSpriteExtra.hpp>

#include <Geode/loader/Event.hpp>
#include <Geode/utils/web.hpp>
#include "../utils/Structs.hpp"

#include "./PrivateServerNode.hpp"

using namespace geode::prelude;

class GDPSHubLayer : public CCLayer
{
protected:
    bool init() override;
    void keyBackClicked() override;
    LoadingCircle *m_loadingCircle = LoadingCircle::create();
    EventListener<web::WebTask> m_listener;
public:
    static GDPSHubLayer *create();
    static cocos2d::CCLayer *scene();
    void onGoBack(CCObject *);
    ScrollLayer *scroll;
    void updateList();
};