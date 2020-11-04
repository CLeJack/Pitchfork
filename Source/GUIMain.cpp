/*
  ==============================================================================

    GUIMain.cpp
    Created: 1 Oct 2020 2:52:15pm
    Author:  cjack

  ==============================================================================
*/

#include "GUIMain.h"


GuiMainSlider::GuiMainSlider(const char* text, bool showText, double min, double max, double step) : 
    showText(showText)
{
    slider.setRange(min, max, step);
    this->text.setText(text, juce::NotificationType::dontSendNotification);
    this->text.setJustificationType(juce::Justification::centredTop);
    this->text.setBorderSize(juce::BorderSize<int>(0));

    slider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    slider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 0, 0);

    addAndMakeVisible(this->text);
    addAndMakeVisible(slider);
}

void GuiMainSlider::resized()
{
    auto area = getLocalBounds();
    float third = area.getHeight() * 0.333f;
    if (showText) 
    {

        auto sliderArea = area.removeFromTop(third * 2);

        auto textArea = area;

        slider.setBounds(sliderArea);
        text.setBounds(textArea);
    }
    else 
    {
        slider.setBounds(area);
    }
    slider.setTextBoxStyle(
        juce::Slider::TextEntryBoxPosition::TextBoxBelow,
        false,
        slider.getWidth(), 
        slider.getHeight() * 0.5f
    );
    
}

void GuiMainSlider::paint(juce::Graphics& g) 
{
    auto area = slider.getBoundsInParent();

    float padH = area.getHeight() * 0.333;

    area.removeFromTop(padH);
    area.removeFromBottom(padH);

    g.drawText(juce::String(slider.getValue(), 0, false), area, juce::Justification::centred);
}

GuiMainPanel::GuiMainPanel() :
    ratioSlider("Octave Ratio", true, 1, 10, 1),
    indexSlider("History Index", true, 0, 599, 1) //tie this into Pitchfork vars later
{

    addAndMakeVisible(ratioSlider);
    addAndMakeVisible(indexSlider);
    addAndMakeVisible(hoverButton);
    addAndMakeVisible(recordButton);

    hoverButton.setButtonText("Hover Info");
    recordButton.setButtonText("Record");

}


void GuiMainPanel::resized() 
{
    auto area = getLocalBounds();
    float qtr = area.getHeight() * 0.25f;
    float pad = 0;

    auto ratioArea = area.removeFromTop(qtr);
    pad = ratioArea.getHeight() * 0.05;

    REMOVE_FROM_ALL_SIDES(ratioArea, pad);
    ratioSlider.setBounds(ratioArea);

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    auto indexArea = area.removeFromTop(qtr);;

    REMOVE_FROM_ALL_SIDES(indexArea, pad);
    indexSlider.setBounds(indexArea);

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define BUTTON_SETUP(subArea, guiObj) {\
    subArea.removeFromLeft(pad);\
    subArea.removeFromRight(pad);\
    subArea.removeFromTop(subArea.getHeight()* 0.333f);\
    subArea.removeFromBottom(subArea.getHeight() * 0.5f);\
    guiObj.setBounds(subArea);\
}
    auto hoverArea = area.removeFromTop(qtr);
    BUTTON_SETUP(hoverArea, hoverButton);

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    auto recordArea = area.removeFromTop(qtr);
    BUTTON_SETUP(recordArea, recordButton)

#undef BUTTON_SETUP



}