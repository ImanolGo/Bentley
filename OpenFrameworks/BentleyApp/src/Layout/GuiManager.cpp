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
    this->setupLedsGui();
    this->setupVideoGui();
    this->setupProcessingGroup();
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



void GuiManager::setupScenesGui()
{
     auto scenesManager = &AppManager::getInstance().getSceneManager();
    
    m_scenesGroup.setName("Scenes");
    m_sceneMode.set("Scene", 0);
    m_sceneMode.addListener(scenesManager, &SceneManager::changeSceneIndex);
    m_scenesGroup.add(m_sceneMode);
    
    for(int i=0; i< scenesManager->getNumberScenes(); i++){
        m_sceneNames.push_back(scenesManager->getSceneName(i));
    }
}


void GuiManager::setupModesGui()
{
    m_modeGroup.setName("View");
    m_viewMode.set("View Mode", 0);
    m_modeGroup.add(m_viewMode);
}


void GuiManager::setupVideoGui()
{
    auto videoManager = &AppManager::getInstance().getVideoManager();
    
    m_videoPath.set("VideoPath", " ");
    m_videoPath.addListener(videoManager, &VideoManager::loadVideos);
    m_parameters.add(m_videoPath);
    m_videoGroup.add(m_videoPath);
    
}


void GuiManager::setupProcessingGroup()
{
    auto videoManager = &AppManager::getInstance().getVideoManager();
    
    m_postProcessingGroup.setName("Post Processing");
    
    m_contrast.set("Contrast", 1.0, 0.0, 2.0);
    m_contrast.addListener(videoManager, &VideoManager::setContrast);
    m_parameters.add(m_contrast);
    m_postProcessingGroup.add(m_contrast);
    
    m_saturation.set("Saturation", 1.0, 0.0, 2.0);
    m_saturation.addListener(videoManager, &VideoManager::setSaturation);
    m_parameters.add(m_saturation);
    m_postProcessingGroup.add(m_saturation);
    
    m_brightness.set("Brightness", 1.0, 0.0, 2.0);
    m_brightness.addListener(videoManager, &VideoManager::setBrightness);
    m_parameters.add(m_brightness);
    m_postProcessingGroup.add(m_brightness);
    
    m_gamma.set("Gamma", 1.0, 0.0, 2.0);
    m_gamma.addListener(videoManager, &VideoManager::setGamma);
    m_parameters.add(m_gamma);
    m_postProcessingGroup.add(m_gamma);
    
    m_blur.set("Blur", 0.0, 0.0, 2.0);
    m_blur.addListener(videoManager, &VideoManager::setBlurScale);
    m_parameters.add(m_blur);
    m_postProcessingGroup.add(m_blur);
    
    m_minInput.set("MinInput", 0.0, 0.0, 1.0);
    m_minInput.addListener(videoManager, &VideoManager::setMinInput);
    m_parameters.add(m_minInput);
    m_postProcessingGroup.add(m_minInput);
    
    m_maxInput.set("MaxInput", 1.0, 0.0, 1.0);
    m_maxInput.addListener(videoManager, &VideoManager::setMaxInput);
    m_parameters.add(m_maxInput);
    m_postProcessingGroup.add(m_maxInput);
    
    m_minOutput.set("MinOutput", 0.0, 0.0, 1.0);
    m_minOutput.addListener(videoManager, &VideoManager::setMinOutput);
    m_parameters.add(m_minOutput);
    m_postProcessingGroup.add(m_minOutput);
    
    m_maxOutput.set("MaxOutput", 1.0, 0.0, 1.0);
    m_maxOutput.addListener(videoManager, &VideoManager::setMaxOutput);
    m_parameters.add(m_maxOutput);
    m_postProcessingGroup.add(m_maxOutput);
}


void GuiManager::setupLedsGui()
{
    auto ledsManager = &AppManager::getInstance().getLedsManager();
    
    m_ledsGroup.setName("Leds");
    m_ledsSize.set("Size", 1.0, 0.0, 5.0);
    m_ledsSize.addListener(ledsManager, &LedsManager::setSize);
    m_ledsGroup.add(m_ledsSize);
    m_parameters.add(m_ledsSize);
}

