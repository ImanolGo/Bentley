/*
 *  GuiManager.cpp
 *  Bentley Gestures App
 *
 *  Created by Imanol Gomez on 19/11/18.
 *
 */


#include "ofMain.h"

#include "AppManager.h"

#include "GuiManager.h"
#include "GuiTheme.h"


const string GuiManager::GUI_SETTINGS_FILE_NAME = "xmls/GuiSettings.xml";
const string GuiManager::GUI_SETTINGS_NAME = "GUI";
//const int GuiManager::GUI_WIDTH = 350;


GuiManager::GuiManager(): Manager(), m_showGui(true)
{
    //Intentionally left empty
}


GuiManager::~GuiManager()
{
    this->saveGuiValues();
    ofLogNotice() <<"GuiManager::Destructor";
}


void GuiManager::setup()
{
    if(m_initialized)
        return;
    
    Manager::setup();
    
    
    this->setupGuiParameters();
    this->loadGuiValues();

    
    ofLogNotice() <<"GuiManager::initialized";
    
}

void GuiManager::setupGuiParameters()
{
    
    m_gui.setup(new GuiTheme());
    ofxImGui::Settings().windowPos  = ofVec2f(500,500);
    ofxImGui::Settings().windowSize = ofVec2f(300,ofGetHeight());
    
    //ImGui::GetIO().MouseDrawCursor = false;
    
//    //m_gui.setAssetPath( ofFilePath::getCurrentExeDir() + "/../Resources/data");
//    m_gui.setAssetPath(ofToDataPath("fonts/"));
//    //m_gui.setAssetPath("../Resources/data/fonts/");
//    m_gui.setTheme(new GuiTheme());
//
//
//   // int margin =  LayoutManager::MARGIN;
//    m_gui.setAutoDraw(false);
//    //auto pos = m_gui.getPosition();
//    //m_gui.setPosition(pos.x + margin, pos.y + margin);
//    m_gui.addHeader(GUI_SETTINGS_NAME, false);
//
//    m_gui.addFRM(0.1);
//
//    m_gui.addBreak();
}





void GuiManager::update()
{
    //m_gui.update();
    m_gui.setTheme(new GuiTheme());
}


void GuiManager::draw()
{
    if(!m_showGui)
        return;
    
    //this->drawRectangle();
    this->drawGui();
}


void GuiManager::drawGui()
{
    ofEnableAlphaBlending();
    m_gui.begin();
    
        auto mainSettings = ofxImGui::Settings();
        mainSettings.windowPos  = ofVec2f(0,0);
        mainSettings.windowSize = ofVec2f(300,ofGetHeight());
        if (ofxImGui::BeginWindow("GUI", mainSettings, false))
        {
            ImGui::Text("%.1f FPS (%.3f ms/frame)", ofGetFrameRate(), 1000.0f / ImGui::GetIO().Framerate);
            
        }
        ofxImGui::EndWindow(mainSettings);
    m_gui.end();
    
    ofDisableAlphaBlending();
}


void GuiManager::saveGuiValues(string path)
{
    ofXml xml;
    ofSerialize(xml, m_parameters);
    //xml.serialize(m_parameters);
    
    if(path.empty()){
        xml.save(GUI_SETTINGS_FILE_NAME);
    }
    else{
        xml.save(path);
    }
    
    
}

void GuiManager::loadGuiValues(string path)
{
    ofXml xml;
    if(path.empty()){
         xml.load(GUI_SETTINGS_FILE_NAME);
    }
    else{
         xml.load(path);
    }
    
    //xml.deserialize(m_parameters);
    ofDeserialize(xml, m_parameters);
}



void GuiManager::toggleGui()
{
    m_showGui = !m_showGui;
}

void GuiManager::drawRectangle()
{
    int margin =  LayoutManager::MARGIN;
    ofPushStyle();
    ofSetColor(15);
    ofDrawRectangle( this->getPosition().x - margin, 0, this->getWidth() + 2*margin, ofGetHeight());
    ofPopStyle();
}



