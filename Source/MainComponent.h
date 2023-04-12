#pragma once

#include <JuceHeader.h>
#include "FractalBox.h"
#include "JuliaBox.h"

class MainComponent  : public juce::Component
{
public:
    MainComponent();
    ~MainComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    FractalBox mandelBox{};
    JuliaBox juliaBox{};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
