#pragma once

#ifndef _Heartbeat_
#define _Heartbeat_


///////////////////////////////////////////////////////////////////////////////////////

class Heartbeat {
public:
    
    Heartbeat(){};
    ~Heartbeat(){};
    
    void Start(float time, bool firstPulse = false){
        if ( active ) return;
        timeDelta = time;
        active = true;
        timeNextPulse = ofGetElapsedTimef() + timeDelta;
        if ( firstPulse ) pulse();
        ofAddListener(ofEvents().update,this, &Heartbeat::_update);
    }
    
    //---------------------------------------------------------------------------------
    
    float timeDelta = 10.0;
    float timeNextPulse = 0.0;
    float active = false;
    ofEvent<string> onPulse;
    
    //---------------------------------------------------------------------------------
    
    
    void pulse(){
        string s;
        ofNotifyEvent( onPulse, s, this );
        timeNextPulse = ofGetElapsedTimef() + timeDelta;
    }
    
    
/////////////////////
private:
    
    
    void _update(ofEventArgs & args){
        if ( timeNextPulse < ofGetElapsedTimef() ){
            pulse();
        }
    }

    
/////////////////////
protected:
    
    
};

///////////////////////////////////////////////////////////////////////////////////////
#endif /* defined(_Heartbeat_) */

