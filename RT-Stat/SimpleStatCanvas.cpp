//
//  SimpleStatCanvas.cpp
//  SimpleStat
//
//  Created by Clayton Barnes on 6/26/16.
//  Copyright © 2016 Clayton Barnes. All rights reserved.
//

#include "SimpleStatCanvas.h"
//Was: SimpleStatCanvas::SimpleStatCanvas(SimpleStatProcessor* parentNode) : processor(parentNode)
SimpleStatCanvas::SimpleStatCanvas(SimpleStatProcessor* parentNode) : processor(parentNode)
{
    viewport = new Viewport();

    viewport->setScrollBarsShown(true, true);
    addAndMakeVisible(viewport);
    scrollBarThickness = viewport->getScrollBarThickness();
    
    // to change neuron being viewed
    nextNeuron = new UtilityButton("Next Sorted ID", Font("Small Text", 13, Font::plain));
    nextNeuron->setRadius(3.0f);
    nextNeuron->addListener(this);
    addAndMakeVisible(nextNeuron);
    
    previousNeuron = new UtilityButton("Prev. Sorted ID", Font("Small Text", 13, Font::plain));
    previousNeuron->setRadius(3.0f);
    previousNeuron->addListener(this);
    addAndMakeVisible(previousNeuron);
    
    
    /* Adding output channel user interface, should match gate channel on arduino output module*/
    outputSelector = new ComboBox();
    outputSelector->addItem("Output CH",1);
    outputSelector->addListener(this);
    for (int i = 1; i < 9; i++)
    {
        outputSelector->addItem(String(i),i+1);
    }
    outputSelector->setSelectedId(1);
    outputSelector->setTooltip("Select the channel that will be used to communicate with other modules.");
    outputSelector->setColour(Label::textColourId, Colours::white);
    outputSelector->setColour(Label::backgroundColourId, Colours::black);
    addAndMakeVisible(outputSelector);
    
    /* Sets direction the spike-count will be trained */
    directionSelector = new ComboBox();
    directionSelector->addItem("No TTL",1);
    directionSelector->addItem("Decrease",2);
    directionSelector->addItem("Increase",3);
    directionSelector->addListener(this);
    directionSelector->setSelectedId(1);
    directionSelector->setTooltip("Choose the direction the spike-count will be trained.");
    directionSelector->setColour(Label::textColourId, Colours::white);
    directionSelector->setColour(Label::backgroundColourId, Colours::black);
    addAndMakeVisible(directionSelector);
    
    // button to control the number of spikes needed before TTL can be sent
    numOfSpikes = new Label("Number of Spikes","100");
    numOfSpikes->setFont(Font("Default", 13, Font::plain));
    numOfSpikes->setEditable(true);
    numOfSpikes->addListener(this);
    numOfSpikes->setColour(Label::textColourId, Colours::black);
    numOfSpikes->setColour(Label::backgroundColourId, Colours::transparentBlack);
    numOfSpikes->setTooltip("Enter in the number of spikes required before a TTL message can be sent, default is 100.");
    addAndMakeVisible(numOfSpikes);
    
    
    /* Adding button to control the time that must elapse before TTL can be sent */
    requiredTime = new Label("Required Time","120");
    requiredTime->setFont(Font("Default", 13, Font::plain));
    requiredTime->setEditable(true);
    requiredTime->addListener(this);
    requiredTime->setColour(Label::textColourId, Colours::black);
    requiredTime->setColour(Label::backgroundColourId, Colours::transparentBlack);
    requiredTime->setTooltip("Enter in seconds that must be recorded before a TTL message can be sent, the default is 120 seconds.");
    addAndMakeVisible(requiredTime);
    
    standDevFactSelector = new Label("STDV Factor","1.0");
    standDevFactSelector->setFont(Font("Default", 13, Font::plain));
    standDevFactSelector->setEditable(true);
    standDevFactSelector->addListener(this);
    standDevFactSelector->setColour(Label::textColourId, Colours::black);
    standDevFactSelector->setColour(Label::backgroundColourId, Colours::transparentBlack);
    standDevFactSelector->setTooltip("Enter in how many standard deviations a instant rate must be outside of in order to trigger a TTL, the default is 1.0 (can use decimals).");
    addAndMakeVisible(standDevFactSelector);
    
    
    binNumSelector = new Label("binNumSelector","100");
    binNumSelector->setFont(Font("Default", 13, Font::plain));
    binNumSelector->setEditable(true);
    binNumSelector->addListener(this);
    binNumSelector->setColour(Label::textColourId, Colour(0,18,43));
    binNumSelector->setColour(Label::backgroundColourId, Colours::white);
    binNumSelector->setTooltip("Enter in the number of bins the histogram should use.");
    addAndMakeVisible(binNumSelector);
    
}


void SimpleStatCanvas::refreshState(){
    
}


void SimpleStatCanvas::update(){
    repaint();
}



void SimpleStatCanvas::beginAnimation(){
   startCallbacks();
}


