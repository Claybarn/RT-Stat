//
//  RTStatsProcessor.cpp
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

#include <stdio.h>
#include "RT-StatsProcessor.h"
#include "RT-StatsEditor.h"
//#include <chrono>


RTStatsProcessor::RTStatsProcessor()
: GenericProcessor("RTStats")
{
}


RTStatsProcessor::~RTStatsProcessor()
{
}


AudioProcessorEditor* RTStatsProcessor::createEditor()
{
    editor = new RTStatsEditor(this, true);
    return editor;
}


void RTStatsProcessor::setParameter(int parameterIndex, float newValue)
{
    if (parameterIndex == 0)
    setUserReady(newValue);
    
    else if (parameterIndex == 1)
    setOutputChan(newValue);
    
    else if (parameterIndex == 2){
    setDirection(newValue);
        std::cout<<"parameter index 2, setDirection with " << newValue << " Called \n";
    }
    
    else if (parameterIndex == 3)
    updateSpikeRequirement(newValue);
    
    else if (parameterIndex == 4)
        resetAllThresh();
    
    else if (parameterIndex == 5)
    updateRequiredTime(newValue);
    
    else if (parameterIndex == 6)
        setStandDevFactor(newValue);
    
    else if (parameterIndex == 7)
        allSameDirection = newValue;
    
    else if (parameterIndex == 8){
        numberOfBins = newValue;
    }
}
 

void RTStatsProcessor::process(AudioSampleBuffer& buffer, MidiBuffer& events)
{
    //long long t1 = std::chrono::seconds(std::time(NULL)).count();
    //std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
    /*
    if(t1 > storage){
        storage = t1;
        std::cout<<"\n"<<"SampleRate: " << sampleCounter << "\n";
        std::cout<<"Samples in next buffer: "<<buffer.getNumSamples()<<"\n";
        sampleCounter = 0;
    }
    else{
        sampleCounter += 1;
    }
    */
    checkForEvents(events);
    
    for(TTLbuffer ; TTLbuffer > 0  && outputChan > -1 ; TTLbuffer--){ //
        addEvent(events, TTL, 0, 1, outputChan);
        }
    for(Readybuffer ; Readybuffer > 0  && outputChan > -1 ; Readybuffer--) // to facilitate post-hoc data analysis (know which spikes were included)
        addEvent(events, MESSAGE, 0, 0, outputChan); // find way to add to all_channels.events
}


RTStatsProcessor::Neuron RTStatsProcessor::make_Neuron(int64_t timestamp){
    Neuron neuron;
    neuron.currentAP =timestamp; // ITI's kept in miliseconds
    neuron.countAP = 1;
    neuron.direction = allSameDirection;
    return neuron;
}


void RTStatsProcessor::NeuronSUpdate(uint16_t sortedId, int64_t timestamp){
    
    if (sortedId < NeuronS.size()){ // check to see if already in vector
        NeuronS[sortedId].previousAP = NeuronS[sortedId].currentAP; //make old current AP the last AP

        NeuronS[sortedId].currentAP = timestamp;


        //std::cout<<"Milli since epoch " << Time::currentTimeMillis() << "\n";
        
        int64 currentRate = 1000*get_CurrentITI(sortedId)/double(ticksPerSecond);
        
        /* set min and max */
        if (NeuronS[sortedId].Rates.size() <= requiredSpikes && Time::getCurrentTime().toMilliseconds() <= requiredTime && NeuronS[sortedId].shouldMakeSTDV){
        if (currentRate < NeuronS[sortedId].minRate)
            NeuronS[sortedId].minRate = currentRate;
        if (currentRate > NeuronS[sortedId].maxRate)
            NeuronS[sortedId].maxRate = currentRate;
        }
        
        
        
        if(NeuronS[sortedId].shouldMakeSTDV){ //collect and count current rates until have enough to make histogram
            NeuronS[sortedId].Rates.push_back(currentRate);
            NeuronS[sortedId].countAP += 1;
        }
        
        //std::cout<<"Current Rate: " << currentRate << "\n";
        if ((NeuronS[sortedId].Rates.size() >= requiredSpikes) && Time::getCurrentTime().toMilliseconds() >= requiredTime){
            if ((currentRate >= NeuronS[sortedId].increaseThresh && NeuronS[sortedId].direction == 1) || (currentRate <= NeuronS[sortedId].decreaseThresh && NeuronS[sortedId].direction == -1)){
                NeuronS[sortedId].rewardCount += 1;
                TTLbuffer += 1;
            }
            
    }
}
    else{
        NeuronS.push_back(make_Neuron(timestamp));
    }
}


