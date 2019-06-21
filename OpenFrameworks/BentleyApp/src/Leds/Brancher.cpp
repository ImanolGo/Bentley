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
    //Intentionaly left empty
}

Brancher::~Brancher()
{
    //Intentionaly left empty
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




