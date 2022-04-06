/*
  ==============================================================================

    JuliaBox.cpp
    Created: 26 Mar 2022 12:57:01pm
    Author:  Thomas Boggs

  ==============================================================================
*/

#include <JuceHeader.h>
#include "JuliaBox.h"
#include <complex>

//==============================================================================
JuliaBox::JuliaBox()
{
}

JuliaBox::~JuliaBox()
{
    m_mandelBox = nullptr;
    m_pool.removeAllJobs (true, 2000);
}

void JuliaBox::paint (juce::Graphics& g) {
    auto bounds = getLocalBounds();
    g.setColour(juce::Colours::black);
    g.drawRect(bounds);

    if (HasSizeChanged(getWidth(), getHeight())) {
        InitImage();
        m_shouldDrawFractal = true;
    }
    if (m_shouldDrawFractal) {

        
        m_pool.addJob(FindJob(), true);
        m_shouldDrawFractal = false;
//        DrawFractal(m_image, m_currMathPoint);
    }
    
    g.drawImageAt(m_image, 0, 0);
    g.drawLine(getWidth()/2, 0, getWidth()/2, getHeight());
    g.drawLine(0, getHeight()/2, getWidth(), getHeight()/2);
}

PoolJob* JuliaBox::FindJob() {
    if (!job1) {
        job1 = std::make_unique<PoolJob>(CreatePoolJob(m_currMathPoint));
        return job1.get();
    } else if (!job2) {
        job2 = std::make_unique<PoolJob>(CreatePoolJob(m_currMathPoint));
        return job2.get();
    } else if (!job3) {
        job3 = std::make_unique<PoolJob>(CreatePoolJob(m_currMathPoint));
        return job3.get();
    }
    
    if (!job1->isRunning()) {
        job1.reset();
        job1 = std::make_unique<PoolJob>(CreatePoolJob(m_currMathPoint));
        return job1.get();
    } else if (!job2->isRunning()) {
        job2.reset();
        job2 = std::make_unique<PoolJob>(CreatePoolJob(m_currMathPoint));
        return job2.get();
    } else if (!job3->isRunning()) {
        job3.reset();
        job3 = std::make_unique<PoolJob>(CreatePoolJob(m_currMathPoint));
        return job3.get();
    }
}

void JuliaBox::resized() {}

void JuliaBox::sliderValueChanged(juce::Slider* slider) {
    m_maxIterations = slider->getValue();
}


bool JuliaBox::HasSizeChanged(const int curWidth, const int curHeight) {
    if (curWidth != m_width) return true;
    if (curHeight != m_height) return true;
    return false;
}

void JuliaBox::InitImage() {
    m_width = getWidth();
    m_height = getHeight();
    m_imageRatio = static_cast<double>(getWidth())/static_cast<double>(getHeight());
    if (m_imageRatio >= 1.f) {
        m_widthMath = 4 * m_imageRatio;
        m_heightMath = 4;
    } else {
        m_widthMath = 4;
        m_heightMath = 4 / m_imageRatio;
    }
    m_image = juce::Image(juce::Image::RGB, m_width, m_height, true);
}

juce::Point<double> JuliaBox::GetMathCoord(const int x, const int y) {
    juce::Point<double> pointOut;
    if (m_imageRatio >= 1) {
        pointOut = juce::Point<double>(
               4*m_imageRatio*(static_cast<double>(x) / getWidth() - 0.5),  4*(static_cast<double>(y) / getHeight() - 0.5));
    } else {
        pointOut = juce::Point<double>(
               4*(static_cast<double>(x) / getWidth() - 0.5),  4/m_imageRatio*(static_cast<double>(y) / getHeight() - 0.5));
    }
    return pointOut;
}

juce::Point<int> JuliaBox::GetDispCoord(const double x, const double y) {
    double xDisp = static_cast<double>(m_width) * (x + static_cast<double>(m_widthMath) / 2.f) / static_cast<double>(m_widthMath);
    double yDisp = static_cast<double>(m_height) * (y + static_cast<double>(m_heightMath) / 2.f) / static_cast<double>(m_heightMath);
    auto outPoint = juce::Point<int>(std::round(xDisp), std::round(yDisp));

    return outPoint;
}

void JuliaBox::DrawFractal(juce::Image& image, juce::Point<double> zPoint) {
    juce::Colour value;
    int nIterations = 0;
    using IntPoint = juce::Point<int>;
    
    for (int ptX = 0; ptX < getWidth(); ptX++)
    {
        for (int ptY = 0; ptY < getHeight(); ptY++)
        {
            nIterations = CalcIterations(zPoint, IntPoint(ptX, ptY));
            if (nIterations == 0) value = juce::Colour(0,0,0);
            else value = juce::Colour(255, 255-nIterations, 255-nIterations);
            image.setPixelAt(ptX, ptY, value);
        }
    }
}

int JuliaBox::CalcIterations(juce::Point<double> zPoint, juce::Point<int> pixelPoint) {
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

void JuliaBox::mouseDown (const juce::MouseEvent& event) {
    auto point = juce::Point<int>(event.getMouseDownPosition());
    m_currMathPoint = GetMathCoord(point.getX(), point.getY());
    m_shouldDrawFractal = true;
    repaint();
//    auto poolJob = std::make_unique<PoolJob>(CreatePoolJob(m_currMathPoint));
//    m_pool.addJob(poolJob.get(), true);
    
    m_mandelBox->mouseDownRemote(event);
}

void JuliaBox::mouseDrag (const juce::MouseEvent& event) {
    auto point = event.getPosition();
    m_currMathPoint = GetMathCoord(point.getX(), point.getY());
    m_shouldDrawFractal = true;
    repaint();
//    auto poolJob = std::make_unique<PoolJob>(CreatePoolJob(m_currMathPoint));
//    m_pool.addJob(poolJob.get(), true);
    m_mandelBox->mouseDownRemote(event);
}
void JuliaBox::mouseDownRemote(const juce::MouseEvent& event) {
    auto point = event.getPosition();
    m_currMathPoint = GetMathCoord(point.getX(), point.getY());
    m_shouldDrawFractal = true;
    repaint();
//    auto poolJob = CreatePoolJob(m_currMathPoint);
//    m_pool.addJob(&poolJob, false);
}

void JuliaBox::SetFractalPartner(MandelBox* box) {
    m_mandelBox = box;
}

PoolJob JuliaBox::CreatePoolJob(juce::Point<double> zPoint) {
    PoolOptions options;
    options.juliaBox = this;
    options.minIterations = m_minIterations;
    options.maxIterations = m_maxIterations;
    options.fracSize = m_fracSize;
    options.width = m_width;
    options.height = m_height;
    options.imageRatio = m_imageRatio;
    options.currMathPoint = zPoint;
    
    PoolJob poolJob("Demo Job", options);
    return poolJob;
}

void JuliaBox::SetImage(PoolJob* job, juce::Image&& image) {
    juce::ScopedLock scopedLock(objectLock);
    m_image = std::move(image);
    m_pool.removeJob(job, true, 1000);
    repaint();
    
}
