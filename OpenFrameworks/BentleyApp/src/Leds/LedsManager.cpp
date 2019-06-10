/*
 *  LedsManager.cpp
 *  Bentley App
 *
 *  Created by Imanol Gomez on 06/02/19.
 *
 */



#include "ofMain.h"

#include "LedsManager.h"
#include "AppManager.h"


const string LedsManager::LEDS_FOLDER_PATH = "leds/";


LedsManager::LedsManager(): Manager(), m_isNewFrame(false), m_is3D(true), m_ledsBrightness(1.0), m_offset(100), m_bcr(10), m_bcg(10), m_bcb(10)
{
	//Intentionally left empty
}


LedsManager::~LedsManager()
{
    ofLogNotice() <<"LedsManager::Destructor" ;
}


void LedsManager::setup()
{
	if(m_initialized)
		return;


	Manager::setup();
    
    this->setupLeds();
    this->setupShader();
    
    ofLogNotice() <<"LedsManager::initialized" ;
    
}


void LedsManager::setupLeds()
{
    //this->readLeds();
    this->createLedPositions();
    this->arrangeLeds();
    this->createLayout();
    
    m_meshModel.save("leds/test.ply");
}

bool LedsManager::readLeds()
{
    
    ofDirectory dir(LEDS_FOLDER_PATH);
    if( !dir.exists()){
        ofLogNotice() <<"LedsManager::setupLeds -> Folder not found: " << LEDS_FOLDER_PATH;
        return false;
    }
    
    dir.listDir();
    
    ofLogNotice() <<"LedsManager::directory size: " << dir.size();
    
    
    this->clearLeds();
    
    for(int i = 0; i < dir.size(); i++)
    {
        string pathAux =  dir.getPath(i);
        ofDirectory dirAux(pathAux);
        dirAux.listDir();
        dirAux.sort();
        this->loadSubfolder(dirAux);
    }

    return true;
}


void LedsManager::arrangeLeds()
{
    
    this->normalizeLeds();
    this->centreLeds();
    //this->normalizeLeds();
    
    int total = (int)m_points3D.size();
    m_vbo.setVertexData(&m_points3D[0], total, GL_DYNAMIC_DRAW);
    m_vbo.setNormalData(&m_sizes[0], total, GL_DYNAMIC_DRAW);
    m_vbo.setColorData(&m_colors[0], m_points3D.size(), GL_DYNAMIC_DRAW);
    
    
    m_vboModel.setVertexData(&m_points3D[0], total, GL_DYNAMIC_DRAW);
    m_vboModel.setNormalData(&m_sizesModel[0], total, GL_DYNAMIC_DRAW);
    m_vboModel.setColorData(&m_colorsBlack[0], m_points3D.size(), GL_DYNAMIC_DRAW);
    
}

void LedsManager::setupShader()
{
    ofLogNotice() <<"LedsManager::setupShader ";
    
    m_vboShader.load("shaders/vboShader");
    m_maskShader.load("shaders/LuminanceMaskingShader");
    
    ofDisableArbTex();
    ofLoadImage(m_texture, "images/general/dot.png");
    ofLoadImage(m_textureModel, "images/general/black_dot.png");
    //ofLoadImage(m_textureModel, "images/general/dot.png");
    ofEnableArbTex();
}

