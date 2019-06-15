/*
 *  SinglePixelScene.cpp
 *  Bentley App
 *
 *  Created by Imanol Gomez on 15/06/19.
 *
 */


#include "SinglePixelScene.h"
#include "AppManager.h"

SinglePixelScene::SinglePixelScene(): ofxScene("SinglePixelScene"), m_initialized(false)
{
    //Intentionally left empty
}

SinglePixelScene::~SinglePixelScene()
{
    //Intentionally left empty
}


void SinglePixelScene::setup() {
    
    if(m_initialized){
        return;
    }
    
    ofLogNotice("SinglePixelScene::::setup");
    this->setupTimer();
    this->setupRectangles();
    
    m_initialized = true;
    
}

void SinglePixelScene::setupTimer()
{
    m_timer.setup( 1000 );
    m_timer.start( false ) ;
    ofAddListener( m_timer.TIMER_COMPLETE , this, &SinglePixelScene::sceneTimerCompleteHandler ) ;
    
    ofLogNotice() <<"SinglePixelScene::setupTimer << Time = : " << time << "s";
}

void SinglePixelScene::setupRectangles()
{
}


void SinglePixelScene::update()
{
    this->updateTimer();
}


void SinglePixelScene::updateTimer()
{
    m_timer.update();
}

void SinglePixelScene::updateRectangles()
{
    
}


void SinglePixelScene::draw()
{
    ofBackground(0,0,0);
    this->drawRectangles();
}

void SinglePixelScene::drawRectangles()
{
    for(auto rectangle: m_rectangles){
        rectangle.draw();
    }
    
}


void SinglePixelScene::sceneTimerCompleteHandler( int &args )
{
    m_timer.start(false,true);
    this->updateRectangles();
}


void SinglePixelScene::willFadeIn() {
    ofLogNotice("SinglePixelScene::willFadeIn");
    m_timer.start(false,true);
}

void SinglePixelScene::willDraw() {
    ofLogNotice("SinglePixelScene::willDraw");
}

void SinglePixelScene::willFadeOut() {
    ofLogNotice("SinglePixelScene::willFadeOut");
}

void SinglePixelScene::willExit() {
    ofLogNotice("SinglePixelScene::willExit");
}
