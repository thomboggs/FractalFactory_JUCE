/*
  ==============================================================================

    FractalBox.h
    Created: 11 Apr 2023 10:31:23pm
    Author:  Thomas Boggs

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class JuliaBox;

class FractalBox : public juce::Component
{
public:
    FractalBox();
    ~FractalBox() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    void mouseDown (const juce::MouseEvent& event) override;
    void mouseDrag (const juce::MouseEvent& event) override;
    void mouseUp (const juce::MouseEvent& event) override;
    
    void setNewOrbit(const juce::Point<double> orbitStart);
    void setJuliaBox(JuliaBox& juliaBox);
    
private:
    void drawOrbit(juce::Graphics& g);
    int calcIterations(const int x, const int y);
    bool hasSizeChanged(const int curWidth, const int curHeight);
    void initImage();
    void drawFractal();
    juce::Point<double> getMathCoord(const int x, const int y);
    juce::Point<int> getDispCoord(const double x, const double y);
    std::vector<juce::Point<int>> calcOrbit(juce::Point<double> coordinate);
    
    std::shared_ptr<JuliaBox> m_juliaBox{nullptr};
    
    juce::Image m_image;
    
    uint m_minIterations {1};
    uint m_maxIterations {40};
    uint m_maxOrbitLen {25};
    uint m_width{0}, m_height{0};
    
    double m_imageRatio {1.f};
    double m_fracSize {4};
    double m_widthMath {0.f}, m_heightMath {0.f};
    
    std::vector<juce::Point<int>> m_orbitVec;
    
    bool m_mouseIsPressed {false};
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FractalBox)
};

