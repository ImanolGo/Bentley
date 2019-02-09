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
    
private:
    
    void setupGuiParameters();
    
    void drawRectangle();
    
    void drawGui();
    
    void setupCameraGui();
    
    void setupModesGui();
    
    void setupLeapGui();
    
    void setupLedsGui();
    
    void setupOpenCVGui();
        
    
private:
    
    ofxImGui::Gui   m_gui;
        
    
    ofParameterGroup      m_parameters;
    
    ofParameterGroup m_modeGroup;
    ofParameterGroup m_cameraGroup;
    ofParameterGroup m_leapGroup;
    ofParameterGroup m_ledsGroup;
    ofParameterGroup m_openCvGroup;
    ofParameterGroup m_realSenseGroup;
    
    ofParameter<int> m_cameraMode;
    ofParameter<int> m_leapMode;
    ofParameter<int> m_viewMode;
    ofParameter<float>      m_ledsSize;
    
    float m_width, m_height;
    ofPoint   m_position;
    
    
    bool        m_showGui;  //It defines the whether the gui should be shown or not
};

//==========================================================================


