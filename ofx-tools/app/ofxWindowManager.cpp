#include "ofxWindowManager.h"

ofxWindowManager& ofxWindowManager::get()
{
    static Poco::SingletonHolder<ofxWindowManager> sh;
    sh.get()->init();
    return *sh.get();
}