void LedsManager::createLayout()
{
    float resolution = 6000;
    float percentage = 0.2;
    float width = m_maxPos.x - m_minPos.x;
    float height = m_maxPos.y - m_minPos.y;
    float ratio = width/height;
    if(ratio > 1.0){
        width = resolution;
        height = width/ratio;
    }
    else{
        height = resolution;
        width = height*ratio;
    }
    
   
    ofPixels pix;
    
    ofFbo fbo;
    fbo.allocate(width, height);
    fbo.begin();
    ofClear(0, 0, 0, 255);
    ofSetColor(255);
    
    float size = 20;
    for(auto led: m_points2D){
        float x = ofMap(led.x, m_minPos.x, m_maxPos.x, 0.0, width);
        float y = ofMap(led.y, m_minPos.y, m_maxPos.y, 0.0, height);
        //ofDrawCircle(x, y, 4);
        ofDrawRectangle(x - size*0.5, y - size*0.5, size, size);
    }
    
    
    fbo.end();
    
    fbo.readToPixels(pix);
    ofSaveImage(pix, "images/layout/leds_layout.png");
    
    width*=percentage;
    height*=percentage;
    
    m_fboMask.allocate(width, height, GL_RGB);
    m_fbo.allocate(width, height, GL_RGB);
    m_fboMaskee.allocate(width, height, GL_RGB);
    
    m_fboMask.begin();
    ofSetColor(255);
    
    for(auto led: m_points2D){
        float x = ofMap(led.x, m_minPos.x, m_maxPos.x, 0.0, width);
        float y = ofMap(led.y, m_minPos.y, m_maxPos.y, 0.0, height);
        //ofDrawCircle(x, y, 4);
        ofDrawRectangle(x - size*0.5, y - size*0.5, size, size);
    }
    
    m_fboMask.end();
    
    //ofLogNotice() <<"LedsManager::createLayout: saved layout -> w = " << width << ", h = " << height << ", ratio = " << ratio;
    ofLogNotice() <<"LedsManager::createLayout: new app width -> w = " << width << ", new app height = " << height << ", ratio = " << ratio;
    AppManager::getInstance().getSettingsManager().setAppWidth(width);
    AppManager::getInstance().getSettingsManager().setAppHeight(height);
    
}



void LedsManager::createLedPositions()
{
    ofLogNotice() <<"LedsManager::createLedPositions" ;
    
    Brancher brancher(20);
    int x = 0;
    for(int i = 0; i<2; i++){
        for(int j = 0; j<4; j++)
        {
            ofPoint pos (j,i) ;
            createLedPair(pos, pos);
            brancher.addPixel(x);
            x++;
        }
        
    }
    
    m_branchers.push_back(brancher);
    ofLogNotice() <<"LedsManager::createLedPositions -> created brancher:" <<   brancher.getId();
    AppManager::getInstance().getUdpManager().setupConnection(brancher.getId());
}

bool LedsManager::addLedPair(string& pathTwoD, string& pathThreeD)
{
    ofBuffer buffer2D = ofBufferFromFile(pathTwoD);
    ofBuffer buffer3D = ofBufferFromFile(pathThreeD);
 
    ofLogNotice() <<"LedsManager::addLedPair -> buffer2D: " << pathTwoD ;
    ofLogNotice() <<"LedsManager::addLedPair -> buffer3D: " << pathThreeD ;
    
    if(buffer2D.size()== 0 || buffer2D.size()== 0){
        ofLogNotice() <<"LedsManager::addLedPair -> zero buffer size" ;
        return false;
    }

    ofBuffer::Line it2d = buffer2D.getLines().begin();
    ofBuffer::Line it3d = buffer3D.getLines().begin();
    
    ofPoint ledPosition2D;
    ofPoint ledPosition3D;
    int id = 0;
    
    while(it2d != buffer2D.getLines().end() ||  it3d != buffer3D.getLines().end()){
        
        string line2D = *it2d;
        string line3D = *it3d;
        
        if(!line2D.empty() && parseLedLine(line2D,ledPosition2D) &&  !line3D.empty() && parseLedLine(line3D,ledPosition3D))
        {
            this->createLedPair(ledPosition2D, ledPosition3D);
        }
        
        ++it2d; ++it3d;
        id++;
    }
    
    return true;
    
}

void LedsManager::createLedPair(const ofPoint& position2D,const ofPoint& position3D)
{
    float size = AppManager::getInstance().getGuiManager().getLedsSize();
    
    m_points3D.push_back(position3D);
    m_points2D.push_back(position2D);
    m_sizes.push_back(ofVec3f(size));
    m_sizesModel.push_back(ofVec3f(size*0.5));
    m_colors.push_back(ofFloatColor(0,0,0));
    m_colorsBlack.push_back(ofFloatColor(255,255,255));
    m_meshModel.addVertex(position3D);
}


void LedsManager::normalizeLeds()
{
    //this->normalize2DLeds();
    this->normalize3DLeds();
    
}

void LedsManager::normalize2DLeds()
{
    float max = 0;
    for (auto position: m_points2D)
    {
        
        if(max < abs(position.x)){
            max = abs(position.x);
        }
        
        if(max < abs(position.y)){
            max = abs(position.y);
        }
    }
    
    ofLogNotice() <<"LedsManager::normalize2DLeds -> max value =  " << max;
    
    int id = 0;
    for (auto& position: m_points2D)
    {
        position/=max;
        
        ofLogNotice() <<"LedsManager::normalizeLeds -> id " << id << ", x = "  << position.x << ", y = "  << position.y << ", z = " << position.z ;
        id++;
    }
    
}


