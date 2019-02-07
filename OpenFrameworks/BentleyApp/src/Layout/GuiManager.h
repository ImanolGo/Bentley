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
   // static const int GUI_WIDTH;
    
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
    
    int getWidth() {return ofxImGui::Settings().windowSize.x;}
    
    int getHeight() {return ofxImGui::Settings().windowSize.y;}
    
    ofPoint  getPosition() {return ofxImGui::Settings().windowPos;}


    
private:
    
    void setupGuiParameters();
    
    void drawRectangle();
    
    void drawGui();
    
    void setupCameraGui();
        
    
private:
    
    ofxImGui::Gui   m_gui;
        
    
    ofParameterGroup      m_parameters;
    
    ofParameterGroup m_cameraGroup;
    ofParameter<int> m_cameraMode;

    
    bool        m_showGui;  //It defines the whether the gui should be shown or not
};

//==========================================================================


