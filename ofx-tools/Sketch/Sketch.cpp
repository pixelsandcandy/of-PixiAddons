#include "Sketch.h"

void Sketch::setup(){};
void Sketch::update(){
    if ( !ready ) return;
    if ( !active ) return;
    
    if ( updateBrush ) {
        brush->setDiameter( newBrushDiameter );
    }
};

void Sketch::onKeyPressed( ofKeyEventArgs& e ){
    //cout <<"Sketch:" << e.key << endl;
    //ofLog() << ofGetKeyPressed(GLFW_KEY_CAPS_LOCK);
    switch ( e.key ) {
        case 91:
            brush->decreaseSize();
            break;
        case 93:
            brush->increaseSize();
            break;
        case 768:
        case 769: // ctrl
            ctrlKeyDown = true;
            
            break;
        case 1280: // option
        case 1281:
        case 1282:
            optionKeyDown = true;
            
            ///if ( optionKeyDown && optionKeyDown )
            break;
        case OF_KEY_LEFT_SHIFT:
        case OF_KEY_RIGHT_SHIFT:
            shiftKeyDown = true;
            enableEraser(true);
            break;
    }
    
    
}


void Sketch::onKeyReleased( ofKeyEventArgs& e ){
    //cout << e.key << endl;
    switch ( e.key ) {
        case 768:
        case 769: // ctrl
            ctrlKeyDown = false;
            //if ( updateBrush ) updateBrush = false;
            if ( updateBrush ) updateBrush = false;
            break;
        case 1280: // option
        case 1281:
        case 1282:
            optionKeyDown = false;
            //if ( updateBrush ) updateBrush = false;
            if ( updateBrush ) updateBrush = false;
            break;
        case OF_KEY_LEFT_SHIFT:
        case OF_KEY_RIGHT_SHIFT:
            shiftKeyDown = false;
            enableEraser(false);
            
            break;
    }
}

void Sketch::drawBrush(){
    if ( !brushDrawing ) return;
    ofPushStyle();
    ofSetCircleResolution(72);
    ofFill();
    
    float x;
    float y;
    
    if ( updateBrush ) {
        x = lastMousePos.x;
        y = lastMousePos.y;
    } else {
        x = normalizeX(ofGetMouseX());
        y = normalizeY(ofGetMouseY());
    }
    
    ofSetColor( ofColor::red, 100 );
    if ( updateBrush ) ofCircle( x, y, brush->radius - 1 );
    
    ofNoFill();
    ofSetColor( ofColor::white, 100 );
    
    if ( updateBrush ) ofCircle( x, y, brush->radius );
    else ofCircle( x, y, brush->radius - 1 );
    
    ofSetColor( ofColor::black, 100 );
    
    if ( updateBrush ) ofCircle( x, y, brush->radius + 1 );
    else ofCircle( x, y, brush->radius );
    
    ofPopStyle();
}

void Sketch::draw(int layerOpacity, int brushOpacity){
    if ( !ready ) return;
    if ( !active ) return;
    ofEnableAlphaBlending();
    ofSetColor(255, 255, 255, layerOpacity);
    layer.draw(0,0);
    
    ofSetColor(255, 255, 255, brushOpacity);
    drawBrush();
    
    ofSetColor(255, 255, 255,255);
};

//----------------------------------------------


void Sketch::setSize( int width, int height ){
    this->width = width;
    this->height = height;
    
    layer.allocate( width, height, GL_RGBA );
    texture.allocate( width, height, GL_RGBA );
    
    layer.begin();
    ofClear( 255, 255, 255, 0 );
    layer.end();
    
    texture.begin();
    ofClear( 255, 255, 255, 0 );
    texture.end();
    
    ready = true;
};


