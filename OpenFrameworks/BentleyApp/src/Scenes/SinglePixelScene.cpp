/*
 *  SinglePixelScene.cpp
 *  Bentley App
 *
 *  Created by Imanol Gomez on 15/06/19.
 *
 */


#include "SinglePixelScene.h"
#include "AppManager.h"

SinglePixelScene::SinglePixelScene(): ofxScene("SinglePixelScene"), m_initialized(false), m_numPixels(1000), m_refreshTimeMs(1000), m_pixelIndex(0)
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
    m_timer.setup( m_refreshTimeMs );
    m_timer.start( false ) ;
    ofAddListener( m_timer.TIMER_COMPLETE , this, &SinglePixelScene::sceneTimerCompleteHandler ) ;
    
    ofLogNotice() <<"SinglePixelScene::setupTimer << Time = : " << time << "s";
}

void SinglePixelScene::setupRectangles()
{
    
    unsigned short startId = 100;
    unsigned short endId = 254;
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height = AppManager::getInstance().getSettingsManager().getAppHeight();
    float w = 2;
    float h = 2;
    
    for(unsigned short _id = startId; _id<endId; _id++)
    {
        auto pos = ofPoint(-width, -height);
        bool success = AppManager::getInstance().getLedsManager().get2dPositionFromBrancher(_id, m_pixelIndex, pos);
        if(success){
            auto rectangle = make_shared<RectangleVisual>(pos,w,h,true);
            auto pos = ofPoint(-width, -height);
            m_rectangles[_id] = rectangle;
            ofLogNotice() <<"SinglePixelScene::setupRectangles << id = : " << _id ;
        }
    }
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
     m_pixelIndex = (m_pixelIndex+1)%m_numPixels;
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    //ofLogNotice() <<"SinglePixelScene::setupRectangles << m_pixelIndex  =  " << m_pixelIndex ;
    for(auto rectangle: m_rectangles){
        auto pos = ofPoint(-width, -height);
        bool success = AppManager::getInstance().getLedsManager().get2dPositionFromBrancher(rectangle.first, m_pixelIndex, pos);
        rectangle.second->setPosition(pos);
        //ofLogNotice() <<"SinglePixelScene::setupRectangles << pos -> x =  " << pos.x  << ", y = " << pos.y;
    }
    
}


void SinglePixelScene::draw()
{
    ofBackground(0,0,0);
    this->drawRectangles();
}

void SinglePixelScene::drawRectangles()
{
    
    for(auto rectangle: m_rectangles){
        rectangle.second->draw();
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
    m_pixelIndex = 0;
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
