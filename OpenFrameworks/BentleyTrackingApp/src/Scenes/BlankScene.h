/*
 *  BlankScene.h
 *  Bentley App
 *
 *  Created by Imanol Gomez on 06/02/19.
 *
 */

#pragma once


#include "ofxScene.h"

class BlankScene : public ofxScene {

public:

    //! Constructor
    BlankScene();
    
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
};

