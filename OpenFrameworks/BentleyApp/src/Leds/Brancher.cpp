/*
 *  Brancher.h
 *  Bentley App
 *
 *  Created by Imanol Gomez on 16/05/19.
 *
 */



#include "Brancher.h"
#include "AppManager.h"



Brancher::Brancher( unsigned short _id): m_id(_id), m_ledType(Brancher::TLC)
{
    this->setup();
}

Brancher::~Brancher()
{
    //Intentionaly left empty
}



void Brancher::setup()
{
    this->setupCurrentSettings();
}

void Brancher::setupCurrentSettings()
{
    m_path = "xmls/brancher" + ofToString(m_id - 100) + ".xml";
    
    m_bcr.set("BCR", 17, 0, 127);
    m_bcg.set("BCG", 17, 0, 127);
    m_bcb.set("BCB", 17, 0, 127);

    m_parameters.add(m_bcr);
    m_parameters.add(m_bcg);
    m_parameters.add(m_bcb);
    
    this->loadCurrentSettings();
    
}



void Brancher::setPixels(const vector<ofFloatColor>& colors)
{
    for(int i=0; i< m_indexes.size(); i++){
        auto index = m_indexes[i];
        if(index<colors.size()){
            m_pixels[i] = colors[index];
//            m_pixels[i].r = colors[index].r * 255;
//            m_pixels[i].g = colors[index].g * 255;
//            m_pixels[i].b = colors[index].b * 255;
        }
    }
}

void Brancher::addPixel(unsigned int index)
{
    m_indexes.push_back(index);
    m_pixels.push_back(ofColor(0));
}

void Brancher::addStemPixel(const string& _id, unsigned int index)
{
    m_stems[_id].push_back(index);
}

bool Brancher::getIndexFromStem(const string& _id, unsigned int index, unsigned int& returned_index)
{
    if(m_stems.find(_id) == m_stems.end()){
        return false;
    }
    
    if( index >= m_stems[_id].size()){
        return false;
    }
    
    returned_index =  m_stems[_id][index];
    return true;
}

vector<string> Brancher::getStemIds()
{
    vector<string> ids;
    for(auto stem: m_stems)
    {
        ids.push_back(stem.first);
    }
    return ids;
}


void Brancher::getCurrentSettings(int & bcr, int& bcg, int& bcb)
{
    bcr = m_bcr.get();
    bcg = m_bcg.get();
    bcb = m_bcb.get();
}

void Brancher::setCurrentSettings(int bcr, int bcg, int bcb)
{
    m_bcr = bcr;
    m_bcg = bcg;
    m_bcb = bcb;
    
    this->saveCurrentSettings();
}

void Brancher::saveCurrentSettings()
{
    ofXml xml;
    ofSerialize(xml, m_parameters);
    xml.save(m_path);
    
    ofLogNotice() <<"Brancher::saveCurrentSettings -> path: " << m_path;
}

void Brancher::loadCurrentSettings()
{
    ofXml xml;
    xml.load(m_path);
    ofDeserialize(xml, m_parameters);
    ofLogNotice() <<"Brancher::loadCurrentSettings -> path: " << m_path;
}



