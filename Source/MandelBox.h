/*
  ==============================================================================

    MandelBox.h
    Created: 26 Mar 2022 12:56:34pm
    Author:  Thomas Boggs

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "JuliaBox.h"
class JuliaBox;

//==============================================================================
class MandelBox  : public juce::Component, public juce::Slider::Listener
{
public:
    MandelBox();
    ~MandelBox() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    void sliderValueChanged(juce::Slider* slider) override;
    void sliderDragStarted(juce::Slider* slider) override {}
    void sliderDragEnded(juce::Slider* slider) override {}

    
    void DrawFractal(juce::Image& image);
    int CalcIterations (const int x, const int y);
    
    bool HasSizeChanged(const int curWidth, const int curHeight);
    void InitImage();
    
    juce::Point<double> GetMathCoord(const int x, const int y);
    juce::Point<int> GetDispCoord(const double x, const double y);
    
    void DrawOrbit(juce::Graphics& g);
    std::vector<juce::Point<int>> CalcOrbit(juce::Point<double> coordinate);
    
    void mouseDown (const juce::MouseEvent& event) override;
    void mouseDrag (const juce::MouseEvent& event) override;
    
    void mouseDownRemote(const juce::MouseEvent& event);

    void SetFractalPartner(JuliaBox* box);
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MandelBox)
    JuliaBox* m_juliaBox{nullptr};
    juce::Image m_image;
    uint m_minIterations {1};
    uint m_maxIterations {30};
    uint m_maxOrbitLen {100};
    uint m_width{0};
    uint m_height{0};
    double m_imageRatio {1.f};
    double m_fracSize {4};
    double m_widthMath {0.f};
    double m_heightMath {0.f};
    std::vector<juce::Point<int>> m_orbitVec;
};
