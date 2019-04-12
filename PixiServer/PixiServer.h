#pragma once

#ifndef _PixiServer_
#define _PixiServer_

#include "ofxLibwebsockets.h"
#include "ofxSweetJSON.h"
#include "Websocket.h"
#include "ofxCpr.h"


///////////////////////////////////////////////////////////////////////////////////////

class PixiServer {
public:
    
    enum Status {
        STATUS_UNKNOWN,
        STATUS_DOWN,
        STATUS_RUNNING
    };
    
    PixiServer(){
        ofAddListener(ofEvents().update,this, &PixiServer::update);
        
        ofAddListener( ofxCpr::events().onResponse, this, &PixiServer::onHttpResponse );
        ofxCpr::Get("CheckServer", "localhost:4242", cpr::Timeout{2250} );
    };
    ~PixiServer(){};
    
    //---------------------------------------------------------------------------------
    
    float timeCheckServer = 0.0;
    Status status = STATUS_UNKNOWN;
    
    //---------------------------------------------------------------------------------
    
    void onHttpResponse(ofxCpr::responseEventArgs& evt){
        ofLog() << "PixiServer::onHttpResponse << " << evt.id << ": " << evt.response;
        
    }
    
    void update(ofEventArgs & args){
        if ( status < STATUS_RUNNING ){
            
        }
    }
    
    //---------------------------------------------------------------------------------
    
    
/////////////////////
protected:
    
    
};

///////////////////////////////////////////////////////////////////////////////////////
#endif /* defined(_PixiServer_) */

