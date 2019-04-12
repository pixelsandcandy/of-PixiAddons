#pragma once

#ifndef ofxGridBackground_h
#define ofxGridBackground_h

#include "ofMain.h"

class ofxGridBackground {
    
public:
    ofxGridBackground(){};
    ~ofxGridBackground(){};
    
    ofFbo fbo;
    ofImage image;
    
    int colorA;
    int colorB;
    
    void init(int width, int height, int colorA, int colorB, int rectW, int rectH, int dpi = 1 );
    
    void draw(int x = 0, int y = 0){
        image.draw(x,y);
    }
    
};

#endif /* defined(ofxGridBackground_h) */
