/*
 *  ShaderScene.h
 *  Bentley App
 *
 *  Created by Imanol Gomez on 18/06/19.
 *
 */

#pragma once

#include "ofxScene.h"

class ShaderScene : public ofxScene {

public:

    //! Constructor
    ShaderScene(std::string name);
    
    //! Destructor
    ~ShaderScene();
    
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
    
    void setupShader();
    
    void setupFbo();
    
    void updateTime();
    
    void updateFbo();
    
    void drawShader();
    
    void drawFbo();
    
private:
    
    
    ofShader 	m_shader;
    ofFbo       m_fbo;
    float       m_elapsedTime;
    bool        m_initialized;

};




