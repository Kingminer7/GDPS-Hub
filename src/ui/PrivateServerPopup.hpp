#pragma once
#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
#include "../utils/Structs.hpp"
#include <km7dev.gdps-switcher/include/GDPSUtils.hpp>

using namespace geode::prelude;

class PrivateServerPopup : public Popup<Server> {
protected:
  Ref<LoadingCircle> m_loadingIndicator;
  EventListener<web::WebTask> m_downloadListener;
  Ref<CCImage> m_image;
  Server server;
  std::mutex m;

  bool setup(Server server) override;
  void viewServer(CCObject *);
  void saveServer(CCObject *);
  void onDiscord(CCObject *sender);
  void onTools(CCObject *sender);
  void onWeb(CCObject *sender);
  void onInfo(CCObject *);

  // Icon stuff

  void loadIcon();

  void startDownload();
  void addImage(CCImage *image);
  void onDownloadFailed();
  void handleFinish();
  void onDownloadFinished(CCSprite *image);

public:
  static PrivateServerPopup *create(Server server);
};
