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
    this->setupScenesGui();
    //this->setupModesGui();
    this->setupCommunicationsGui();
    this->setupLedsGui();
    this->setupVideoGui();
    this->setupProcessingGroup();
    this->setupShadersGui();
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
    
    m_textIP =  "IP Address: " + AppManager::getInstance().getUdpManager().getIpAddress();
    m_textTcp = "TCP Port: " + ofToString(AppManager::getInstance().getSettingsManager().getTcpPortReceive());
    m_textOsc = "OSC Port: " + ofToString(AppManager::getInstance().getSettingsManager().getOscPortReceive());
    
    
    auto appManager = &AppManager::getInstance();
    
    m_fps.set("Frame Rate", 60, 0, 60);
    m_fps.addListener(appManager, &AppManager::changeFrameRate);
    m_parameters.add(m_fps);
}



void GuiManager::setupScenesGui()
{
     auto scenesManager = &AppManager::getInstance().getSceneManager();
    
    m_sceneNames.clear();
    m_scenesGroup.setName("Scenes");
    m_sceneMode.set("Scene", 0);
    m_sceneMode.addListener(scenesManager, &SceneManager::changeSceneIndex);
    m_scenesGroup.add(m_sceneMode);
    
    for(int i=0; i< scenesManager->getNumberScenes(); i++){
        m_sceneNames.push_back(scenesManager->getSceneName(i));
    }
    
    m_solidColor.set( "Color", ofFloatColor::white );
    m_parameters.add(m_solidColor);
    
    m_sceneTransitionTime.set("TransitionTime", 0.5, 0.0, 10);
    m_sceneTransitionTime.addListener(scenesManager, &SceneManager::onTransitionTimeChange);
    m_parameters.add(m_sceneTransitionTime);
}

void GuiManager::onSceneChange(int sceneIndex)
{
    int size = AppManager::getInstance().getSceneManager().getNumberScenes();
    ofLogNotice() <<"GuiManager::onSceneChange << size -> " << size;

    if(sceneIndex >=0 && sceneIndex < size){
        m_sceneMode = sceneIndex;
        ofLogNotice() <<"GuiManager::onSceneChange << m_sceneMode -> " << m_sceneMode;
    }
}


