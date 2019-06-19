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
    this->setupFbo();
    
    m_initialized = true;
}



void ShaderScene::setupFbo()
{
    float resolution = 0.5;
    float width = AppManager::getInstance().getSettingsManager().getAppWidth()*resolution;
    float height = AppManager::getInstance().getSettingsManager().getAppHeight()*resolution;
    m_fbo.allocate(width, height);
    m_fbo.begin(); ofClear(0); m_fbo.end();
}
void ShaderScene::setupShader()
{
    ofLogNotice() << getName() + "::setupShader";
    
    string path = "shaders/" + getName();
    
    m_shader.unload();
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
    this->updateTime();
    this->updateFbo();
}

void ShaderScene::updateTime()
{
    m_fbo.begin();
    ofClear(0);
    this->drawShader();
    m_fbo.end();
    
}

void ShaderScene::updateFbo()
{
   this->drawShader();
}

void ShaderScene::draw()
{
     //ofEnableArbTex();
    ofBackground(0,0,0);
    this->drawFbo();
}

void ShaderScene::drawFbo()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height = AppManager::getInstance().getSettingsManager().getAppHeight();
    m_fbo.draw(0,0, width, height);
}


void ShaderScene::drawShader()
{
    float width = m_fbo.getWidth();
    float height = m_fbo.getHeight();
    auto floatColor = AppManager::getInstance().getGuiManager().getSolidColor();
    float parameter = AppManager::getInstance().getGuiManager().getShaderParameter();
    int direction = AppManager::getInstance().getGuiManager().getShaderDirection();
    
    m_shader.begin();
    m_shader.setUniform4f("iColor", floatColor);
    m_shader.setUniform1f("iGlobalTime", m_elapsedTime);
    m_shader.setUniform3f("iResolution", width, height,0.0);
    m_shader.setUniform1f("parameter", parameter);
    m_shader.setUniform1i("direction", direction);
    
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
