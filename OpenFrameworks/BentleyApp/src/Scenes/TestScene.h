/*
 *  TestScene.h
 *  Bentley App
 *
 *  Created by Imanol Gomez on 30/04/19.
 *
 */

#pragma once

#include "ofxScene.h"
#include "ofxSimpleTimer.h"
#include "RectangleVisual.h"


class TestScene : public ofxScene {
    
public:
    
    //! Constructor
    TestScene();
    
    //! Destructor
    ~TestScene();
    
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
    
    void drawRectangles();
    
    void deleteAnimations();
    
    void startAnimations();
    
private:
    
    
    float                                               m_totalTime;
    ofxSimpleTimer                                      m_timer;
    bool                                                m_initialized;
    map<string, shared_ptr<RectangleVisual>>            m_rectangles;
    
};
