#include "Images.hpp"
#include "Geode/ui/Layout.hpp"

ImageCache *ImageCache::m_instance = nullptr;

CCImage *ImageCache::getImage(std::string id) {
  log::info("{}", static_cast<CCImage*>(m_cache->objectForKey(id)));
  return static_cast<CCImage*>(m_cache->objectForKey(id));
}

bool ImageCache::addImage(std::string id, CCImage *image) {
    if (m_cache->objectForKey(id)) return false;
    if (!image) return false;
    log::debug(":fish:");
    image->retain();
    log::debug(":fish~1:");
    m_cache->setObject(image, id);
    log::debug(":fish~2:");
    return true;
}

bool ImageCache::removeImage(std::string id) {
    if (auto image = m_cache->objectForKey(id)) {
      image->release();
      m_cache->removeObjectForKey(id);
      return true;
    }
    return false;
}

bool ImageCache::hasImage(std::string id) {
    return m_cache->objectForKey(id) == nullptr;
}

ImageCache *ImageCache::get() {
    if (!m_instance) {
        m_instance = new ImageCache;
    }
    return m_instance;
}


// IconNode

void IconNode::loadFinished() {
    if (m_loadingWheel) m_loadingWheel->fadeAndRemove();
    release();
}

void IconNode::downloadImage(std::string id, std::string url) {
  retain();
  auto req = web::WebRequest();
  m_downloadListener.bind([this, id](web::WebTask::Event *e) {
    if (auto res = e->getValue()) {
      if (!res->ok()) {
        m_naLabel = CCLabelBMFont::create("N/A", "bigFont.fnt");
        m_naLabel->setScale(0.82f);
        m_naLabel->setAnchorPoint({0.5, 0.5});
        m_naLabel->setOpacity(150);
        m_naLabel->setID("no-icon");
        addChildAtPosition(m_naLabel, Anchor::Center);
        loadFinished();
      } else {
        auto data = res->data();
        std::thread imageThread = std::thread([data, this, id]() {
          m_mutex.lock();
          auto image = new CCImage();
          image->initWithImageData(const_cast<uint8_t *>(data.data()),
                                     data.size());
          image->retain();
          geode::Loader::get()->queueInMainThread([this, data, image, id]() {
            ImageCache::get()->addImage(id, image);
            image->release();
            CCTexture2D *texture = new CCTexture2D();
            texture->initWithImage(image);
            auto sprite = CCSprite::createWithTexture(texture);
            float imgScale = 50 / sprite->getContentSize().height;
            sprite->setScale(imgScale);
            sprite->setID("ps-logo");
            addChildAtPosition(sprite, Anchor::Center);
            loadFinished();
            texture->release();
          });
          m_mutex.unlock();
        });
        imageThread.detach();
      }
    } else if (web::WebProgress *progress = e->getProgress()) {
      if (!progress->downloadProgress().has_value()) {
        return;
      }
    } else if (e->isCancelled()) {
      geode::log::warn("Exited before finishing");
    }
  });
  auto downloadTask = req.get(url);
  m_downloadListener.setFilter(downloadTask);
}

bool IconNode::init(std::string id, std::string url) {
    if (!CCNode::init()) return false;
    if (auto image = ImageCache::get()->getImage(id)) {
        CCTexture2D *texture = new CCTexture2D();
        texture->initWithImage(image);
        auto sprite = CCSprite::createWithTexture(texture);
        float imgScale = 50 / sprite->getContentSize().height;
        sprite->setScale(imgScale);
        sprite->setID("ps-logo");
        addChildAtPosition(sprite, Anchor::Center);
        texture->release();
    } else {
      m_loadingWheel = LoadingCircle::create();
      m_loadingWheel->ignoreAnchorPointForPosition(false);
      m_loadingWheel->setScale(0.75f);
      m_loadingWheel->setParentLayer(this);
      m_loadingWheel->show();
      m_loadingWheel->setID("loading-circle");
      downloadImage(id, url);
    }
    return true;
}

IconNode *IconNode::create(std::string id, std::string url) {
    auto node = new IconNode();
    if (node && node->init(id, url)) {
        node->autorelease();
        return node;
    }
    delete node;
    return nullptr;
}