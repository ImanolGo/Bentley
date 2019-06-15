/*
 *  SinglePixelScene.h
 *  Bentley App
 *
 *  Created by Imanol Gomez on 15/06/19.
 *
 */

#pragma once

#include "ofxScene.h"
#include "ofxSimpleTimer.h"
#include "RectangleVisual.h"

class SinglePixelScene : public ofxScene {
    
public:
    
    //! Constructor
    SinglePixelScene();
    
    //! Destructor
    ~SinglePixelScene();
    
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
    
    void setupTimer();
    
    void setupRectangles();
    
    void updateTimer();
    
    void updateRectangles();
    
    void drawRectangles();
    
private:
    
    
    ofxSimpleTimer              m_timer;
    bool                        m_initialized;
    vector<RectangleVisual>     m_rectangles;
    
};
