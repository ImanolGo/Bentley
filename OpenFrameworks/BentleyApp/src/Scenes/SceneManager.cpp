/*
 *  SceneManager.cpp
 *  Bentley App
 *
 *  Created by Imanol Gomez on 06/02/19.
 *
 */

#include "ofMain.h"


#include "SceneManager.h"
#include "scenes.h"
#include "AppManager.h"

SceneManager::SceneManager(): Manager(), m_alpha(-1), m_transitionTime(1.0), m_sceneOffset(2), m_currentVideoIndex(0), m_status(false)
{
	//Intentionally left empty
}


SceneManager::~SceneManager()
{
    ofLogNotice() <<"SceneManager::Destructor";
}


void SceneManager::setup()
{
	if(m_initialized)
		return;

	Manager::setup();

    this->createScenes();
    this->setupFbo();
    this->setupLevels();
   // this->setupTimer();
    this->initializeSceneList();

    ofLogNotice() <<"SceneManager::initialized";

}


void SceneManager::createScenes()
{
    //m_mySceneManager.setTransitionFade();
	m_mySceneManager.setTransitionDissolve();
    
    ofPtr<ofxScene> scene;
    
    //Create Blank Scene
    scene = ofPtr<ofxScene> (new BlankScene());
    m_mySceneManager.addScene(scene);
    
    //Create Test Scene
    auto sceneTest = ofPtr<TestScene> (new TestScene());
    sceneTest->setup();
    m_mySceneManager.addScene(sceneTest);
    
    m_sceneOffset = 2;
    
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height = AppManager::getInstance().getSettingsManager().getAppHeight();

    m_mySceneManager.run(width, height);
    this->onTransitionTimeChange(m_transitionTime);
}


void SceneManager::setupFbo()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    //float width = ofGetWidth();
    //float height = ofGetHeight();

    m_fbo.allocate(width, height, GL_RGBA);
    m_fbo.begin(); ofClear(0); m_fbo.end();
}

void SceneManager::setupLevels()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    m_levels.setup(width,height);
}

void SceneManager::setupTimer()
{
    
    m_sceneTimer.setup( 1000 );
    m_sceneTimer.start( false ) ;
    ofAddListener( m_sceneTimer.TIMER_COMPLETE , this, &SceneManager::sceneTimerCompleteHandler ) ;
    
    ofLogNotice() <<"SceneManager::setupTimer << Time = : " << time << "s";
}

void SceneManager::onChangeSceneDuration(float& value)
{
    m_sceneTimer.setup( value*1000 );
    m_sceneTimer.start( false ) ;
    ofLogNotice() <<"SceneManager::setupTimer << Time = : " << value << "s";
}

void SceneManager::initializeSceneList()
{
    m_sceneList.clear();
    m_sceneList  = { "Weather", "Tides", "Traffic", "Wind", "Planes"};
}


void SceneManager::update()
{
    this->updateScenes();
    this->updateFbo();
    this->updatePixels();
    //this->updateTimer();
}

void SceneManager::updateFbo()
{
    m_levels.begin();
        ofClear(0);
        ofEnableAlphaBlending();
         m_mySceneManager.draw();
         ofDisableAlphaBlending();
    m_levels.end();
    
    m_fbo.begin();
        ofClear(0);
        ofPushStyle();
        ofSetColor(255);
        ofEnableAlphaBlending();
            m_levels.draw();
        ofDisableAlphaBlending();
        ofPopStyle();
    m_fbo.end();
}


void SceneManager::updatePixels()
{
    ofPixels pixels;
    m_reader.readToPixels(m_fbo, pixels);
    //m_exportFbo.readToPixels(pixels);
    AppManager::getInstance().getLedsManager().setPixels(pixels);
}


void SceneManager::updateScenes()
{
    m_mySceneManager.update();
}

void SceneManager::updateTimer()
{
    m_sceneTimer.update();
}


