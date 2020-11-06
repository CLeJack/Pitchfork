/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"


//==============================================================================

class PitchforkAudioProcessorEditor  : public juce::AudioProcessorEditor, 
    public juce::Slider::Listener,
    public juce::Button::Listener
{
public:
    PitchforkAudioProcessorEditor (PitchforkAudioProcessor&);
    ~PitchforkAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void sliderValueChanged(juce::Slider* slider) override ;
    void buttonClicked(juce::Button* button) override;
    
    void updateSpectrum();

    void setComponents();
    GUIState getTabState();
    
    
    

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PitchforkAudioProcessor& audioProcessor;

    GuiMainPanel guiMainPanel;
    GuiSpectrum guiLive;
    GuiSpectrum guiHistoryA;
    GuiSpectrum guiHistoryB;
    GuiTabs guiTabs;
    

    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PitchforkAudioProcessorEditor)
};