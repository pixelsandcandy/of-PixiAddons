#pragma once

#include "ofMain.h"
#include "ofxHapPlayer.h"
#include "ofxSyphon.h"
#include "ofxCpr.h"

#include "PixiServer.h"

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
    
    //
    
    void onHttpResponse(ofxCpr::responseEventArgs& evt);
    
    //
    
    string folder_path = "/Users/stardust/Documents/PixiMapper/assets/videos/hap";
    ofDirectory dir;
    
    struct Video {
        bool hap = false;
        ofxHapPlayer player;
    };
    
    vector<Video*> videoPlayers = vector<Video*>();
    int video_index = 0;
};
