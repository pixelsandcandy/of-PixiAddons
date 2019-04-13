#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    dir = ofDirectory(folder_path);
    dir.listDir();
    
    for(int i = 0; i < dir.size(); i++){
        ofLog() << dir.getPath(i);
        
        videoPlayers.push_back(new Video());
        videoPlayers.back()->player.load( dir.getPath(i) );
        if ( i == 0 ) videoPlayers.back()->player.play();
    }
    
    //
    
    outputSyphonServer.setName("PixiVJ");
    
    fbo.allocate(1920, 1080, GL_RGBA);
    fbo.begin();
    ofClear(0);
    fbo.end();
    
    //
    
    ofAddListener( ofxCpr::events().onResponse, this, &ofApp::onHttpResponse );
    
    ofAddListener( SERVER.onSocketMessage, this, &ofApp::onSocketMessage );
    ofAddListener( SERVER.onConnected, this, &ofApp::onServerConnected );
    ofAddListener( HEART.onPulse, this, &ofApp::onPulse );
    SERVER.Start("PixiVJ");
    
    
    
}

void ofApp::setVideoIndex(int newIndex){
    video_index = newIndex;
    
    int index = 0;
    for( auto& it : videoPlayers ){
        if ( video_index == index ) it->player.play();
        else it->player.stop();
        index++;
    }
}

void ofApp::onHttpResponse(ofxCpr::responseEventArgs& evt){
    int fpos = evt.id.find("saved-image-");
    if ( fpos != string::npos ) {
        int index = ofToInt(evt.id.substr(12));
        //ofLog() << index;
        //ofLog() << evt.response;
        //ofLog() << "response";
        ofxJSONElement json;
        if ( json.parse(evt.response) ){
            videoPlayers.at(index)->img_src = json["filepath"].asString();
        }
        videoPlayers.at(index)->uploadingAsset = false;
        updateServer(false);
    }
    
    if ( evt.id.size() < 5 ){
        
    }
}

void ofApp::onServerConnected(string& res){
    HEART.Start(15, true);
    updateServer();
    
    
    //screenImg.allocate(ofGetWidth(), ofGetHeight(), OF_IMAGE_COLOR);
    //screenImg.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
    
    //SERVER.saveImage(screenImg, "test.jpg", "save-image");
}

void ofApp::onPulse(string& str){
    //ofLog() << "[pulse] " << ofGetElapsedTimef();
    updateServer();
    
}

void ofApp::updateServer(bool uploadImages){
    if ( uploadImages ) shouldUpdate = true;
    else {
        int index = 0;
        bool stillUploading = false;
        for( auto& it : videoPlayers ){
            if ( it->uploadingAsset ) stillUploading = true;
            index++;
        }
        
        if ( !stillUploading ) {
            //ofLog() << "all images uploaded";
            ofxSweetJSON json;
            json.start();
            json.addKeyValue("video_index", video_index, true);
            json.arrayStart("videos");
            
            index = 0;
            for( auto& it : videoPlayers ){
                json.objectStart();
                json.addKeyValueStr("img_src", videoPlayers.at(index)->img_src );
                json.objectEnd( (index == videoPlayers.size()-1) ? false : true);
                index++;
            }
            
            json.arrayEnd();
            json.end();
            
            SERVER.update(json);
        }
    }
}

void ofApp::onSocketMessage(ofxJSONElement& json){
    //ofLog() << "[JSON.getRawString()] " << json.getRawString();
    int index = ofToInt(json["video_index"].asString());
    if ( index >= 0 && index <= videoPlayers.size()-1 ) setVideoIndex(index);
}

//--------------------------------------------------------------
void ofApp::update(){
    int index = 0;
    for( auto& it : videoPlayers ){
        if ( video_index == index ) it->player.update();
        index++;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    int index = 0;
    for ( auto& it : videoPlayers ){
        if ( video_index == index ){
            fbo.begin();
            ofClear(0);
            it->player.draw(0,0,1920,1080);
            fbo.end();
            fbo.draw(0,index*300,530,300);
        } else {
            it->player.draw(0,index*300,530,300);
        }
        
        index++;
    }
    
    outputSyphonServer.publishTexture(&fbo.getTexture());
    
    if ( shouldUpdate ){
        int index = 0;
        
        for( auto& it : videoPlayers ){
            ofVec2f size = ofxUtils::clampSizeByWidth( it->player.getWidth(), it->player.getHeight(), 800 );
            it->uploadingAsset = true;
            //ofFbo fbo;
            saveImageFbo.allocate(size.x, size.y, GL_RGBA);
            saveImageFbo.begin();
            ofClear(0);
            it->player.draw(0,0,size.x,size.y);
            saveImageFbo.end();
            
            SERVER.saveImage(saveImageFbo.getTexture(), "video-" + ofToString(index) + ".jpg", "saved-image-" + ofToString(index) );
            
            //ofxCpr::Post(ofToString(index), SERVER.getUrl() + "/update", payload, cpr::Header{{"Content-Type","application/json"}} );
            index++;
        }
        
        shouldUpdate = false;
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    int newIndex = video_index + 1;
    if ( newIndex > videoPlayers.size()-1 ) newIndex = 0;
    setVideoIndex( newIndex );
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
