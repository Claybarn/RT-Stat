//
//  RTStatsEditor.cpp
//  RTStats
//
//  Created by Clayton Barnes on 6/24/16.
//  Copyright © 2016 Clayton Barnes. All rights reserved.
//

/*
 ------------------------------------------------------------------
 
 This file is part of the Open Ephys GUI
 Copyright (C) 2014 Open Ephys
 
 ------------------------------------------------------------------
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 
 */

#include "RT-StatsEditor.h"


RTStatsEditor::RTStatsEditor(GenericProcessor* parentNode, bool useDefaultParameterEditors = true)
: VisualizerEditor(parentNode, useDefaultParameterEditors) //GenericEditor(parentNode, useDefaultParameterEditors)
{
    desiredWidth = 240;
   
    tabText = "RTStats";
    
    // Adding user ready button
    userReadyButton = new UtilityButton("Sorted", Font("Default", 15, Font::plain));
    userReadyButton->setBounds(10, 50, 60, 40); //Set position and size (X, Y, XSize, YSize)
    userReadyButton->addListener(this);
    userReadyButton->setClickingTogglesState(true);
    userReadyButton->setTooltip("Press when sorted all neurons of interest");
    addAndMakeVisible(userReadyButton);
    
    allIncreaseButton = new TriangleButton(1);
    allIncreaseButton->setBounds(85, 50, 60, 40); //Set position and size (X, Y, XSize, YSize)
    allIncreaseButton->addListener(this);
    allIncreaseButton->setClickingTogglesState(true);
    allIncreaseButton->setTooltip("Press to increase spike-count rate of all sorted ID's");
    addAndMakeVisible(allIncreaseButton);
    
    
    allDecreaseButton = new TriangleButton(2);
    allDecreaseButton->setBounds(160, 50, 60, 40); //Set position and size (X, Y, XSize, YSize)
    allDecreaseButton->addListener(this);
    allDecreaseButton->setClickingTogglesState(true);
    allDecreaseButton->setTooltip("Press to decrease spike-count rate of all sorted ID's where possible");
    addAndMakeVisible(allDecreaseButton);
   
}

RTStatsEditor::~RTStatsEditor(){
    
}


void RTStatsEditor::editorShown (Label*, TextEditor& textEditorShown){
    
}




TextEditor* createEditorComponent();

Visualizer* RTStatsEditor::createNewCanvas()
{
    
    RTStatsProcessor* processor = (RTStatsProcessor*) getProcessor();

    return new class RTStatsCanvas(processor);
    
}


void RTStatsEditor::buttonCallback(Button* button)
{
    RTStatsProcessor* processor = (RTStatsProcessor*) getProcessor();
    if (button == userReadyButton)
    {
        if (button->getToggleState()) //Button is pressed
            processor->setParameter(0,1); // Sets userReady variable in processor module to true
            
        else
            processor->setParameter(0,0); // Sets userReady variable in processor module to false
        
    }
    
    else if( button == allIncreaseButton)
        processor->trainUpAll();
    
    else if( button == allDecreaseButton)
        processor->trainDownAll();
        
    
    int gId = button->getRadioGroupId();
    
    if (gId > 0)
    {
        if (canvas != 0)
        {
            canvas->setParameter(gId-1, button->getName().getFloatValue());
        }
        
    }
    
}


