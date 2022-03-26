#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
//class FractalBox : public juce::Component {
//public:
//
//    FractalBox() {
//        setSize(100, 100);
//    }
//    ~FractalBox() override {};
//
//    void paint(juce::Graphics& g) override {
//        g.setColour(juce::Colours::black);
//        g.drawRect(juce::getLocalBounds());
//    }
//
//    void resized() override {};
//private:
//
//};



class MainComponent  : public juce::Component
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    //==============================================================================
    // Your private member variables go here...
//    std::unique_ptr<FractalBox> mandelbox{nullptr};
//    std::unique_ptr<FractalBox> juliabox{nullptr};
//    FractalBox mandelBox{}, juliaBox{};
    
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
