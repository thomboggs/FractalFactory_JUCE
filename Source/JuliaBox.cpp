/*
  ==============================================================================

    JuliaBox.cpp
    Created: 11 Apr 2023 10:57:43pm
    Author:  Thomas Boggs

  ==============================================================================
*/

#include <JuceHeader.h>
#include "JuliaBox.h"
#include <complex>

//==============================================================================
JuliaBox::JuliaBox() {}

JuliaBox::~JuliaBox() {}

void JuliaBox::paint (juce::Graphics& g)
{
    if (hasSizeChanged(getWidth(), getHeight())) {
        initImage();
        drawFractal(juce::Point<double>{0.0,0.0});
    }
    auto bounds = getLocalBounds();
    g.setColour(juce::Colours::black);
    g.drawRect(bounds);
    
    g.drawImageAt(m_image, 0, 0);
    
    g.drawLine(getWidth()/2, 0, getWidth()/2, getHeight());
    g.drawLine(0, getHeight()/2, getWidth(), getHeight()/2);
}

void JuliaBox::resized() {}

void JuliaBox::drawFractal(juce::Point<double> zPoint) {
    // Calculate Brot
    int nIterations = 0;
    
    juce::Colour value;

    for (double ptX = 0; ptX < getWidth(); ptX++)
    {
        for (double ptY = 0; ptY < getHeight(); ptY++)
        {
            nIterations = calcIterations(zPoint, juce::Point<double>{ptX, ptY});
            if (nIterations == 0) value = juce::Colour(0,0,0);
            else value = juce::Colour(255, 255-nIterations, 255-nIterations);
            m_image.setPixelAt(ptX, ptY, value);
        }
    }
}

int JuliaBox::calcIterations (juce::Point<double> zPoint, juce::Point<double> cPoint) {
    std::complex<double> complexZ(4*(cPoint.getX() / m_width) - 2,
                                          4*(cPoint.getY() / m_height) - 2);
    std::complex<double> complexPoint(zPoint.getX(), zPoint.getY());

    int nIterations = m_minIterations;
    while ((abs(complexZ) < 2 ) && ( nIterations <= m_maxIterations ))
    {
        complexZ = complexZ * complexZ + complexPoint;
        nIterations++;
    }

    if (nIterations < m_maxIterations) {
        return ( 255 * nIterations ) / m_maxIterations;
    } else {
        return 0;
    }
}

bool JuliaBox::hasSizeChanged(const int curWidth, const int curHeight) {
    if (curWidth != m_width) return true;
    if (curHeight != m_height) return true;
    return false;
}

void JuliaBox::initImage() {
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

juce::Point<double> JuliaBox::getMathCoord(const int x, const int y) {
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

juce::Point<int> JuliaBox::getDispCoord(const double x, const double y) {
    double xDisp = static_cast<double>(m_width) * (x + static_cast<double>(m_widthMath) / 2.f) / static_cast<double>(m_widthMath);
    double yDisp = static_cast<double>(m_height) * (y + static_cast<double>(m_heightMath) / 2.f) / static_cast<double>(m_heightMath);
    auto outPoint = juce::Point<int>(std::round(xDisp), std::round(yDisp));

    return outPoint;
}

void JuliaBox::mouseDown (const juce::MouseEvent& event) {
    auto point = juce::Point<int>(event.getMouseDownPosition());
    drawFractal(getMathCoord(point.getX(), point.getY()));
    m_mouseIsPressed = true;
    repaint();
}

void JuliaBox::mouseDrag (const juce::MouseEvent& event) {
    juce::Point<int> point = event.getPosition();
    auto pointD = getMathCoord(point.getX(), point.getY());
    drawFractal(pointD);
    repaint();
}

void JuliaBox::mouseUp (const juce::MouseEvent& event) {
    m_mouseIsPressed = false;
    repaint();
}
