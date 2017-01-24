//
//  SimpleStatProcessor.hpp
//  SimpleStat
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

#ifndef EXAMPLEPROCESSOR_H_INCLUDED
#define EXAMPLEPROCESSOR_H_INCLUDED

#ifdef _WIN32
#include <Windows.h>
#include <vector>
#include<String>
#endif

#include <ProcessorHeaders.h>
#include <SpikeLib.h>


/**
 
 Collects spike-count rates till user specified time and number of spike counts, creates histogram, then broadcasts a TTL if user specified requirements are met
 
 @see GenericProcessor
 
 */

class SimpleStatProcessor : public GenericProcessor

{
public:
    
    
    SimpleStatProcessor();
    
    
    ~SimpleStatProcessor();
    
    struct Neuron { // Enables sortedId specific information storage
        double currentAP = 0;
        double previousAP = 0;
        unsigned long long countAP = 0;
        unsigned long long rewardCount = 0;
        double maxRate = 0;
        double average = 0;
        double minRate = 9999999999;
        double standardDev = 0;
        unsigned long long maximumCount = 0;
        double increaseThresh;
        double decreaseThresh;
        int direction = 0; // 0 not training, 1 increase, -1 decrease
        bool shouldMakeSTDV = true;
        bool shouldMakeHistoInfo = true;
        std::vector <double> Rates;
        std::vector <unsigned int> histoInfo;
        std::vector <float> histoFactors;
    };
    
    bool isSource()
    {
        return false;
    }
    
    
    bool isSink()
    {
        return false;
    }
    

    bool hasEditor() const
    {
        return true;
    }
    
    AudioProcessorEditor* createEditor();
    void process(AudioSampleBuffer& buffer, MidiBuffer& events);
    void setParameter(int parameterIndex, float newValue);
    void handleEvent(int eventType, MidiMessage& event, int sampleNum);
    void NeuronSUpdate(uint16_t sortedId, int64_t timestamp);
    bool enable();
    Neuron make_Neuron(int64_t);
    double get_CurrentITI(uint16_t sortedId);
    void setOutputChan(int chan);
    void updateSpikeRequirement(uint16_t);
    void updateRequiredTime(int64 time);
    void setDirection(int num);
    void setStandDevFactor(float num);
    void createStandDevInfo(uint16_t sortedId);
    void setThresh(uint16_t sortedId);
    void setUserReady(int);
    void createHistoInfo(uint16_t sortedId);
    void createHistoFactors(uint16_t sortedId);
    void trainUpAll();
    void trainDownAll();
    int showDirection (uint16_t sortedId);
    double showSTDV(uint16_t sortedId);
    void resetAllThresh();
    double binMe(unsigned int startBin, unsigned int endBin, double binSize, double zeroedRate);
    
    
    /* reveal stored info, could just make friend classes though */
    double showAverageRate(uint16_t sortedId);
    unsigned int * showHistoInfo(uint16_t sortedId);
    float * showHistoFactors(uint16_t sortedId);
    bool showHistoStatus(uint16_t sortedId);
    unsigned long getNeuronSSize();
    double showIncreaseThresh(uint16_t sortedId);
    double showDecreaseThresh(uint16_t sortedId);
    double showMaxRate(uint16_t sortedId);
    double showMinRate(uint16_t sortedId);
    unsigned long long showMaximumCount(uint16_t sortedId);
    bool showSTDVStatus(uint16_t sortedId);
    void trueShouldMakeHisto(uint16_t sortedId);
    long long showSpikesToGo(uint16_t sortedId);
    int64 showTimeToGo();
    bool showShouldMakeHistoInfoStatus(uint16_t sortedId);
    unsigned long long showRewardCount(uint16_t sortedId);
    
private:
    long long sampleCounter = 0;
    //std::chrono::high_resolution_clock::time_point storage = std::chrono::high_resolution_clock::now();
    long long storage = std::chrono::seconds(std::time(NULL)).count();
    
    unsigned int numberOfBins = 100;
    bool userReady = false;
    bool trigger = false;
    int outputChan = -1;
    double standDevFactor = 1.0;
    unsigned long long requiredSpikes = 100; // default: must collect 100 spikes before TTL
    int64 requestedTime = 120000;
    int64 requiredTime = Time::getCurrentTime().toMilliseconds() + requestedTime; // default, must collect for 2 min before TTL
    unsigned int TTLbuffer = 0;
    unsigned int Readybuffer = 0;
    std::vector <Neuron> NeuronS;
    int allSameDirection = 0;
    Time t;
    //int64 ticksPerSecond = t.getHighResolutionTicksPerSecond();
    int64 ticksPerSecond = t.getHighResolutionTicksPerSecond();
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SimpleStatProcessor);
    
};

#endif  // SIMPLESTAT_H_INCLUDED

