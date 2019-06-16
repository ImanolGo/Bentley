/*
 *  StemsScene.cpp
 *  Bentley App
 *
 *  Created by Imanol Gomez on 16/06/19.
 *
 */


#include "StemsScene.h"
#include "AppManager.h"

StemsScene::StemsScene(): ofxScene("StemsScene"), m_initialized(false), m_numPixels(1000)
{
    //Intentionally left empty
}

StemsScene::~StemsScene()
{
    //Intentionally left empty
}


void StemsScene::setup() {
    
    if(m_initialized){
        return;
    }
    
    ofLogNotice("StemsScene::::setup");
    this->setupRectangles();
    
    m_initialized = true;
    
}


void StemsScene::setupRectangles()
{
    
    unsigned short startId = 100;
    unsigned short endId = 254;
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height = AppManager::getInstance().getSettingsManager().getAppHeight();
    float w = 2;
    float h = 2;
    
    int colorHue = 0;
    int increaseHue = 255/3;
    for(unsigned short brancherId = startId; brancherId<endId; brancherId++)
    {
        vector<string> stemIds;
        bool success = AppManager::getInstance().getLedsManager().getStemIdsFromBrancher(brancherId, stemIds);
        if(success){
            ofColor color = ofColor::fromHsb(colorHue, 255, 255);
            
            ofLogNotice() <<"StemsScene::setupRectangles << brancher id = : " << brancherId ;
            ofLogNotice() <<"StemsScene::setupRectangles << brancher color hue = : " << colorHue ;
            for(auto stemId : stemIds)
            {
                
                //ofLogNotice() <<"StemsScene::setupRectangles << stemId = : " << stemId ;
                
                for(int i = 0; i<m_numPixels; i++)
                {
                    auto pos = ofPoint(-width, -height);
                    success = AppManager::getInstance().getLedsManager().get2dPositionFromStem(brancherId, stemId, i, pos);
                    
                    if(success){
                        auto rectangle = make_shared<RectangleVisual>(pos,w,h,true);
                        rectangle->setColor(color);
                        m_rectangles.push_back(rectangle);
                        //ofLogNotice() <<"StemsScene::setupRectangles << id = : " << _id ;
                    }
                    
                }
               
            }
            
             colorHue = (colorHue +increaseHue)%256;
            
        }
    }
}


void StemsScene::update()
{
    //
   
}

void StemsScene::draw()
{
    ofBackground(0,0,0);
    this->drawRectangles();
}

void StemsScene::drawRectangles()
{
    for(auto rectangle: m_rectangles){
        rectangle->draw();
    }
    
}


void StemsScene::willFadeIn() {
    ofLogNotice("StemsScene::willFadeIn");
}

void StemsScene::willDraw() {
    ofLogNotice("StemsScene::willDraw");
}

void StemsScene::willFadeOut() {
    ofLogNotice("StemsScene::willFadeOut");
}

void StemsScene::willExit() {
    ofLogNotice("StemsScene::willExit");
}
