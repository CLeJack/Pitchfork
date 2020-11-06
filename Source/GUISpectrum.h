/*
  ==============================================================================

    GUISpectrum.h
    Created: 1 Oct 2020 2:51:19pm
    Author:  cjack

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "GUIGlobals.h"
#include "Stats.h"



class SpectrumBackground : public juce::Component 
{
public:
    SpectrumBackground(float barCount, float octaveSize);

    void paint(juce::Graphics& g) override;
    
    float barCount;
    float octaveSize;

};

class SpectrumBars : public juce::Component 
{
public:
    SpectrumBars(float barCount);
    void paint(juce::Graphics& g) override;

    std::vector<float> weights;
};


class SpectrumNotes : public juce::Component 
{
public:
    SpectrumNotes(float barCount, float octaveSize);
    void paint(juce::Graphics& g) override;

    //white key = 0; black key = 1
    std::vector<int> keyReference = { 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0 };
    std::vector<int> keys;
};


//1. GuiSpectrum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class GuiSpectrum : public juce::Component
{
public:
    GuiSpectrum(int barCount, int octaveSize);
    void resized() override;

    SpectrumBackground background1;
    SpectrumNotes notes1;
    SpectrumBars freqs;

    SpectrumBackground background2;
    SpectrumBars harmonics;

    float maxfreq = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GuiSpectrum);
};
