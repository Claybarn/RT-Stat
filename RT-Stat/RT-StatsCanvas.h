//
//  RTStatsCanvas.h
//  RTStats
//
//  Created by Clayton Barnes on 6/26/16.
//  Copyright © 2016 Clayton Barnes. All rights reserved.
//

#ifndef RT-StatsCanvas_h
#define RT-StatsCanvas_h

#include "../../../JuceLibraryCode/JuceHeader.h"
#include <VisualizerWindowHeaders.h>
#include "RT-StatsEditor.h"
#include <stdio.h>

class RTStatsCanvas;



class RTStatsCanvas : public Visualizer, public Button::Listener, public Label::Listener, public ComboBox::Listener
{
    
public:
    
    RTStatsCanvas(RTStatsProcessor* parentNode);
    
    void update();
    void resized();
    void paint(Graphics& g);
    void refresh();
    void refreshState();
    
    void beginAnimation();
    void endAnimation();
    
    void setParameter(int, float);
    void setParameter(int, int, int, float);
    
    // for buttons
    void buttonClicked(Button* button);
    void labelTextChanged(Label* labelThatHasChanged);
    void comboBoxChanged(ComboBox* c);
    
private:
    RTStatsProcessor* processor;
    RTStatsCanvas* canvas;
    Viewport* viewport;
    int scrollBarThickness;
    uint16_t sortedId = 0;
    bool fullRedraw = true;
    bool histoRedraw = true;
    bool noSortedText = true;
 
    ScopedPointer<Label> binNumSelector;
    ScopedPointer<ComboBox> directionSelector;
    ScopedPointer<ComboBox> outputSelector;
    ScopedPointer<Label> neuronSelector;
    ScopedPointer<Label> numOfSpikes;
    ScopedPointer<Label> requiredTime;
    ScopedPointer<Label> standDevFactSelector;
    
    ScopedPointer<UtilityButton> nextNeuron;
    ScopedPointer<UtilityButton> previousNeuron;
    String verticalLabel = "Spike Count";
    unsigned long long paintCount = 0;
    int rotation = 0;
    String periods;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RTStatsCanvas);
    
    
};


#endif //RTStatsCanvas_h 
