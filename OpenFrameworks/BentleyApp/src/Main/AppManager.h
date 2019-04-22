/*
 *  AppManager.h
 *  Bentley App
 *
 *  Created by Imanol Gomez on 06/02/19.
 *
 */


#pragma once

#include "ofMain.h"

#include "managers.h"

//========================== class AppManager ==============================
//============================================================================
/** \class AppManager AppManager.h
 *	\brief Class managing the whole application
 *	\details it set-ups, updates and renders all the different managers used for the application
 */

class AppManager: public Manager
{
public:

    //! Destructor
    ~AppManager();

    //! Returns the singleton instance.
    static AppManager& getInstance();

    //! Compares two transition objects
    void setup();

    //! updates the logic
    void update();

    //! calls the view manager to draw
    void draw();

    //==========================================================================

    //! Returns the settings manager
    SettingsManager& getSettingsManager() { return m_settingsManager; }
    
    //! Returns the layout manager
    LayoutManager& getLayoutManager() { return m_layoutManager; }
    
    //! Returns the resource manager
    ResourceManager& getResourceManager() { return m_resourceManager; }
    
    //! Returns the view manager
    ViewManager&  getViewManager() { return m_viewManager; }
    
    //! Returns the visual effects manager
    VisualEffectsManager&  getVisualEffectsManager() { return m_visualEffectsManager; }
    
    //! Returns the GUI manager
    GuiManager&   getGuiManager() { return m_guiManager; }
    
    //! Returns the cam manager
    CamManager&  getCamManager() { return m_camManager; }
    
    //! Returns the udp manager
    UdpManager&  getUdpManager() { return m_udpManager; }
    
    //! Returns the leds manager
    LedsManager&  getLedsManager() { return m_ledsManager; }
    
    //! Returns the model manager
    ModelManager&  getModelManager() { return m_modelManager; }
    
    //! Returns the scenes manager
    SceneManager&  getSceneManager() { return m_sceneManager; }
    
    //! Returns the VideoManager manager
    VideoManager&  getVideoManager() { return m_videoManager; }

    
    //==========================================================================
    
    void toggleDebugMode();
    
    void setDebugMode(bool showDebug);
    

private:

     //! Constructor
     AppManager();

	//! Stop the compiler generating methods of copy the object
	 AppManager(AppManager const& copy);              // Don't Implement

    //! Prevent operator= being generated.
     AppManager& operator=(AppManager const& copy);     // Don't implement

    //==========================================================================

    //! Set-up all the managers
    void setupManagers();

    //! Set-up openFrameworks
    void setupOF();

    //! update all the managers
    void updateManagers();

private:


    SettingsManager          m_settingsManager;          ///< Manages the application's settings
    ResourceManager          m_resourceManager;          ///< Manages the application's resources
    ViewManager              m_viewManager;              ///< Manages visuals
    VisualEffectsManager     m_visualEffectsManager;     ///< Manages visual effects
    LayoutManager            m_layoutManager;            ///< Manages the layout
    KeyboardManager          m_keyboardManager;          ///< Manages the keyboard
    GuiManager               m_guiManager;               ///< Manages the graphical user interface

    CamManager               m_camManager;               ///< Manages the web cam and processing
    ModelManager             m_modelManager;             ///< Manages the model and processing

    SceneManager             m_sceneManager;             ///< Manages all the scenes
    UdpManager               m_udpManager;               ///< Manages the udp communications
    LedsManager              m_ledsManager;              ///< Manages the leds
    VideoManager             m_videoManager;             ///< Manages the video playback
    
    
    bool                     m_debugMode;
    bool                     m_initialized;
    
};

//==========================================================================


