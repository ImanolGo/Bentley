/*
 *  SceneManager.h
 *  Bentley App
 *
 *  Created by Imanol Gomez on 06/02/19.
 *
 */

#pragma once

#include "Manager.h"
#include "ofxSceneManager.h"
#include "ofxSimpleTimer.h"
#include "ofxFastFboReader.h"
#include "ofxPSLevels.h"


//========================== class SceneManager ==============================
//============================================================================
/** \class SceneManager SceneManager.h
 *	\brief Class managing the scenes of the application
 *	\details It creates and manages all the scenes 
 */


class SceneManager: public Manager
{
    
public:

    //! Constructor
    SceneManager();

    //! Destructor
    ~SceneManager();

    //! Set-up the scenes
    void setup();
    
    //! Update the scenes
    void update();
    
    //! Draw the sceneManager
    void draw();
    
    //! Draw the sceneManager within a window rectangle
    void draw(const ofRectangle& rect);
    
    //! Changes the scene according to a name
    void changeScene(string sceneName);
    
    //! Changes the scene according to a index
    void changeSceneIndex(int& sceneIndex);
    
    //! changes transition time
    void onTransitionTimeChange(float value);
    
    //! Returns the number of total secenes
    int getNumberScenes(){return m_mySceneManager.scenes.size();};
    
    //! Returns the name of a scene given the index
    string getSceneName(int sceneIndex);
    
    //! Returns the index a scene given a name. It returns -1 if it doesn't find any
    int getIndex(string& sceneName);
    
    const ofFbo& getFbo() {return m_fbo;}
    
    void sceneTimerCompleteHandler( int &args ) ;
    
    void onChangeSceneDuration(float& value);
    
    const string& getCurrentSceneName() const {return m_currentSceneName;}
    
    void setContrast(float& value) {m_levels.contrast = value;}
    
    void setSaturation(float& value) {m_levels.saturation = value;}
    
    void setBrightness(float& value) {m_levels.brightness = value;}
    
    void setGamma(float& value) {m_levels.gamma = value;}
    
    void setMinInput(float& value) {m_levels.minInput = value;}
    
    void setMaxInput(float& value) {m_levels.maxInput = value;}
    
    void setMinOutput(float& value) {m_levels.minOutput = value;}
    
    void setMaxOutput(float& value) {m_levels.maxOutput = value;}

    void removeVideos();
    
    void addVideos();
    
    void setVideoIndex(int value);
    
    int getSceneOffset() const {return m_sceneOffset;}
    
    int getCurrentVideoIndex() const {return m_currentVideoIndex;}
    
    bool getCurrentStatus() const {return m_status;}
    
private:
    
    //! Create the scenes
    void createScenes();
    
    //! Set up the fbo that saves the texture.
    void setupFbo();
    
    void setupLevels();
    
    //! Set up the scene timer
    void setupTimer();
    
    //! Update the scene timer
    void updateTimer();
    
    //! updates all the scenes
    void updateScenes();
    
    //! updates the fbo
    void updateFbo();
    
    //! updates the fbo
    void updatePixels();
    
    void sendSceneChange();

private:

    ofxSceneManager          m_mySceneManager;
    ofFbo                    m_fbo;
    ofxSimpleTimer           m_sceneTimer;
    float                    m_alpha;
    ofxFastFboReader         m_reader;
    ofxPSLevels              m_levels;
    ofParameterGroup         m_parameters;
    
    vector<string>           m_sceneList;
    string                   m_currentSceneName;
    float                    m_transitionTime;
    int                      m_sceneOffset;
    bool                     m_status;
    int                      m_currentVideoIndex;
    
};

//==========================================================================


