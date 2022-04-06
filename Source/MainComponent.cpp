#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    juliaIterationSlider = std::make_unique<juce::Slider>(juce::String("Julia Iterations"));
    mandelIterationSlider = std::make_unique<juce::Slider>(juce::String("Mandelbrot Iterations"));
    CreateSliders();
    
    mandelIterationSlider->addListener(&mandelBox);
    juliaIterationSlider->addListener(&juliaBox);
    
    
    setSize (600, 400);
    
    addAndMakeVisible(&mandelBox);
    addAndMakeVisible(&juliaBox);
    addAndMakeVisible(mandelIterationSlider.get());
    addAndMakeVisible(juliaIterationSlider.get());
    
    mandelBox.SetFractalPartner(&juliaBox);
    juliaBox.SetFractalPartner(&mandelBox);
}

void MainComponent::CreateSliders() {
    juliaIterationSlider->setRange(1.0, 500.0, 1.0);
    mandelIterationSlider->setRange(1.0, 100.0, 1.0);
    
    juliaIterationSlider->setValue(50.0);
    mandelIterationSlider->setValue(50.0);
    
    juliaIterationSlider->setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    mandelIterationSlider->setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    
    juliaIterationSlider->setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 20);
    mandelIterationSlider->setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 20);
}

MainComponent::~MainComponent()
{
    mandelIterationSlider->removeListener(&mandelBox);
    juliaIterationSlider->removeListener(&juliaBox);
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    
}

void MainComponent::resized()
{
    auto bounds = getLocalBounds();
    
    auto controlsBounds = bounds.removeFromTop(100);
    auto mandelControlsBounds = controlsBounds.removeFromLeft(getWidth() / 2);
    mandelIterationSlider->setBounds(mandelControlsBounds);
    juliaIterationSlider->setBounds(controlsBounds);
    
    auto mandelBounds = bounds.removeFromLeft(bounds.getWidth() / 2);
    
    mandelBox.setBounds(mandelBounds.reduced(5, 5));
    juliaBox.setBounds(bounds.reduced(5, 5));
}
