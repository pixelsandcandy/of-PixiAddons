#include "ofxAppBaseNewWindow.h"
#include "ofxWindowManager.h"

#ifdef USING_ofxVui
#include "ofxVui.h"
#endif

//#include <Foundation/Foundation.h>

#ifdef TARGET_OSX
#include <Cocoa/Cocoa.h>
#define GLFW_EXPOSE_NATIVE_COCOA
#define GLFW_EXPOSE_NATIVE_NSGL
#include "GLFW/glfw3native.h"
#endif


//#include <Cocoa/Cocoa.h>
//#include "GLFW/glfw3native.h"

ofxAppBaseNewWindow::ofxAppBaseNewWindow(){
    windowSettings.width = 300;
    windowSettings.height = 300;
    _windowUID = (int)ofRandom(1,999999999);
    setWindowName( "windowUID-" + ofToString(_windowUID) );
}

int ofxAppBaseNewWindow::getWindowUID(){
    return _windowUID;
}

ofxAppBaseNewWindow* ofxAppBaseNewWindow::setWindowTitle( string title ){
    windowPtr->setWindowTitle( title );
    return this;
}

ofPoint ofxAppBaseNewWindow::getWindowSize(){
    /*glfwGetWindowSize(glfwWindow,&windowW,&windowH);
    int scale = ((ofAppGLFWWindow*)windowPtr.get())->getPixelScreenCoordScale();
    return ofPoint(windowW*scale,windowH*scale);*/
    return ofPoint(windowW,windowH);
}

bool ofxAppBaseNewWindow::isWindowFocused(){
    return (ofxWindowManager::get().isWindowFocused(glfwWindow));
    //return glfwGetWindowAttrib(getGLFWWindow(), GLFW_FOCUSED);
}

bool ofxAppBaseNewWindow::isMouseInside(){
    return _isMouseInside;
}

shared_ptr<ofAppBaseWindow> ofxAppBaseNewWindow::getWindowPtr(){
    return windowPtr;
}

GLFWwindow* ofxAppBaseNewWindow::getGLFWWindow(){
    if ( glfwWindow == NULL ){
        glfwWindow = ((ofAppGLFWWindow*)windowPtr.get())->getGLFWWindow();
    }
    return glfwWindow;
}


void ofxAppBaseNewWindow::closeUsingCmdW( bool val ){
    _closeUsingCmdW = val;
}

string ofxAppBaseNewWindow::getWindowName(){
    return _windowName;
}

bool ofxAppBaseNewWindow::getFullscreen(){
    return _isFullscreen;
}

ofCoreEvents& ofxAppBaseNewWindow::events(){
    return windowPtr->events();
}

void ofxAppBaseNewWindow::centerOnOpen( bool center ){
    _centerOnOpen = center;
}

void ofxAppBaseNewWindow::toggleFullscreen(){
    if ( !windowPtr ) return;
    _isFullscreen = !_isFullscreen;
    windowPtr->setFullscreen( _isFullscreen );
}

ofxAppBaseNewWindow::~ofxAppBaseNewWindow(){
    if ( windowPtr != nullptr ){
        ofRemoveListener(windowPtr->events().update, this, &ofxAppBaseNewWindow::update);
        ofRemoveListener(windowPtr->events().draw, this, &ofxAppBaseNewWindow::draw);
        ofRemoveListener(windowPtr->events().exit, this, &ofxAppBaseNewWindow::exit);
        
        ofRemoveListener(windowPtr->events().keyPressed, this, &ofxAppBaseNewWindow::keyPressed);
        ofRemoveListener(windowPtr->events().keyReleased, this, &ofxAppBaseNewWindow::keyReleased);
        
        ofRemoveListener(windowPtr->events().mouseMoved, this, &ofxAppBaseNewWindow::mouseMoved);
        ofRemoveListener(windowPtr->events().mouseDragged, this, &ofxAppBaseNewWindow::mouseDragged);
        ofRemoveListener(windowPtr->events().mousePressed, this, &ofxAppBaseNewWindow::mousePressed);
        ofRemoveListener(windowPtr->events().mouseReleased, this, &ofxAppBaseNewWindow::mouseReleased);
        ofRemoveListener(windowPtr->events().mouseScrolled, this, &ofxAppBaseNewWindow::mouseScrolled);
        ofRemoveListener(windowPtr->events().mouseEntered, this, &ofxAppBaseNewWindow::mouseEntered);
        ofRemoveListener(windowPtr->events().mouseExited, this, &ofxAppBaseNewWindow::mouseExited);
        
        ofRemoveListener(windowPtr->events().windowResized, this, &ofxAppBaseNewWindow::windowResized);
    }
    
    closeWindow();
};

