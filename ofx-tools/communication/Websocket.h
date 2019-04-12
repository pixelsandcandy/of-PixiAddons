#pragma once

#ifndef __Websocket__
#define __Websocket__

#include "ofxLibwebsockets.h"

class Websocket {
public:
    
    enum Status {
        STATUS_CLOSED,
        STATUS_OPEN,
        STATUS_CONNECTED
    };
    
    Websocket();
    ~Websocket();
    
    void setup( string host, int port, string connectMessage = "" );
    void send( string message );
    
    //---------------------------------------------------------------------------------
    
    string host;
    int port;
    string connectMessage;
    bool listening = false;
    
    ofEvent<string> onMessageEvent;
    ofEvent<string> onConnectedEvent;
    
    //---------------------------------------------------------------------------------
    
    ofxLibwebsockets::Client client;
    int status = STATUS_CLOSED;
    
    float timeConnectionClosed;
    float timeNextConnectionAttempt = 0;
    float timeNextPing = 0;
    
    //---------------------------------------------------------------------------------
    
    // websocket methods
    void onConnect( ofxLibwebsockets::Event& args );
    void onOpen( ofxLibwebsockets::Event& args );
    void onClose( ofxLibwebsockets::Event& args );
    void onIdle( ofxLibwebsockets::Event& args );
    void onMessage( ofxLibwebsockets::Event& args );
    void onBroadcast( ofxLibwebsockets::Event& args );
    
protected:
    
    void update(ofEventArgs &e);
};

#endif /* defined(__Websocket__) */
