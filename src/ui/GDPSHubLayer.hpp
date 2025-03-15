#pragma once

#include <Geode/Geode.hpp>
#include <Geode/binding/CCMenuItemSpriteExtra.hpp>

#include <Geode/loader/Event.hpp>
#include <Geode/utils/web.hpp>

#include "./PrivateServerNode.hpp"
#include "Geode/ui/Scrollbar.hpp"

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
    Scrollbar *m_scrollbar;
    CCMenuItemSpriteExtra *m_leftArrow;
    CCMenuItemSpriteExtra *m_rightArrow;
    bool m_fetching = false;
public:
    /*
    top (default)
    search
    recent
    */
    std::string m_queryType = "top";
    std::string m_search = "";
    int m_page = 1;
    int m_pages = 0;
    static CCScene *scene();
    void onGoBack(CCObject *);
    ScrollLayer *m_scroll;
    void updateList();
    void fetchServers();
    void onLeftArrow(CCObject *);
    void onRightArrow(CCObject *);
    void onSearch(CCObject *);
}; 