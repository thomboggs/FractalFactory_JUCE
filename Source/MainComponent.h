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
        // Init Image
//        addAndMakeVisible(m_imageComponent);
        
    }
    ~FractalBox() override {}

    void paint(juce::Graphics& g) override {
        if (HasSizeChanged(getWidth(), getHeight())) {
            InitImage();
        }
        auto bounds = getLocalBounds();
        g.setColour(juce::Colours::black);
        g.drawRect(bounds);
        
        drawFractal(m_image);
//        g.drawImage(&m_image, bounds.toFloat());
        g.drawImageAt(m_image, 0, 0);
        
        g.drawLine(getWidth()/2, 0, getWidth()/2, getHeight());
        g.drawLine(0, getHeight()/2, getWidth(), getHeight()/2);
    }

    void resized() override {}
    
    void drawFractal(juce::Image image) {
        // Calculate Brot
        int nIterations = 0;
        
        juce::Colour value;

        for (int ptX = 0; ptX < getWidth(); ptX++)
        {
            for (int ptY = 0; ptY < getHeight(); ptY++)
            {
                nIterations = calcIterations(static_cast<double>(ptX),
                                            static_cast<double>(ptY));
                if (nIterations == 0) value = juce::Colour(0,0,0);
                else value = juce::Colour(255, 255-nIterations, 255-nIterations);
                image.setPixelAt(ptX, ptY, value);
            }
        }
    }
    
    int calcIterations (double ptX, double ptY) {//{return 200;}
        
        float imageRatio = static_cast<float>(getWidth())/static_cast<float>(getHeight());
        
        std::complex<double> complexPoint;
        
        
//        complexPoint = std::complex<double>(4*(ptX * imageRatio / getWidth()) - 2,
//                                            4*(ptY / imageRatio / getHeight()) - 2);
        if (imageRatio >= 1) {
            complexPoint = std::complex<double>(4*imageRatio*(ptX / getWidth() - 0.5),
                                              4*(ptY / getHeight() - 0.5));
        } else {
            complexPoint = std::complex<double>(4*(ptX / getWidth() - 0.5),
                                                4/imageRatio*(ptY / getHeight() - 0.5));
        }
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
    
    bool HasSizeChanged(int curWidth, int curHeight) {
        if (curWidth != m_width) return true;
        if (curHeight != m_height) return true;
        return false;
    }
    
    void InitImage() {
        m_width = getWidth();
        m_height = getHeight();
        m_image = juce::Image(juce::Image::RGB, m_width, m_height, true);
    }
    
private:
    juce::Image m_image;
//    juce::ImageComponent m_imageComponent;
    uint m_minIterations {10};
    uint m_maxIterations {30};
    uint m_width{0}, m_height{0};
    
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