void RTStatsProcessor::handleEvent(int eventType, MidiMessage& event, int sampleNum)
{
    const uint8_t* buffer = event.getRawData();
    uint8_t type = buffer[0];
    if (type == SPIKE){
            SpikeObject spikeObj;
            unpackSpike(&spikeObj, buffer, event.getRawDataSize());
        
        //std::cout<<"regular: " << spikeObj.timestamp + mcObj.startDate<< "\n";
        
        //long long unix_timestamp = std::chrono::seconds(std::time(NULL)).count();
        //std::chrono::high_resolution_clock::time_point highRez = std::chrono::high_resolution_clock::now();
        //long long unix_timestamp_x_1000 = std::chrono::high_resolution_clock::now();
        //std::chrono::time_point<double> time_span =std::chrono::time_point_cast<std::chrono::duration<double>>(highRez);
        
        //std::cout<<"chrono " << unix_timestamp_x_1000 << "\n";
        //MessageCenter mcObj;
        //std::cout<<"start date: " << mcObj.getStartDate() << "\n";
        //std::cout<<"timestamp: " << spikeObj.timestamp << "\n";
        //std::cout<<"timestamp/40000: " << spikeObj.timestamp/double(40000) << "\n";
        //std::cout<<"stamps: "<<spikeObj.timestamp/double(40000) + mcObj.startDate/double(1000);
        //std::cout<<"\n";
        //std::cout<<time_span<<"\t"<<spikeObj.timestamp<<"\n";
        //std::cout<<"\n";
        //std::cout<<double(spikeObj.timestamp/double(40000) + mcObj.startDate/double(1000))/double(unix_timestamp);
       // std::cout<<"\n";
        //std::cout<<"software: " << spikeObj.timestamp_software << "\n";
            if (spikeObj.sortedId > 0 && userReady){ // If sorted && user ready
                
                NeuronSUpdate(spikeObj.sortedId-1, spikeObj.timestamp_software);
                if (NeuronS[spikeObj.sortedId-1].Rates.size() >= requiredSpikes && Time::getCurrentTime().toMilliseconds() >= requiredTime && NeuronS[spikeObj.sortedId-1].shouldMakeSTDV){
                    createStandDevInfo(spikeObj.sortedId-1);
                    setThresh(spikeObj.sortedId-1);
                    createHistoInfo(spikeObj.sortedId-1);
                    createHistoFactors(spikeObj.sortedId-1);
                    NeuronS[spikeObj.sortedId-1].shouldMakeSTDV = false;
                    NeuronS[spikeObj.sortedId-1].shouldMakeHistoInfo = false;
            }
        }
    }
}


/* Should run:
 if (NeuronS[sortedId].countAP == RTStatsProcessor::requiredSpikes)
 before this function */

void RTStatsProcessor::createStandDevInfo(uint16_t sortedId){
    double workingAverage = 0;
    for (unsigned int i = 0 ; i < NeuronS[sortedId].Rates.size() ; i++){
        workingAverage += NeuronS[sortedId].Rates[i];
    }
    NeuronS[sortedId].average = workingAverage/double(NeuronS[sortedId].Rates.size());
    double  workingStandDev = 0;
    if (NeuronS[sortedId].Rates.size() >= 3){
        for (unsigned int i = 0 ; i < NeuronS[sortedId].Rates.size(); i++){
            workingStandDev += pow(((NeuronS[sortedId].Rates[i]) - NeuronS[sortedId].average), 2);
            std::cout<< double(NeuronS[sortedId].Rates[i]) << "\n";
        }
        workingStandDev /= (double(NeuronS[sortedId].Rates.size() - 1));
        workingStandDev = sqrt(workingStandDev);
        NeuronS[sortedId].standardDev = workingStandDev;
    }
}


