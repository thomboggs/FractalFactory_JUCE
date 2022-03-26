#pragma once

#include <JuceHeader.h>
#include <complex>

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class FractalBox : public juce::Component {
public:

    FractalBox() {
        orbitVec.reserve(static_cast<size_t>(m_maxOrbitLen));
    }
    ~FractalBox() override {}

    void paint(juce::Graphics& g) override {
        if (HasSizeChanged(getWidth(), getHeight())) {
            InitImage();
            drawFractal(m_image);
        }
        auto bounds = getLocalBounds();
        g.setColour(juce::Colours::black);
        g.drawRect(bounds);
        
        
        g.drawImageAt(m_image, 0, 0);
        
        g.drawLine(getWidth()/2, 0, getWidth()/2, getHeight());
        g.drawLine(0, getHeight()/2, getWidth(), getHeight()/2);
        
        // Draw Orbit
        DrawOrbit(g);
    }

    void DrawOrbit(juce::Graphics& g) {
        auto orbitSize = static_cast<int>(orbitVec.size());
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
                auto line = juce::Line<int>(orbitVec[i], orbitVec[i+1]);
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
                g.drawEllipse(orbitVec[i].getX(), orbitVec[i].getY(), circleRadius, circleRadius, 1.f);
            }
        }
    }
    
    void resized() override {}
    
    void drawFractal(juce::Image& image) {
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
                image.setPixelAt(ptX, ptY, value);
            }
        }
    }
    
    int calcIterations (const int x, const int y) {
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
    
    bool HasSizeChanged(const int curWidth, const int curHeight) {
        if (curWidth != m_width) return true;
        if (curHeight != m_height) return true;
        return false;
    }
    
    void InitImage() {
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
    
    juce::Point<double> GetMathCoord(const int x, const int y) {
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
    
    juce::Point<int> GetDispCoord(const double x, const double y) {
        double xDisp = static_cast<double>(m_width) * (x + static_cast<double>(m_widthMath) / 2.f) / static_cast<double>(m_widthMath);
        double yDisp = static_cast<double>(m_height) * (y + static_cast<double>(m_heightMath) / 2.f) / static_cast<double>(m_heightMath);
        auto outPoint = juce::Point<int>(std::round(xDisp), std::round(yDisp));

        return outPoint;
    }
    
    std::vector<juce::Point<int>> CalcOrbit(juce::Point<double> coordinate) {
        //  Do the same calculation as get color value
        std::complex<double> complexPoint(coordinate.getX(), coordinate.getY());
        std::complex<double> complexZ(0, 0);

        auto currentPoint = GetDispCoord(coordinate.getX(), coordinate.getY());
        orbitVec.clear();
        orbitVec = {currentPoint};

        int currentstep = 0;
        while ((abs(complexZ) < 2 ) && ( currentstep < m_maxOrbitLen ))
        {
            complexZ = complexZ * complexZ + complexPoint;
            currentPoint = GetDispCoord(complexZ.real(), complexZ.imag());

            orbitVec.push_back(currentPoint);
            currentstep++;
        }
        return orbitVec;
    }
    
    void mouseDown (const juce::MouseEvent& event) override {
        auto point = juce::Point<int>(event.getMouseDownPosition());
        orbitVec = CalcOrbit(GetMathCoord(point.getX(), point.getY()));
        mouseIsPressed = true;
        repaint();
    }
    
    void mouseDrag (const juce::MouseEvent& event) override {
        juce::Point<int> point = event.getPosition();
        auto pointD = GetMathCoord(point.getX(), point.getY());
        orbitVec = CalcOrbit(pointD);
        repaint();
    }
    
    void mouseUp (const juce::MouseEvent& event) override {
        mouseIsPressed = false;
        repaint();
    }
    
private:
    juce::Image m_image;
    uint m_minIterations {1};
    uint m_maxIterations {30};
    uint m_maxOrbitLen {100};
    uint m_width{0}, m_height{0};
    double m_imageRatio {1.f};
    double m_fracSize {4};
    double m_widthMath {0.f}, m_heightMath {0.f};
    std::vector<juce::Point<int>> orbitVec;
    bool mouseIsPressed {false};
    
};



class MainComponent  : public juce::Component
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    //==============================================================================
    // Your private member variables go here...
//    std::unique_ptr<FractalBox> mandelbox{nullptr};
//    std::unique_ptr<FractalBox> juliabox{nullptr};
    FractalBox mandelBox{}, juliaBox{};
    
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
