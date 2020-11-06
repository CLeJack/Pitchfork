/*
  ==============================================================================

    GUISpectrum.cpp
    Created: 1 Oct 2020 2:51:19pm
    Author:  cjack

  ==============================================================================
*/

#include "GUISpectrum.h"


GuiSpectrum::GuiSpectrum(int barCount, int octaveSize) : 
    background1(barCount, octaveSize),
    notes1(barCount, octaveSize),
    freqs(barCount),

    background2(32, 1),
    harmonics(32)
{
    addAndMakeVisible(background1);
    addAndMakeVisible(notes1);
    addAndMakeVisible(freqs);

    addAndMakeVisible(background2);
    addAndMakeVisible(harmonics);

}

void GuiSpectrum::resized() 
{
    auto area = getLocalBounds();
    
    float X = 0;
    float Y = 0;
    float H = area.getHeight();
    float W = area.getWidth();

    float pad = area.getHeight() * 0.05;

    REMOVE_FROM_ALL_SIDES(area, pad);

    X = area.getX();
    Y = area.getY();
    H = area.getHeight();
    W = area.getWidth();

    auto displayArea1 = area.removeFromTop(H * 0.5);

    displayArea1.removeFromBottom(pad * 0.5);

    auto displayArea2 = area;
    displayArea2.removeFromTop(pad * 0.5);

    //set all bounds~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    background1.setBounds (displayArea1);
    notes1.setBounds      (displayArea1.removeFromBottom(displayArea1.getHeight() * 0.05f));
    freqs.setBounds     (displayArea1);

    background2.setBounds (displayArea2);
    harmonics.setBounds     (displayArea2);
    

    

}

SpectrumBackground::SpectrumBackground(float barCount, float octaveSize) : barCount(barCount), octaveSize(octaveSize) 
{
}

void SpectrumBackground::paint(juce::Graphics& g) 
{

    int segments = barCount / octaveSize;
    float W = getWidth();
    float w = W / segments;
    float X = 0;
    float Y = 0;
    float H = getHeight();

    for (int i = 0; i < segments; i++) 
    {
        g.setColour(i % 2 == 0 ? PAPER_DARK : PAPER);
        g.fillRect(X, Y, w, H);
        
        X += w;
    }
    
}




SpectrumBars::SpectrumBars(float barCount) : weights(barCount) 
{
}

void SpectrumBars::paint(juce::Graphics& g) 
{
    float Y = 0;
    float X = 0;
    float W = getWidth() / (float)weights.size();
    float right = X+W;
    float H = getHeight();

    int maxInd = maxArg(weights);

    g.setColour(BOLD_BLACK_INK);

    for (int i = 0; i < weights.size(); i++) 
    {
        Y = H * weights[i];

        if (i == maxInd) 
        {
            g.setColour(BOLD_GREEN_INK);
            g.fillRect(X, H - Y, W, Y);
            g.setColour(BOLD_BLACK_INK);
        }


        g.drawRect(X,H - Y, W, Y, 2.0f);

        X += W;
    }

    g.setColour(BOLD_GREEN_INK);

    Y = H * weights[maxInd];
    g.drawLine(0, H - Y, getWidth(), H - Y, 2.0f);

    g.setColour(BOLD_BLACK_INK);
    g.drawRect(getLocalBounds(), 2.0f);
}



SpectrumNotes::SpectrumNotes(float barCount, float octaveSize) : keys(barCount)
{
    for(int i = 0; i < keys.size(); i++)
    {
        keys[i] = keyReference[i % (int)octaveSize];
    }
}



void SpectrumNotes::paint(juce::Graphics& g) 
{
    float X = 0;
    float Y = 0;
    float W = getWidth() / (float)keys.size();
    float H = getHeight();
    for (int i = 0; i < keys.size(); i++) 
    {
        g.setColour(keys[i] == 0 ? PAPER_LIGHT : FADE_BLACK_INK);
        g.fillRect(X, Y, W, H);
        X += W;
    }
}
