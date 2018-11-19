/*
 *  CocoonScene.h
 *  Bentley Gestures App
 *
 *  Created by Imanol Gomez on 19/11/18.
 *
 */

#pragma once


#include "ofxScene.h"
#include "TextVisual.h"

class CocoonScene : public ofxScene {

public:

    //! Constructor
    CocoonScene();
    
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