ofTexture Sketch::getTexture( bool background, int offsetX, int offsetY ){
    
    if ( offsetX != 0 || offsetY != 0 ){
        ofFbo fbo;
        fbo.allocate( this->width, this->height, GL_RGBA );
        fbo.begin();
        ofClear(255, 255, 255, 0);
        
        ofEnableAlphaBlending();
        layer.draw( offsetX, offsetY );
        
        fbo.end();
        
        ofPixels pixels;
        fbo.readToPixels( pixels );
        
        ofTexture tex;
        tex.loadData( pixels );
        
        return tex;
        
    }
    
    ofPixels pixels;
    layer.readToPixels( pixels );
    
    ofTexture tex;
    tex.loadData( pixels );
    
    //return texture.getTextureReference();
    return tex;
    
    
    /*return;
    
    ofEnableAlphaBlending();
    texture.begin();
    glClear(0);
    ofClear( 255, 255, 255, 0 );
    
    
    if ( background ) {
        ofFill();
        ofSetColor( backgroundColor, 255 );
        ofRect( 0, 0, width, height );
    }
    
    
    ofSetColor( ofColor::white );
    ofEnableBlendMode( OF_BLENDMODE_DISABLED );
    //glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    //ofDisableAlphaBlending();
    layer.draw( 0, 0 );
    
    texture.end();
    
    //ofEnableAlphaBlending();
    //ofDisableAlphaBlending();
    
    ofPixels pixels;
    texture.readToPixels( pixels );
    
    ofTexture tex;
    tex.loadData( pixels );
    
    //return texture.getTextureReference();
    return tex;*/
};


//----------------------------------------------




void Sketch::mouseMoved(ofMouseEventArgs & args){
    if ( !active ) return;
    //cout << args.x << "x" << args.y << endl;
};


void Sketch::mouseDragged(ofMouseEventArgs & args){
    if ( !active ) return;
    
    //cout << ctrlKeyDown << " * " << optionKeyDown << endl;
    
    if ( optionKeyDown ) {
        ofPoint delta = lastMousePos - ofPoint(normalizeX(args.x), normalizeY(args.y));
        float dist = sqrt( delta.x*delta.x + delta.y*delta.y );
        
        if ( normalizeX(args.x) < lastMousePos.x ) {
            newBrushDiameter = lastBrushDiameter - dist;
            brush->setDiameter( lastBrushDiameter - dist );
        } else if ( normalizeX(args.x) > lastMousePos.x ) {
            newBrushDiameter = lastBrushDiameter + dist;
            brush->setDiameter( lastBrushDiameter + dist );
        }
        
        updateBrush = true;
        
        return;
    }
    
    drawBrush( args );
    //ofSetColor( 255, 0, 0 );
    
    
    //ofDisableBlendMode();
    //ofDisableAlphaBlending();
};

void Sketch::drawBrush( ofMouseEventArgs& args ){
    if ( !brushActive ) return;
    if ( ofGetKeyPressed(GLFW_KEY_SPACE) || optionKeyDown ) return;
    
    //ofEnableBlendMode( OF_BLENDMODE_ALPHA );
    layer.begin();
    ofSetColor( 255, 255, 255, 255 );
    ofEnableAlphaBlending();
    
    if ( erasing ) {
        // ERASE
        glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_ALPHA);
    } else {
        // DRAW NORMAL
        glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    }
    
    
    
    
    
    //glBlendFuncSeparate(GL_DST_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    //glBlendFunc(GL_ONE, GL_ZERO);
    
    
    //ofEnableBlendMode( OF_BLENDMODE_ALPHA );
    
    //glClear(0);
    //ofClear(255,255,255,0);
    
    //ofRect( args.x, args.y, 2, 2 );
    
    //glEnable(GL_BLEND);
    //glBlendEquation(GL_FUNC_ADD);
    //glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    //glBlendFunc(GL_ONE, GL_ONE);
    //ofEnableAlphaBlending();
    
    float steps = 50;
    
    float deltaX = (normalizeX(args.x) - lastBrushPos.x)/steps;
    float deltaY = (normalizeY(args.y) - lastBrushPos.y)/steps;
    
    for ( int i = 0; i < steps; i++ ){
        //brush->draw( args.x, args.y );
        brush->draw( lastBrushPos.x + (deltaX*i), lastBrushPos.y + (deltaY*i) );
    }
    
    //ofDisableAlphaBlending();
    //glDisable(GL_BLEND);
    //ofDisableBlendMode();
    layer.end();
    lastBrushPos.set( normalizeX(args.x), normalizeY(args.y) );
}


void Sketch::mousePressed(ofMouseEventArgs & args){
    if ( !active ) return;
    if ( !updateBrush ) {
        lastBrushDiameter = brush->getDiameter();
        newBrushDiameter = brush->getDiameter();
        lastMousePos.set( normalizeX(args.x), normalizeY(args.y) );
        lastBrushPos.set( normalizeX(args.x), normalizeY(args.y) );
    }
    if ( !optionKeyDown ) drawBrush( args );
};


void Sketch::mouseReleased(ofMouseEventArgs & args){
    if ( updateBrush ) updateBrush = false;
    if ( !active ) return;
};
