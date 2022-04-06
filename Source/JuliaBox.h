/*
  ==============================================================================

    JuliaBox.h
    Created: 26 Mar 2022 12:57:01pm
    Author:  Thomas Boggs

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "MandelBox.h"
#include "PoolJob.h"

//==============================================================================
class MandelBox;
class PoolJob;

class JuliaBox  : public juce::Component, public juce::Slider::Listener
{
public:
    juce::CriticalSection objectLock;
    
    JuliaBox();
    ~JuliaBox() override;

    void paint (juce::Graphics& g) override;
    void resized() override;
    
    void sliderValueChanged(juce::Slider* slider) override;
    void sliderDragStarted(juce::Slider* slider) override {}
    void sliderDragEnded(juce::Slider* slider) override {}

    bool HasSizeChanged(const int curWidth, const int curHeight);
    void InitImage();
    
    juce::Point<double> GetMathCoord(const int x, const int y);
    juce::Point<int> GetDispCoord(const double x, const double y);
    
    void DrawFractal(juce::Image& image, juce::Point<double> zPoint);
    int CalcIterations (juce::Point<double> zPoint, juce::Point<int> pixelPoint);
    
    void mouseDown (const juce::MouseEvent& event) override;
    void mouseDrag (const juce::MouseEvent& event) override;
    
    void mouseDownRemote(const juce::MouseEvent& event);
    
    void SetFractalPartner(MandelBox* box);
    
    PoolJob CreatePoolJob(juce::Point<double> zPoint);
    
    void SetImage(PoolJob* job, juce::Image&&);
    
    PoolJob* FindJob();

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JuliaBox)
    MandelBox* m_mandelBox{nullptr};
    juce::Image m_image;
    uint m_minIterations {1};
    uint m_maxIterations {65};
    uint m_width{0};
    uint m_height{0};
    double m_imageRatio {1.f};
    double m_fracSize {4};
    double m_widthMath {0.f};
    double m_heightMath {0.f};
    juce::Point<double> m_currMathPoint {0,0};
    bool m_shouldDrawFractal{false};
    
    juce::ThreadPool m_pool{3};
    std::unique_ptr<PoolJob> job1{nullptr}, job2{nullptr}, job3{nullptr};
};
