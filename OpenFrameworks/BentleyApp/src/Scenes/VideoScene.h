/*
 *  VideoScene.h
 *  Bentley App
 *
 *  Created by Imanol Gomez on 30/04/19.
 *
 */

#pragma once

#include "ofxScene.h"

class VideoScene : public ofxScene {

public:

    //! Constructor
    VideoScene(std::string name);
    
    //! Destructor
    ~VideoScene();
    
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
    
    void setupVideo();
    
    void updateVideo();
    
    void drawVideo();
    
private:
    
    
    ofVideoPlayer 	m_videoPlayer;

};




