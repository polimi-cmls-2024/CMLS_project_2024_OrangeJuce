/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"


//==============================================================================
/**
*/
class OrangeJuceAudioProcessorEditor  : public juce::AudioProcessorEditor, 
                                        private juce::Slider::Listener,
                                        private juce::ComboBox::Listener,
                                        private juce::Button::Listener,
                                        private juce::OSCReceiver,
                                        private juce::OSCReceiver::ListenerWithOSCAddress<juce::OSCReceiver::MessageLoopCallback>
{
public:
    OrangeJuceAudioProcessorEditor (OrangeJuceAudioProcessor&);
    ~OrangeJuceAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
   
    
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    OrangeJuceAudioProcessor& audioProcessor;
        
    //first set of knobs
    
    juce::Slider phase1;
    
    juce::Slider detune1;
    
    juce::Slider attack1;
    
    juce::Slider sustain1;
    
    juce::Slider release1;
    
    juce::Slider freq1;
    
    juce::Slider dep1;
    
    juce::TextButton activation_lfo1;
    
    
    // change the octave of the first set
    
    juce::TextButton octaveUpButton1;
    juce::TextButton octaveDownButton1;
    juce::Label octaveLabel1;
    int currentOctave1 = 0; // Default starting octave
    
    
    //second set of knobs
    
    
    juce::Slider phase2;
    
    //spazio per il pulsante per octave
    
    juce::Slider detune2;
    
    juce::Slider attack2;
    
    juce::Slider sustain2;
    
    juce::Slider release2;
    
    juce::Slider freq2;
    
    juce::Slider dep2;
    
    juce::TextButton activation_lfo2;
    
    
    // change the octave of the second set
    
    juce::TextButton octaveUpButton2;
    juce::TextButton octaveDownButton2;
    juce::Label octaveLabel2;
    int currentOctave2 = 0; // Default starting octave
    
    

    //third set of knobs
    
    
    juce::Slider phase3;
    
    //spazio per il pulsante per octave
    
    juce::Slider detune3;
    
    juce::Slider attack3;
    
    juce::Slider sustain3;
    
    juce::Slider release3;
    
    juce::Slider freq3;
    
    juce::Slider dep3;
    
    juce::TextButton activation_lfo3;
    
    
    // change the octave of the second set
    
    juce::TextButton octaveUpButton3;
    juce::TextButton octaveDownButton3;
    juce::Label octaveLabel3;
    int currentOctave3 = 0; // Default starting octave
    
    
    // low pass controls
    
    juce::Slider cutoff;
    
    juce::Slider resonance;
    
    // general controls
    
    juce::Slider pan;
    juce::Slider level;
    
    
    // sequencer controls
    
    juce::Slider time;
    juce::Slider speed;
    juce::Slider detune_seq;
    juce::TextButton record;
    juce::TextButton play_pause;
    
    

    
    juce::ComboBox choice_1;
    juce::ComboBox choice_2;
    juce::ComboBox choice_3;
    
    juce::TextButton button_1;
    juce::TextButton button_2;
    juce::TextButton button_3;
    
    void sliderValueChanged(juce::Slider * slider) override;
    void comboBoxChanged(juce::ComboBox * box) override;
    void updateWaveform(juce::Graphics&, juce::Path& waveform);
    void buttonClicked(juce::Button * button) override;
    void oscMessageReceived(const juce::OSCMessage& message) override;
    void changeOctave1(int delta);
    void changeOctave2(int delta);
    void changeOctave3(int delta);
    
    juce::Image background;
    
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OrangeJuceAudioProcessorEditor)
};



