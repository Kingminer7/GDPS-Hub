#pragma once

#include <Geode/Geode.hpp>
#include <Geode/binding/CCMenuItemSpriteExtra.hpp>

#include <Geode/loader/Event.hpp>
#include <Geode/utils/web.hpp>

#include "./PrivateServerNode.hpp"

using namespace geode::prelude;

class GDPSHubLayer : public CCLayer
{
protected:
    static GDPSHubLayer *create();
    bool init() override;
    void keyBackClicked() override;
    LoadingCircle *m_loadingCircle = LoadingCircle::create();
    EventListener<web::WebTask> m_listener;
    CCLabelBMFont *m_infoLabel;
    CCMenuItemSpriteExtra *leftArrow;
    CCMenuItemSpriteExtra *rightArrow;
    /*
    top (default)
    search
    recent
    */
    std::string queryType = "top";
    std::string search = "";
    int page = 1;
    int pages = 0;
    bool fetching = false;
public:
    static CCScene *scene();
    void onGoBack(CCObject *);
    ScrollLayer *scroll;
    void updateList();
    void fetchServers();
    void onLeftArrow(CCObject *);
    void onRightArrow(CCObject *);
};