void SimpleStatCanvas::endAnimation(){
    stopCallbacks();
}

void SimpleStatCanvas::resized(){
    nextNeuron->setBounds(getWidth()-130, 10, 120, 20);
    previousNeuron->setBounds(10, 10, 120, 20);
    numOfSpikes->setBounds(170,getHeight()-70, 60, 20); //70
    requiredTime->setBounds(170,getHeight()-50,60,20); //50
    standDevFactSelector->setBounds(170,getHeight()-30,60,20); //30
    outputSelector->setBounds(390,getHeight()-30,100,20);
    directionSelector->setBounds(390,getHeight()-50,100,20);
    binNumSelector->setBounds(getWidth()/2+getWidth()/20-20,getHeight()-135,30,20);
    if(processor->showDirection(sortedId) == 1)
        directionSelector->setSelectedId(3);
    else if(processor->showDirection(sortedId) == -1)
        directionSelector->setSelectedId(2);
    else if(processor->showDirection(sortedId) == 0)
        directionSelector->setSelectedId(1);
    
}

void SimpleStatCanvas::setParameter(int index, float newValue){
    if (index == 0){
        sortedId = newValue;
    }
    else if (index == 1){
        repaint();
    }
}


void SimpleStatCanvas::setParameter(int, int, int, float){
    
}

void SimpleStatCanvas::paint(Graphics& g){

    paintCount += 1; 
    int width = getWidth();
    int height = getHeight();
    g.setColour(Colour(0,18,43));
    g.fillRect(getLocalBounds());

    g.setColour(Colours::white);
    g.setFont(Font("Default", 20, Font::bold));
    g.drawText("Sorted ID: " + String(sortedId + 1), width/2-100, 10, 200, 20, Justification::centredTop, true);
    
    
    g.setColour(Colours::white);
    g.fillRoundedRectangle(5, 0, width-10, 40, 5);
    
     //encloses graph
    g.fillRect(5, 70, width/2+width/20+10, height-180);
    
    // encloses TTL info
   g.fillRoundedRectangle(5, height-105, 490, 100, 10);
    
    g.setColour(Colour(0,18,43)); //background color
    g.setFont(Font("Default", 20, Font::bold));
    g.drawText("Sorted ID: " + String(sortedId + 1), getWidth()/2-100, 10, 200, 20, Justification::centredTop, true);
    
    g.setColour(Colour(255,140,0));
    
    // axis
    g.fillRect(width/20-5, 80,width/2+5, height-230);
    
    //graph background
    g.setColour(Colours::lightgrey);
    g.fillRect(width/20, 80,width/2, height-235);
    
    
    
    g.setColour(Colour(0,18,43));
    g.setFont(Font("Default", 14, Font::plain));
    for(int i = 0 ; i < 12 ; i++){
        g.drawText(verticalLabel.String::substring(i,i+1),width/40,i*height/30 + height/5+30, 20, 20,Justification::left,false);
    }
    g.drawText("Rate (Hz)",9*(width/40) + width/20,getHeight()-135,100,20,Justification::left,false);
    g.drawText("Number of Bins:",getWidth()/2+getWidth()/20-150,getHeight()-135,150,20,Justification::left, false);
    
    g.setColour(Colour(0,18,43));
    g.setFont(Font("Default", 16, Font::underlined | Font::bold));
    g.drawText("TTL Requirement Info", 10, height - 100, 480, 20, Justification::centred, true);
    
    g.setFont(Font("Default", 16, Font::plain | Font::bold));
    g.drawText("Output Channel:",230 , height - 30, 150, 20, Justification::left, true);
    g.drawText("Direction:",230 , height - 50, 160, 20, Justification::left, true);
    g.drawText("Number of Spikes:", 10, height - 70 , 160, 20, Justification::left, true);
    g.drawText("Time (s):", 10, height - 50, 160, 20, Justification::left, true);
    g.drawText("STDV factor:",10 , height - 30, 160, 20, Justification::left, true);
    
    
    g.setColour(Colour(255,140,0)); // Histogram Color
    g.setFont(Font("Default", 16, Font::plain));
    if(processor->getNeuronSSize() > 0){
        if(!processor->showSTDVStatus(sortedId)){
            g.setColour(Colour(255,140,0));
        
            float * histoFactor = processor->showHistoFactors(sortedId);
            
            int rectWidth = width/(2*(*histoFactor));
            g.drawText("Bin Size: " + String((processor->showMaxRate(sortedId)-processor->showMinRate(sortedId))/double(*histoFactor*1000)) + " Hz",9*(width/20) + width/10 + width/25+50,180,300,20,Justification::left, true);
            for(int i = 1 ; i < *histoFactor ; i++){
                
                int rectHeight = *(histoFactor + i)*width/3;
                int start = i*rectWidth + width/20;
                int end = height-155-rectHeight;
                g.fillRect(start,end,rectWidth,rectHeight);
                
            }
          
            g.drawText("Highest Count: " + String(processor->showMaximumCount(sortedId)), width/10, 80, 300, 30, Justification::centred, true);
            g.drawText("Average Rate: " + String(processor->showAverageRate(sortedId)/double(1000)) + " Hz",9*(width/20) + width/10 + width/25+50,60,300,20,Justification::left, true);
            g.drawText("Standard Deviation: " + String(processor->showSTDV(sortedId)/double(1000)) + " Hz",9*(width/20) + width/10 + width/25+50,80,300,20,Justification::left, true);
            g.drawText("Increase Threshold: " + String(processor->showIncreaseThresh(sortedId)/double(1000)) + " Hz",9*(width/20) + width/10 + width/25+50,100,300,20,Justification::left, true);
            g.drawText("Decease Threshold: " + String(processor->showDecreaseThresh(sortedId)/double(1000)) + " Hz",9*(width/20) + width/10 + width/25+50,120,300,20,Justification::left, true);
            g.drawText("Maximum Rate: " + String(processor->showMaxRate(sortedId)/double(1000)) + " Hz",9*(width/20) + width/10 + width/25+50,140,300,20,Justification::left, true);
            g.drawText("Minimum Rate: " + String(processor->showMinRate(sortedId)/double(1000)) + " Hz",9*(width/20) + width/10 + width/25+50,160,300,20,Justification::left, true);
            g.drawText("TTL Count: " + String(processor->showRewardCount(sortedId)),9*(width/20) + width/10 + width/25+50,200,300,20,Justification::left, true);
            
        }
        else{
            g.setColour(Colour(255,140,0)); // Histogram Color
            g.setFont(Font("Default", 16, Font::plain));
            
            
            if(paintCount%20<(paintCount-1)%20){
                rotation += 1;
               if (rotation == 1)
                    periods = ".";
               else if( rotation == 2)
                    periods = "..";
               else if (rotation == 3)
                    periods = "...";
               else if (rotation == 4){
                    periods = "";
                    rotation = 0;
                }
            }
            
            g.drawText("Collecting data"+ periods,9*(width/20) + width/10 + width/25+50,height-50,300,20,Justification::left, true);
            
            String spikeInfo;
            if(processor->showSpikesToGo(sortedId)>0)
                spikeInfo = String(processor->showSpikesToGo(sortedId));
            else
                spikeInfo = String(0);
            g.drawText("Spikes To Go: " + spikeInfo,9*(width/20) + width/10 + width/25+50,60,300,20,Justification::left, true);
            
            String timeInfo;
            if(processor->showTimeToGo()>0)
                timeInfo = String(processor->showTimeToGo());
            else
                timeInfo = String(0);
            g.drawText("Time: " + timeInfo + " Seconds",9*(width/20) + width/10 + width/25+50,80,300,20,Justification::left, true);
        }
    }
    else{
        g.setColour(Colour(255,140,0)); // Histogram Color
        g.setFont(Font("Default", 16, Font::plain));
        g.drawText("No sorted neurons available",9*(width/20) + width/10 + width/25+50,height-50,300,20,Justification::centred, true);
    }
}


