#include "GDPSHub.hpp"

GDPSHub *GDPSHub::get() {
  if (!instance) instance = new GDPSHub();
  return instance;
}

void GDPSHub::storeInfo(bool useExistingDummyInfo) {
  if (!useExistingDummyInfo) {
      if (onlineLevelsDummy) onlineLevelsDummy->release();
      if (dailyLevelsDummy) dailyLevelsDummy->release();
      if (downloadedLevelsDummy) downloadedLevelsDummy->release();
      if (gauntletLevelsDummy) gauntletLevelsDummy->release();
      if (savedGauntletsDummy) savedGauntletsDummy->release();
      if (likedLevelsDummy) likedLevelsDummy->release();
      if (ratedLevelsDummy) ratedLevelsDummy->release();
      if (reportedLevelsDummy) reportedLevelsDummy->release();
      if (storedLevelsDummy) storedLevelsDummy->release();

      onlineLevelsDummy = new CCDictionary();
      onlineLevelsDummy->retain();
      dailyLevelsDummy = new CCDictionary();
      dailyLevelsDummy->retain();
      downloadedLevelsDummy = new CCDictionary();
      downloadedLevelsDummy->retain();
      gauntletLevelsDummy = new CCDictionary();
      gauntletLevelsDummy->retain();
      savedGauntletsDummy = new CCDictionary();
      savedGauntletsDummy->retain();
      likedLevelsDummy = new CCDictionary();
      likedLevelsDummy->retain();
      ratedLevelsDummy = new CCDictionary();
      ratedLevelsDummy->retain();
      reportedLevelsDummy = new CCDictionary();
      reportedLevelsDummy->retain();
      storedLevelsDummy = new CCDictionary();
      storedLevelsDummy->retain();
  }

  GameLevelManager *glm = GameLevelManager::get();
  GJAccountManager *am = GJAccountManager::get();
  
  accId = am->m_accountID;
  am->m_accountID = 0;

  gjp = am->m_GJP2;
  am->m_GJP2 = "";

  onlineLevels = glm->m_onlineLevels;
  glm->m_onlineLevels = onlineLevelsDummy;
  
  dailyLevels = glm->m_dailyLevels;
  glm->m_dailyLevels = dailyLevelsDummy;

  downloadedLevels = glm->m_downloadedLevels;
  glm->m_downloadedLevels = downloadedLevelsDummy;

  gauntletLevels = glm->m_gauntletLevels;
  glm->m_gauntletLevels = gauntletLevelsDummy;

  savedGauntlets = glm->m_savedGauntlets;
  glm->m_savedGauntlets = savedGauntletsDummy;

  likedLevels = glm->m_likedLevels;
  glm->m_likedLevels = likedLevelsDummy;

  ratedLevels = glm->m_ratedLevels;
  glm->m_ratedLevels = ratedLevelsDummy;

  reportedLevels = glm->m_reportedLevels;
  glm->m_reportedLevels = reportedLevelsDummy;

  storedLevels = glm->m_storedLevels;
  glm->m_storedLevels = storedLevelsDummy;
}

void GDPSHub::restoreInfo(bool saveDummyInfo) {
  GameLevelManager *glm = GameLevelManager::get();
  GJAccountManager *am = GJAccountManager::get();
  
  am->m_accountID = accId;
  am->m_GJP2 = gjp;
  glm->m_onlineLevels = onlineLevels;
  glm->m_dailyLevels = dailyLevels;
  glm->m_downloadedLevels = downloadedLevels;
  glm->m_gauntletLevels = gauntletLevels;
  glm->m_savedGauntlets = savedGauntlets;
  glm->m_likedLevels = likedLevels;
  glm->m_ratedLevels = ratedLevels;
  glm->m_reportedLevels = reportedLevels;
  glm->m_storedLevels = storedLevels;

  if (!saveDummyInfo) {
      onlineLevelsDummy->release();
      dailyLevelsDummy->release();
      downloadedLevelsDummy->release();
      gauntletLevelsDummy->release();
      savedGauntletsDummy->release();
      likedLevelsDummy->release();
      ratedLevelsDummy->release();
      reportedLevelsDummy->release();
      storedLevelsDummy->release();
  }
}

void GDPSHub::beginPreview(Server server) {
  if (this->serverId != -1) return;
  this->server = &server;
  this->serverId = ServerAPIEvents::registerServer(server.url, 5).id;
  if (!this->overriding) {
      overriding = true;
      storeInfo();
  }
}

void GDPSHub::endPreview() {
  if (this->serverId == -1) return;
  ServerAPIEvents::removeServer(this->serverId);
  this->serverId = -1;
  this->server = nullptr;
  if (this->overriding) {
      overriding = false;
      restoreInfo();
  }
}

void GDPSHub::switchPreview(Server server) {
  if (this->serverId == -1) return;
  this->server = &server;
  ServerAPIEvents::updateServer(this->serverId, server.url);
}

Server *GDPSHub::getCurrentServer() {
  if (this->serverId == -1) return nullptr;
  
  return this->server;
}

bool GDPSHub::isPreviewing() {
  return this->serverId != -1;
}

std::string GDPSHub::stampToDateTime(int stamp)
{
  time_t t = stamp;
  tm *tm = localtime(&t);
  char buf[80];
  strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tm);
  return buf;
}