void LedsManager::normalize3DLeds()
{
    float max = 0;
    for (auto position: m_points3D)
    {
        
        if(max < abs(position.x)){
            max = abs(position.x);
        }
        
        if(max < abs(position.y)){
            max = abs(position.y);
        }
        
        if(max < abs(position.z)){
            max = abs(position.z);
        }
    }
    
    ofLogNotice() <<"LedsManager::normalize3DLeds -> max value =  " << max;
    
    int id = 0;
    for (auto& position: m_points3D)
    {
        position/=max;
        
        //ofLogNotice() <<"LedsManager::normalizeLeds -> id " << id << ", x = "  << position.x << ", y = "  << position.y << ", z = " << position.z ;
        id++;
    }
    
}

void LedsManager::centreLeds()
{
    this->centre2DLeds();
    this->centre3DLeds();
}

void LedsManager::centre3DLeds()
{
    
    ofPoint maxPos, minPos;
    bool firstIteration = true;
    
    for (auto position: m_points3D)
    {
        if(firstIteration){
            firstIteration = false;
            maxPos = position;
            minPos = position;
        }
        
        if(maxPos.x < position.x){
            maxPos.x = position.x;
        }
        
        if(maxPos.y < position.y){
            maxPos.y = position.y;
        }
        
        if(maxPos.z < position.z){
            maxPos.z = position.z;
        }
        
        if(minPos.x > position.x){
            minPos.x = position.x;
        }
        
        if(minPos.y > position.y){
            minPos.y = position.y;
        }
        
        if(minPos.z > position.z){
            minPos.z = position.z;
        }
        
    }
    
    ofLogNotice() <<"LedsManager::centreLeds -> min position: x = "  << minPos.x << ", y = "  << minPos.y << ", z = " << minPos.z ;
    ofLogNotice() <<"LedsManager::centreLeds -> max position: x = "  << maxPos.x << ", y = "  << maxPos.y << ", z = " << maxPos.z ;
    
    ofPoint shift = (maxPos- minPos)*0.5  + minPos;
    
    ofLogNotice() <<"LedsManager::centreLeds -> shift position: x = "  << shift.x << ", y = "  << shift.y << ", z = " << shift.z ;
    
    for (auto& position: m_points3D)
    {
        position-=shift;
        position*=m_offset;
    }
    
//    m_maxPos -=shift;
//    m_minPos -=shift;
}


void LedsManager::centre2DLeds()
{
    
    bool firstIteration = true;
    
    for (auto position: m_points2D)
    {
        if(firstIteration){
            firstIteration = false;
            m_maxPos = position;
            m_minPos = position;
        }
        
        if(m_maxPos.x < position.x){
            m_maxPos.x = position.x;
        }
        
        if(m_maxPos.y < position.y){
            m_maxPos.y = position.y;
        }
        
    
        if(m_minPos.x > position.x){
            m_minPos.x = position.x;
        }
        
        if(m_minPos.y > position.y){
            m_minPos.y = position.y;
        }
        
    }
    
    
    ofLogNotice() <<"LedsManager::centreLeds -> min position: x = "  << m_minPos.x << ", y = "  << m_minPos.y ;
    ofLogNotice() <<"LedsManager::centreLeds -> max position: x = "  << m_maxPos.x << ", y = "  << m_maxPos.y;
    
    ofPoint shift = m_minPos;
    
    ofLogNotice() <<"LedsManager::centreLeds -> shift position: x = "  << shift.x << ", y = "  << shift.y << ", z = " << shift.z ;
    
    for (auto& position: m_points2D)
    {
        position-=shift;
    }
    
    m_maxPos -=shift;
    m_minPos -=shift;
    
    //Add margin
    float max_x = abs(m_maxPos.x - m_minPos.x);
    float max_y = abs(m_maxPos.y - m_minPos.y);
    float max = max_y;
    if(max_x<max_y){
        max = max_x;
    }
    
    float margin = max/20;
    m_maxPos.x+=margin;
    m_maxPos.y+=margin;
    m_minPos.x-=margin;
    m_minPos.y-=margin;
}


