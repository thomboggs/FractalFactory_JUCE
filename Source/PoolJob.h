/*
  ==============================================================================

    PoolJob.h
    Created: 5 Apr 2022 9:42:35pm
    Author:  Thomas Boggs

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "JuliaBox.h"

class PoolJob;
class JuliaBox;

struct PoolOptions {
    JuliaBox* juliaBox;
    uint minIterations;
    uint maxIterations;
    uint width;
    uint height;
    double imageRatio;
    double fracSize;
    juce::Point<double> currMathPoint;
};

class PoolJob : public juce::ThreadPoolJob
{
public:
    PoolJob(juce::String name, PoolOptions options);
    PoolJob(const PoolJob& job);
    
    JobStatus runJob() override;
    
    void DrawFractal(juce::Image& image, juce::Point<double> zPoint);
    int CalcIterations (juce::Point<double> zPoint, juce::Point<int> pixelPoint);
    
    juce::Point<double> GetMathCoord(const int x, const int y);
    
    juce::Image getImage() {return m_poolImage;}
    
private:
    JuliaBox* m_juliaBox{nullptr};
    juce::Image m_poolImage;
    uint m_minIterations {1};
    uint m_maxIterations {65};
    uint m_width{0};
    uint m_height{0};
    double m_imageRatio {1.f};
    double m_fracSize {4};
    double m_widthMath {0.f};
    double m_heightMath {0.f};
    juce::Point<double> m_currMathPoint {0,0};
};
