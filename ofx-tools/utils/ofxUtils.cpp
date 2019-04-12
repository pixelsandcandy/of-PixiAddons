#include "ofxUtils.h"

ofxUtils& ofxUtils::get()
{
    static Poco::SingletonHolder<ofxUtils> sh;
    return *sh.get();
}