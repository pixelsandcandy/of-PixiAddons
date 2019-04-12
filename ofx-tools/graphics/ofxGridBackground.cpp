#include "ofxGridBackground.h"

void ofxGridBackground::init(int width, int height, int colorA, int colorB, int rectW, int rectH, int dpi ){
    rectW *= dpi;
    rectH *= dpi;
    
    fbo.allocate(width, height, GL_RGBA);
    fbo.begin();
    ofClear( 255, 255, 255, 255 );
    ofEnableAlphaBlending();
    ofSetColor(255,255,255,255);
    
    int maxSide = width;
    if ( height > maxSide ) maxSide = height;
    
    
    int maxX = roundf((maxSide*dpi / rectW)*100)/100;
    int maxY = roundf((maxSide*dpi / rectH)*100)/100;
    
    
    for ( int y = 0; y < maxY; y++ ){
        for ( int x = 0; x < maxX; x++ ){
            if ( y % 2 == 0 ){
                if ( x % 2 == 0 ){
                    ofSetHexColor( colorA );
                } else {
                    ofSetHexColor( colorB );
                }
            } else {
                if ( x % 2 == 0 ){
                    ofSetHexColor( colorB );
                } else {
                    ofSetHexColor( colorA );
                }
            }
            
            
            ofDrawRectangle(x*rectW, y*rectH, rectW, rectH);
        }
    }
    
    fbo.end();
    
    ofPixels pixels;
    fbo.getTexture().readToPixels(pixels);
    
    image.setFromPixels( pixels );
    
    ofSetColor(255,255,255,255);
}
