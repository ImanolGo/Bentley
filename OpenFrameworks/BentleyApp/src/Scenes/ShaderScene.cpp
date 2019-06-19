/*
 *  ShaderScene.cpp
 *  Bentley App
 *
 *  Created by Imanol Gomez on 18/06/19.
 *
 */

#include "ShaderScene.h"
#include "AppManager.h"

ShaderScene::ShaderScene(std::string name): ofxScene(name), m_initialized(false), m_elapsedTime(0.0)
{
    //Intentionally left empty
}

ShaderScene::~ShaderScene()
{
   //Intentionally left empty
}


void ShaderScene::setup() {
    
    if(m_initialized){
        return;
    }
        
    ofLogNotice(getName() + "::setup");
    this->setupShader();
    
    m_initialized = true;
}

void ShaderScene::setupShader()
{
    ofLogNotice() << getName() + "::setupShader";
    
    string path = "shaders/" + getName();
    
    if(m_shader.load(path))
    {
        ofLogNotice() << getName() + "::setupShader-> successfully loaded " << path;
    }
    else{
         ofLogNotice() << getName() + "::setupShader-> Cannot load " << path;
    }
}


void ShaderScene::update()
{
    this->updateShader();
}

void ShaderScene::updateShader()
{
    float speed = AppManager::getInstance().getGuiManager().getShaderSpeed();
    m_elapsedTime += (speed*ofGetLastFrameTime());
    
    if(m_elapsedTime<0){
        m_elapsedTime = 0.0;
    }
}

void ShaderScene::draw()
{
     //ofEnableArbTex();
    ofBackground(0,0,0);
    this->drawShader();
}

void ShaderScene::drawShader()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height = AppManager::getInstance().getSettingsManager().getAppHeight();
    auto floatColor = AppManager::getInstance().getGuiManager().getSolidColor();
    float parameter = AppManager::getInstance().getGuiManager().getShaderParameter();
    
    m_shader.begin();
    m_shader.setUniform4f("iColor", floatColor);
    m_shader.setUniform1f("iGlobalTime", m_elapsedTime);
    m_shader.setUniform3f("iResolution", width, height,0.0);
    m_shader.setUniform1f("parameter", parameter);
    
    ofDrawRectangle(0, 0, width, height);
    m_shader.end();
}

void ShaderScene::willFadeIn() {
     this->setupShader();
     ofLogNotice("ShaderScene::willFadeIn");
    
}

void ShaderScene::willDraw() {
    ofLogNotice("ShaderScene::willDraw");
}

void ShaderScene::willFadeOut() {
    ofLogNotice("ShaderScene::willFadeOut");
}

void ShaderScene::willExit() {
    ofLogNotice("ShaderScene::willExit");
}
