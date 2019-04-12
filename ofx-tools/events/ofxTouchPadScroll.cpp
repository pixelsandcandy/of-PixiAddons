#include "ofxTouchPadScroll.h"

ofxTouchPadScroll::ofxTouchPadScroll(){
    
}

void ofxTouchPadScroll::update(ofEventArgs & args){
    TouchPad& pad = TouchPad::instance();
    TouchPad::Touches touches = pad.getTouches();
    
    if ( touches.size() == 2 ){
        
        float yAvg = (touches[0].y + touches[1].y) * 0.5;
        float xAvg = (touches[0].x + touches[1].x) * 0.5;
        
        //cout << abs(touches[0].y - touches[1].y) << endl;
        //cout << "scroll -- " << abs(touches[0].y - touches[1].y) << "(" << 280*dpi << ") - " << ofRandomf() << endl;
        if ( abs(touches[0].y - touches[1].y) > 280*dpi ) return;
        //cout << "scroll YAY" << ofRandomf() << endl;
        //cout << touches[0] << endl;
        
        if ( !touching ){
            startTouch.set( xAvg, yAvg, ofGetElapsedTimef() );
            
            TouchPadScrollEventArgs event;
            event.set( startTouch.x, startTouch.y, SCROLL_START );
            ofNotifyEvent( onScrollStart, event, this);
            touching = true;
            
        }
        
        //cout << xAvg << "," << yAvg << endl;
        
        endTouch.set( xAvg, yAvg, ofGetElapsedTimef() );
        deltaTouch.set( endTouch.x - startTouch.x, endTouch.y - startTouch.y, endTouch.time - startTouch.time );
        
        //ofNotifyEvent( onScrollUp, deltaTouch.y, this);
        TouchPadScrollEventArgs event;
        
        if ( deltaTouch.y > 0 ) {
            if ( scroll_direction == 1.0 ){
                ofNotifyEvent( onScrollDown, deltaTouch.y, this);
                //event.set( deltaTouch.x, deltaTouch.y, SCROLL_DOWN );
                event.set( deltaTouch.x, deltaTouch.y, SCROLL_DOWN );
            } else {
                float y = deltaTouch.y*scroll_direction;
                ofNotifyEvent( onScrollUp, y, this);
                event.set( deltaTouch.x, y, SCROLL_UP );
            }
            
        } else {
            if ( scroll_direction == 1.0 ){
                ofNotifyEvent( onScrollUp, deltaTouch.y, this);
                //event.set( deltaTouch.x, deltaTouch.y, SCROLL_UP );
                event.set( deltaTouch.x, deltaTouch.y, SCROLL_UP );
            } else {
                float y = deltaTouch.y*scroll_direction;
                ofNotifyEvent( onScrollDown, y, this);
                event.set( deltaTouch.x, y, SCROLL_DOWN );
            }
            
        }
        
        ofNotifyEvent( onScroll, event, this);
        
        
        //if ( )
        
        
        
        
        //cout << deltaTouch.x << "," << deltaTouch.y << " (" << deltaTouch.time << ")" << endl;

        
        
    } else if (touching) {
        touching = false;
        
        inertiaMult = ofMap(deltaTouch.time, 0.001, 0.25, 0.9, 0.8);
        if ( abs(deltaTouch.time) < 0.25 ) {
            inertiaX = deltaTouch.x*0.75;
            inertiaY = deltaTouch.y*0.75;
            inertiaY *= scroll_direction;
        } else {
            TouchPadScrollEventArgs event;
            event.set( endTouch.x, endTouch.y, SCROLL_END );
            ofNotifyEvent( onScrollEnd, event, this);
        }
        //inertiaMult *= 0.3;
        //inertiaY = deltaTouch.y * multiplier*0.1;
        //inertiaX = deltaTouch.x * multiplier*0.1;
    }
    
    if ( !touching ){
        if ( inertiaY != 0.0 ){
            inertia = true;
            if ( inertiaY > 0.0 ){
                inertiaY *= inertiaMult;
                inertiaY -= 0.05;
                if ( inertiaY < 0.0 ) inertiaY = 0.0;
            } else {
                inertiaY *= inertiaMult;
                inertiaY += 0.05;
                if ( inertiaY > 0.0 ) inertiaY = 0.0;
            }
            
            TouchPadScrollEventArgs event;
            event.set( 0, inertiaY, SCROLL_INERTIA_Y );
            ofNotifyEvent( onScrollInertia, event, this);
        }
        
        if ( inertiaX != 0.0 ){
            inertia = true;
            if ( inertiaX > 0.0 ){
                inertiaX *= inertiaMult;
                inertiaX -= 1.0;
                if ( inertiaX < 0.0 ) inertiaX = 0.0;
            } else {
                inertiaX *= inertiaMult;
                inertiaX += 1.0;
                if ( inertiaX > 0.0 ) inertiaX = 0.0;
            }
            
            
            TouchPadScrollEventArgs event;
            event.set( inertiaX, 0, SCROLL_INERTIA_X );
            ofNotifyEvent( onScrollInertia, event, this);
        }
        
        if ( inertia && inertiaX == 0.0 && inertiaY == 0.0 ) {
            inertia = false;
            TouchPadScrollEventArgs event;
            event.set( 0, 0, SCROLL_END );
            ofNotifyEvent( onScrollEnd, event, this);
        }
    }
}

void ofxTouchPadScroll::setup(ofEventArgs & args){
    TouchPad& pad = TouchPad::instance();
    
    pad.setScalingRect(ofRectangle(100, 100, 160 * 5, 120 * 5));
    pad.setScalingMode(TouchPad::SCALE_TO_RECT);
    
    //  The following code attempts to prevent conflicts between system-wide
    //  gesture support and the raw TouchPad data provided by ofxTouchPad.
    ofSystem("killall -STOP Dock"); // turn off OS level gesture support ...
    CGAssociateMouseAndMouseCursorPosition(false);
    //ofHideCursor();
    
}

void ofxTouchPadScroll::exit(ofEventArgs & args){
    ofSystem("killall -CONT Dock"); // turn on OS level gesture support
    CGAssociateMouseAndMouseCursorPosition(true);
    //ofShowCursor();
    
}
