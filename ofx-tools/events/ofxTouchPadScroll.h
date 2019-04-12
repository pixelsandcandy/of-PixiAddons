#pragma once

#ifndef ofxTouchPadScroll_h
#define ofxTouchPadScroll_h

#include "ofxTouchPad.h"
#include "ofMain.h"
#include "ofxAppBaseNewWindow.h"

namespace ofx {
    enum TouchPadScrollState{
        SCROLL_UP,
        SCROLL_DOWN,
        SCROLL_LEFT,
        SCROLL_RIGHT,
        SCROLL_START,
        SCROLL_END,
        SCROLL_INERTIA_Y,
        SCROLL_INERTIA_X,
        SCROLL_INERTIA_END
    };
    
    static string TouchPadScrollStateKeys [] = {"SCROLL_UP","SCROLL_DOWN","SCROLL_LEFT","SCROLL_RIGHT"};
    
    struct TouchPadScrollEventArgs {
        float x = 0;
        float y = 0;
        TouchPadScrollState direction;
        
        void set( float x, float y, TouchPadScrollState direction){
            this->x = x;
            this->y = y;
            this->direction = direction;
        }
    };
}

using namespace ofx;

class ofxTouchPadScroll {

public:
    
    struct Touch {
        float x = 0;
        float y = 0;
        float time = 0;
        
        void set( float x, float y, float time){
            this->x = x;
            this->y = y;
            this->time = time;
        }
    };
    
    int dpi = 1;
    
    void enableRetina(){
        dpi = 2;
    }
    
    ofEvent<float> onScrollDown;
    ofEvent<float> onScrollUp;
    ofEvent<TouchPadScrollEventArgs> onScrollInertia;
    ofEvent<TouchPadScrollEventArgs> onScroll;
    ofEvent<TouchPadScrollEventArgs> onScrollStart;
    ofEvent<TouchPadScrollEventArgs> onScrollEnd;
    
    ofxTouchPadScroll();
    ~ofxTouchPadScroll(){
        if ( window == NULL ){
            ofRemoveListener(ofEvents().setup,this,&ofxTouchPadScroll::setup);
            ofRemoveListener(ofEvents().update,this,&ofxTouchPadScroll::update);
            ofRemoveListener(ofEvents().exit,this,&ofxTouchPadScroll::exit);
        } else {
            ofRemoveListener(window->events().setup,this,&ofxTouchPadScroll::setup);
            ofRemoveListener(window->events().update,this,&ofxTouchPadScroll::update);
            ofRemoveListener(window->events().exit,this,&ofxTouchPadScroll::exit);
        }
        
        window.reset();
    };
    
    bool touching = false;
    
    Touch startTouch;
    Touch endTouch;
    Touch deltaTouch;
    
    float inertiaMult = 0.0;
    float inertiaY = 0.0;
    float inertiaX = 0.0;
    bool inertia = false;
    float scroll_direction = 1.0;
    shared_ptr<ofAppBaseWindow> window = NULL;
    
    void init( shared_ptr<ofAppBaseWindow> window = NULL ){
        //ofLog() << ofSystem("defaults read NSGlobalDomain com.apple.swipescrolldirection" );
        
        if ( ofSystem("defaults read NSGlobalDomain com.apple.swipescrolldirection" ).substr(0,1) == "0" ) scroll_direction = -1.0;
        else scroll_direction = 1.0;
        
        if ( window == NULL ){
            ofAddListener(ofEvents().setup,this,&ofxTouchPadScroll::setup);
            ofAddListener(ofEvents().update,this,&ofxTouchPadScroll::update);
            ofAddListener(ofEvents().exit,this,&ofxTouchPadScroll::exit);
        } else {
            this->window = window;
            
            ofRemoveListener(ofEvents().setup,this,&ofxTouchPadScroll::setup);
            ofRemoveListener(ofEvents().update,this,&ofxTouchPadScroll::update);
            ofRemoveListener(ofEvents().exit,this,&ofxTouchPadScroll::exit);
            
            ofAddListener(window->events().setup,this,&ofxTouchPadScroll::setup);
            ofAddListener(window->events().update,this,&ofxTouchPadScroll::update);
            ofAddListener(window->events().exit,this,&ofxTouchPadScroll::exit);
        }
    }

    void setup(ofEventArgs & args);
    void update(ofEventArgs & args);
    void exit(ofEventArgs & args);
    
};

#endif /* defined(ofxTouchPadScroll) */
