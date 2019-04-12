#pragma once

#ifndef __Stardust_Brush__
#define __Stardust_Brush__

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#include "ofMain.h"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


class Brush {
public:
    Brush(){
        brush.allocate( maxDiameter + padding, maxDiameter + padding, GL_RGBA );
        setDiameter( 20 );
        update();
    };
    ~Brush(){};
    
    // VARS ---------------------------------------
    
    float hardness = 1.0;
    int diameter = 20;
    float maxDiameter = 400;
    float minDiameter = 1;
    
    int radius = 10;
    int halfRadius = 5;
    float opacity = 1.0;
    
    int padding = 20;
    int halfPadding = 10;
    ofColor color = ofColor::black;
    
    ofImage brushImage;
    bool hasBrushImage = false;
    
    void loadBrushImage( string filepath ){
        brushImage.load( filepath );
        hasBrushImage = true;
    }
    
    ofFbo brush;
    
    // VARS ---------------------------------------
    
    void update(){
        //ofEnableAlphaBlending();
        //ofEnableBlendMode( OF_BLENDMODE_ALPHA );
        ofPushStyle();
        ofEnableAlphaBlending();
        
        //cout << "update " << ofRandom(1.0) << endl;
        brush.begin();
        ofClear(255,255,255,0);
        ofClear(0);
        //ofSetColor( ofColor::green );
        ofSetColor( color, 255 );
        ofFill();
        //ofSetLineWidth( 1.0 );
        
        //cout << "halfRadius: " << halfRadius << endl;
        //cout << "radius: " << radius << endl;
        ofPushStyle();
        ofSetCircleResolution( 144 );
        
        int max = hardness*radius;
        int diff = radius - max;
        
        ofPushMatrix();
        ofTranslate((maxDiameter*0.5)+halfPadding, (maxDiameter*0.5)+halfPadding);
        //ofCircle( halfRadius + halfPadding, halfRadius + halfPadding, max );
        ofCircle( 0, 0, max );
        ofNoFill();
        
        for ( int i = max; i < radius; i++ ) {
            //cout << "[" << i << "] " << ((halfRadius - (i-halfRadius))/(float)halfRadius) << endl;
            //cout << ((halfRadius - (i-halfRadius))/(float)halfRadius) << endl;
            //ofSetColor( color, (((min - (i-min))/(float)min)) * 255 );
            ofSetColor( color, 255 - (((i - max )/(float)diff) * 255) );
            //ofCircle( halfRadius + halfPadding, halfRadius + halfPadding, i );
            ofCircle( 0, 0, i );
        }
        ofPopStyle();
        brush.end();
        ofPopMatrix();
        //offset = halfRadius + halfPadding;
        
        ofFill();
        ofPopStyle();
        //ofDisableAlphaBlending();
        //ofDisableBlendMode();
    }
    
    float offset = (maxDiameter*0.5)+halfPadding;
    
    void draw(int x, int y){
        //ofPushMatrix();
        //ofTranslate( -halfRadius, -halfRadius );
        
        //ofEnableBlendMode( OF_BLENDMODE_ALPHA );
        if ( hasBrushImage ) {
            float wh = (diameter*0.9);
            brushImage.draw( x - radius, y - radius, radius*2.0, radius*2.0 );
        }
        else brush.draw(x - offset,y - offset);
        
        //ofDisableBlendMode();
        //ofPopMatrix();
    }
    
    // SET ---------------------------------------
    Brush* setHardness( float f ) {
        if ( f < 0.01 ) return this;
        else if ( f > 1.0 ) return this;
        hardness = f;
        update();
        return this;
    }
    
    Brush* setMaxDiameter( int i ) {
        maxDiameter = i;
        brush.allocate( maxDiameter + padding, maxDiameter + padding, GL_RGBA );
        offset = (maxDiameter*0.5)+halfPadding;
        return this;
    }
    
    Brush* setDiameter( int i ) {
        if ( i > maxDiameter ) return;
        if ( i < minDiameter ) return;
        diameter = i;
        radius = (int)diameter*0.5;
        halfRadius = (int)radius*0.5;
        
        diameter += padding;
        
        update();
        return this;
    }
    
    Brush* increaseSize(){
        int newSize = diameter - padding;
        if ( diameter < 50 ) {
            newSize += 5;
        } else if ( diameter < 100 ){
            newSize += 10;
        } else if ( diameter < 300 ) {
            newSize += 25;
        } else {
            newSize += 50;
        }
        
        setDiameter( newSize );
        return this;
    }
    
    Brush* decreaseSize(){
        int newSize = diameter - padding;
        if ( diameter < 50 ) {
            newSize -= 5;
        } else if ( diameter < 100 ){
            newSize -= 10;
        } else if ( diameter < 300 ) {
            newSize -= 25;
        } else {
            newSize -= 50;
        }
        
        setDiameter( newSize );
        return this;
    }
    
    Brush* setOpacity( float f ) {
        opacity = f;
        update();
        return this;
    }
    
    Brush* setColor( int hex ) {
        color = ofColor::fromHex( hex );
        update();
        return this;
    }
    
    
    // GET ---------------------------------------
    float getHardness() {
        return hardness;
    }
    
    int getDiameter() {
        return diameter - padding;
    }
    
    float getOpacity() {
        return opacity;
    }
    
    
};


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#endif /* defined(__Stardust_Brush__) */
