/*
 *  GuiManager.cpp
 *  Bentley App
 *
 *  Created by Imanol Gomez on 06/02/19.
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
    this->setupCameraGui();
    this->loadGuiValues();

    
    ofLogNotice() <<"GuiManager::initialized";
    
}

void GuiManager::setupGuiParameters()
{
    //create an actual ImGui context before setting up the addon
    ImGui::CreateContext();
    
    //specify a font to use
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF(&ofToDataPath("fonts/roboto/Roboto-Regular.ttf")[0], 18.f);
    
    //ImGui::GetIO().FontGlobalScale = 1.0 / 2.0;
    
    //finally setup the addon ofxImGui::Gui setup;
    m_gui.setup(new GuiTheme());
    ofxImGui::Settings().windowPos  = ofVec2f(500,500);
    ofxImGui::Settings().windowSize = ofVec2f(300,ofGetHeight());

}



void GuiManager::setupCameraGui()
{
    m_cameraGroup.setName("Camera");
    m_cameraMode.set("Camera Mode", 0);
    m_cameraGroup.add(m_cameraMode);
}

void GuiManager::update()
{
    //m_gui.update();
    m_gui.setTheme(new GuiTheme());
    AppManager::getInstance().getLayoutManager().setCameraMode(m_cameraMode);
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
            
            if (ofxImGui::BeginTree(m_cameraGroup, mainSettings))
            {
                static const std::vector<std::string> labels = { "WebCam", "LeapCam", "Hands" };
                
                ofxImGui::AddRadio(m_cameraMode, labels, 3);
                ofxImGui::EndTree(mainSettings);
            }
            
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



