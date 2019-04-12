#include "Websocket.h"

Websocket::~Websocket(){};
Websocket::Websocket(){};

void Websocket::setup( string host, int port, string connectMessage ){
    this->host = host;
    this->port = port;
    this->connectMessage = connectMessage;
    
    client.addListener(this);
    ofAddListener(ofEvents().update, this, &Websocket::update);
};

void Websocket::update(ofEventArgs &e){
    //cout << "updating.. " << host << "[" << port << "]   " << ofRandom(1.0) << endl;
    
    if ( connectionState == CS_CLOSED && ofGetElapsedTimef() > timeNextConnectionAttempt ) {
        cout << "CONNECT >> " << host << "[" << port << "]   " << ofGetElapsedTimef() << endl;
        client.connect( host, port );
        timeNextConnectionAttempt = ofGetElapsedTimef() + 5.0;
    }
    
    if (connectionState == CS_CONNECTED && ofGetElapsedTimef() > timeNextPing) {
        timeNextPing = ofGetElapsedTimef() + 15.0;
        send("[Pong]");
    }
}

void Websocket::send( string message ) {
    //cout << connectionState << endl;
    if ( connectionState == CS_CONNECTED ) {
        //cout << host << "[" << port << "] >> " << message << endl;
        client.send( message );
    }
}



//--------------------------------------------------------------
void Websocket::onConnect( ofxLibwebsockets::Event& args ){
    cout << "on connected" << endl;
}

//--------------------------------------------------------------
void Websocket::onOpen( ofxLibwebsockets::Event& args ){
    //cout << "[client] opened connection" << endl;
    
    connectionState = CS_OPEN;
    if ( connectMessage != "" ) {
        //cout << "[" << host << "] >> " << connectMessage << endl;
        client.send( connectMessage );
    }
}

//--------------------------------------------------------------
void Websocket::onClose( ofxLibwebsockets::Event& args ){
    //cout << "[client] closed connection" << endl;
    connectionState = CS_CLOSED;
    timeNextConnectionAttempt = ofGetElapsedTimef() + 5;
}

//--------------------------------------------------------------
void Websocket::onIdle( ofxLibwebsockets::Event& args ){
    //cout << "[client] idle" <<endl;
}

//--------------------------------------------------------------
void Websocket::onMessage( ofxLibwebsockets::Event& args ){
    
    //cout << "[" << host << "] >> " << args.message <<endl;
    
    if ( args.message == "[Ping]" ) return;
    
    if ( args.message == "[Connection Accepted]" ) {
        cout << host << "[" << port << "] >> CONNECTED" <<endl;
        connectionState = CS_CONNECTED;
        return;
    }
    
    ofNotifyEvent( onMessageEvent, args.message, this );
    
}

//--------------------------------------------------------------
void Websocket::onBroadcast( ofxLibwebsockets::Event& args ){
    //cout<<"got broadcast "<<args.message<<endl;
}
