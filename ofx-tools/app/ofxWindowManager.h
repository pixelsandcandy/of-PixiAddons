#pragma once
#include "Poco/SingletonHolder.h"
#include "ofxAppBaseNewWindow.h"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

class ofxWindowManager {
    friend ofxAppBaseNewWindow;
    
public:
    
    ofxWindowManager(){};
    ~ofxWindowManager(){};
    
    ofEvent<ofxAppBaseNewWindow::windowEventArgs> onSecondaryWindowFocus;
    ofEvent<ofxAppBaseNewWindow::windowEventArgs> onMainWindowFocus;
    
    bool ready = false;
    
    void init(){
        if ( ready ) return;
        ready = true;
        //ofLog() << "hello";
        //cout << "ofxWindowManager.init()" << endl;
        mainWindow = ofGetMainLoop()->getCurrentWindow();
        currentWindow = ofGetMainLoop()->getCurrentWindow();
        
        
        
        mainGLFWwindow = ((ofAppGLFWWindow*)ofGetWindowPtr())->getGLFWWindow();
        focusedWindow = mainGLFWwindow;
    }
    
    bool isWindowFocused(GLFWwindow* window){
        return (window == focusedWindow);
    }
    
    bool refreshWindows = false;
    float elapsedTime = 0.0;
    float refreshStartTime = 0.0;
    
    vector<bool> refreshWindowsVec = vector<bool>();
    
    void update(){
        if ( refreshWindows ){
            if ( ofGetElapsedTimef() > refreshStartTime ){
                float delta = ofGetElapsedTimef() - refreshStartTime;
                
                int max = refreshWindowsVec.size();
                float dTime = .16;
                
                //ofLog() << index << " -- " << delta << "     " << min << "->" << max;
                
                for ( int i = 0; i < max; i++ ){
                    float minTime = i * dTime;
                    float maxTime = minTime + dTime;
                    
                    if ( delta > minTime && delta < maxTime ){
                        if ( refreshWindowsVec[i] == false ){
                            if ( i == 0 ){
                                //ofLog() << "focus MAIN " << ofGetElapsedTimef();
                                glfwFocusWindow(mainGLFWwindow);
                            } else if ( i == max - 1){
                                //ofLog() << "focus FINAL " << ofGetElapsedTimef();
                                glfwFocusWindow(focusedWindow);
                                refreshWindows = false;
                            } else {
                                //ofLog() << "focus WINDOW " << ofGetElapsedTimef();
                                glfwFocusWindow( windows[i-1]->getGLFWWindow() );
                            }
                            refreshWindowsVec[i] = true;
                        }
                    }
                }
                
            }
        }
    };
    
    void refreshAllWindows(){
        refreshWindows = true;
        refreshStartTime = ofGetElapsedTimef() + .2;
        
        int index = 0;
        refreshWindowsVec.clear();
        for ( iter_windows it = windows.begin(); it != windows.end(); it++ ){
            refreshWindowsVec.push_back(false);
            index++;
        }
        
        refreshWindowsVec.push_back(false);
        refreshWindowsVec.push_back(false);
        
        //glfwFocusWindow(mainGLFWwindow);
        return;
        
        for ( iter_windows it = windows.begin(); it != windows.end(); it++ ){
            glfwFocusWindow( (*it)->getGLFWWindow() );
        }
        glfwFocusWindow(focusedWindow);
        
    }
    
    void focusMainWindow(){
        prevFocusedWindow = focusedWindow;
        focusedWindow = mainGLFWwindow;
        glfwFocusWindow(mainGLFWwindow);
        
        //ofGetMainLoop()->setCurrentWindow( getMainWindow() );
        //getMainWindow()->makeCurrent();
        
        ofxAppBaseNewWindow::windowEventArgs args;
        args.focused = true;
        args.eventType = ofxAppBaseNewWindow::WINDOW_FOCUS;
        args.numSecondaryWindows = windows.size();
        ofNotifyEvent(onMainWindowFocus, args, this);
    }
    
    bool isMainWindowFocused(){
        return (focusedWindow == mainGLFWwindow);
    }
    
    int getNumWindows(){
        return windows.size();
    }
    
