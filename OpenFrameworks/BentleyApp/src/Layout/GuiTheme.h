/*
 *  GuiTheme.h
 *  Bentley App
 *
 *  Created by Imanol Gomez on 06/02/19.
 *
 */



#pragma once

//#include "ofxImGui.h"
#include "BaseTheme.h"


//========================== class GuiTheme ==============================
//============================================================================
/** \class GuiTheme GuiTheme.h
 *	\brief Costume GUI theme
 *	\details Based on ThemeTest
 */


class GuiTheme: public ofxImGui::BaseTheme
{
public:
    
    void setup() override;
    
};

