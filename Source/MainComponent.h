#pragma once

#include <JuceHeader.h>
#include "MandelBox.h"
#include "JuliaBox.h"

class MainComponent  : public juce::Component
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    void CreateSliders();
    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    //==============================================================================
    // Your private member variables go here...
//    std::unique_ptr<FractalBox> mandelbox{nullptr};
//    std::unique_ptr<FractalBox> juliabox{nullptr};
    MandelBox mandelBox{};
    JuliaBox juliaBox{};
    std::unique_ptr<juce::Slider> juliaIterationSlider{nullptr};
    std::unique_ptr<juce::Slider> mandelIterationSlider{nullptr};
    
    
    
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