    bool closeSecondaryWindows(){
        for(vector<ofxAppBaseNewWindow*>::reverse_iterator it = windows.rbegin(); it != windows.rend(); it++ )
        {
            (*it)->closeWindow();
        };
    }
    
    static ofxWindowManager& get();
    
    void setFocusedWindow( int windowUID ){
        for (vector<ofxAppBaseNewWindow*>::iterator it = windows.begin(); it != windows.end(); it++){
            if ( (*it)->getWindowUID() == windowUID ) {
                setFocusedWindow( (*it)->getGLFWWindow() );
                return;
            }
        }
    }
    
    
protected:
    
    shared_ptr<ofAppBaseWindow> getMainWindow(){
        return mainWindow;
    }
    
    /*shared_ptr<ofAppBaseWindow> pushedWindow;
    shared_ptr<ofAppBaseWindow> poppedWindow;
    
    void pushWindow( shared_ptr<ofAppBaseWindow> win ){
        poppedWindow = currentWindow;
        win->makeCurrent();
        ofGetMainLoop()->setCurrentWindow( win );
    }
    
    void pushMainWindow(){
        poppedWindow = currentWindow;
        mainWindow->makeCurrent();
        ofGetMainLoop()->setCurrentWindow( mainWindow );
    }
    
    void popWindow(){
        poppedWindow->makeCurrent();
        ofGetMainLoop()->setCurrentWindow( poppedWindow );
    }*/
    
    shared_ptr<ofAppBaseWindow> mainWindow;
    GLFWwindow* mainGLFWwindow = NULL;
    GLFWwindow* focusedWindow = NULL;
    GLFWwindow* prevFocusedWindow = NULL;
    
    vector<ofxAppBaseNewWindow*> windows;
    typedef vector<ofxAppBaseNewWindow*>::iterator iter_windows;
    
    shared_ptr<ofAppBaseWindow> currentWindow;
    
    shared_ptr<ofAppBaseWindow> getCurrentWindow(){
        return currentWindow;
    }
    
    /*bool mainAppCurrent = true;
    
    void makeMainAppCurrent(){
        ofGetMainLoop()->setCurrentWindow( mainWindow );
        ofSystem("osascript -e 'activate application \"PixiMapper\"'");
    }
    
    bool isMainAppCurrent(){
        return mainAppCurrent;
    }*/
    
    
    
    
    
    void focusPrevWindow(){
        GLFWwindow* window = NULL;
        
        for(vector<ofxAppBaseNewWindow*>::iterator it = windows.begin(); it != windows.end(); it++ )
        {
            window = (*it)->getGLFWWindow();
            
        };
        
        if ( window != NULL ) setFocusedWindow( window );
        
        window = 0;
        delete window;
    }
    
    
    
    
    
    void updateFocus( GLFWwindow* window, bool focus ){
        ofLog() << "updateFocus()";
        
        bool secondaryWindowFocused = false;
        bool found = false;
        for ( iter_windows it = windows.begin(); it != windows.end(); it++ ){
            if ( (*it)->getGLFWWindow() == window ){
                ofLog() << "FOUND";
                found = true;
                
                if ( focus ) {
                    ofLog() << (*it)->getWindowUID() << " -- FOCUS WINDOW";
                    ofGetMainLoop()->setCurrentWindow( (*it)->getWindowPtr() );
                    (*it)->getWindowPtr()->makeCurrent();
                    secondaryWindowFocused = true;
                    prevFocusedWindow = &(*focusedWindow);
                    focusedWindow = window;
                } else {
                    ofLog() << (*it)->getWindowUID() << " -- UNFOCUS WINDOW";
                }
                
                ofxAppBaseNewWindow::windowEventArgs args;
                args.window = (*it);
                args.windowUID = (*it)->getWindowUID();
                args.focused = focus;
                if ( focus ) args.eventType = ofxAppBaseNewWindow::WINDOW_FOCUS;
                else args.eventType = ofxAppBaseNewWindow::WINDOW_UNFOCUS;
                ofNotifyEvent(onSecondaryWindowFocus, args, this);
                
                (*it)->triggerFocusEvent(focus);
                
            }
        }
        
        if (!focus && found && windows.size() == 1){
            //glfwFocusWindow( mainGLFWwindow );
            focusedWindow = mainGLFWwindow;
        }
        
        return;
        
        if ( focus && !found ) {
            ofLog() << "focus mainGLFWwindow";
            //prevFocusedWindow = NULL;
            //focusedWindow = mainGLFWwindow;
            //glfwFocusWindow( mainGLFWwindow );
            glfwFocusWindow( mainGLFWwindow );
            prevFocusedWindow = NULL;
        } else if ( (!focus && !found) || (!focus && found && windows.size() == 1) ){
            ofLog() << "focus mainGLFWwindow";
            glfwFocusWindow( mainGLFWwindow );
            prevFocusedWindow = NULL;
            //focusedWindow = mainGLFWwindow;
            
        }
        
        return;
        if ( focus ) focusedWindow = window;
        else {
            
            if ( windows.size() == 1 ) {
                focusMainWindow();
                return;
            }
            
            bool secondaryWindowFocused = true;
            
            for ( iter_windows it = windows.begin(); it != windows.end(); it++ ){
                if ( !(*it)->isWindowFocused() ) secondaryWindowFocused = false;
            }
            
            if ( !secondaryWindowFocused ) focusMainWindow();
        }
        
    }
    
    
    
