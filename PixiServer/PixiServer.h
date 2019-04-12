#pragma once

#ifndef _PixiServer_
#define _PixiServer_

#include "ofxLibwebsockets.h"
#include "ofxSweetJSON.h"
#include "Websocket.h"
#include "ofxCpr.h"
#include "ofxUtils.h"


///////////////////////////////////////////////////////////////////////////////////////

class PixiServer {
public:
    
    enum Status {
        STATUS_DOWN,
        STATUS_RUNNING,
        STATUS_CHECKING
    };
    
    PixiServer(){
        ofAddListener(ofEvents().update,this, &PixiServer::_update);
        
        setup("PixiVJ");
        
        testJSON.start();
        testJSON.addKeyValue("hello", "world");
        testJSON.end();
        
        ofAddListener( ofxCpr::events().onResponse, this, &PixiServer::_onHttpResponse );
        _check();
    };
    ~PixiServer(){};
    
    //---------------------------------------------------------------------------------
    
    string app_name = "";
    string server_url = "";
    string server_port = "4242";
    float timeCheckStatus = 0.0;
    Status status = STATUS_CHECKING;
    
    ofxSweetJSON testJSON;
    
    //---------------------------------------------------------------------------------
    
    void setup(string app_name ){
        this->app_name = app_name;
    }
    
    void update(ofxJSONElement data){
        string payload;
        
        ofxSweetJSON json;
        json.start();
        
        json.addKeyValue("app_name", app_name, true);
        json.objectStart("data", false);
        json.output += "<<DATA>>";
        //json.objectEnd();
        json.end();
        
        payload = json.getOutput();
        
        ofxUtils::stringReplaceAll(payload, "<<DATA>>", data.getRawString());
        
        ofxCpr::Post("post", server_url + "/update", payload, cpr::Header{{"Content-Type","application/json"}} );
    }
    
    void update(ofxSweetJSON data){
        string payload;
        
        ofxSweetJSON json;
        json.start();
        
        json.addKeyValue("app_name", app_name, true);
        json.objectStart("data", false);
        json.output += data.getOutput();
        json.end();
        
        payload = json.getOutput();
        
        ofxCpr::Post("post", server_url + "/update", payload, cpr::Header{{"Content-Type","application/json"}} );
    }
    
    
    
/////////////////////
private:
    void _onHttpResponse(ofxCpr::responseEventArgs& evt){
        ofLog() << "PixiServer::onHttpResponse << " << evt.id << ": " << evt.response;
        
        if ( evt.id == "PixiServer::CheckServer" ){
            if ( evt.response == "" ){
                // STATUS_DOWN
                //ofLog() << "PixiServer:STATUS_DOWN - " << ofGetElapsedTimef();
                timeCheckStatus = ofGetElapsedTimef() + 10.0;
                status = STATUS_DOWN;
            } else {
                // STATUS_RUNNING
                //ofLog() << "PixiServer:STATUS_RUNNING - " << ofGetElapsedTimef();
                timeCheckStatus = ofGetElapsedTimef() + 10.0;
                status = STATUS_RUNNING;
                server_url = evt.response + ":" + server_port;
                
                update(testJSON);
            }
        }
    }
    
    void _update(ofEventArgs & args){
        if ( status != STATUS_CHECKING ){
            if ( timeCheckStatus < ofGetElapsedTimef() ){
                status = STATUS_CHECKING;
                _check();
            }
        }
    }
    
    void _check(){
        ofxCpr::Get("PixiServer::CheckServer", "localhost:4242", cpr::Timeout{2250} );
    }

    
/////////////////////
protected:
    
    
};

///////////////////////////////////////////////////////////////////////////////////////
#endif /* defined(_PixiServer_) */

