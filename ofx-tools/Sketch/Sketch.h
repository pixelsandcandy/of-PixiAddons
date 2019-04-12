#pragma once

#ifndef __Stardust_Sketch__
#define __Stardust_Sketch__

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#include "ofMain.h"
#include "Brush.h"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


class Sketch {
public:
    Sketch(){
        ofRegisterMouseEvents( this );
        ofAddListener( ofEvents().keyPressed, this, &Sketch::onKeyPressed );
        ofAddListener( ofEvents().keyReleased, this, &Sketch::onKeyReleased );
        brush = new Brush();
    };
    ~Sketch(){};
    
    void onKeyPressed( ofKeyEventArgs& e );
    void onKeyReleased( ofKeyEventArgs& e );
    
    //
    
    ofFbo layer;
    ofFbo texture;
    Brush* brush;
    
    bool ctrlKeyDown = false;
    bool optionKeyDown = false;
    bool shiftKeyDown = false;
    
    //
    
    ofPoint lastMousePos;
    int lastBrushDiameter;
    
    bool updateBrush = false;
    int newBrushDiameter;
    
    //
    
    bool active = false;
    float reverse_zoom = 1.0;
    
    void enable(){
        active = true;
    }
    
    void disable(){
        active = false;
    }
    
    void drawBrush();
    bool brushActive = true;
    bool brushDrawing = true;
    ofPoint offset;
    
    float normalizeX(float f){
        return (f - offset.x) * reverse_zoom;
    }
    
    float normalizeY(float f){
        return (f - offset.y) * reverse_zoom;
    }
    
    void setOffset(ofPoint offset){
        this->offset = offset;
    }
    
    bool enableBrush( bool active = true, bool drawing = true ){
        brushActive = active;
        brushDrawing = drawing;
    }
    
    ofVec2f lastBrushPos;
    
    //
    
    int width = 1024;
    int height = 1024;
    int backgroundColorHex = 0xffffff;
    ofColor backgroundColor = ofColor::fromHex( 0xffffff );
    bool ready = false;
    bool erasing = false;
    
    void enableEraser( bool erase = true ){
        erasing = erase;
    }
    
    
    //
    
    void setSize( int width, int height );
    void setBackgroundColor( int color ) {
        backgroundColorHex = color;
        backgroundColor = ofColor::fromHex( color );
    }
    
    //
    
    ofTexture getTexture( bool background = true, int offsetX = 0, int offsetY = 0);
    
    //
    
    void loadLayer( ofTexture tex, int offsetX = 0, int offsetY = 0 ){
        clear();
        
        ofSetColor(255,255,255,255);
        
        layer.begin();
            tex.draw(offsetX, offsetY);
        layer.end();
    }
    
    void clear(){
        texture.begin();
            ofClear(255,255,255,0);
        texture.end();
        
        layer.begin();
            ofClear(255,255,255,0);
        layer.end();
    }
    
    void setup();
    void update();
    void draw(int layerOpacity = 255, int brushOpacity = 255);
    
    void drawBrush( ofMouseEventArgs& args );
    
    //
    
    void mouseMoved(ofMouseEventArgs & args);
    void mouseDragged(ofMouseEventArgs & args);
    void mousePressed(ofMouseEventArgs & args);
    void mouseReleased(ofMouseEventArgs & args);
    
    void mouseScrolled(ofMouseEventArgs & args){};
    void mouseEntered(ofMouseEventArgs & args){};
    void mouseExited(ofMouseEventArgs & args){};
    
    
    
};


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#endif /* defined(__Stardust_Sketch__) */