    void store( ofxAppBaseNewWindow* window ){
        windows.push_back( window );
        //ofAddListener( window->onWindowFocus, this, &ofxWindowManager::focusWindow );
        currentWindow = window->getWindowPtr();
    }
    
    void remove( ofxAppBaseNewWindow* window ){
        ofLog() << "REMOVE -- " << window->getWindowUID();
        
        /*for(iter_windows it = windows.begin(); it != windows.begin() + windows.size(); )
        {
            if ( (*it) == window ) {
                ofRemoveListener( window->onWindowFocus, this, &ofxWindowManager::focusWindow );
                it = windows.erase( it );
            } else {
                it++;
            }
        }*/
        
        for(vector<ofxAppBaseNewWindow*>::reverse_iterator it = windows.rbegin(); it != windows.rend(); it++ )
        {
            //cout << "intUID:" << (*it)->getIntUID() << endl;
            
            if ( window->getWindowUID() == (*it)->getWindowUID() ) {
                //delete (*it);
                windows.erase( --(it.base()) );
            }
            
        };
        
        if ( windows.size() == 0 ) {
            focusMainWindow();
            prevFocusedWindow = NULL;
        } else {
            focusOnLastFocusedWindow();
        }
        
        return;
        
        if ( windows.size() == 0 ) {
            focusMainWindow();
        } else if (windows.size() == 1 ){
            glfwFocusWindow( windows.at(0)->getGLFWWindow() );
        } else {
            focusOnLastFocusedWindow();
        }
        
        return;
        
        if ( windows.size() == 0 ) {
            focusMainWindow();
        } else focusPrevWindow();
    }
    
    void focusOnLastFocusedWindow(){
        ofLog() << "focusOnLastFocusedWindow()";
        if ( prevFocusedWindow != NULL ) {
            ofLog() << "worked...";
            
            glfwFocusWindow( prevFocusedWindow );
            
            //if ( windows.size() > 0 && )
            //std::swap(prevFocusedWindow, focusedWindow);
        }
    }
    
    
    
    
    friend class Poco::SingletonHolder<ofxWindowManager>;
    
private:
    void setFocusedWindow( GLFWwindow* window ){
        return;
        //prevFocusedWindow = focusedWindow;
        focusedWindow = window;
        glfwFocusWindow( window );
        
        for ( iter_windows it = windows.begin(); it != windows.end(); it++ ){
            if ( (*it)->getGLFWWindow() == window ){
                ofxAppBaseNewWindow::windowEventArgs args;
                args.window = (*it);
                args.windowUID = (*it)->getWindowUID();
                args.focused = true;
                args.eventType = ofxAppBaseNewWindow::WINDOW_FOCUS;
                ofNotifyEvent(onSecondaryWindowFocus, args, this);
            }
        }
    }
};