/* Calculate ITI info */
double RTStatsProcessor::get_CurrentITI(uint16_t sortedId){
    return NeuronS[sortedId].currentAP - NeuronS[sortedId].previousAP;
}


double RTStatsProcessor::showAverageRate(uint16_t sortedId){
    return NeuronS[sortedId].average;
}




void RTStatsProcessor::setOutputChan(int chan){
    outputChan = chan - 1;
}


void RTStatsProcessor::updateSpikeRequirement(uint16_t userInput){
    requiredSpikes = userInput;
}


void RTStatsProcessor::updateRequiredTime(int64 time){
    requiredTime += 1000*time - requestedTime;
    requestedTime =1000*time;
}


void RTStatsProcessor::setDirection(int num){
    uint16_t sortedId = num/10;
    num = num%10;
    if (NeuronS.size() > 0){
    switch (num){
        case 1:
            NeuronS[sortedId].direction = 0;
            break;
        case 2:// choose to increase
            NeuronS[sortedId].direction = 1;
            break;
        case 3: // choose to decrease
            NeuronS[sortedId].direction = -1;
            break;
    }
    }
}


void RTStatsProcessor::setUserReady(int num){
    userReady = num;
    requiredTime = Time::getCurrentTime().toMilliseconds() + requestedTime;
    if (num == 1)
        Readybuffer +=1;
}


void RTStatsProcessor::setStandDevFactor(float num){
    standDevFactor = num;
}



void RTStatsProcessor::setThresh(uint16_t sortedId){
    //sortedId -=1;
    NeuronS[sortedId].increaseThresh = showAverageRate(sortedId) + (standDevFactor * NeuronS[sortedId].standardDev)*standDevFactor;
    NeuronS[sortedId].decreaseThresh = showAverageRate(sortedId) - (standDevFactor * NeuronS[sortedId].standardDev)*standDevFactor;
}


void RTStatsProcessor::createHistoInfo(uint16_t sortedId){ //Take user visible sortedId or internal (1 vs 0 index)?
    std::cout<<"Creating histoInfo \n";
    double binSize = (NeuronS[sortedId].maxRate - NeuronS[sortedId].minRate) / numberOfBins;
    if(NeuronS[sortedId].shouldMakeHistoInfo){
        NeuronS[sortedId].histoInfo.clear();
        for(unsigned int binParse = 0 ; binParse < numberOfBins ; binParse++){
            NeuronS[sortedId].histoInfo.push_back(0);
        }
    }
    std::cout<<"HistoInfo Size: " << NeuronS[sortedId].histoInfo.size() << "\n";
    for(unsigned int rateParse = 0 ; rateParse < NeuronS[sortedId].Rates.size() ; rateParse++)
                NeuronS[sortedId].histoInfo[binMe(0, numberOfBins-1, binSize, NeuronS[sortedId].Rates[rateParse]-NeuronS[sortedId].minRate)] += 1;
    NeuronS[sortedId].histoInfo[NeuronS[sortedId].histoInfo.size()-1] += 1;
    for (int i = 0 ; i < NeuronS[sortedId].histoInfo.size() ; i++)
        std::cout<< NeuronS[sortedId].histoInfo[i] << "\n";
}


void RTStatsProcessor::createHistoFactors(uint16_t sortedId){
    std::cout<<"Creating HistoFactors \n";
    NeuronS[sortedId].histoFactors.clear();
    unsigned int maxBin = 0;
    for (int i = 0 ; i < NeuronS[sortedId].histoInfo.size() ; i++){
        if (NeuronS[sortedId].histoInfo[i] > NeuronS[sortedId].histoInfo[maxBin]){
            maxBin = i;
        }
    }
    NeuronS[sortedId].maximumCount = NeuronS[sortedId].histoInfo[maxBin];
    NeuronS[sortedId].histoFactors.push_back(NeuronS[sortedId].histoInfo.size());
    for (int i = 0 ; i <  NeuronS[sortedId].histoFactors[0]; i++){
        NeuronS[sortedId].histoFactors.push_back(NeuronS[sortedId].histoInfo[i]/ double(NeuronS[sortedId].histoInfo[maxBin]));
    }
}

