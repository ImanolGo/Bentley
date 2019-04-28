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


LedsManager::LedsManager(): Manager(), m_isNewFrame(false), m_is3D(true), m_ledsBrightness(1.0), m_laserBrightness(0.25), m_offset(100)
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
    this->readLeds();
     //this->createLedPositions();
    this->arrangeLeds();
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
    
}


void LedsManager::arrangeLeds()
{
    this->normalizeLeds();
    this->centreLeds();
    //this->normalizeLeds();
    
    int total = (int)m_points3D.size();
    m_vbo3D.setVertexData(&m_points3D[0], total, GL_DYNAMIC_DRAW);
    m_vbo3D.setNormalData(&m_sizes[0], total, GL_DYNAMIC_DRAW);
    m_vbo3D.setColorData(&m_colors[0], m_points3D.size(), GL_DYNAMIC_DRAW);
}

void LedsManager::setupShader()
{
    m_shader.load("shaders/vboShader");
    ofDisableArbTex();
    ofLoadImage(m_texture, "images/general/dot.png");
    ofEnableArbTex();
    
    
}

void LedsManager::createLedPositions()
{
    ofLogNotice() <<"LedsManager::createLedPositions" ;
    
    
    int size = 100;
    
    for(int i = 0; i<size; i++){
        for(int j = 0; j<size; j++)
        {
            ofPoint pos (j,i) ;
            createLedPair(pos, pos);
        }
        
    }
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
    
    
}


void LedsManager::createLedPair(const ofPoint& position2D,const ofPoint& position3D)
{
    float size = AppManager::getInstance().getGuiManager().getLedsSize();
    
    m_points3D.push_back(position3D);
    m_points2D.push_back(position2D);
    m_sizes.push_back(ofVec3f(size));
    m_colors.push_back(ofFloatColor(0,0,0));
}


void LedsManager::sortLeds()
{
    
    LedVector aux;
    
    while (!m_leds.empty())
    {
        int min = 10000;
        int n = 0;
        
        for (int i = 0; i<m_leds.size(); i++) {
            if(m_leds[i]->getId() < min)
            {
                n = i;
                min = m_leds[i]->getId() ;
            }
        }
        
        aux.push_back(m_leds[n]);
        m_leds.erase(m_leds.begin() + n);
    }
    
    m_leds = aux;
    //std::sort(m_leds.begin(), m_leds.end(), compare);
    
    //auto comp = []( const Led& w1, const Led& w2 ){ return w1.getId() < w2.getId(); }
    
    //std::sort(m_leds.begin(), m_leds.end(), [] (Led const& a, Led const& b) { return a.getId() < b.getId(); });
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
    m_colors.push_back(ofFloatColor(0,0,0));
    
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
         //AppManager::getInstance().getImageManager().update();
    }
   
}

void LedsManager::setPixels(ofPixelsRef pixels)
{
    for(int i=0; i<m_points3D.size(); i++){
        this->setPixelColor(pixels, i);
    }
    
    m_vbo3D.setColorData(&m_colors[0], m_points3D.size(), GL_DYNAMIC_DRAW);
    m_isNewFrame = true;
}


void LedsManager::setPixelColor(ofPixelsRef pixels, int index)
{
    if(index<0 || index>=m_points3D.size()){
        return;
    }
    
    
    ofPoint pixelPos;
    
    pixelPos.x = ofMap(m_points2D[index].x, m_minPos.x, m_maxPos.x, 0, pixels.getWidth()-1);
    pixelPos.y = ofMap(m_points2D[index].y, m_minPos.y, m_maxPos.y,  pixels.getHeight()-1, 0);
    
    
//    if(m_is3D)
//    {
//        float treshold = m_minPos.y + (m_maxPos.y - m_minPos.y)*0.5;
//
//        if(m_points3D[index].y >= treshold ){
//            pixelPos.x = ofMap(m_points3D[index].x/m_offset, m_minPos.x, m_maxPos.x, 0, (pixels.getWidth()-1)*0.5);
//            pixelPos.y = ofMap(m_points3D[index].z/m_offset, m_maxPos.z, m_minPos.z, 0,  pixels.getHeight()-1);
//        }
//        else{
//            pixelPos.x = ofMap(m_points3D[index].x/m_offset, m_minPos.x, m_maxPos.x, pixels.getWidth()-1, (pixels.getWidth()-1)*0.5);
//            pixelPos.y = ofMap(m_points3D[index].z/m_offset, m_maxPos.z, m_minPos.z, 0,  pixels.getHeight()-1);
//        }
//    }
//    else
//    {
//        pixelPos.x = ofMap(m_points3D[index].x/m_offset, m_minPos.x, m_maxPos.x, 0, pixels.getWidth()-1);
//        pixelPos.y = ofMap(m_points3D[index].y/m_offset, m_maxPos.y, m_minPos.y, 0,  pixels.getHeight()-1);
//    }
    
    //ofLogNotice() <<  m_position.x ; ofLogNotice() <<  m_position.y;
    //ofLogNotice() <<  pixelPos.x ; ofLogNotice() <<  pixelPos.y;
    //ofLogNotice() << pixelPos.x;
    
    auto color = pixels.getColor((int)pixelPos.x, (int)pixelPos.y);
    m_colors[index] = ofFloatColor(color.r/255.0f, color.g/255.0f, color.b/255.0f);
}

void LedsManager::draw()
{
//    for(auto led: m_leds){
//        led->draw();
//    }
    
    m_shader.begin();
    m_texture.bind();
        m_vbo3D.draw(GL_POINTS, 0, (int)m_points3D.size());
    m_texture.unbind();
    m_shader.end();
}

void LedsManager::setSize(float& value)
{
//    for(auto led: m_leds){
//        led->setWidth(value);
//    }
    
    //m_sizes.clear();
    for(auto& size: m_sizes){
        size = ofVec3f(10*value);
    }
    
    m_vbo3D.setNormalData(&m_sizes[0], m_sizes.size(), GL_DYNAMIC_DRAW);
}






bool LedsManager::loadSubfolder(ofDirectory& dir)
{
     ofLogNotice() <<"LedsManager::loading subfolders ..." ;
    //only show txt files
     dir.allowExt("txt");
    
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
    m_leds.clear();
    m_vbo3D.clear();
    m_points3D.clear();
    m_sizes.clear();
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





