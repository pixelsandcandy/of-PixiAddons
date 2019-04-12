#pragma once

#ifndef ofxAppBaseNewWindow_h
#define ofxAppBaseNewWindow_h

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

#include "ofMain.h"

class ofxAppBaseNewWindow : public ofBaseApp {

public:
    enum WindowEvent {
        WINDOW_FOCUS,
        WINDOW_UNFOCUS,
        WINDOW_FOCUS_CHANGE
    };
    
    //------------------------------------------------------------------------------ Constructor/Deconstructor
    ofxAppBaseNewWindow();
    virtual ~ofxAppBaseNewWindow();
    
    
    //------------------------------------------------------------------------------ Events
    struct windowEventArgs {
        ofxAppBaseNewWindow* window;
        bool focused = false;
        int windowUID = 0;
        int eventType;
        int numSecondaryWindows = 0;
    };
    
    ofEvent<windowEventArgs> onWindowFocus;
    ofEvent<windowEventArgs> onWindowUnfocus;
    ofEvent<windowEventArgs> onWindowFocusChange;
    ofEvent<windowEventArgs> onWindowClose;
    
    //------------------------------------------------------------------------------ Set / Get
    void toggleFullscreen();
    void closeUsingCmdW( bool val );
    void centerOnOpen( bool center );
    
    bool getKeyPressed(int key){
        return ofGetKeyPressed(key);
    }
    
    void overrideFullscreenValue(bool val){
        _isFullscreen = val;
    }
    
    ofCoreEvents& events();
    
    //------------------------------------------------------------------------------ Set / Get
    ofxAppBaseNewWindow*                                setWindowTitle( string title );
    void                                                setWindowName( string name );
    void                                                setWindowPosition(int x, int y);
    
    int                                                 getWindowUID();
    ofPoint                                             getWindowSize();
    bool                                                isWindowFocused();
    shared_ptr<ofAppBaseWindow>                         getWindowPtr();
    GLFWwindow*                                         getGLFWWindow();
    string                                              getWindowName();
    bool                                                getFullscreen();
    bool                                                isMouseInside();
    
    int                                                 getWindowDpi();
    
    bool                                                isMousePressed(){ return _isMousePressed;}
    
    //------------------------------------------------------------------------------ Create Window
    shared_ptr<ofAppBaseWindow>                         open(ofGLFWWindowSettings settings, bool darkMode = true);
    shared_ptr<ofAppBaseWindow>                         open(ofGLWindowSettings settings, bool darkMode = true);
    shared_ptr<ofAppBaseWindow>                         open(int width, int height, ofVec2f position = ofVec2f(0,0), bool resizable = true, bool multiMonitorFullScreen = false, bool darkMode = true );
    shared_ptr<ofAppBaseWindow>                         open();
    
    
    //------------------------------------------------------------------------------ Basic Stuff
    virtual void setup(){}
    virtual void update(){}
    virtual void draw(){}
    virtual void exit(){}
    
    virtual void windowResized(int w, int h){}
    virtual void windowMoved(int x, int y){};
    
    virtual void keyPressed( int key ){}
    virtual void keyReleased( int key ){}
    
    /// \brief Called on the active window when the mouse is moved
    virtual void mouseMoved( int x, int y ){}
    
    /// \brief Called on the active window when the mouse is dragged, i.e.
    /// moved with a button pressed
    virtual void mouseDragged( int x, int y, int button ){}
    
    /// \brief Called on the active window when a mouse button is pressed
    virtual void mousePressed( int x, int y, int button ){}
    
    /// \brief Called on the active window when a mouse button is released
    virtual void mouseReleased(int x, int y, int button ){}
    
    /// \brief Called on the active window when the mouse wheel is scrolled
    virtual void mouseScrolled(int x, int y, float scrollX, float scrollY ){}
    
    /// \brief Called on the active window when the mouse cursor enters the
    /// window area
    ///
    /// Note that the mouse coordinates are the last known x/y before the
    /// event occurred, i.e. from the previous frame
    virtual void mouseEntered( int x, int y ){}
    
    /// \brief Called on the active window when the mouse cursor leaves the
    /// window area
    ///
    /// Note that the mouse coordinates are the last known x/y before the
    /// event occurred, i.e. from the previous frame
    virtual void mouseExited( int x, int y){}
    
    virtual void dragEvent(ofDragInfo dragInfo) { }
    virtual void gotMessage(ofMessage msg){ }
    
    virtual void setup(ofEventArgs & args){
        setup();
    }
    
    int fcount = 0;
    int windowx = -1;
    int windowy = -1;
    int dpi = 1;
    
    float timeCheckDpi = 0.0;
    
    
    virtual void update(ofEventArgs & args){
        fcount++;
        if ( fcount >= 6 ){
        //if ( timeCheckDpi < ofGetElapsedTimef() ){
            
            ofPoint p = ((ofAppGLFWWindow*)windowPtr.get())->getWindowPosition();
            ofVec3f size = ((ofAppGLFWWindow*)windowPtr.get())->getWindowSize();
            
            
            if ( (p.x != windowx || p.y != windowy) || (size.x != windowW || size.y != windowH) ){
                
                windowW = size.x;
                windowH = size.y;
                
                windowx = p.x;
                windowy = p.y;
                windowMoved(p.x, p.y);
                
                dpi = ((ofAppGLFWWindow*)windowPtr.get())->getWindowDpi();
                dpiUpdated();
            }
            
           
            
            //timeCheckDpi = ofGetElapsedTimef() + .05;
            
            fcount = 0;
            
            
        }
        
        update();
    }
    
