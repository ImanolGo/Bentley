/*
 *  SceneManager.h
 *  AmazonOfficeApp
 *
 *  Created by Imanol Gomez on 30/03/18.
 *
 */

#pragma once

#include "Manager.h"
#include "ofxSceneManager.h"
#include "ofxSimpleTimer.h"


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
    void changeScene(int sceneIndex);
    
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
    
    void nextScene();
    
    const string& getCurrentSceneName() const {return m_currentSceneName;}
    
private:
    
    //! Create the scenes
    void createScenes();
    
    //! Set up the fbo that saves the texture.
    void setupFbo();
    
    //! Set up the scene timer
    void setupTimer();
    
     //! Initializes the scene list
    void initializeSceneList();
    
    //! Update the scene timer
    void updateTimer();
    
    //! updates all the scenes
    void updateScenes();
    
    //! updates the fbo
    void updateFbo();
    
    void sendSceneChange();

private:

    ofxSceneManager          m_mySceneManager;
    ofFbo                    m_fbo;
    ofxSimpleTimer           m_sceneTimer;
    float                    m_alpha;
    
    vector<string>           m_sceneList;
    string                   m_currentSceneName;
    float                    m_transitionTime;
    
};

//==========================================================================


