#pragma once

#ifndef __stardust_utils_ofxTrig__
#define __stardust_utils_ofxTrig__

#include "ofMain.h"
#include <iostream>

class ofxTrig {

public:
    static float xFromAngleHypotenuse( float angle, float hypotenuse ) {
        /*if ( angle == 0 || angle == 180 ) return 0;
        else if ( angle == 90 ) return hypotenuse;
        else if ( angle == 270 ) return -hypotenuse;
        
        return hypotenuse * cos( ofDegToRad( -90 + angle ) );*/
        
        if ( angle == 0 ) return hypotenuse;
        else if ( angle == 90 || angle == 270 ) return 0;
        else if ( angle == 180 ) return -hypotenuse;
        
        return hypotenuse * cos( ofDegToRad( angle ) );
    };
    
    static float yFromAngleHypotenuse( float angle, float hypotenuse ) {
        /*if ( angle == 0 ) return -hypotenuse;
        else if ( angle == 90 || angle == 270 ) return  0;
        else if ( angle == 180 ) return hypotenuse;*/
        
        if ( angle == 0 || angle == 180 ) return 0;
        else if ( angle == 90 ) return -hypotenuse;
        else if ( angle == 270 ) return hypotenuse;
        
        return hypotenuse * sin( ofDegToRad( angle ) );
    };
    
    static ofVec2f xyFromAngleHypotenuse( float angle, float hypotenuse ) {
        ofVec2f v;
        v.x = xFromAngleHypotenuse( angle, hypotenuse );
        v.y = yFromAngleHypotenuse( angle, hypotenuse );
        return v;
    };
    
    static vector<ofVec2f*> pointsBetweenPoints( ofVec2f* pt1, ofVec2f* pt2, int numPoints ) {
        
        vector<ofVec2f*> points;
        ofVec2f diff( (pt2->x-pt1->x)/(numPoints), (pt2->y-pt1->y)/(numPoints) );
        
        for ( int i = 0; i < numPoints + 1; i++ ) {
            if ( i == 0 ) points.push_back( new ofVec2f(pt1->x, pt1->y) );
            else {
                points.push_back( new ofVec2f( pt1->x + (i*diff.x), pt1->y + (i*diff.y) ) );
            }
        }
        
        return points;
        
    }
    
    
    struct Angle {
        float degX;
        float degY;
        float radX;
        float radY;
        
        Angle(){};
        ~Angle(){};
        
        void degrees( float x_, float y_ ) {
            degX = x_;
            degY = y_;
            radX = ofDegToRad( degX );
            radY = ofDegToRad( degY );
        };
        
        void rad( float x_, float y_ ) {
            radX = x_;
            radY = y_;
            degX = ofRadToDeg( radX );
            degY = ofRadToDeg( radY );
        };
        
    };
    
    static Angle fixAngles3D( float angleX_, float angleY_ ) {
        Angle a;
        
        /*angleY_ -= 180.0;
        angleY_ = abs( angleY_ );
        angleY_ += 270.0;*/
        
        /*angleX_ -= 180.0;
        angleX_ = abs( angleX_ );
        angleX_ += 270.0;*/
        
        if ( angleY_ >= 360.0 ) angleY_ = (int)angleY_ % 360;
        else if ( angleY_ <= -360.0 ) angleY_ = -((int)-angleY_ % 360);
        
        if ( angleX_ >= 360.0 ) angleX_ = (int)angleX_ % 360;
        else if ( angleX_ <= -360.0 ) angleX_ = -((int)-angleX_ % 360);
        
        /*angleX_ -= 180.0;
        angleX_ = abs( angleX_ );
        angleX_ += 270.0;*/
        
        if ( angleY_ == 90.0 ) angleY_ = 90.001;
        else if ( angleY_ == -90.0 ) angleY_ = -90.001;
        else if ( angleY_ == 270.0 ) angleY_ = 270.001;
        else if ( angleY_ == -270.0 ) angleY_ = -270.001;
        a.degrees( angleX_, angleY_ );
        
        //cout << a.degX << ", " << a.degY << endl;
        return a;
    };
    
    static ofVec3f sphereLocation( float radius_, float angleX_, float angleY_ ) {
        Angle a = fixAngles3D( angleX_, angleY_ );
        return _sphereLocationRad( radius_, a );
    };
    
    template <class CamClass>
    static ofVec3f sphereLocation( float radius_, float angleX_, float angleY_, CamClass* cam_ ) {
        Angle a = fixAngles3D( angleX_, angleY_ );
        
        //cout << a.degX << ", " << a.degY << endl;
        
        /*if ( angleX_ == 0.0 || angleX_ == 180.0 ) {
            if ( a.degY >= 90.0 && a.degY < 270.0 ) {
                
                cam_->setScale(-1, -1, 1);
                
            } else {
                cam_->setScale(1, 1, 1);
            }
        } else if ( angleY_ == 90.0 || angleY_ == 270.0 ) {
            if ( a.degX >= 0.0 && a.degX < 180.0 ) {
                
                cam_->setScale(1, 1, 1);
            } else {
                cam_->setScale(-1, -1, 1);
            }
        } else {
            cam_->setScale(1, 1, 1);
        }*/
        
        
        
        
        //cout << scale << endl;
        ofVec3f pos = _sphereLocationRad( radius_, a, true );
        //cout << a.degX << ", " << a.degY << endl;
        
        if ( a.degY == 180.0 || a.degY == -180.0 ) {
            //cout << 1 << endl;
            cam_->setScale( -1, -1, 1);
        } else if ( (a.degY <= 270.0 && a.degY >= 90.0) || (a.degY >= -270.0 && a.degY <= -90.0) ) {
            //cout << 2 << endl;
            cam_->setScale(-1, -1, 1);
            //cam_->setScale(1, 1, 1);
            //cout << "flip-" << ofRandom(1.0) << endl;
            //pos.z = -pos.z;
        } else {
            //cout << 3 << endl;
            cam_->setScale(1, 1, 1);
        }
        
        pos.x = round( pos.x * 1000.0 ) / 1000.0;
        pos.y = round( pos.y * 1000.0 ) / 1000.0;
        pos.z = round( pos.z * 1000.0 ) / 1000.0;
        
        ofVec3f scale = cam_->getScale();
        
        //cout << scale << endl;
        //cout << pos << endl;
        
        return pos;
    };
    
