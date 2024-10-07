#pragma once

#include <Geode/Geode.hpp>
#include "../utils/Structs.hpp"
#include "Geode/cocos/label_nodes/CCLabelBMFont.h"

using namespace geode::prelude;

class PSCreatorLayer : public CCLayer
{
public:
    static CCScene *scene();
    void onGoBack(CCObject *);
    void onScores(CCObject *);
    void onDaily(CCObject *);
    void onWeekly(CCObject *);
    void onEvent(CCObject *);
    void onGauntlets(CCObject *);
    void onFeatured(CCObject *);
    void onLists(CCObject *);
    void onMapPacks(CCObject *);
    void onSearch(CCObject *);
    // void onDebug(CCObject *);
    CCLabelBMFont *infoLabel;
protected:
    Server m_server;
    static PSCreatorLayer *create();
    bool init() override;
    void keyBackClicked() override;
};