void SceneManager::draw()
{
    string name = "Scene";
    ofRectangle frame;
    auto rect = AppManager::getInstance().getLayoutManager().getWindowRect(name);
    float ratio = m_fbo.getWidth()/ m_fbo.getHeight();
    frame.height = rect->getHeight();
    frame.width = frame.height*ratio;
    
    if( frame.width > rect->getWidth() ){
        frame.width = rect->getWidth();
        frame.height = frame.width/ratio;
    }
    
    frame.x = rect->getWidth()*0.5 - frame.width*0.5;
    frame.y = rect->getHeight()*0.5 - frame.height*0.5;
    
    m_fbo.draw(frame);
    
}

void SceneManager::draw(const ofRectangle& rect)
{
    m_fbo.draw(rect.x,rect.y,rect.width,rect.height);
}


void SceneManager::removeVideos()
{
    this->changeScene("Blank");
    
    auto& videoPaths = AppManager::getInstance().getVideoManager().getVideoResourcesPath();
    auto& scenes = m_mySceneManager.scenes;
    for(auto& path : videoPaths){
        for(auto& scene : scenes){
            if(scene->getName() == path.first){
                m_mySceneManager.removeScene(scene);
            }
        }
    }
}

void SceneManager::addVideos()
{
    this->removeVideos();
    
    auto& videoPaths = AppManager::getInstance().getVideoManager().getVideoResourcesPath();
  
    for(auto& path : videoPaths){
        auto videoScene = ofPtr<VideoScene>(new VideoScene(path.first));
        videoScene->setup();
        m_mySceneManager.addScene(videoScene);
    }
    
    AppManager::getInstance().getGuiManager().setupScenesGui();
}



void SceneManager::changeScene(string sceneName)
{
    for(auto scene: m_mySceneManager.scenes){
        if(scene->getName() == sceneName){
            m_mySceneManager.changeScene(sceneName);
            m_sceneTimer.start(false,true);
            m_currentSceneName = sceneName;
            this->sendSceneChange();
            
            break;
        }
    }
}

void SceneManager::setVideoIndex(int value)
{
    int size = m_mySceneManager.scenes.size() - m_sceneOffset;
    if(value >= 0  &&  value<size){
        m_currentVideoIndex = value;
    }
}

void SceneManager::changeSceneIndex(int& sceneIndex)
{
    if(sceneIndex < 0 || sceneIndex >= m_mySceneManager.scenes.size()){
        return;
    }
    
     m_mySceneManager.changeScene(sceneIndex);
     m_sceneTimer.start(false,true);
     m_currentSceneName = this->getSceneName(sceneIndex);
     this->sendSceneChange();
    
    if(sceneIndex < m_sceneOffset){
        m_status = false;
    }
    else{
         m_status = true;
        m_currentVideoIndex = sceneIndex - m_sceneOffset;
    }
}


void SceneManager::onTransitionTimeChange(float value)
{
   m_mySceneManager.setSceneDuration(value,value);
   m_sceneTimer.start(false,true);
}

string SceneManager::getSceneName(int sceneIndex)
{
    string name = "";
    if(sceneIndex < 0 || sceneIndex >= m_mySceneManager.scenes.size()){
        return name;
    }
    
    return m_mySceneManager.scenes[sceneIndex]->getName();
   
}

int SceneManager::getIndex(string& sceneName)
{
    for(int i = 0; i< m_mySceneManager.scenes.size(); i++){
        if(m_mySceneManager.scenes[i]->getName() == sceneName){
            return i;
        }
    }
    
    return -1;
}

void SceneManager::sceneTimerCompleteHandler( int &args )
{
    m_sceneTimer.start(false);
    
    this->nextScene();
}

void SceneManager::nextScene()
{
    if(m_sceneList.empty()){
        this->initializeSceneList();
    }
    
    string sceneName = m_sceneList.back();  m_sceneList.pop_back();
   // AppManager::getInstance().getGuiManager().onSceneChange(sceneName);
    
    ofLogNotice() <<"SceneManager::nextScene << Change Scene: " << sceneName;
}

void SceneManager::sendSceneChange()
{
    
    string address = "preset" + m_currentSceneName;
    
    ofLogNotice() <<"SceneManager::sendSceneChange << OSC Message Scene: " << address;
    
   // AppManager::getInstance().getOscManager().sendIntMessage(1, address);
  
}





