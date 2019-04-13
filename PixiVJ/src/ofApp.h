#pragma once

#include "ofMain.h"
#include "ofxHapPlayer.h"
#include "ofxSyphon.h"
#include "ofxCpr.h"

#include "PixiServer.h"
#include "Heartbeat.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    //
    
    ofxSyphonServer outputSyphonServer;
    ofFbo fbo;
    
    //
    
    PixiServer SERVER;
    Heartbeat HEART;
    
    //
    
    void onSocketMessage(ofxJSONElement& json);
    void onServerConnected(string& res);
    void onPulse(string& res);
    void setVideoIndex(int newIndex);
    void updateServer(bool uploadImages = true);
    void onHttpResponse(ofxCpr::responseEventArgs& evt);
    ofImage screenImg;
    ofFbo saveImageFbo;
    
    bool shouldUpdate = false;
    
    //
    
    string folder_path = "/Users/stardust/Documents/PixiMapper/assets/videos/hap";
    ofDirectory dir;
    
    struct Video {
        bool hap = false;
        ofxHapPlayer player;
        string img_src = "";
        bool uploadingAsset = false;
    };
    
    vector<Video*> videoPlayers = vector<Video*>();
    int video_index = 0;
};
