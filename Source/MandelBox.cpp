/*
  ==============================================================================

    MandelBox.cpp
    Created: 26 Mar 2022 12:56:34pm
    Author:  Thomas Boggs

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MandelBox.h"
#include <complex>

//==============================================================================
MandelBox::MandelBox()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    m_orbitVec.reserve(static_cast<size_t>(m_maxOrbitLen));
}

MandelBox::~MandelBox() {
    m_juliaBox = nullptr;
}

void MandelBox::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds();
    g.setColour(juce::Colours::black);
    g.drawRect(bounds);

    if (HasSizeChanged(getWidth(), getHeight())) {
        InitImage();
        DrawFractal(m_image);
        m_orbitVec.clear();
    }
    g.drawImageAt(m_image, 0, 0);

    g.drawLine(getWidth()/2, 0, getWidth()/2, getHeight());
    g.drawLine(0, getHeight()/2, getWidth(), getHeight()/2);

    // Draw Orbit
    DrawOrbit(g);
}

void MandelBox::resized() {}

void MandelBox::DrawOrbit(juce::Graphics& g) {
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

void MandelBox::DrawFractal(juce::Image& image) {
    // Calculate Brot
    int nIterations = 0;
    
    juce::Colour value;

    for (int ptX = 0; ptX < getWidth(); ptX++)
    {
        for (int ptY = 0; ptY < getHeight(); ptY++)
        {
            nIterations = CalcIterations(ptX, ptY);
            if (nIterations == 0) value = juce::Colour(0,0,0);
            else value = juce::Colour(255, 255-nIterations, 255-nIterations);
            image.setPixelAt(ptX, ptY, value);
        }
    }
}

int MandelBox::CalcIterations (const int x, const int y) {
    auto mathPoint = GetMathCoord(x, y);
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

void MandelBox::sliderValueChanged(juce::Slider* slider) {
    auto sliderValue = slider->getValue();
    m_maxIterations = sliderValue;
    m_maxOrbitLen = sliderValue;
}

bool MandelBox::HasSizeChanged(const int curWidth, const int curHeight) {
    if (curWidth != m_width) return true;
    if (curHeight != m_height) return true;
    return false;
}

void MandelBox::InitImage() {
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

juce::Point<double> MandelBox::GetMathCoord(const int x, const int y) {
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

juce::Point<int> MandelBox::GetDispCoord(const double x, const double y) {
    double xDisp = static_cast<double>(m_width) * (x + static_cast<double>(m_widthMath) / 2.f) / static_cast<double>(m_widthMath);
    double yDisp = static_cast<double>(m_height) * (y + static_cast<double>(m_heightMath) / 2.f) / static_cast<double>(m_heightMath);
    auto outPoint = juce::Point<int>(std::round(xDisp), std::round(yDisp));

    return outPoint;
}

std::vector<juce::Point<int>> MandelBox::CalcOrbit(juce::Point<double> coordinate) {
    //  Do the same calculation as get color value
    std::complex<double> complexPoint(coordinate.getX(), coordinate.getY());
    std::complex<double> complexZ(0, 0);

    auto currentPoint = GetDispCoord(coordinate.getX(), coordinate.getY());
    m_orbitVec.clear();
    m_orbitVec = {currentPoint};

    int currentstep = 0;
    while ((abs(complexZ) < 2 ) && ( currentstep < m_maxOrbitLen ))
    {
        complexZ = complexZ * complexZ + complexPoint;
        currentPoint = GetDispCoord(complexZ.real(), complexZ.imag());

        m_orbitVec.push_back(currentPoint);
        currentstep++;
    }
    return m_orbitVec;
}

void MandelBox::mouseDown (const juce::MouseEvent& event) {
    auto point = juce::Point<int>(event.getMouseDownPosition());
    m_orbitVec = CalcOrbit(GetMathCoord(point.getX(), point.getY()));
    repaint();
    m_juliaBox->mouseDownRemote(event);
}

void MandelBox::mouseDrag (const juce::MouseEvent& event) {
    juce::Point<int> point = event.getPosition();
    auto pointD = GetMathCoord(point.getX(), point.getY());
    m_orbitVec = CalcOrbit(pointD);
    repaint();
    m_juliaBox->mouseDownRemote(event);
}

void MandelBox::mouseDownRemote(const juce::MouseEvent& event) {
    juce::Point<int> point = event.getPosition();
    auto pointD = GetMathCoord(point.getX(), point.getY());
    m_orbitVec = CalcOrbit(pointD);
    repaint();
}

void MandelBox::SetFractalPartner(JuliaBox *box) {
    m_juliaBox = box;
}
