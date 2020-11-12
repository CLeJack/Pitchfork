#pragma once
#include <JuceHeader.h>
#include "GUIGlobals.h"
#include "Stats.h"
#include "CircularBuffer.h"


inline void initText(juce::Label& component, const char* text, int justification)
{
    component.setText(text, juce::NotificationType::dontSendNotification);
    component.setJustificationType(juce::Justification(justification));
}

inline void resizeH(juce::Rectangle<int>& area, int pixels) 
{
    area.removeFromTop(pixels);
    area.removeFromBottom(pixels);
}

inline void resizeW(juce::Rectangle<int>& area, float pixels) 
{
    area.removeFromLeft(pixels);
    area.removeFromRight(pixels);
}

inline void resizeHW(juce::Rectangle<int>& area, float pixh, float pixw)
{
    resizeH(area, pixh);
    resizeW(area, pixw);
}



//5. GuiTabs ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class GuiTabs : public juce::TabbedComponent
{
public:

    GuiTabs() : juce::TabbedComponent (juce::TabbedButtonBar::TabsAtTop){};
    void currentTabChanged(int newCurrentTabIndex, const juce::String& newCurrentTabName) override;
    GUIState guiState = GUIState::live;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GuiTabs);
};

class PaperLookAndFeel : public juce::LookAndFeel_V4 
{
public:
    PaperLookAndFeel();
};