void SimpleStatCanvas::refresh(){
    repaint();
    
}


void SimpleStatCanvas::comboBoxChanged(ComboBox* c){

        if (c == outputSelector){
            int chan = outputSelector->getText().getIntValue();
            processor->setParameter(1,chan);
        }
        
        else if (c == directionSelector){
            int num = directionSelector->getSelectedId();
            if (num == 1)
                processor->setParameter(2,10*sortedId+0);
            else if (num == 2)
                processor->setParameter(2,10*sortedId+3);
            else if (num == 3)
                processor->setParameter(2,10*sortedId+2);
        }
    
        repaint();
        resized();
}


void SimpleStatCanvas::labelTextChanged(Label* labelThatHasChanged){
    
    if(labelThatHasChanged == numOfSpikes){
        processor->setParameter(3,numOfSpikes->getText().getIntValue());
    }

    
    else if(labelThatHasChanged == requiredTime){
        processor->setParameter(5,requiredTime->getText().getIntValue());
    }
    else if(labelThatHasChanged == standDevFactSelector){
        processor->setParameter(6,standDevFactSelector->getText().getFloatValue());
        processor->setParameter(4,0);
    }
    else if(labelThatHasChanged == binNumSelector){
        processor->trueShouldMakeHisto(sortedId);
        processor->setParameter(8,binNumSelector->getText().getIntValue());
        processor->createHistoInfo(sortedId);
        processor->createHistoFactors(sortedId);
    }
    repaint();
    resized();
}


void SimpleStatCanvas::buttonClicked(Button* button){
    if (button == nextNeuron && sortedId+1 < processor->getNeuronSSize()){
        setParameter(0, sortedId+1);
    }
    else if (button == previousNeuron && sortedId > 0){
        setParameter(0, sortedId-1);
    }
    repaint();
    resized();
}


