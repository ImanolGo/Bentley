/*
 *  StemsScene.h
 *  Bentley App
 *
 *  Created by Imanol Gomez on 16/06/19.
 *
 */

#pragma once

#include "ofxScene.h"
#include "RectangleVisual.h"

class StemsScene : public ofxScene {
    
public:
    
    //! Constructor
    StemsScene();
    
    //! Destructor
    ~StemsScene();
    
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
    
    void sceneTimerCompleteHandler( int &args ) ;
    
private:
    
    void setupRectangles();
    
    void drawRectangles();
    
private:
    
    unsigned int                                        m_numPixels;
    bool                                                m_initialized;
    vector< shared_ptr<RectangleVisual>>                m_rectangles;
    
};