bool RTStatsProcessor::enable()
{
    RTStatsEditor* editor = (RTStatsEditor*) getEditor();
    editor->enable();
    return true;
    
}


unsigned int * RTStatsProcessor::showHistoInfo(uint16_t sortedId){
    return & NeuronS[sortedId].histoInfo[0];
}


float * RTStatsProcessor::showHistoFactors(uint16_t sortedId){
    return & NeuronS[sortedId].histoFactors[0];
}


bool RTStatsProcessor::showSTDVStatus(uint16_t sortedId){
    return NeuronS[sortedId].shouldMakeSTDV;
}


double RTStatsProcessor::showIncreaseThresh(uint16_t sortedId){
    return NeuronS[sortedId].increaseThresh;
}


double RTStatsProcessor::showDecreaseThresh(uint16_t sortedId){
   return NeuronS[sortedId].decreaseThresh;
}

double RTStatsProcessor::showMaxRate(uint16_t sortedId){
    return NeuronS[sortedId].maxRate;
}


double RTStatsProcessor::showMinRate(uint16_t sortedId){
    return NeuronS[sortedId].minRate;
}


unsigned long RTStatsProcessor::getNeuronSSize(){
    return NeuronS.size();
}


void RTStatsProcessor::trainUpAll(){
    for (unsigned int i = 0 ; i < NeuronS.size() ; i++){
        setParameter(2, i*10+2); // update existing
    }
    setParameter(7, 1); // All New will be set to train up
}


void RTStatsProcessor::trainDownAll(){
    for (unsigned int i = 0 ; i < NeuronS.size() ; i++)
        setParameter(2, i*10+3); // update existing
        setParameter(7, -1); //All New will be set to train down
}


int RTStatsProcessor::showDirection (uint16_t sortedId){
    if(NeuronS.size() > 0)
    return NeuronS[sortedId].direction;
    else{
        return 0;
    }
}

double RTStatsProcessor::showSTDV(uint16_t sortedId){
    return NeuronS[sortedId].standardDev;
}

void RTStatsProcessor::resetAllThresh(){
    for(unsigned int i = 0 ; i < NeuronS.size() ; i++)
        setThresh(i);
}


double RTStatsProcessor::binMe(unsigned int startBin, unsigned int endBin, double binSize, double zeroedRate){
    
    unsigned int binsInRange = (endBin - startBin)+1;
    unsigned int binsToSearch = binsInRange/2;
    if (binsToSearch <= 1){
        return startBin;
    }
    else if (zeroedRate <= (startBin+binsToSearch-1)*binSize){
        return binMe(startBin,startBin+(binsToSearch-1),binSize,zeroedRate);
    }
    else if (zeroedRate >= (startBin+binsToSearch-1) * binSize){
        return binMe(startBin+(binsToSearch-1),endBin,binSize,zeroedRate);
    }
    else{
        std::cout<<"Else happened in binMe \n";
        return 0;
    }
}

long long RTStatsProcessor::showSpikesToGo(uint16_t sortedId){
    return requiredSpikes - NeuronS[sortedId].countAP;
}
int64 RTStatsProcessor::showTimeToGo(){
    return (requiredTime - Time::getCurrentTime().toMilliseconds())/1000; //return in seconds
}

unsigned long long RTStatsProcessor::showMaximumCount(uint16_t sortedId){
    return NeuronS[sortedId].maximumCount;
}


void RTStatsProcessor::trueShouldMakeHisto(uint16_t sortedId){
    NeuronS[sortedId].shouldMakeHistoInfo = true;
}

bool RTStatsProcessor::showShouldMakeHistoInfoStatus(uint16_t sortedId){
    return NeuronS[sortedId].shouldMakeHistoInfo;
}

unsigned long long RTStatsProcessor::showRewardCount(uint16_t sortedId){
    return NeuronS[sortedId].rewardCount;
}