void LedsManager::createLed(const ofPoint& position, int& id)
{
    //ofPtr<Led> led = ofPtr<Led> (new Led ( position, id ) );
    float size = AppManager::getInstance().getGuiManager().getLedsSize();
    //float size = 5;
//    led->setWidth(size);
//    m_leds.push_back(led);
    
//    ofLogNotice() <<"LedsManager::createLed -> id " << led->getId() << ", x = "  << led->getPosition().x << ", y = "  << led->getPosition().y << ", z = " << led->getPosition().z ;
    
    m_points3D.push_back(position);
    m_sizes.push_back(ofVec3f(size));
    m_sizesModel.push_back(ofVec3f(size*0.5));
    m_colors.push_back(ofFloatColor(0,0,0));
    m_colorsBlack.push_back(ofFloatColor(255,255,255));
    
}

bool LedsManager::parseLedLine(string& line, ofPoint& position)
{
    if(line.size() == 0){
        return false;
    }

    char chars[] = "{}";
    removeCharsFromString(line, chars);
    
    //vector <string> strings = ofSplitString(line, ". " );
    
    //id = ofToInt(strings[0]);
    
    vector <string> positionsStrings = ofSplitString(line, ", " );
    
    if(positionsStrings.size()!=3){
        return false;
    }
    
    position.x = ofToFloat(positionsStrings[0]);
    position.y = ofToFloat(positionsStrings[1]);
    position.z = ofToFloat(positionsStrings[2]);
    
    return true;
}

void LedsManager::removeCharsFromString( string &str, char* charsToRemove ) {
    for ( unsigned int i = 0; i < strlen(charsToRemove); ++i ) {
        str.erase( remove(str.begin(), str.end(), charsToRemove[i]), str.end() );
    }
}

void LedsManager::update()
{
    if(m_isNewFrame){
         m_isNewFrame = false;
        this->updateBranches();
        AppManager::getInstance().getUdpManager().updatePixels();
    }
       
}

void LedsManager::updateBranches()
{
    for(auto& brancher: m_branchers){
        brancher.setPixels(m_colors);
    }
}

void LedsManager::setPixels(ofPixelsRef pixels)
{
    for(int i=0; i<m_points2D.size(); i++){
        this->setPixelColor(pixels, i);
    }
    
    m_vbo.setColorData(&m_colors[0], m_points3D.size(), GL_DYNAMIC_DRAW);
    m_isNewFrame = true;
}


void LedsManager::setPixelColor(ofPixelsRef pixels, int index)
{
    if(index<0 || index>=m_points2D.size()){
        return;
    }
    
    
    ofPoint pixelPos;
    
    pixelPos.x = ofMap(m_points2D[index].x, m_minPos.x, m_maxPos.x, 0, pixels.getWidth()-1);
    pixelPos.y = ofMap(m_points2D[index].y, m_minPos.y, m_maxPos.y,  pixels.getHeight()-1,0);
    
    auto color = pixels.getColor((int)pixelPos.x, (int)pixelPos.y);
    m_colors[index] = ofFloatColor(color.r/255.0f, color.g/255.0f, color.b/255.0f);
}

void LedsManager::draw()
{
    m_vboShader.begin();
    m_texture.bind();
        m_vbo.draw(GL_POINTS, 0, (int)m_points3D.size());
    m_texture.unbind();
    m_vboShader.end();
}

void LedsManager::drawModel()
{
    m_vboShader.begin();
    m_textureModel.bind();
    m_vboModel.draw(GL_POINTS, 0, (int)m_points3D.size());
    m_textureModel.unbind();
    m_vboShader.end();
}

void LedsManager::drawLayout()
{
    ofEnableAlphaBlending();
    
    ofRectangle rect1(0,0,m_fboMask.getWidth(), m_fboMask.getHeight());
    m_fboMaskee.begin();
        ofClear(0);
        AppManager::getInstance().getSceneManager().draw(rect1);
    m_fboMaskee.end();
    
    
    m_fbo.begin();
        ofClear(0);
        m_maskShader.begin();
        m_maskShader.setUniformTexture("imageMask", m_fboMask.getTexture(), 1);
        m_fboMaskee.draw(0,0);
        m_maskShader.end();
    m_fbo.end();
    
    string name = "2D";
    auto rect = AppManager::getInstance().getLayoutManager().getWindowRect(name);
    float ratio = m_fboMask.getWidth()/ m_fboMask.getHeight();
    float height = rect->getHeight();
    float width = height*ratio;
    
    if( width > rect->getWidth() ){
        width = rect->getWidth();
        height = width/ratio;
    }
    
    float x = rect->getWidth()*0.5 - width*0.5;
    float y = rect->getHeight()*0.5 - height*0.5;
    
    m_fbo.draw(x,y, width, height);
    
}


