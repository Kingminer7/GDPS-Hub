#pragma once

#include "Geode/binding/LoadingCircle.hpp"
#include "Geode/cocos/platform/CCImage.h"
#include "Geode/utils/web.hpp"
using namespace geode::prelude;

class ImageCache {
    protected:
        static ImageCache *m_instance;
        CCDictionary *m_cache;
        ImageCache() {
            m_cache = CCDictionary::create();
            m_cache->retain();
        }
        ~ImageCache() {
            m_cache->release();
        }
    public:
        CCImage *getImage(std::string id);
        bool addImage(std::string id, CCImage *image);
        bool hasImage(std::string id);
        bool removeImage(std::string id);

        static ImageCache *get();
};
class IconNode : public CCLayer {
    protected:
        LoadingCircle *m_loadingWheel = nullptr;
        CCSprite *m_sprite = nullptr;
        CCLabelBMFont *m_naLabel = nullptr;

        EventListener<web::WebTask> m_downloadListener;
        std::mutex m_mutex;

        void downloadImage(std::string id, std::string url);
        void setupSprite(CCImage *image);
        bool init(std::string id, std::string url);
    public:
        /**
        * @brief Creating without a URL just loads from cache, or N/A if not in cache.
        */
        static IconNode *create(std::string id, std::string url = "");
};