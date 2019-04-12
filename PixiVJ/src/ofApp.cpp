#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    dir = ofDirectory(folder_path);
    dir.listDir();
    
    for(int i = 0; i < dir.size(); i++){
        ofLog() << dir.getPath(i);
        
        videoPlayers.push_back(new Video());
        videoPlayers.back()->player.load( dir.getPath(i) );
        videoPlayers.back()->player.play();
    }
    
    //
    
    outputSyphonServer.setName("PixiVJ");
    
    fbo.allocate(1920, 1080, GL_RGBA);
    fbo.begin();
    ofClear(0);
    fbo.end();
    
    //
    
    ofAddListener( SERVER.onSocketMessage, this, &ofApp::onSocketMessage );
    SERVER.Start("PixiVJ");
    
}

void ofApp::onSocketMessage(ofxJSONElement& json){
    //ofLog() << "[JSON.getRawString()] " << json.getRawString();
    int index = ofToInt(json["video_index"].asString());
    if ( index >= 0 && index <= videoPlayers.size()-1 ) video_index = index;
}

//--------------------------------------------------------------
void ofApp::update(){
    for( auto& it : videoPlayers ){
        it->player.update();
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
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    video_index++;
    if ( video_index > videoPlayers.size()-1 ) video_index = 0;
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
