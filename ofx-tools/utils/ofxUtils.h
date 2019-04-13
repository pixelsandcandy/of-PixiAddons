#ifndef ofxUtils_h
#define ofxUtils_h

#pragma once
#include "Poco/SingletonHolder.h"
#include "ofMain.h"

class ofxUtils {
public:
    ofxUtils(){};
    ~ofxUtils(){};
    
    static bool isEven( int i ) {
        if ( i%2 == 1 ) return false;
        else return true;
    }
    static bool isOdd( int i ) {
        if ( i%2 == 1 ) return true;
        else return false;
    }
    
    static ofVec2f clampSizeByWidth( float width, float height, float clampWidth ){
        float ratio = height / width;
        ofVec2f size;
        size.x = clampWidth;
        size.y = clampWidth*ratio;
        return size;
    }
    
    static string getTextFromFile( string filepath ){
        string text = "";
        ofBuffer buffer = ofBufferFromFile(filepath);
        int lIndex = 0;
        for (auto line : buffer.getLines()){
            if ( lIndex != 0 ) text += "\n";
            text += line;
            lIndex++;
        }
        return text;
    }
    
    static string getSwipeScrollDirection(){
        // 1 = natural
        // 0 = reversed
        
        if ( ofSystem("defaults read NSGlobalDomain com.apple.swipescrolldirection" ) == "1" ) return "natural";
        return "reversed";
    }
    
    static bool hasInternetConnection(){
        string res = ofSystem( "ping -c 1 google.com | grep -m1 \"\"" );
        if ( res.size() == 0 || res.find("resolve") != string::npos ) return false;
        else return true;
    }
    
    static string getMacSerial(){
        string macSerial = ofSystem( "system_profiler SPHardwareDataType | grep Serial" );
        int startIndex = macSerial.find_last_of( " " ) + 1;
        int endIndex = macSerial.size() - 1 - startIndex;
        macSerial = macSerial.substr( startIndex, endIndex );
        return macSerial;
    }
    
    static string getIP(){
        if ( !hasInternetConnection() ) return "";
        string res = ofSystem( "curl ifconfig.me/ip" );
        if ( res.find("resolve") == string::npos ) return res;
        else return "";
    }
    
    static string getMacComputerName(){
        return ofSystem( "scutil --get ComputerName" );
    }
    
    static ofImage* fboToImage( ofFbo* fbo ){
        ofPixels pixels;
        fbo->getTexture().readToPixels(pixels);
        
        ofImage* image;
        image->setFromPixels( pixels );
        
        return image;
    };
    
    static string gen_random(int len) {
        static const char alphanum[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        
        string s = "";
        
        for (int i = 0; i < len; ++i) {
            s += alphanum[rand() % (sizeof(alphanum) - 1)];
        }
        
        return s;
    }
    
    static vector<string> stringSplit( const string& str, const string& splitStr ) {
        vector<string> tokens;
        int startIndex;
        int endIndex;
        string s = str;
        while( s.find( splitStr ) != string::npos ) {
            startIndex = 0;
            endIndex = s.find( splitStr );
            tokens.push_back( s.substr( startIndex, endIndex ) );
            s = s.substr( endIndex+1, s.length() );
        }
        
        tokens.push_back( s );
        
        return tokens;
    }
    
    static void stringReplaceAll(std::string& str, const std::string& from, const std::string& to) {
        if(from.empty())
            return;
        size_t start_pos = 0;
        while((start_pos = str.find(from, start_pos)) != std::string::npos) {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length();
        }
    }
    
    
    static void fboToImage( ofFbo* fbo, ofImage* image ){
        ofPixels pixels;
        fbo->getTexture().readToPixels(pixels);
        
        image->setFromPixels( pixels );
    };
    
    static ofTexture& coverAndCrop( ofImage* image, int width, int height, bool center = true ) {
        ofFbo* fbo = new ofFbo();
        fbo->allocate( width, height, GL_RGBA );
        fbo->begin();
        
        //
        ofEnableAlphaBlending();
        ofClear(0);
        ofClear( 255, 255, 255, 0 );
        ofSetHexColor( 0xffffff );
        
        glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        
        int h = width*(image->getHeight()/image->getWidth());
        int w;
        int testW;
        int testH;
        if ( h < height ) {
            testH = height;
            testW = height*( image->getWidth()/image->getHeight() );
            if ( testW >= width ) {
                h = testH;
                w = testW;
            }
        }
        
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        
        if ( h > height && center ) {
            image->draw( 0, -(h - height)*.5, width, h );
        } else if ( w > width && center ) {
            image->draw( -(w - width)*.5, 0, w, h );
        } else {
            image->draw( 0, 0, width, h );
        }
        
        ofDisableAlphaBlending();
        
        fbo->end();
        
        return fbo->getTexture();
    }
    
    static ofTexture& coverAndCrop( ofTexture texture_, int width, int height, bool center = true ) {
        ofFbo* fbo = new ofFbo();
        fbo->allocate( width, height, GL_RGBA );
        fbo->begin();
        
        //
        ofEnableAlphaBlending();
        ofClear( 255, 255, 255, 0 );
        ofSetHexColor( 0xffffff );
        
        int h = width*(texture_.getHeight()/texture_.getWidth());
        int w;
        int testW;
        int testH;
        if ( h < height ) {
            testH = height;
            testW = height*( texture_.getWidth()/texture_.getHeight() );
            if ( testW >= width ) {
                h = testH;
                w = testW;
            }
        }
        if ( h > height && center ) {
            texture_.draw( 0, -(h - height)*.5, width, h );
        } else if ( w > width && center ) {
            texture_.draw( -(w - width)*.5, 0, w, h );
        } else {
            texture_.draw( 0, 0, width, h );
        }
        
        ofDisableAlphaBlending();
        
        fbo->end();
        
        return fbo->getTexture();
    }
    
    static ofxUtils& get();
    friend class Poco::SingletonHolder<ofxUtils>;
    
};

#endif