void GuiManager::update()
{
    //m_gui.update();
    m_gui.setTheme(new GuiTheme());

    AppManager::getInstance().getLayoutManager().setDrawMode(m_viewMode);

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
            
//            if (ofxImGui::BeginTree(m_modeGroup, mainSettings))
//            {
//                static const std::vector<std::string> labels1 = { "Normal", "Camera", "Scene", "Leap", "Leds" };
//
//                ofxImGui::AddRadio(m_viewMode, labels1, 5);
//                ofxImGui::EndTree(mainSettings);
//            }
            
//            if (ofxImGui::BeginTree(m_scenesGroup, mainSettings))
//            {
//                ofxImGui::AddCombo(m_sceneMode, m_sceneNames);
//                ofxImGui::EndTree(mainSettings);
//            }
            
            if (ofxImGui::BeginTree(m_videoGroup, mainSettings))
            {
                ofxImGui::AddParameter(m_videoPath);
                
                if (ImGui::Button("Load Video Folder..."))
                {
                    auto dialogResult = ofSystemLoadDialog("Load Video Folder", true, ofToDataPath(""));
                    if (dialogResult.bSuccess)
                    {
                        m_videoPath = dialogResult.filePath;
                    }
                }
                
                if (ImGui::Button("Play"))
                {
                    AppManager::getInstance().getVideoManager().play();
                }
                
                if (ImGui::Button("Stop"))
                {
                    AppManager::getInstance().getVideoManager().stop();
                }
                
                if (ImGui::Button("Next"))
                {
                    AppManager::getInstance().getVideoManager().next();
                }
                
                ofxImGui::EndTree(mainSettings);
                
            }
            
            if (ofxImGui::BeginTree(m_ledsGroup, mainSettings))
            {
                ofxImGui::AddParameter(m_ledsSize);
                ofxImGui::EndTree(mainSettings);
            
            }

            
            if (ofxImGui::BeginTree(m_postProcessingGroup, mainSettings))
            {
                //auto & group =
                
                ofxImGui::AddParameter(m_postProcessingGroup.getFloat("Contrast"));
                ofxImGui::AddParameter(m_postProcessingGroup.getFloat("Saturation"));
                ofxImGui::AddParameter(m_postProcessingGroup.getFloat("Brightness"));
                ofxImGui::AddParameter(m_postProcessingGroup.getFloat("Blur"));
                ofxImGui::AddParameter(m_postProcessingGroup.getFloat("Gamma"));
                ofxImGui::AddParameter(m_postProcessingGroup.getFloat("MinInput"));
                ofxImGui::AddParameter(m_postProcessingGroup.getFloat("MaxInput"));
                ofxImGui::AddParameter(m_postProcessingGroup.getFloat("MinOutput"));
                ofxImGui::AddParameter(m_postProcessingGroup.getFloat("MaxOutput"));
                ofxImGui::EndTree(mainSettings);
            }
            
            
        }
    
        ofxImGui::EndWindow(mainSettings);
    m_gui.end();
    
    ofDisableAlphaBlending();
    
    this->updateSize(mainSettings);
    
   
}

void GuiManager::updateSize(const ofxImGui::Settings& settings)
{
    if(m_width!= settings.windowSize.x){
         m_width = settings.windowSize.x;
         AppManager::getInstance().getLayoutManager().windowResized(ofGetWidth(), ofGetHeight());
    }
   
    m_height = settings.windowSize.y;
    m_position = ofPoint(settings.windowPos.x, settings.windowPos.y);
}

void GuiManager::saveGuiValues(string path)
{
    ofLogNotice() <<"GuiManager::saveGuiValues-> loading values from: " << GUI_SETTINGS_FILE_NAME;
    
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
    
    ofLogNotice() <<"GuiManager::loadGuiValues-> loading values from: " << GUI_SETTINGS_FILE_NAME;
    
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