shared_ptr<ofAppBaseWindow> ofxAppBaseNewWindow::open(ofGLFWWindowSettings settings, bool darkMode){
    windowSettings = settings;
    windowW = settings.width;
    windowH = settings.height;
    
    shared_ptr<ofAppBaseWindow> window = ofCreateWindow(windowSettings);
    window->setVerticalSync(true);
    windowPtr = window;
    
    
    //ofAddListener(window->events().setup, this, &ofxAppBaseNewWindow::setup);
    ofAddListener(window->events().update, this, &ofxAppBaseNewWindow::update);
    ofAddListener(window->events().draw, this, &ofxAppBaseNewWindow::draw);
    ofAddListener(window->events().exit, this, &ofxAppBaseNewWindow::exit);
    
    ofAddListener(window->events().keyPressed, this, &ofxAppBaseNewWindow::keyPressed);
    ofAddListener(window->events().keyReleased, this, &ofxAppBaseNewWindow::keyReleased);
    
    ofAddListener(window->events().mouseMoved, this, &ofxAppBaseNewWindow::mouseMoved);
    ofAddListener(window->events().mouseDragged, this, &ofxAppBaseNewWindow::mouseDragged);
    ofAddListener(window->events().mousePressed, this, &ofxAppBaseNewWindow::mousePressed);
    ofAddListener(window->events().mouseReleased, this, &ofxAppBaseNewWindow::mouseReleased);
    ofAddListener(window->events().mouseScrolled, this, &ofxAppBaseNewWindow::mouseScrolled);
    ofAddListener(window->events().mouseEntered, this, &ofxAppBaseNewWindow::mouseEntered);
    ofAddListener(window->events().mouseExited, this, &ofxAppBaseNewWindow::mouseExited);
    ofAddListener(window->events().fileDragEvent, this, &ofxAppBaseNewWindow::dragged);

    ofAddListener(window->events().windowResized, this, &ofxAppBaseNewWindow::windowResized);
    
    
    ofVec2f vec;
    
    ofxWindowManager::get().store( this );
    setup();
    
    glfwSetWindowFocusCallback(getGLFWWindow(), focus_cb);
    

    
        
        
        #ifdef TARGET_OSX
    if ( darkMode ){
        NSWindow * cocoaWindow = glfwGetCocoaWindow(getGLFWWindow());
        
        //[[cocoaWindow standardWindowButton:NSWindowZoomButton] setHidden:YES];
        //[[cocoaWindow standardWindowButton:NSWindowFullScreenButton] setHidden:YES];
        //[[cocoaWindow standardWindowButton:NSWindowMiniaturizeButton] setHidden:YES];
        [cocoaWindow setAppearance:[NSAppearance appearanceNamed:NSAppearanceNameVibrantDark]];
        //cocoaWindow.titlebarAppearsTransparent = true;
    }
        #endif
        
    
    
    //glfwSetWindow
    ofGetMainLoop()->setCurrentWindow( windowPtr );
    
    //ofLog() << "================== " << ofRandomf();
    //ofLog() << "dpi=" << dpi;
    dpi = ((ofAppGLFWWindow*)windowPtr.get())->getWindowDpi();
    //ofLog() << "dpi > " << dpi;
    dpiUpdated();
    
    return windowPtr;
}

