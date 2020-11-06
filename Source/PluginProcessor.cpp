/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/
#pragma once


#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PitchforkAudioProcessor::PitchforkAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{

    pluginState = PluginState::waiting;
    
}

PitchforkAudioProcessor::~PitchforkAudioProcessor()
{
}

//==============================================================================
const juce::String PitchforkAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PitchforkAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PitchforkAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PitchforkAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double PitchforkAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PitchforkAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int PitchforkAudioProcessor::getCurrentProgram()
{
    return 0;
}

void PitchforkAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String PitchforkAudioProcessor::getProgramName (int index)
{
    return {};
}

void PitchforkAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void PitchforkAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void PitchforkAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool PitchforkAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void PitchforkAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    
    switch(pluginState)
    {
        case PluginState::ready :   
            ready (buffer);
            break;
        default :
            waiting();
            break;
    }

    auto* in = buffer.getReadPointer(0);
    auto* out = buffer.getWritePointer(0);

    for (int i = 0; i < buffer.getNumSamples(); i++) 
    {
        out[i] = in[i];
    }
    
}

//==============================================================================
bool PitchforkAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* PitchforkAudioProcessor::createEditor()
{
    return new PitchforkAudioProcessorEditor (*this);
    //return nullptr;
}

//==============================================================================
void PitchforkAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void PitchforkAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PitchforkAudioProcessor();
}


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// custom functions


//plugin state processing ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void PitchforkAudioProcessor::waiting()
{
    if(getSampleRate() > 0)
    {
        pitchfork.initialize(getSampleRate(), getBlockSize());
        pluginState = PluginState::ready;
    }
}

void PitchforkAudioProcessor::ready(juce::AudioBuffer<float>& buffer)
{

    
    //add the block to history
    auto* channelData = buffer.getReadPointer(0);
    for (int i = 0; i < buffer.getNumSamples(); i++)
    {
        pitchfork.signalHistory.get()->push(normToInt16Range(channelData[i]));
    }

    /*
    //down sample the signal; currently without filtering
    fvec trueSignal = scribe.history.get()->toOrderedVec();
    
    for (int i = 0; i < scribe.historyDS.size(); i++)
    {
        scribe.historyDS[i] = trueSignal[i * scribe.audio.ds.factor];
    }
    */

    pitchfork.updateFundamental();

    
    pitchfork.frameCounter = (pitchfork.frameCounter + 1) % pitchfork.fpsBlocks; 

    auto editor = (PitchforkAudioProcessorEditor*)getActiveEditor();
    
    if (pitchfork.frameCounter == 0 && editor != nullptr)
    {
        //const juce::MessageManagerLock mmLock;
        editor->updateSpectrum();
        /*
        switch (editor->getTabState()) 
        {
        case GUIState::live:
            editor->updateSpectrum();
            break;
        case GUIState::historyA:
            editor->updateSpectrum();
            break;
        case GUIState::historyB:
            editor->updupdateSpectrumateSignal();
            break;
        }
        */
        editor->repaint();
        
    }
    

}

//Gui state processing ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~