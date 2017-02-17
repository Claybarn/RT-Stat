//
//  RTStatsEditor.hpp
//  RTStats
//
//  Created by Clayton Barnes on 6/24/16.
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

#ifndef RTStatsEDITOR_H_INCLUDED
#define RTStatsEDITOR_H_INCLUDED

#include "../../../JuceLibraryCode/JuceHeader.h"
#include <EditorHeaders.h>
#include <VisualizerWindowHeaders.h>
#include <VisualizerEditorHeaders.h>
#include "ProcessorHeaders.h"
#include "RT-StatsProcessor.h"
#include "RT-StatsCanvas.h"
#include "../../UI/EditorViewport.h"

/**
 
 Editor with buttons to signal sorted Id's have been set, and two arrows to set all sorted Id's to trigger a TTL when spike-count rate above or below user specified threshold. Creates Canvas.
 
 @see GenericEditor
 
 */

class RTStatsEditor : public VisualizerEditor
{
public:
    
    RTStatsEditor(GenericProcessor* parentNode, bool useDefaultParameterEditors);
    
    ~RTStatsEditor();
    
    void labelTextChanged (Label* labelThatHasChanged);
    void editorShown (Label*, TextEditor& textEditorShown);
    int getOutputChan();
    TextEditor* createEditorComponent();
    void comboBoxChanged(ComboBox* c);
    Visualizer* createNewCanvas();
    RTStatsProcessor* RTStatsCanvas;
    void buttonCallback(Button* button);
    
private:
    ScopedPointer<Button> userReadyButton;
    ScopedPointer<TriangleButton> allIncreaseButton;
    ScopedPointer<TriangleButton> allDecreaseButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RTStatsEditor);
    
};

#endif
