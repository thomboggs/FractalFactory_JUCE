/*
  ==============================================================================

    FractalBox.cpp
    Created: 11 Apr 2023 10:31:23pm
    Author:  Thomas Boggs

  ==============================================================================
*/

#include <JuceHeader.h>
#include "FractalBox.h"
#include "JuliaBox.h"
#include <complex>

//==============================================================================
FractalBox::FractalBox() {
    m_orbitVec.reserve(static_cast<size_t>(m_maxOrbitLen));
}

FractalBox::~FractalBox() {}

void FractalBox::paint (juce::Graphics& g)
{
    if (hasSizeChanged(getWidth(), getHeight())) {
        initImage();
        drawFractal();
    }
    auto bounds = getLocalBounds();
    g.setColour(juce::Colours::black);
    g.drawRect(bounds);
    
    
    g.drawImageAt(m_image, 0, 0);
    
    g.drawLine(getWidth()/2, 0, getWidth()/2, getHeight());
    g.drawLine(0, getHeight()/2, getWidth(), getHeight()/2);
    
    drawOrbit(g);
}

void FractalBox::resized() {}

void FractalBox::drawOrbit(juce::Graphics& g) {
    auto orbitSize = static_cast<int>(m_orbitVec.size());
    if ( orbitSize > 0 )
    {
        int circleRadius = 4;
        juce::Colour baseLineColor = juce::Colours::cyan;
        juce::Colour baseCircleColor = juce::Colours::yellow;

        for (uint i = 0; i < (orbitSize-1); i++)
        {
            float newAlpha = (255.f-static_cast<float>(i)*2.f)/255.f;
            auto lineColor = baseLineColor.withAlpha(newAlpha);
            g.setColour(lineColor);
            auto line = juce::Line<int>(m_orbitVec[i], m_orbitVec[i+1]);
            g.drawLine(line.toFloat(), 2.f);
        }

        for (int i = 0; i < (orbitSize); i++)
        {
            float newAlpha = (255.f-static_cast<float>(i)*2.f)/255.f;
            auto circleColor = baseCircleColor.withAlpha(newAlpha);
            g.setColour(circleColor);
            if (i == 0)
            {
                g.setColour(juce::Colours::magenta);
            }
            g.drawEllipse(m_orbitVec[i].getX(), m_orbitVec[i].getY(), circleRadius, circleRadius, 1.f);
        }
    }
}

void FractalBox::drawFractal() {
    // Calculate Brot
    int nIterations = 0;
    
    juce::Colour value;

    for (int ptX = 0; ptX < getWidth(); ptX++)
    {
        for (int ptY = 0; ptY < getHeight(); ptY++)
        {
            nIterations = calcIterations(ptX, ptY);
            if (nIterations == 0) value = juce::Colour(0,0,0);
            else value = juce::Colour(255, 255-nIterations, 255-nIterations);
            m_image.setPixelAt(ptX, ptY, value);
        }
    }
}

int FractalBox::calcIterations (const int x, const int y) {
    auto mathPoint = getMathCoord(x, y);
    auto complexPoint = std::complex<double>(mathPoint.getX(), mathPoint.getY());
    
    std::complex<double> complexZ(0, 0);

    int nIterations = m_minIterations;

    while ((abs(complexZ) < 2 ) && ( nIterations <= m_maxIterations ))
    {
        complexZ = complexZ * complexZ + complexPoint;
        nIterations++;
    }

    if (nIterations < m_maxIterations) {
        return static_cast<int>(( 255 * nIterations ) / m_maxIterations );
    } else {
        return 0;
    }
}

bool FractalBox::hasSizeChanged(const int curWidth, const int curHeight) {
    if (curWidth != m_width) return true;
    if (curHeight != m_height) return true;
    return false;
}

void FractalBox::initImage() {
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

juce::Point<double> FractalBox::getMathCoord(const int x, const int y) {
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

juce::Point<int> FractalBox::getDispCoord(const double x, const double y) {
    double xDisp = static_cast<double>(m_width) * (x + static_cast<double>(m_widthMath) / 2.f) / static_cast<double>(m_widthMath);
    double yDisp = static_cast<double>(m_height) * (y + static_cast<double>(m_heightMath) / 2.f) / static_cast<double>(m_heightMath);
    auto outPoint = juce::Point<int>(std::round(xDisp), std::round(yDisp));

    return outPoint;
}

std::vector<juce::Point<int>> FractalBox::calcOrbit(juce::Point<double> coordinate) {
    //  Do the same calculation as get color value
    std::complex<double> complexPoint(coordinate.getX(), coordinate.getY());
    std::complex<double> complexZ(0, 0);

    auto currentPoint = getDispCoord(coordinate.getX(), coordinate.getY());
    m_orbitVec.clear();
    m_orbitVec = {currentPoint};

    int currentstep = 0;
    while ((abs(complexZ) < 2 ) && ( currentstep < m_maxOrbitLen ))
    {
        complexZ = complexZ * complexZ + complexPoint;
        currentPoint = getDispCoord(complexZ.real(), complexZ.imag());

        m_orbitVec.push_back(currentPoint);
        currentstep++;
    }
    return m_orbitVec;
}

void FractalBox::mouseDown (const juce::MouseEvent& event) {
    auto point = juce::Point<int>(event.getMouseDownPosition());
    m_orbitVec = calcOrbit(getMathCoord(point.getX(), point.getY()));
    m_juliaBox->setNewFractal(getMathCoord(point.getX(), point.getY()));
    m_mouseIsPressed = true;
    repaint();
}

void FractalBox::mouseDrag (const juce::MouseEvent& event) {
    juce::Point<int> point = event.getPosition();
    auto pointD = getMathCoord(point.getX(), point.getY());
    m_orbitVec = calcOrbit(pointD);
    m_juliaBox->setNewFractal(pointD);
    repaint();
}

void FractalBox::mouseUp (const juce::MouseEvent& event) {
    m_mouseIsPressed = false;
    repaint();
}

void FractalBox::setNewOrbit(const juce::Point<double> orbitStart) {
    m_orbitVec = calcOrbit(orbitStart);
    repaint();
}

void FractalBox::setJuliaBox(JuliaBox &juliaBox) {
    m_juliaBox = std::shared_ptr<JuliaBox>(&juliaBox);
}
