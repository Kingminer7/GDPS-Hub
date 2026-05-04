#pragma once
#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
#include "../utils/Structs.hpp"
#include <km7dev.gdps-switcher/include/GDPSUtils.hpp>

using namespace geode::prelude;

class PrivateServerPopup : public Popup {
protected:
  Server m_server;

  bool init(Server server);
  void viewServer(CCObject *);
  // void saveServer(CCObject *);
  void onDiscord(CCObject *sender);
  void onTools(CCObject *sender);
  void onWeb(CCObject *sender);
  void onInfo(CCObject *);

  // Icon stuff

  void loadIcon();

public:
  static PrivateServerPopup* create(Server server);
};
