/*
 *  AppManager.cpp
 *  Bentley App
 *
 *  Created by Imanol Gomez on 06/02/19.
 *
 */

#include "ofMain.h"

#include "AppManager.h"

AppManager& AppManager::getInstance()
{
	// The only instance
	// Guaranteed to be lazy initialized
	// Guaranteed that it will be destroyed correctly
	static AppManager    m_instance;
    return m_instance;

}

AppManager::AppManager(): Manager(), m_debugMode(false), m_initialized(false)
{
   //Intentionally left empty
}


AppManager::~AppManager()
{
    ofLogNotice() <<"AppManager::Destructor";
}


void AppManager::setup()
{
	if(m_initialized)
		return;
    
    ofLogNotice() << "AppManager::initialized";

	Manager::setup();
    
    this->setupOF();
	this->setupManagers();
    //ofSetVerticalSync(false);
    
    m_initialized = true;
    
}

void AppManager::setupOF()
{
    ofSetLogLevel(OF_LOG_NOTICE);
    ofSetVerticalSync(false);
    ofSetEscapeQuitsApp(false);
    //ofSetDataPathRoot("./data/");
    //ofSetDataPathRoot(ofFilePath::getCurrentExeDir() + "./data");
    
     ofSetBackgroundAuto(true);
    
    bool isOFDataFolderBundled = false;
    if( ofFile::doesFileExist("../Resources/data", false) ){
        isOFDataFolderBundled = true;
    }
    if( isOFDataFolderBundled ){
        ofSetDataPathRoot(ofFilePath::getCurrentExeDir() + "/../Resources/data/"); // > = of0.10.0
        //ofSetDataPathRoot("../Resources/data/"); // < = of0.9.8
    }
    
    //ofLogNotice() << "AppManager::setupOF -> "<< ofFilePath::getCurrentExeDir() ;
    ofLogNotice() << "AppManager::setupOF -> OF data path is:  "<< ofToDataPath("",true);
    //ofLogNotice() << " OF data path is " << ofFilePath::getAbsolutePath( ofToDataPath("") ) << endl;
    //ofSetDataPathRoot("../Resources/data/");
    //ofSetDataPathRoot(ofFilePath::getCurrentExeDir() + "/../Resources/data");
   
}


void AppManager::setupManagers()
{
    m_settingsManager.setup();
    m_resourceManager.setup();
    m_viewManager.setup();
    m_visualEffectsManager.setup();
   
    
    m_layoutManager.setup();
    m_colorManager.setup();
    //m_camManager.setup();
    m_realSenseManager.setup();
    m_leapMotionManager.setup();
    m_udpManager.setup();
    m_ledsManager.setup();
    m_modelManager.setup();
    m_keyboardManager.setup();
    m_sceneManager.setup();
    
    m_guiManager.setup();
}

void AppManager::update()
{
    if(!m_initialized)
        return;
    
    m_guiManager.update();
    m_visualEffectsManager.update();
    m_viewManager.update();
    //m_camManager.update();
    m_realSenseManager.update();
    m_leapMotionManager.update();
    m_udpManager.update();
    m_colorManager.update();
    m_sceneManager.update();
    m_ledsManager.update();
    m_modelManager.update();
    m_layoutManager.update();
}


void AppManager::draw()
{
    
    if(!m_initialized)
        return;
    
    
    ofBackground(17,17,17);
    m_layoutManager.draw();
    m_guiManager.draw();    
}

void AppManager::toggleDebugMode()
{
    m_debugMode = !m_debugMode;
    setDebugMode(m_debugMode);
}


void AppManager::setDebugMode(bool showDebug)
{
    m_debugMode = showDebug;
    
    ofLogNotice()<<"AppManager::setDebugMode-> " << m_debugMode;
    
    if(m_debugMode){
        //ofSetLogLevel(OF_LOG_VERBOSE);
    }
    else{
        ofSetLogLevel(OF_LOG_NOTICE);
    }
    

    m_guiManager.showGui(m_debugMode);
    
}



