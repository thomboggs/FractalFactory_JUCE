/*
  ==============================================================================

    JuliaBox.h
    Created: 11 Apr 2023 10:57:43pm
    Author:  Thomas Boggs

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>


class FractalBox;

class JuliaBox  : public juce::Component
{
public:
    JuliaBox();
    ~JuliaBox() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    void drawFractal(juce::Point<double> zPoint);
    int calcIterations(juce::Point<double> zPoint, juce::Point<double> cPoint);
    bool hasSizeChanged(const int curWidth, const int curHeight);
    void initImage();
    juce::Point<double> getMathCoord(const int x, const int y);
    juce::Point<int> getDispCoord(const double x, const double y);
    
    void mouseDown (const juce::MouseEvent& event) override;
    void mouseDrag (const juce::MouseEvent& event) override;
    void mouseUp (const juce::MouseEvent& event) override;

    void setNewFractal(const juce::Point<double> point);
    void setFractalBox(FractalBox& fractalBox);
    
private:
    std::shared_ptr<FractalBox> m_fractalBox{nullptr};
    juce::Image m_image;
    
    uint m_minIterations {1};
    uint m_maxIterations {100};
    uint m_width{0}, m_height{0};
    
    double m_imageRatio {1.f};
    double m_fracSize {4};
    double m_widthMath {0.f}, m_heightMath {0.f};
    
    bool m_mouseIsPressed {false};
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JuliaBox)
};
