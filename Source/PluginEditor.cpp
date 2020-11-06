/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/
#pragma once
#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PitchforkAudioProcessorEditor::PitchforkAudioProcessorEditor(PitchforkAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p),
    guiLive(pitchfork.frequencies.size(), pitchfork.tuning.octaveSize),
    guiHistoryA(pitchfork.frequencies.size(), pitchfork.tuning.octaveSize),
    guiHistoryB(pitchfork.frequencies.size(), pitchfork.tuning.octaveSize)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (REFX, REFY);

    setLookAndFeel(new PaperLookAndFeel());

    guiMainPanel.setLookAndFeel(new PaperLookAndFeel());
    guiTabs.setLookAndFeel(new PaperLookAndFeel());
    guiLive.setLookAndFeel(new PaperLookAndFeel());
    guiHistoryA.setLookAndFeel(new PaperLookAndFeel());
    guiHistoryB.setLookAndFeel(new PaperLookAndFeel());

    addAndMakeVisible(guiMainPanel);
    addAndMakeVisible(guiTabs);

    guiTabs.addTab("Live",   PAPER, &guiLive,   false);
    guiTabs.addTab("History A", PAPER, &guiHistoryA, false);
    guiTabs.addTab("History B", PAPER, &guiHistoryB,   false);

    
    guiMainPanel.resized();
    guiLive.resized();
    guiHistoryA.resized();
    guiHistoryB.resized();

    guiMainPanel.ratioSlider.slider.addListener(this);
    guiMainPanel.indexSlider.slider.addListener(this);

    guiMainPanel.recordButtonA.addListener(this);
    guiMainPanel.recordButtonB.addListener(this);
    guiMainPanel.hoverButton.addListener(this);
    
    setComponents();
    

}

PitchforkAudioProcessorEditor::~PitchforkAudioProcessorEditor()
{
}

//==============================================================================
void PitchforkAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    
}

void PitchforkAudioProcessorEditor::resized()
{
    auto area = getLocalBounds();
    guiMainPanel.setBounds(area.removeFromLeft(area.getWidth() * .333));
    guiTabs.setBounds (area);
}




GUIState PitchforkAudioProcessorEditor::getTabState() 
{
    return guiTabs.guiState;
}

void PitchforkAudioProcessorEditor::sliderValueChanged(juce::Slider* slider) 
{

    if (slider == &guiMainPanel.ratioSlider.slider)
    {
        params.octaveRatio = slider->getValue();
    }

    else if (slider == &guiMainPanel.indexSlider.slider)
    {
        params.historyIndex = (int)slider->getValue();
    }



}

void PitchforkAudioProcessorEditor::buttonClicked(juce::Button* button) 
{
    if (button == &guiMainPanel.hoverButton) 
    {
        params.hoverOn = button->getToggleState();
    }
    else if (button == &guiMainPanel.recordButtonA) 
    {
        params.recordA = button->getToggleState();
    }
    else if (button == &guiMainPanel.recordButtonA) 
    {
        params.recordB = button->getToggleState();
    }
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void PitchforkAudioProcessorEditor::setComponents()
{
    guiMainPanel.ratioSlider.slider.setValue(params.octaveRatio);
    guiMainPanel.indexSlider.slider.setValue(params.historyIndex);
    guiMainPanel.hoverButton.setToggleState(params.hoverOn, juce::NotificationType::dontSendNotification);
    guiMainPanel.recordButtonA.setToggleState(false, juce::NotificationType::dontSendNotification);
    guiMainPanel.recordButtonB.setToggleState(false, juce::NotificationType::dontSendNotification);

}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void PitchforkAudioProcessorEditor::updateSpectrum()
{
    for (int i = 0; i < pitchfork.freqWeights.size(); i++)
    {
        guiLive.freqs.weights[i] = pitchfork.freqWeights[i];
    }
    for (int i = 0; i < pitchfork.harmonicWeights.size(); i++) 
    {
        guiLive.harmonics.weights[i] = pitchfork.harmonicWeights[i];
    }

    if (params.recordA) 
    {
        for (int i = 0; i < pitchfork.freqWeights.size(); i++)
        {
            guiHistoryA.freqs.weights[i] = pitchfork.freqHistA[params.historyIndex][i];
        }
        for (int i = 0; i < pitchfork.harmonicWeights.size(); i++)
        {
            guiHistoryA.harmonics.weights[i] = pitchfork.harmHistA[params.historyIndex][i];
        }
    }

    if (params.recordB)
    {
        for (int i = 0; i < pitchfork.freqWeights.size(); i++)
        {
            guiHistoryB.freqs.weights[i] = pitchfork.freqHistB[params.historyIndex][i];
        }
        for (int i = 0; i < pitchfork.harmonicWeights.size(); i++)
        {
            guiHistoryB.harmonics.weights[i] = pitchfork.harmHistB[params.historyIndex][i];
        }
    }


}