void LedsManager::setSize(float& value)
{
    float value_ = value;
    for(auto& size: m_sizes){
        size = ofVec3f(value_);
    }
    
    for(auto& size: m_sizesModel){
        size = ofVec3f(value_);
    }
    
    m_vbo.setNormalData(&m_sizes[0], m_sizes.size(), GL_DYNAMIC_DRAW);
    m_vboModel.setNormalData(&m_sizesModel[0], m_sizesModel.size(), GL_DYNAMIC_DRAW);
}





bool LedsManager::loadSubfolder(ofDirectory& dir)
{
     ofLogNotice() <<"LedsManager::loading subfolders ..." ;
    //only show txt files
    // dir.allowExt("txt");
     dir.allowExt("CSV");
    
    if( dir.listDir() == 0){
        ofLogNotice() <<"LedsManager::setupLeds -> No led files found in: " << dir.getAbsolutePath();
        return false;
    }
    
    ofLogNotice()<< "LedsManager::loadSubfolder-> Path: " << dir.getAbsolutePath();
    ofLogNotice()<< "LedsManager::loadSubfolder-> Size: " << dir.size();
    
    string twodfile = "";
    string threedfile = "";
    
    //go through and print out all the paths
    for(int i = 0; i < dir.size(); i++){
        string path = dir.getPath(i);
        if(ofIsStringInString(path, "POINTS_2D")){
            twodfile = path;
        }
        
        if(ofIsStringInString(path, "POINTS_3D")){
            threedfile = path;
        }
    }
    
    
    if(twodfile.empty() || threedfile.empty()){
        ofLogNotice()<< "LedsManager::loadSubfolder-> No position's pair found ";
        return false;
    }
    
    return this->loadPair(twodfile, threedfile);
    
}


bool LedsManager::loadPair(string& pathTwoD, string& pathThreeD)
{
    bool success = isValidFile(pathTwoD) && isValidFile(pathThreeD) ;
    if(success){
        AppManager::getInstance().getModelManager().resetCamera();
        this->addLedPair(pathTwoD,pathThreeD);
    }
    
    return success;
}



void LedsManager::clearLeds()
{
    m_colors.clear();
    m_colorsBlack.clear();
    m_vbo.clear();
    m_vboModel.clear();
    m_points3D.clear();
    m_sizes.clear();
    m_sizesModel.clear();
}

bool LedsManager::isValidFile(const string& path)
{
    ofBuffer buffer = ofBufferFromFile(path);
    
    if(buffer.size()==0){
         ofLogNotice() <<"LedsManager::isValidFile -> empty file";
        return false;
    }
    
    ofPoint ledPosition;
    
    for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it)
    {
        
        string line = *it;
        
        //ofLogNotice() << line;
        
        // copy the line to draw later
        // make sure its not a empty line
        if(!line.empty() && !parseLedLine(line,ledPosition)) {
            ofLogNotice() <<"LedsManager::isValidFile -> File not valid";
            return false;
        }
        
        // print out the line
        //cout << line << endl;
        
    }
    
    return true;
}

void LedsManager::setBCR(int& value)
{
     m_bcr = ofClamp(value, 0, 127);
     AppManager::getInstance().getUdpManager().sendTlcSettings(m_bcr,m_bcg,m_bcb);
}

void LedsManager::setBCG(int& value)
{
    m_bcr = ofClamp(value, 0, 127);
    AppManager::getInstance().getUdpManager().sendTlcSettings(m_bcr,m_bcg,m_bcb);
}

void LedsManager::setBCB(int& value)
{
    m_bcr = ofClamp(value, 0, 127);
    AppManager::getInstance().getUdpManager().sendTlcSettings(m_bcr,m_bcg,m_bcb);
}





