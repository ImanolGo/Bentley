/*
 *  ColorGradientScene.h
 *  Bentley App
 *
 *  Created by Imanol Gomez on 30/04/19.
 *
 */

#pragma once

#include "ofxScene.h"

class ColorGradientScene : public ofxScene {

public:

    //! Constructor
    ColorGradientScene();
    
    //! Destructor
    ~ColorGradientScene();
    
    //! Set up the scene
    void setup();

    //! Update the scene
    void update();

    //! Draw the scene
    void draw();
    
    //! Called when fading in
    void willFadeIn();

    //! Called when to start drawing
    void willDraw();

    //! Called fading out
    void willFadeOut();

    //! Called when exit
    void willExit();
    
private:
    
    void setupImage();
    
    void drawImage();
    
private:
    
    
    ofImage 	    m_image;
    bool            m_initialized;

};