void GuiManager::onVideoChange(int videoIndex)
{
    int size = AppManager::getInstance().getSceneManager().getNumberScenes();
    int offset = AppManager::getInstance().getSceneManager().getSceneOffset();
    videoIndex+= offset;
    if(videoIndex >=offset && videoIndex < size){
        m_sceneMode = videoIndex;
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
    
    m_videoGroup.setName("Videos");
    m_videoPath.set("VideoPath", " ");
    m_videoPath.addListener(videoManager, &VideoManager::loadVideos);
    m_parameters.add(m_videoPath);
    m_videoGroup.add(m_videoPath);
    
}


void GuiManager::setupShadersGui()
{
    m_shadersGroup.setName("Shaders");
    
    m_shaderSpeed.set("Speed", 1.0, 0.0, 2.0);
    m_parameters.add(m_shaderSpeed);
    m_shadersGroup.add(m_shaderSpeed);
    
    m_shaderParameter.set("Parameter", 1.0, 0.0, 5.0);
    m_parameters.add(m_shaderParameter);
    m_shadersGroup.add(m_shaderParameter);
    
    m_shaderDirection.set("Direction", 0, 0, 3);
    m_parameters.add(m_shaderDirection);
    m_shadersGroup.add(m_shaderDirection);
    
}


void GuiManager::setupProcessingGroup()
{
    auto scenesManager = &AppManager::getInstance().getSceneManager();
    
    m_postProcessingGroup.setName("Post Processing");
    
    m_contrast.set("Contrast", 1.0, 0.0, 2.0);
    m_contrast.addListener(scenesManager, &SceneManager::setContrast);
    m_parameters.add(m_contrast);
    m_postProcessingGroup.add(m_contrast);
    
    m_saturation.set("Saturation", 1.0, 0.0, 2.0);
    m_saturation.addListener(scenesManager, &SceneManager::setSaturation);
    m_parameters.add(m_saturation);
    m_postProcessingGroup.add(m_saturation);
    
    m_brightness.set("Brightness", 1.0, 0.0, 2.0);
    m_brightness.addListener(scenesManager, &SceneManager::setBrightness);
    m_parameters.add(m_brightness);
    m_postProcessingGroup.add(m_brightness);
    
    m_gamma.set("Gamma", 1.0, 0.0, 2.0);
    m_gamma.addListener(scenesManager, &SceneManager::setGamma);
    m_parameters.add(m_gamma);
    m_postProcessingGroup.add(m_gamma);
    
    m_minInput.set("MinInput", 0.0, 0.0, 1.0);
    m_minInput.addListener(scenesManager, &SceneManager::setMinInput);
    m_parameters.add(m_minInput);
    m_postProcessingGroup.add(m_minInput);
    
    m_maxInput.set("MaxInput", 1.0, 0.0, 1.0);
    m_maxInput.addListener(scenesManager, &SceneManager::setMaxInput);
    m_parameters.add(m_maxInput);
    m_postProcessingGroup.add(m_maxInput);
    
    m_minOutput.set("MinOutput", 0.0, 0.0, 1.0);
    m_minOutput.addListener(scenesManager, &SceneManager::setMinOutput);
    m_parameters.add(m_minOutput);
    m_postProcessingGroup.add(m_minOutput);
    
    m_maxOutput.set("MaxOutput", 1.0, 0.0, 1.0);
    m_maxOutput.addListener(scenesManager, &SceneManager::setMaxOutput);
    m_parameters.add(m_maxOutput);
    m_postProcessingGroup.add(m_maxOutput);
}


void GuiManager::setupLedsGui()
{
    auto ledsManager = &AppManager::getInstance().getLedsManager();
    auto sceneManager = &AppManager::getInstance().getSceneManager();
    
    m_ledsGroup.setName("Leds");
    m_ledsSize.set("Size", 1.0, 0.0, 20.0);
    m_ledsSize.addListener(ledsManager, &LedsManager::setSize);
    m_ledsGroup.add(m_ledsSize);
    m_parameters.add(m_ledsSize);
    
    m_manualServo.set("Manual Servo", false);
    m_manualServo.addListener(sceneManager, &SceneManager::setManualServo);
    m_ledsGroup.add(m_manualServo);
    m_parameters.add(m_manualServo);
    
    m_servoPosition.set("Servo", 0.0, 0.0, 1.0);
    m_servoPosition.addListener(sceneManager, &SceneManager::setServoPosition);
    m_ledsGroup.add(m_servoPosition);
    m_parameters.add(m_servoPosition);
    
    m_aiBC.set("AI", 3, 0, 127);
    m_aiBC.addListener(ledsManager, &LedsManager::setAI);
    m_ledsGroup.add(m_aiBC);
    m_parameters.add(m_aiBC);
    
    m_dotstarsBC.set("DOTSTARS", 10, 0, 127);
    m_dotstarsBC.addListener(ledsManager, &LedsManager::setDotStars);
    m_ledsGroup.add(m_dotstarsBC);
    m_parameters.add(m_dotstarsBC);
    
    m_ledsBCR.set("BCR", 10, 0, 127);
    m_ledsBCR.addListener(ledsManager, &LedsManager::setBCR);
    m_ledsGroup.add(m_ledsBCR);
   // m_parameters.add(m_ledsBCR);
    
    m_ledsBCG.set("BCG", 10, 0, 127);
    m_ledsBCG.addListener(ledsManager, &LedsManager::setBCG);
    m_ledsGroup.add(m_ledsBCG);
    //m_parameters.add(m_ledsBCG);
    
    m_ledsBCB.set("BCB", 10, 0, 127);
    m_ledsBCB.addListener(ledsManager, &LedsManager::setBCB);
    m_ledsGroup.add(m_ledsBCB);
    //m_parameters.add(m_ledsBCB);
    
    
    auto brancherIds = AppManager::getInstance().getLedsManager().gerBrancherIds();
    
    for(auto id: brancherIds){
        string name = "Brancher " + ofToString(id-100);
        m_brancherNames.push_back(name);
    }
    
    m_brancherMode.set("Brancher", 0);
    m_brancherMode.addListener(ledsManager, &LedsManager::changeBrancher);
    m_ledsGroup.add(m_brancherMode);
    
    int brancherIndex = m_brancherMode.get();
    AppManager::getInstance().getLedsManager().changeBrancher(brancherIndex);

}

void GuiManager::setupCommunicationsGui()
{
    auto udpManager = &AppManager::getInstance().getUdpManager();
    
    m_maxDataPacketSize.set("MaxDataSize", 1450, 100, 1500);
    m_maxDataPacketSize.addListener(udpManager, &UdpManager::setMaxDataPacketSize);
    m_communicationsGroup.add(m_maxDataPacketSize);
    m_parameters.add(m_maxDataPacketSize);
    
    m_communicationsGroup.setName("Communications");
    m_streaming.set("Streaming", true);
    m_streaming.addListener(udpManager, &UdpManager::setStreaming);
    m_communicationsGroup.add(m_streaming);
    m_parameters.add(m_streaming);
    
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
            ofxImGui::AddParameter(m_fps);
            ImGui::Text("%.1f FPS (%.3f ms/frame)", ofGetFrameRate(), 1000.0f / ImGui::GetIO().Framerate);
            
            int oscPort = AppManager::getInstance().getSettingsManager().getOscPortReceive();
            
            ImGui::Text(m_textIP.c_str());
            ImGui::Text(m_textTcp.c_str());
            ImGui::Text(m_textOsc.c_str());
            
           
            if (ofxImGui::BeginTree(m_scenesGroup, mainSettings))
            {
                ofxImGui::AddParameter(m_sceneTransitionTime);
                ofxImGui::AddParameter(m_solidColor);
                ofxImGui::AddCombo(m_sceneMode, m_sceneNames);
                ofxImGui::EndTree(mainSettings);
            }
            
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
                
                
                ofxImGui::EndTree(mainSettings);
                
            }
            
            if (ofxImGui::BeginTree(m_communicationsGroup, mainSettings))
            {
                ofxImGui::AddParameter(m_maxDataPacketSize);
                
                ofxImGui::AddParameter(m_streaming);
                if (ImGui::Button("Next Frame..."))
                {
                    AppManager::getInstance().getUdpManager().nextFrame();
                }
                
                ofxImGui::EndTree(mainSettings);
                
                
            }
            
            if (ofxImGui::BeginTree(m_ledsGroup, mainSettings))
            {
                ofxImGui::AddParameter(m_ledsSize);
                ofxImGui::AddParameter(m_manualServo);
                ofxImGui::AddParameter(m_servoPosition);
                ofxImGui::AddCombo(m_brancherMode, m_brancherNames);
                //ofxImGui::AddParameter(m_aiBC);
                //ofxImGui::AddParameter(m_dotstarsBC);
                ofxImGui::AddParameter(m_ledsBCR);
                ofxImGui::AddParameter(m_ledsBCG);
                ofxImGui::AddParameter(m_ledsBCB);
                ofxImGui::EndTree(mainSettings);
            }

            
            if (ofxImGui::BeginTree(m_postProcessingGroup, mainSettings))
            {
                //auto & group =
                
                ofxImGui::AddParameter(m_postProcessingGroup.getFloat("Contrast"));
                ofxImGui::AddParameter(m_postProcessingGroup.getFloat("Saturation"));
                ofxImGui::AddParameter(m_postProcessingGroup.getFloat("Brightness"));
                //ofxImGui::AddParameter(m_postProcessingGroup.getFloat("Blur"));
                ofxImGui::AddParameter(m_postProcessingGroup.getFloat("Gamma"));
                ofxImGui::AddParameter(m_postProcessingGroup.getFloat("MinInput"));
                ofxImGui::AddParameter(m_postProcessingGroup.getFloat("MaxInput"));
                ofxImGui::AddParameter(m_postProcessingGroup.getFloat("MinOutput"));
                ofxImGui::AddParameter(m_postProcessingGroup.getFloat("MaxOutput"));
                ofxImGui::EndTree(mainSettings);
            }
            
            if (ofxImGui::BeginTree(m_shadersGroup, mainSettings))
            {
                //auto & group =
                
                ofxImGui::AddParameter(m_shadersGroup.getFloat("Speed"));
                ofxImGui::AddParameter(m_shadersGroup.getFloat("Parameter"));
                ofxImGui::AddParameter(m_shadersGroup.getInt("Direction"));
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
    ofLogNotice() <<"GuiManager::saveGuiValues-> saving values from: " << GUI_SETTINGS_FILE_NAME;
    
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

void GuiManager::setCurrentSettings(int bcr, int bcg, int bcb)
{
    m_ledsBCR = bcr;
    m_ledsBCG = bcg;
    m_ledsBCB = bcb;
}



