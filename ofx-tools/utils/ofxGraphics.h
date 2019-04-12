#pragma once

#ifndef __Stardust_Utils_Graphics__
#define __Stardust_Utils_Graphics__

#include "ofMain.h"

class ofxGraphics {

public:
    
    static ofImage& coverAndCrop( ofFbo fbo, ofImage& image, int width, int height, bool center = true ){
        ofFbo f;
        f.allocate( width, height, GL_RGBA );
        f.begin();
        
        ofEnableAlphaBlending();
        ofClear(0);
        ofClear( 255, 255, 255, 0 );
        ofSetHexColor( 0xffffff );
        
        glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        
        
        int h = width*(fbo.getHeight()/fbo.getWidth());
        int w;
        int testW;
        int testH;
        if ( h < height ) {
            testH = height;
            testW = height*( fbo.getWidth()/fbo.getHeight() );
            if ( testW >= width ) {
                h = testH;
                w = testW;
            }
        }
        
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        
        if ( h > height && center ) {
            fbo.draw( 0, -(h - height)*.5, width, h );
        } else if ( w > width && center ) {
            fbo.draw( -(w - width)*.5, 0, w, h );
        } else {
            fbo.draw( 0, 0, width, h );
        }
        
        ofDisableAlphaBlending();
        
        f.end();
        
        ofPixels pixels;
        f.getTexture().readToPixels(pixels);
        
        image.setFromPixels( pixels );
        return image;
    }
    
    static ofImage& fboToImage( ofFbo fbo, ofImage& image, int width, int height ){
        ofFbo f;
        f.allocate( width, height, GL_RGBA );
        f.begin();
        
        ofEnableAlphaBlending();
        ofClear(0);
        ofClear( 255, 255, 255, 0 );
        ofSetHexColor( 0xffffff );
        
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        
        fbo.draw( 0, 0, width, height );
        
        ofDisableAlphaBlending();
        
        f.end();
        
        ofPixels pixels;
        f.getTexture().readToPixels(pixels);
        
        image.setFromPixels( pixels );
        return image;
    }
    
    static ofTexture& coverAndCrop( ofImage* image_, int width_, int height_, bool center_ = true ) {
        ofFbo* fbo = new ofFbo();
        fbo->allocate( width_, height_, GL_RGBA );
        fbo->begin();
        
        //
        ofEnableAlphaBlending();
        ofClear(0);
        ofClear( 255, 255, 255, 0 );
        ofSetHexColor( 0xffffff );
        
        glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        
        int h = width_*(image_->getHeight()/image_->getWidth());
        int w;
        int testW;
        int testH;
        if ( h < height_ ) {
            testH = height_;
            testW = height_*( image_->getWidth()/image_->getHeight() );
            if ( testW >= width_ ) {
                h = testH;
                w = testW;
            }
        }
        
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        
        if ( h > height_ && center_ ) {
            image_->draw( 0, -(h - height_)*.5, width_, h );
        } else if ( w > width_ && center_ ) {
            image_->draw( -(w - width_)*.5, 0, w, h );
        } else {
            image_->draw( 0, 0, width_, h );
        }
        
        ofDisableAlphaBlending();
        
        fbo->end();
        
        return fbo->getTextureReference();
    }
    
    static ofTexture& coverAndCrop( ofTexture texture_, int width_, int height_, bool center_ = true ) {
        ofFbo* fbo = new ofFbo();
        fbo->allocate( width_, height_, GL_RGBA );
        fbo->begin();
        
        //
        ofEnableAlphaBlending();
        ofClear( 255, 255, 255, 0 );
        ofSetHexColor( 0xffffff );
        
        int h = width_*(texture_.getHeight()/texture_.getWidth());
        int w;
        int testW;
        int testH;
        if ( h < height_ ) {
            testH = height_;
            testW = height_*( texture_.getWidth()/texture_.getHeight() );
            if ( testW >= width_ ) {
                h = testH;
                w = testW;
            }
        }
        if ( h > height_ && center_ ) {
            texture_.draw( 0, -(h - height_)*.5, width_, h );
        } else if ( w > width_ && center_ ) {
            texture_.draw( -(w - width_)*.5, 0, w, h );
        } else {
            texture_.draw( 0, 0, width_, h );
        }
        
        ofDisableAlphaBlending();
        
        fbo->end();
        
        return fbo->getTextureReference();
    }
    
    static ofPixels& coverAndCropPixels( ofTexture texture_, int width_, int height_, bool center_ = true ) {
        ofFbo* fbo = new ofFbo();
        fbo->allocate( width_, height_, GL_RGBA );
        fbo->begin();
        
        //
        ofEnableAlphaBlending();
        ofClear( 255, 255, 255, 0 );
        ofSetHexColor( 0xffffff );
        
        int h = width_*(texture_.getHeight()/texture_.getWidth());
        int w;
        int testW;
        int testH;
        if ( h < height_ ) {
            testH = height_;
            testW = height_*( texture_.getWidth()/texture_.getHeight() );
            if ( testW >= width_ ) {
                h = testH;
                w = testW;
            }
        }
        if ( h > height_ && center_ ) {
            texture_.draw( 0, -(h - height_)*.5, width_, h );
        } else if ( w > width_ && center_ ) {
            texture_.draw( -(w - width_)*.5, 0, w, h );
        } else {
            texture_.draw( 0, 0, width_, h );
        }
        
        ofDisableAlphaBlending();
        
        fbo->end();
        ofPixels pixels;
        fbo->getTexture().readToPixels( pixels );
        
        return *&pixels;
    }
    
};

#endif /* defined(__Stardust_Utils_Graphics__) */
