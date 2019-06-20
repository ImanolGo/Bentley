/*
 *  TestScene.cpp
 *  Bentley App
 *
 *  Created by Imanol Gomez on 30/04/19.
 *
 */


#include "TestScene.h"
#include "AppManager.h"

TestScene::TestScene(): ofxScene("Test"), m_initialized(false), m_totalTime(100)
{
    //Intentionally left empty
}

TestScene::~TestScene()
{
   //Intentionally left empty
}


void TestScene::setup() {
    
    if(m_initialized){
        return;
    }
    
    ofLogNotice("TestScene::::setup");
    this->setupTimer();
    this->setupRectangles();
    
    m_initialized = true;
    
}

void TestScene::setupTimer()
{
    m_timer.setup( m_totalTime * 1000);
    m_timer.start( false ) ;
    ofAddListener( m_timer.TIMER_COMPLETE , this, &TestScene::sceneTimerCompleteHandler ) ;
    
    ofLogNotice() <<"TestScene::setupTimer << Time = : " << time << "s";
}



void TestScene::setupRectangles()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height = AppManager::getInstance().getSettingsManager().getAppHeight();
    auto pos = ofPoint(0);
    auto rectangle = make_shared<RectangleVisual>(pos,width,height,false);
    rectangle->setColor(ofColor(255,0,0));
    m_rectangles["Red"] = rectangle;
    
    rectangle = make_shared<RectangleVisual>(pos,width,height,false);
    rectangle->setColor(ofColor(0,255,0));
    m_rectangles["Green"] = rectangle;
    
    rectangle = make_shared<RectangleVisual>(pos,width,height,false);
    rectangle->setColor(ofColor(255,255,255));
    m_rectangles["White"] = rectangle;
    
    pos = ofPoint(width*0.5, height*0.5);
    rectangle = make_shared<RectangleVisual>(pos,width,height,true);
    rectangle->setColor(ofColor(0,0,255));
    m_rectangles["Blue"] = rectangle;
    
}


void TestScene::update()
{
    this->updateTimer();
    
}


void TestScene::updateTimer()
{
    m_timer.update();
}

void TestScene::draw()
{
    ofBackground(0,0,0);
    this->drawRectangles();
}

void TestScene::drawRectangles()
{
    for(auto rectangle: m_rectangles){
        rectangle.second->draw();
    }
}


void TestScene::sceneTimerCompleteHandler( int &args )
{
     ofLogNotice() <<"TestScene::setupTimer << sceneTimerCompleteHandler";
    m_timer.start(false,true);
    this->deleteAnimations();
    this->startAnimations();
}

void TestScene::deleteAnimations()
{
    for(auto rectangle: m_rectangles){
        AppManager::getInstance().getVisualEffectsManager().removeAllVisualEffects(rectangle.second);
    }
}

void TestScene::startAnimations()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    EffectSettings settings;
    settings.function = LINEAR;
    settings.startAnimation = 0.0;
    settings.animationTime = m_totalTime/(2*m_rectangles.size());
    
    settings.type = EASE_OUT;
    m_rectangles["Red"]->setPosition(ofPoint(-width,0));
    AppManager::getInstance().getVisualEffectsManager().createMoveEffect(m_rectangles["Red"], ofPoint(-width,0), ofPoint(0,0), settings);
    
    settings.type = EASE_IN;
    settings.startAnimation += settings.animationTime;
    AppManager::getInstance().getVisualEffectsManager().createMoveEffect(m_rectangles["Red"], ofPoint(0.0,0.0) , ofPoint(width,0), settings);
    
    settings.type = EASE_OUT;
    settings.startAnimation += settings.animationTime;
    m_rectangles["Green"]->setPosition(ofPoint(0,-height));
    AppManager::getInstance().getVisualEffectsManager().createMoveEffect(m_rectangles["Green"], ofPoint(0,-height), ofPoint(0,0), settings);
    
    settings.type = EASE_IN;
    settings.startAnimation += settings.animationTime;
    AppManager::getInstance().getVisualEffectsManager().createMoveEffect(m_rectangles["Green"], ofPoint(0,0), ofPoint(0,height), settings);
    
    settings.type = EASE_OUT;
    settings.startAnimation += settings.animationTime;
    m_rectangles["Blue"]->setScale(ofVec2f(0,0));
    AppManager::getInstance().getVisualEffectsManager().createScaleEffect(m_rectangles["Blue"], ofVec2f(0,0), ofVec2f(1.0,1.0), settings);
    
    settings.type = EASE_IN;
    settings.startAnimation += settings.animationTime;
    AppManager::getInstance().getVisualEffectsManager().createScaleEffect(m_rectangles["Blue"], ofVec2f(1.0,1.0), ofVec2f(0.0,0.0), settings);
    
    settings.function = SINUSOIDAL;
    settings.startAnimation += settings.animationTime;
    settings.type = EASE_OUT;
    m_rectangles["White"]->setAlpha(0.0);
    AppManager::getInstance().getVisualEffectsManager().createFadeEffect(m_rectangles["White"], 0.0, 255.0, settings);
    
    settings.startAnimation += settings.animationTime;
    settings.type = EASE_IN;
    AppManager::getInstance().getVisualEffectsManager().createFadeEffect(m_rectangles["White"],255.0, 0.0, settings);
    
}

void TestScene::willFadeIn()
{ ofLogNotice() <<"TestScene::setupTimer << Time = : " << time << "s";
    ofLogNotice("TestScene::willFadeIn");
    m_timer.start(false,true);
    this->deleteAnimations();
    this->startAnimations();
}

void TestScene::willDraw(){
    ofLogNotice("TestScene::willDraw");
}

void TestScene::willFadeOut() {
    ofLogNotice("TestScene::willFadeOut");
}

void TestScene::willExit() {
    this->deleteAnimations();
    ofLogNotice("TestScene::willExit");
}
