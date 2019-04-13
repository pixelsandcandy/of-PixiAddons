#pragma once

#ifndef _PixiServer_
#define _PixiServer_

#include "ofxLibwebsockets.h"
#include "ofxSweetJSON.h"
#include "Websocket.h"
#include "ofxCpr.h"
#include "ofxUtils.h"

//#include "b64.h"
#include "Poco/Base64Encoder.h"

///////////////////////////////////////////////////////////////////////////////////////

class PixiServer {
public:
    
    enum Status {
        STATUS_DOWN,
        STATUS_RUNNING,
        STATUS_CHECKING
    };
    
    PixiServer(){};
    ~PixiServer(){};
    
    void Start(string app_name){
        setup(app_name);
        
        ofAddListener(ofEvents().update,this, &PixiServer::_update);
        
        testJSON.start();
        testJSON.addKeyValue("hello", "world");
        testJSON.end();
        
        connectionMessage.start();
        connectionMessage.addKeyValueStr("request", "join:pixiserver", true);
        connectionMessage.addKeyValueStr("app_name", app_name );
        connectionMessage.end();
        
        ofAddListener( ofxCpr::events().onResponse, this, &PixiServer::_onHttpResponse );
        _check();
        
        ofAddListener(SOCKET.onMessageEvent, this, &PixiServer::_onSocketMessage);
        ofAddListener(SOCKET.onConnectedEvent, this, &PixiServer::_onSocketConnected);
    }
    
    //---------------------------------------------------------------------------------
    
    string app_name = "";
    string server_url = "";
    string server_host = "";
    int server_port = 4242;
    float timeCheckStatus = 0.0;
    Status status = STATUS_CHECKING;
    
    ofxSweetJSON testJSON;
    ofxSweetJSON connectionMessage;
    ofxJSONElement message;
    Websocket SOCKET;
    
    ofEvent<ofxJSONElement> onSocketMessage;
    ofEvent<string> onConnected;
    
    //---------------------------------------------------------------------------------
    
    void setup(string app_name ){
        this->app_name = app_name;
    }
    
    void getUrl(){
        return server_url;
    }
    
    void getAppName(){
        return app_name;
    }
    
    static void removeCRLF(string &targetStr){
        const char CR = '\r';
        const char LF = '\n';
        string str;
        for (const auto c : targetStr) {
            if (c != CR && c != LF) {
                str += c;
            }
        }
        targetStr = std::move(str);
    }
    
    void saveImage(ofImage& image, string filename, string postID = ""){
        /*ofBuffer buffer;
        ofSaveImage(image.getPixels(), buffer, OF_IMAGE_FORMAT_JPEG, OF_IMAGE_QUALITY_BEST);
        
        stringstream ss;
        Poco::Base64Encoder b64enc(ss);
        b64enc << buffer;*/
        
        ofPixels pixels = image.getPixels();
        ofBuffer buffer;
        ofSaveImage(pixels, buffer);
        
        stringstream ss;
        ss.str("");
        
        Poco::Base64Encoder encoder(ss);
        encoder << buffer;
        encoder.close();
        
        string str = ss.str();
        removeCRLF(str);
        //saveImage(base64_encode(buffer.getText()), filename, postID);
        saveImage(str, filename, postID);
    }
    
    void saveImage(ofTexture& tex, string filename, string postID = ""){
        
        ofPixels pixels;
        tex.readToPixels(pixels);
        
        ofBuffer buffer;
        ofSaveImage(pixels, buffer);
        
        stringstream ss;
        ss.str("");
        
        Poco::Base64Encoder encoder(ss);
        encoder << buffer;
        encoder.close();
        
        string str = ss.str();
        removeCRLF(str);
        //saveImage(base64_encode(buffer.getText()), filename, postID);
        saveImage(str, filename, postID);
    }
    
    void saveImage(string base64, string filename, string postID = ""){
        //ofLog() << base64;
        
        ofxSweetJSON json;
        
        json.start();
        json.addKeyValueStr("app_name", app_name, true);
        json.addKeyValueStr("base64", base64, true);
        json.addKeyValueStr("filename", filename);
        json.end();
        
        //ofLog() << json.getString();
        
        //ofLog() << server_url + "/save-image";
        
        return;
        
        if ( postID == "" ) ofxCpr::Post("PixiServer::saveImage::" + filename, server_url + "/save-image", json.getString(), cpr::Header{{"Content-Type","application/json"}} );
        else ofxCpr::Post(postID, server_url + "/save-image", json.getString(), cpr::Header{{"Content-Type","application/json"}} );
    }
    
    void update(ofxJSONElement data){
        string payload;
        ofxSweetJSON json;
        
        json.start();
        json.addKeyValueStr("app_name", app_name, true);
        json.objectStart("data", false);
        json.output += "<<DATA>>";
        //json.objectEnd();
        json.end();
        
        payload = json.getString();
        
        ofxUtils::stringReplaceAll(payload, "<<DATA>>", data.getRawString());

        ofxCpr::Post("PixiServer::update", server_url + "/update", payload, cpr::Header{{"Content-Type","application/json"}} );
    }
    
    void update(ofxSweetJSON data){
        ofxSweetJSON json;
        
        json.start();
        json.addKeyValueStr("app_name", app_name, true);
        json.objectStart("data", false);
        json.output += data.getString();
        //json.objectEnd();
        json.end();
        
        //ofLog() << json.getString();
        
        ofxCpr::Post("PixiServer::update", server_url + "/update", json.getString(), cpr::Header{{"Content-Type","application/json"}} );
    }
    
    
    
/////////////////////
private:
    void _onSocketMessage(string& msg) {
        ofLog() << "[socket] " << msg;
        if ( message.parse(msg) ){
            //ofLog() << "[JSON] " << msg;
            ofNotifyEvent( onSocketMessage, message, this );
        }
    }
    
    void _onSocketConnected(string& msg) {
        //ofLog() << "CONNECTED ===================================";
        ofNotifyEvent( onConnected, msg, this );
    }
    
    void _onHttpResponse(ofxCpr::responseEventArgs& evt){
        //ofLog() << "PixiServer::onHttpResponse << " << evt.id << ": " << evt.response;
        
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
                server_host = evt.response;
                
                if ( !SOCKET.listening ) {
                    server_url = "http://" + server_host + ":" + ofToString(server_port);
                    ofLog() << "server_host:" << server_host << "  server_port:" << server_port << "  server_url:" << server_url;
                    ofLog() << "[connectionMessage] " << connectionMessage.getString();
                    //return;
                    SOCKET.setup(server_host, server_port, connectionMessage.getString() );
                }
                
                //update(testJSON);
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

