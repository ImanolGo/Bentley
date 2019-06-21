/*
 *  GuiManager.h
 *  Bentley App
 *
 *  Created by Imanol Gomez on 06/02/19.
 *
 */


#pragma once

#include "Manager.h"
#include "ofxImGui.h"


//========================== class GuiManager ==============================
//============================================================================
/** \class GuiManager GuiManager.h
 *    \brief Class managing the application´s grapical user interface (GUI)
 *    \details It creates the gui and the callback functions that will be affected
 *    by the gui
 */

class GuiManager: public Manager
{
    static const string GUI_SETTINGS_FILE_NAME;
    static const string GUI_SETTINGS_NAME;
    static const int GUI_WIDTH;
    
public:
    
    //! Constructor
    GuiManager();
    
    //! Destructor
    ~GuiManager();
    
    //! Set-up the gui
    void setup();
    
    //! update the gui
    void update();
    
    //! Draw the gui
    void draw();
    
    void saveGuiValues(string path = "");
    
    void loadGuiValues(string path = "");
    
    void toggleGui();
    
    void showGui(bool show){m_showGui=show;}
    
    int getWidth() {return m_width;}
    
    int getHeight() {return m_height;}
    
    ofPoint  getPosition() {return m_position;}

    float getLedsSize() const {return m_ledsSize;}
    
    void setVideoPath(const string& path) {m_videoPath = ofToDataPath(path);}
    
    void setupScenesGui();
    
    void onSceneChange(int sceneIndex);
    
    void onVideoChange(int videoIndex);
    
    ofFloatColor getSolidColor() const{return m_solidColor.get();}
    
    float getShaderSpeed() const {return m_shaderSpeed.get();}
    
    float getShaderParameter() const{return m_shaderParameter.get();}
    
    int getShaderDirection() const {return m_shaderDirection.get();}
    
private:
    
    void setupGuiParameters();
    
    void drawRectangle();
    
    void drawGui();
    
    void setupVideoGui();
    
    void setupShadersGui();
    
    void setupModesGui();
    
    void setupCommunicationsGui();
    
    void setupLedsGui();

    void setupProcessingGroup();
    
    void updateSize(const ofxImGui::Settings& settings);
    
private:
    
    ofxImGui::Gui   m_gui;
        
    
    ofParameterGroup      m_parameters;
    
    ofParameterGroup m_modeGroup;
    ofParameterGroup m_videoGroup;
    ofParameterGroup m_ledsGroup;
    ofParameterGroup m_communications;
    ofParameterGroup m_scenesGroup;
    ofParameterGroup m_postProcessingGroup;
    ofParameterGroup m_shadersGroup;
    ofParameterGroup m_communicationsGroup;
    
    ofParameter<int>      m_fps;
    
    ofParameter<float>      m_shaderSpeed;
    ofParameter<float>      m_shaderParameter;
    ofParameter<int>        m_shaderDirection;
    
    ofParameter<string>     m_videoPath;
    ofParameter<int>        m_viewMode;
    ofParameter<int>        m_sceneMode;
    ofParameter<float>      m_ledsSize;
    ofParameter<float>      m_servoPosition;
    ofParameter<bool>       m_manualServo;
    ofParameter<int>        m_ledsBCR;
    ofParameter<int>        m_ledsBCG;
    ofParameter<int>        m_ledsBCB;
    ofParameter<int>        m_dotstarsBC;
    ofParameter<int>        m_aiBC;
    
    ofParameter<int>    m_maxDataPacketSize;
    ofParameter<bool>             m_streaming;
    
    ofParameter<ofFloatColor>  m_solidColor;
    
    std::vector<std::string>    m_sceneNames;
    
    ofParameter<float>      m_contrast;
    ofParameter<float>      m_saturation;
    ofParameter<float>      m_brightness;
    ofParameter<float>      m_gamma;
    ofParameter<float>      m_minInput;
    ofParameter<float>      m_maxInput;
    ofParameter<float>      m_minOutput;
    ofParameter<float>      m_maxOutput;
    ofParameter<float>      m_blur;
    
    float m_width, m_height;
    ofPoint   m_position;
    string m_textOsc, m_textTcp, m_textIP;
    
    
    bool        m_showGui;  //It defines the whether the gui should be shown or not
};

//==========================================================================