    virtual void dpiUpdated(){}
    
    virtual void draw(ofEventArgs & args){
        draw();
    }
    
    virtual void exit(ofEventArgs & args);
    
    
    
    virtual void windowResized(ofResizeEventArgs & resize){
        windowW = resize.width;
        windowH = resize.height;
        windowResized(resize.width,resize.height);
        
        ofLog() << "RESIZED " << ofRandomf();
    }
    
    
    
    virtual void mouseMoved( ofMouseEventArgs & mouse ){
        mouseX=mouse.x;
        mouseY=mouse.y;
        
        if ( mouseX < 0 || mouseY < 0 ) _isMouseInside = false;
        else if (mouseX > windowW || mouseY > windowH ) _isMouseInside = false;
        else _isMouseInside = true;
        
        mouseMoved(mouse.x,mouse.y);
        
        
    }
    virtual void mouseDragged( ofMouseEventArgs & mouse ){
        mouseX=mouse.x;
        mouseY=mouse.y;
        mouseDragged(mouse.x,mouse.y,mouse.button);
    }
    virtual void mousePressed( ofMouseEventArgs & mouse ){
        mouseX=mouse.x;
        mouseY=mouse.y;
        mousePressed(mouse.x,mouse.y,mouse.button);
        _isMousePressed = true;
    }
    virtual void mouseReleased(ofMouseEventArgs & mouse){
        mouseX=mouse.x;
        mouseY=mouse.y;
        mouseReleased(mouse.x,mouse.y,mouse.button);
        _isMousePressed = false;
    }
    virtual void mouseScrolled( ofMouseEventArgs & mouse ){
        mouseScrolled(mouse.x, mouse.y, mouse.scrollX, mouse.scrollY);
    }
    virtual void mouseEntered( ofMouseEventArgs & mouse ){
        _isMouseInside = true;
        mouseEntered(mouse.x,mouse.y);
    }
    virtual void mouseExited( ofMouseEventArgs & mouse ){
        _isMouseInside = false;
        mouseExited(mouse.x,mouse.y);
    }
    virtual void dragged(ofDragInfo & drag){
        dragEvent(drag);
    }
    virtual void messageReceived(ofMessage & message){
        gotMessage(message);
    }
    
    virtual void touchDown(int x, int y, int id) {};
    virtual void touchMoved(int x, int y, int id) {};
    virtual void touchUp(int x, int y, int id) {};
    virtual void touchDoubleTap(int x, int y, int id) {};
    virtual void touchCancelled(int x, int y, int id) {};
    
    virtual void touchDown(ofTouchEventArgs & touch) {
        touchDown(touch.x, touch.y, touch.id);
    };
    virtual void touchMoved(ofTouchEventArgs & touch) {
        touchMoved(touch.x, touch.y, touch.id);
    };
    virtual void touchUp(ofTouchEventArgs & touch) {
        touchUp(touch.x, touch.y, touch.id);
    };
    virtual void touchDoubleTap(ofTouchEventArgs & touch) {
        touchDoubleTap(touch.x, touch.y, touch.id);
    };
    virtual void touchCancelled(ofTouchEventArgs & touch){
        touchCancelled(touch.x, touch.y, touch.id);
    }
    
    void triggerFocusEvent( bool focused ){
        windowEventArgs args;
        args.window = this;
        args.windowUID = getWindowUID();
        
        if ( focused ){
            args.eventType = WINDOW_FOCUS;
            args.focused = true;
            ofNotifyEvent( onWindowFocus, args, this );
            _isFocused = true;
        } else {
            args.eventType = WINDOW_UNFOCUS;
            args.focused = false;
            ofNotifyEvent( onWindowUnfocus, args, this );
            _isFocused = false;
        }
        
        windowEventArgs args2;
        args2.window = this;
        args2.windowUID = getWindowUID();
        args2.focused = focused;
        args2.eventType = WINDOW_FOCUS_CHANGE;
        ofNotifyEvent( onWindowFocusChange, args2, this );
    }
    
    void closeWindow();
    int getWindowWidth(){
        return windowW;
    }
    
    int getWindowHeight(){
        return windowH;
    }
    
private:
    int _windowUID;
    string _windowName = "";
    bool _exited = false;
    bool _closeUsingCmdW = false;
    bool _centerOnOpen = true;
    bool _isFullscreen = false;
    bool _isFocused = false;
    bool _isMouseInside = false;
    GLFWwindow* glfwWindow = NULL;
    ofGLFWWindowSettings windowSettings;
    shared_ptr<ofAppBaseWindow> windowPtr;
    int windowW = 0;
    int windowH = 0;
    bool _isMousePressed = false;
    
    static void focus_cb(GLFWwindow* window, int focused);
    
    void keyPressed( ofKeyEventArgs & key );
    void keyReleased( ofKeyEventArgs & key ){
        keyReleased(key.key);
    }
    
};

#endif /* defined(ofxAppBaseNewWindow_h) */
