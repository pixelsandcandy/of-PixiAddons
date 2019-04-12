#pragma once
#include "Poco/SingletonHolder.h"
#include "ofMain.h"
#include "ofxJSON.h"

class ofxSweetJSON {
public:
    ofxSweetJSON(){};
    ~ofxSweetJSON(){};
    
    ofxJSONElement json;
    
    string output = "";
    
    bool colorToHex = false;
    
    bool open( string filepath ){
        return json.open( filepath );
    }
    
    void parse( string str ){
        json.parse(str);
    }
    
    void useHexForColors(){
        colorToHex = true;
    }
    
    ofxJSONElement get( string key ){
        ofxJSONElement el = json;
        vector<string> keys = stringSplit( key, "." );
        for ( int i = 0, len = keys.size(); i < len; i++){
            if ( keys[i] == string("x") || keys[i] == string("r") ) el = el[0];
            else if ( keys[i] == string("y") || keys[i] == string("g") ) el = el[1];
            else if ( keys[i] == string("z") || keys[i] == string("b") ) el = el[2];
            else el = el[keys[i]];
        }
        return el;
    }
    
    ofxJSONElement get( ofxJSONElement jsonElement, string key ){
        //cout << key << endl;
        ofxJSONElement el = jsonElement;
        vector<string> keys = stringSplit( key, "." );
        //cout << keys.size() << endl;
        for ( int i = 0, len = keys.size(); i < len; i++){
            if ( keys[i] == string("x") || keys[i] == string("r") ) el = el[0];
            else if ( keys[i] == string("y") || keys[i] == string("g") ) el = el[1];
            else if ( keys[i] == string("z") || keys[i] == string("b") ) el = el[2];
            else el = el[keys[i]];
        }
        //cout << &keys << endl;
        return el;
    }
    
    ofxSweetJSON& print(){
        cout << json.getRawString() << endl;
        return *this;
    }
    
    ofxSweetJSON& start(){
        output = "{";
        return *this;
    };
    
    ofxSweetJSON& arrayStart(){
        output += "[";
        return *this;
    }
    
    ofxSweetJSON& arrayStart( string key ){
        output += this->escape(key) + ":[";
        return *this;
    }
    
    ofxSweetJSON& arrayEnd( bool comma = false ){
        output += "]";
        if ( comma ) this->addComma();
        return *this;
    }
    
    ofxSweetJSON& objectStart(){
        output += "{";
        return *this;
    }
    
    ofxSweetJSON& objectStart( string key ){
        output += this->escape(key) + ":{";
        return *this;
    }
    
    ofxSweetJSON& objectEnd( bool comma = false ){
        output += "}";
        if ( comma ) this->addComma();
        return *this;
    }
    
    ofxSweetJSON& add( ofVec2f vec, bool comma = false ){
        output += "[" + ofToString( vec.x ) + "," + ofToString( vec.y ) + "]";
        if ( comma ) this->addComma();
        return *this;
    }
    
    ofxSweetJSON& addArray( int x, int y, bool comma = false ){
        output += "[" + ofToString( x ) + "," + ofToString( y ) + "]";
        if ( comma ) this->addComma();
        return *this;
    }
    
    ofxSweetJSON& add( ofColor color, bool comma = false ){
        if ( colorToHex ) output += ofToString( color.getHex() );
        else output += "[" + ofToString( (int)color.r ) + "," + ofToString( (int)color.g ) + "," + ofToString( (int)color.b ) + "]";
        if ( comma ) this->addComma();
        return *this;
    }
    
    ofxSweetJSON& add( ofVec2f* vec, bool comma = false ){
        output += "[" + ofToString( vec->x ) + "," + ofToString( vec->y ) + "]";
        if ( comma ) this->addComma();
        return *this;
    }
    
    ofxSweetJSON& add( string str, bool comma = false ){
        output += this->escape( str );
        if ( comma ) this->addComma();
        return *this;
    }
    
    ofxSweetJSON& addKeyValue( string key, string value, bool comma = false ){
        output += this->escape( key ) + ":" + this->escape( value );
        if ( comma ) this->addComma();
        return *this;
    }
    
    ofxSweetJSON& addKeyValue( string key, bool value, bool comma = false ){
        output += this->escape( key ) + ":" + ( value ? "true" : "false" );
        if ( comma ) this->addComma();
        return *this;
    }
    
    ofxSweetJSON& addKeyValue( string key, ofColor color, bool comma = false ){
        if ( colorToHex ) output += this->escape( key ) + ":" + ofToString( color.getHex() );
        else {
            output += this->escape( key ) + ":";
            output += "[" + ofToString( (int)color.r ) + "," + ofToString( (int)color.g ) + "," + ofToString( (int)color.b ) + "]";
        }
        if ( comma ) this->addComma();
        return *this;
    }
    
    ofxSweetJSON& addKeyValue( string key, float value, bool comma = false ){
        output += this->escape( key ) + ":" + ofToString( value );
        if ( comma ) this->addComma();
        return *this;
    }
    
    ofxSweetJSON& addKeyValue( string key, int value, bool comma = false ){
        output += this->escape( key ) + ":" + ofToString( value );
        if ( comma ) this->addComma();
        return *this;
    }
    
    ofxSweetJSON& addKeyValue( string key, ofVec2f vec, bool comma = false ){
        output += this->escape( key ) + ":" + this->escape( vec );
        if ( comma ) this->addComma();
        return *this;
    }
    
    ofxSweetJSON& addKeyValue( string key, ofVec2f* vec, bool comma = false ){
        output += this->escape( key ) + ":" + this->escape( vec );
        if ( comma ) this->addComma();
        return *this;
    }
    
    ofxSweetJSON& addComma(){
        output += ",";
        return *this;
    }
    
    ofxSweetJSON& addColon(){
        output += ":";
        return *this;
    }
    
    ofxSweetJSON& end(){
        output += "}";
        return *this;
    }
    
    void save( string filepath, bool pretty = true, bool log = false ){
        if ( log ) cout << output << endl;
        json.parse( output );
        json.save( filepath, pretty );
    }
    
    string getOutput(){
        return output;
    }
    
    
private:
    string escape( string str ) {
        return "\"" + str + "\"";
    };
    
    string escape( ofVec2f vec ) {
        return "[" + ofToString( (float)vec.x ) + "," + ofToString( (float)vec.y ) + "]";
    };
    
    string escape( ofVec2f* vec ) {
        return "[" + ofToString( (float)vec->x ) + "," + ofToString( (float)vec->y ) + "]";
    };
    
    vector<string> stringSplit( const string& str, const string& splitStr ) {
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
    
    //static ofxSweetJSON& get();
    //friend class Poco::SingletonHolder<ofxSweetJSON>;
    
};
