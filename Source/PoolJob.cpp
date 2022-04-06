/*
  ==============================================================================

    PoolJob.cpp
    Created: 5 Apr 2022 9:42:35pm
    Author:  Thomas Boggs

  ==============================================================================
*/

#include "PoolJob.h"
#include <complex.h>

PoolJob::PoolJob (juce::String name, PoolOptions options) :
  juce::ThreadPoolJob(name)
, m_juliaBox(options.juliaBox)
, m_minIterations(options.minIterations)
, m_maxIterations(options.maxIterations)
, m_width(options.width)
, m_height(options.height)
, m_imageRatio(options.imageRatio)
, m_fracSize(options.fracSize)
, m_currMathPoint(options.currMathPoint)
{
}

PoolJob::PoolJob(const PoolJob& job) :
juce::ThreadPoolJob(job.getJobName())
, m_juliaBox(job.m_juliaBox)
, m_minIterations(job.m_minIterations)
, m_maxIterations(job.m_maxIterations)
, m_width(job.m_width)
, m_height(job.m_height)
, m_imageRatio(job.m_imageRatio)
, m_fracSize(job.m_fracSize)
, m_currMathPoint(job.m_currMathPoint)
{
    
}

PoolJob::JobStatus PoolJob::runJob() {
    juce::Colour value;
    int nIterations = 0;
    using IntPoint = juce::Point<int>;
    
    for (int ptX = 0; ptX < m_width; ptX++)
    {
        if (shouldExit()) {
            return PoolJob::JobStatus::jobNeedsRunningAgain;
        }
        for (int ptY = 0; ptY < m_height; ptY++)
        {
            nIterations = CalcIterations(m_currMathPoint, IntPoint(ptX, ptY));
            if (nIterations == 0) value = juce::Colour(0,0,0);
            else value = juce::Colour(255, 255-nIterations, 255-nIterations);
            m_poolImage.setPixelAt(ptX, ptY, value);
        }
    }
    
    m_juliaBox->SetImage(this, getImage());
    
    return PoolJob::JobStatus::jobHasFinished;
}

int PoolJob::CalcIterations(juce::Point<double> zPoint, juce::Point<int> pixelPoint) {
    auto mathPoint = GetMathCoord(pixelPoint.getX(), pixelPoint.getY());
    auto complexZ = std::complex<double>(mathPoint.getX(), mathPoint.getY());
    
    auto complexPoint = std::complex<double>(zPoint.getX(), zPoint.getY());
    
    int nIterations = 1;
    
    while ((abs(complexZ) < 2 ) && ( nIterations <= m_maxIterations )) {
        complexZ = complexZ * complexZ + complexPoint;
        nIterations++;
    }

    if (nIterations < m_maxIterations)
        return static_cast<int>(( 255 * nIterations ) / m_maxIterations );
    else
        return 0;
}

juce::Point<double> PoolJob::GetMathCoord(const int x, const int y) {
    juce::Point<double> pointOut;
    if (m_imageRatio >= 1) {
        pointOut = juce::Point<double>(
               4*m_imageRatio*(static_cast<double>(x) / m_width - 0.5),
               4*(static_cast<double>(y) / m_height - 0.5));
    } else {
        pointOut = juce::Point<double>(
               4*(static_cast<double>(x) / m_width - 0.5),
               4/m_imageRatio*(static_cast<double>(y) / m_height - 0.5));
    }
    return pointOut;
}

