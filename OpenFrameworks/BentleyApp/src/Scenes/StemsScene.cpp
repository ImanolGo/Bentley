/*
 *  StemsScene.cpp
 *  Bentley App
 *
 *  Created by Imanol Gomez on 16/06/19.
 *
 */


#include "StemsScene.h"
#include "AppManager.h"
#include "TextVisual.h"

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
    this->exportStemsColors();
    
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
    
    float hue = 0;
    float increaseHue = 360.0/6.0;
    ofColor color = ofColor::fromHsb(0, 255, 255);
    color.setHueAngle(hue);
    
    for(unsigned short brancherId = startId; brancherId<endId; brancherId++)
    {
        vector<string> stemIds;
        bool success = AppManager::getInstance().getLedsManager().getStemIdsFromBrancher(brancherId, stemIds);
        if(success){
    
            
            ofLogNotice() <<"StemsScene::setupRectangles << brancher id = : " << brancherId ;
            
            
            
            for(auto stemId : stemIds)
            {
                //ofLogNotice() <<"StemsScene::setupRectangles << brancher color hue = : " << hue ;
                
                //ofLogNotice() <<"StemsScene::setupRectangles << stemId = : " << stemId ;
                
                m_stemsColorList.push_back(make_pair(stemId, color));
                
                for(int i = 0; i<m_numPixels; i++)
                {
                    auto pos = ofPoint(-width, -height);
                    success = AppManager::getInstance().getLedsManager().get2dPositionFromStem(brancherId, stemId, i, pos);
                    
                    if(success)
                    {
                        auto rectangle = make_shared<RectangleVisual>(pos,w,h,true);
                        rectangle->setColor(color);
                        m_rectangles.push_back(rectangle);

                        //ofLogNotice() <<"StemsScene::setupRectangles << id = : " << _id ;
                    }
                    
                }
                
                hue += increaseHue;
                hue = fmod(hue, 360);
                
                color.setHueAngle(hue);
                //ofLogNotice() <<"StemsScene::setupRectangles << hue = : " << hue ;
                //ofLogNotice() <<"StemsScene::setupRectangles << color = : " << color ;
               
            }
            
        }
    }
}

void StemsScene::exportStemsColors()
{
    ofPixels pix;
    
    float h = 60;
    float width = 100;
    float height = h*m_stemsColorList.size();
    
    ofFbo fbo;
    fbo.allocate(width, height);
    fbo.begin();
    ofClear(0, 0, 0, 255);
    ofSetColor(255);
    
    ofPoint pos(width*0.5, h*0.5);
    ofPoint posRect(0, 0);
    TextVisual text(pos, width, h, true);
    
    string fontName = "fonts/roboto/Roboto-Bold.ttf";
    float fontSize = h*0.3;
    text.setText("Test", fontName,  fontSize, ofColor(255));
    
    RectangleVisual rect(posRect, width, h, false);
    
    for(int i = 0; i<m_stemsColorList.size(); i++){
       
        // ofLogNotice() <<"StemsScene::exportStemsColors << stemId = : " << m_stemsColorList[i].first ;
        //ofLogNotice() <<"StemsScene::exportStemsColors << color = : " << m_stemsColorList[i].first ;
        
        posRect.y = i*h;
        rect.setColor(m_stemsColorList[i].second);
        rect.setPosition(posRect);
        rect.draw();
        
        pos.y = h*0.5 + i*h;
        text.setPosition(pos);
        text.setText(m_stemsColorList[i].first);
        text.setPosition(pos);
        text.draw();
        
    }
    
    fbo.end();
    
    fbo.readToPixels(pix);
    ofSaveImage(pix, "images/layout/stems.png");
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
