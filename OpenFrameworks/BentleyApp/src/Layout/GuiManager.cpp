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
const int GuiManager::GUI_WIDTH = 300;


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
    this->setupModesGui();
    this->setupCameraGui();
    this->setupLedsGui();
    this->setupLeapGui();
    this->setupOpenCVGui();
    this->loadGuiValues();
    
    //this->drawGui();

    
    ofLogNotice() <<"GuiManager::initialized";
    
}

void GuiManager::setupGuiParameters()
{
    //create an actual ImGui context before setting up the addon
    ImGui::CreateContext();
    
    //specify a font to use
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF(&ofToDataPath("fonts/roboto/Roboto-Regular.ttf")[0], 16.f);
    
    //ImGui::GetIO().FontGlobalScale = 1.0 / 2.0;
    
    //finally setup the addon ofxImGui::Gui setup;
    m_gui.setup(new GuiTheme());
    ofxImGui::Settings().windowPos  = ofVec2f(0,0);
    ofxImGui::Settings().windowSize = ofVec2f(GUI_WIDTH,ofGetHeight());
    
    m_width = 0;
    m_height = 0;

}


void GuiManager::setupModesGui()
{
    m_modeGroup.setName("View");
    m_viewMode.set("View Mode", 0);
    m_modeGroup.add(m_viewMode);
}

void GuiManager::setupLeapGui()
{
    m_leapGroup.setName("Leap");
    m_leapMode.set("Leap Mode", 0);
    m_leapGroup.add(m_leapMode);
}

void GuiManager::setupCameraGui()
{
    m_cameraGroup.setName("Camera");
    m_cameraMode.set("Camera Mode", 1);
    m_cameraGroup.add(m_cameraMode);
    
    ofxGuiGroup* group = AppManager::getInstance().getRealSenseManager().getGui();
    m_cameraGroup.add(group->getIntSlider("IR Exposure").getParameter());
    m_cameraGroup.add(group->getFloatSlider("Min Depth").getParameter());
    m_cameraGroup.add(group->getFloatSlider("Max Depth").getParameter());
    m_cameraGroup.add(group->getToggle("Auto exposure").getParameter());
    m_cameraGroup.add(group->getToggle("Emitter").getParameter());

}

void GuiManager::setupOpenCVGui()
{
    m_openCvGroup = AppManager::getInstance().getRealSenseManager().getParameters();
}


void GuiManager::setupLedsGui()
{
    auto ledsManager = &AppManager::getInstance().getLedsManager();
    
    m_ledsGroup.setName("Leds");
    m_ledsSize.set("Size", 1.0, 0.0, 5.0);
    m_ledsSize.addListener(ledsManager, &LedsManager::setSize);
    m_ledsGroup.add(m_ledsSize);
}

void GuiManager::update()
{
    //m_gui.update();
    m_gui.setTheme(new GuiTheme());
    AppManager::getInstance().getLayoutManager().setCameraMode(m_cameraMode);
    AppManager::getInstance().getLayoutManager().setDrawMode(m_viewMode);
    AppManager::getInstance().getLayoutManager().setLeapMode(m_leapMode);
}


void GuiManager::draw()
{
    if(!m_showGui)
        return;
    
    this->drawRectangle();
    this->drawGui();
}


void GuiManager::drawGui()
{
    ofEnableAlphaBlending();
   
    m_gui.begin();
        auto mainSettings  = ofxImGui::Settings();
        //ofxImGui::Settings().windowPos  = ofVec2f(-LayoutManager::MARGIN,-LayoutManager::MARGIN);
       // ofxImGui::Settings().windowSize = ofVec2f(GUI_WIDTH,ofGetHeight());
        if (ofxImGui::BeginWindow("GUI", mainSettings, false))
        {
            ImGui::Text("%.1f FPS (%.3f ms/frame)", ofGetFrameRate(), 1000.0f / ImGui::GetIO().Framerate);
            
            if (ofxImGui::BeginTree(m_modeGroup, mainSettings))
            {
                static const std::vector<std::string> labels1 = { "Normal", "Camera", "Scene", "Leap", "Leds" };

                ofxImGui::AddRadio(m_viewMode, labels1, 5);
                ofxImGui::EndTree(mainSettings);
            }
            
            if (ofxImGui::BeginTree(m_cameraGroup, mainSettings))
            {
                if(AppManager::getInstance().getRealSenseManager().isRealSenseActive()){
                    static const std::vector<std::string> labels2 = { "Depth", "IR", "Color" };
                    
                    ofxImGui::AddRadio(m_cameraMode, labels2, 3);
                    ofxImGui::AddParameter(m_cameraGroup.getInt("IR Exposure"));
                    ofxImGui::AddParameter(m_cameraGroup.getFloat("Min Depth"));
                    ofxImGui::AddParameter(m_cameraGroup.getFloat("Max Depth"));
                    ofxImGui::AddParameter(m_cameraGroup.getBool("Auto exposure"));
                    ofxImGui::AddParameter(m_cameraGroup.getBool("Emitter"));
                    ofxImGui::EndTree(mainSettings);
                }
        
            }
            
            if (ofxImGui::BeginTree(m_openCvGroup, mainSettings))
            {
                //auto & group =
                
                ofxImGui::AddParameter(m_openCvGroup.getFloat("Learning Time"));
                ofxImGui::AddParameter(m_openCvGroup.getFloat("Threshold Value"));
                ofxImGui::AddParameter(m_openCvGroup.getFloat("Min area"));
                ofxImGui::AddParameter(m_openCvGroup.getFloat("Max area"));
                ofxImGui::AddParameter(m_openCvGroup.getFloat("Threshold"));
                ofxImGui::AddParameter(m_openCvGroup.getBool("Holes"));
                ofxImGui::AddParameter(m_openCvGroup.getBool("Reset Background"));
                ofxImGui::EndTree(mainSettings);
            }
            
            if (ofxImGui::BeginTree(m_leapGroup, mainSettings))
            {
                static const std::vector<std::string> labels3 = { "Camera", "Hands"};
                ofxImGui::AddRadio(m_leapMode, labels3, 2);
                ofxImGui::EndTree(mainSettings);
            }
            
        }
    
        ofxImGui::EndWindow(mainSettings);
    m_gui.end();
    
    ofDisableAlphaBlending();
    
    m_width = mainSettings.windowSize.x;
    m_height = mainSettings.windowSize.y;
    m_position = ofPoint(mainSettings.windowPos.x, mainSettings.windowPos.y);
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
    ofPushStyle();
    ofSetColor(15);
    ofDrawRectangle( 0, 0, this->getWidth(), ofGetHeight());
    ofPopStyle();
}