void ofxAppBaseNewWindow::exit(ofEventArgs & args){
    closeWindow();
}

void ofxAppBaseNewWindow::closeWindow(){
    if ( _exited ) return;
    windowEventArgs args;
    args.windowUID = getWindowUID();
    args.focused = false;
    args.window = this;
    ofNotifyEvent( onWindowClose, args, this );
    
    _exited = true;
    windowPtr->events().disable();
    
    exit();
    
    ofxWindowManager::get().remove( this );
    windowPtr->setWindowShouldClose();
}

void ofxAppBaseNewWindow::keyPressed( ofKeyEventArgs & key ){
    //ofLog() << "keyPressed:" << ofToString(key.key);
    keyPressed(key.key);
    
    if ( _closeUsingCmdW && ofGetKeyPressed(OF_KEY_SUPER) && (key.key == GLFW_KEY_W || key.key == 'f' || key.key == 119) ) closeWindow();
}

void ofxAppBaseNewWindow::setWindowName( string name ){
    _windowName = name;
}

void ofxAppBaseNewWindow::setWindowPosition( int x, int y){
    ((ofAppGLFWWindow*)windowPtr.get())->setWindowPosition(x, y);
}

int ofxAppBaseNewWindow::getWindowDpi(){
    //return ((ofAppGLFWWindow*)windowPtr.get())->getPixelScreenCoordScale();
    //return ((ofAppGLFWWindow*)windowPtr.get())->getWindowDpi();
    return dpi;
}

void ofxAppBaseNewWindow::focus_cb(GLFWwindow* window, int focused)
{
    if (focused)
    {
        // The window gained input focus
        //triggerFocusEvent();
        ofxWindowManager::get().updateFocus(window, true);
    }
    else
    {
        // The window lost input focus
        //ofLog() << "unfocused";
        ofxWindowManager::get().updateFocus(window, false);
    }
}

shared_ptr<ofAppBaseWindow> ofxAppBaseNewWindow::open(int width, int height, ofVec2f position, bool resizable, bool multiMonitorFullScreen, bool darkMode ){
    
    //windowSettings.shareContextWith = ofGetMainLoop()->getCurrentWindow();
    //windowSettings.setGLVersion(3,2);
#ifdef USING_ofxVui
    windowSettings.width = width*VUI::dpi;
    windowSettings.height = height*VUI::dpi;
#else
    windowSettings.width = width;
    windowSettings.height = height;
#endif
    windowSettings.setPosition( position );
    windowSettings.glVersionMajor = ofGetGLRenderer()->getGLVersionMajor();
    windowSettings.glVersionMinor = ofGetGLRenderer()->getGLVersionMinor();
    //windowSettings.decorated = false;
    windowSettings.shareContextWith = ofxWindowManager::get().getMainWindow();
    
    if ( position.x == 0 && position.y == 0 && _centerOnOpen ){
#ifdef USING_ofxVui
        int centerX = ( ofGetScreenWidth()*VUI::divideDpi - width )*0.5;
        int centerY = ( ofGetScreenHeight()*VUI::divideDpi - height )*0.5;
#else
        int centerX = ( ofGetScreenWidth() - width )*0.5;
        int centerY = ( ofGetScreenHeight() - height )*0.5;
#endif
        windowSettings.setPosition( ofVec2f(centerX, centerY) );
    }
    
    windowSettings.width = width;
    windowSettings.height = height;
    
    windowSettings.resizable = resizable;
    windowSettings.multiMonitorFullScreen = multiMonitorFullScreen;
    
    
    return open( windowSettings, darkMode );
    
    /*int numSamples = 4;
    bool doubleBuffering = true;
    int redBits = 8;
    int greenBits = 8;
    int blueBits = 8;
    int alphaBits = 8;
    int depthBits = 24;
    int stencilBits = 0;
    bool stereo = false;
    bool visible = true;
    bool iconified = false;
    bool decorated = true;
    bool resizable = true;
    int monitor = 0;
    bool multiMonitorFullScreen = false;*/
}