    static ofVec3f _sphereLocationRad( float radius_, Angle a_, bool camera_ = false ) {
        ofVec3f v;
        float cosAngleX = cos( a_.radX );
        
        //cout << cosAngleX << endl;
        /*if ( a_.degX == 90.0 || a_.degX == 270.0 ) {
            v.x = radius_ * 0.0 * cos( a_.radY );
            v.y = radius_ * 1.0 * sin( a_.radY );
        } else {
            v.x = radius_ * cosAngleX * cos( a_.radY );
            v.y = radius_ * cosAngleX * sin( a_.radY );
        }*/
        
        
        //v.z = radius_ * sin( a_.radX );
        
        
        
        
        
        //cout << a_.radX << " --- " << a_.radY << endl;
        //cout << a_.degX << " --- " << a_.degY << endl;
        
        //if ( )
        
        
        
        if ( a_.radY == 0.0 && ( a_.degX == 90.0 || a_.degX == 270.0 ) ) {
            //cout << "A" << endl;
            v.x = 0.0;
            v.y = sin( a_.radY ) * radius_;
            v.z = cos( a_.radY ) * radius_;
            
            if ( a_.degX == 270.0 ) v.z = -v.z;
            //cout << "WTF" << endl;
        } else if ( a_.radX == 0.0 && (a_.radX == 0.0 || a_.degX == 180.0) ) {
            //cout << "B" << endl;
            v.y = radius_ * sin( a_.radY );
            v.z = 0.0;
            v.x = cos( a_.radY ) * radius_;
            
            if ( a_.degX == 180.0 ) v.x = -v.x;
        } else if ( a_.degY == 90.0 || a_.degY == 270.0 ) {
            //cout << "C" << endl;
            if ( camera_ ) {
                v.x = 0.001;
                v.z = 0.001;
            } else {
                v.x = 0.0;
                v.z = 0.0;
            }
            if ( a_.degY == 90.0 ) v.y = radius_;
            else v.y = -radius_;
        } else if ( a_.radY == 0.0 || a_.degY == 180.0 ) {
            //cout << "D" << endl;
            v.y = 0.0;
            v.x = cos( a_.radX ) * radius_;
            v.z = sin( a_.radX ) * radius_;
            if ( a_.degY == 180.0 ) {
                v.z = -v.z;
                v.x = -v.x;
            }
            //if ( a_.degY == 180.0 ) v.x = -v.x;
            //if ( a_.degX == 270.0 ) v.z = -v.z;
        } else {
            
            
            //cout << "E" << endl;
            v.y = radius_ * sin( a_.radY );
            
            float side = v.y / tan( a_.radY );
            v.z = sin( a_.radX ) * side;
            v.x = v.z / tan( a_.radX );
        }
        
        
        
        //cout << v << endl;
        
        return v;
    }

    /*static ofVec3f _sphereLocationRad( float radius_, float angleRadX_, float angleRadY_, bool camera_ = false ) {
        ofVec3f v;
        float cosAngleX = cos( angleRadX_ );
        //cout << cosAngleX << endl;
        if ( cosAngleX <= 0.001 ) {
            v.x = radius_ * 0.0 * cos( angleRadY_ );
            v.y = radius_ * 1.0 * sin( angleRadY_ );
        } else {
            v.x = radius_ * cosAngleX * cos( angleRadY_ );
            v.y = radius_ * cosAngleX * sin( angleRadY_ );
        }
        
        
        v.z = radius_ * sin( angleRadX_ );
        
        cout << v << endl;
        
        return v;
    }*/
    
    /*private ofVec3f _sphereLocation( float radius_, float angleX_, float angleY_, bool camera_ = false ) {
        ofVec3f v;
        Angle a = fixAngles3D( angleX_, angleY_ );
        
        
        v.x = radius_ * cos( angleX_ ) * cos( angleY_ );
        v.y = radius_ * cos( angleX_ ) * sin( angleY_ );
        v.z = radius_ * sin( angleX_ );
        
        //if ( angleY_ > 180.0 ) v.y = -v.y;
        //if ( angleY_ > 90.0 && angleY_ < 270.0 ) v.z = -v.z;
        
        //if ( angleX_ > 180.0 ) v.z = -v.z;
        //if ( angle)
        
        return v;
    }*/
};

#endif /* defined(__stardust_utils_ofxTrig__) */
