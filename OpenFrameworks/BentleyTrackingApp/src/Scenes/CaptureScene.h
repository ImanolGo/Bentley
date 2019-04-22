/*
 *  CaptureScene.h
 *  Bentley App
 *
 *  Created by Imanol Gomez on 06/02/19.
 *
 */

#pragma once


#include "ofxScene.h"
#include "TextVisual.h"

class CaptureScene : public ofxScene {

public:

    //! Constructor
    CaptureScene();
    
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
    
    void setupText();
    
    void setupColor();
    
    void drawText();
    
private:
    
    TextVisual m_text;
    ofColor m_color;
};

