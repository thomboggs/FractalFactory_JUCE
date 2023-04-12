#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    setSize (600, 400);
    
    addAndMakeVisible(&mandelBox);
    addAndMakeVisible(&juliaBox);
    
    mandelBox.setJuliaBox(juliaBox);
    juliaBox.setFractalBox(mandelBox);
}

MainComponent::~MainComponent()
{
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds();
    auto mandelBounds = bounds.removeFromLeft(bounds.getWidth() / 2);
    
    mandelBox.setBounds(mandelBounds.reduced(5, 5));
    juliaBox.setBounds(bounds.reduced(5, 5));
}

void MainComponent::resized()
{
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    
    
}
