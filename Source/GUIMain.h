/*
  ==============================================================================

    GUIMain.h
    Created: 1 Oct 2020 2:52:15pm
    Author:  cjack

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "GUIGlobals.h"


class GuiMainSlider : public juce::Component 
{
public:
    GuiMainSlider(const char* text, bool showText, double min, double max, double step);
    void resized() override;
    void paint(juce::Graphics& g) override;
    juce::Slider slider;
    juce::Label text;
    const bool showText;
};

class GuiMainPanel : public juce::Component 
{
public:
    GuiMainPanel();
    void resized() override;
    GuiMainSlider ratioSlider;
    GuiMainSlider indexSlider;
    juce::ToggleButton hoverButton;
    juce::ToggleButton recordButtonA;
    juce::ToggleButton recordButtonB;
};