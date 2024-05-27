/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <juce_gui_basics/juce_gui_basics.h>
#define SAMPLE_RATE (44100)
#ifndef M_PI
#define M_PI (3.14159265)
#endif
//==============================================================================
/**
*/
class OrangeJuceAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    OrangeJuceAudioProcessor();
    ~OrangeJuceAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    //NEW FUNCTION
    void setPhase(int selection, float val);
    void setDetune(int selection, float val);
    void setAttack(int selection, float val);
    void setRelease(int selection, float val);
    void setSustain(int selection, float val);
    void setFreq(int selection, float val);
    void setDepth(int selection, float val);
    void setWaveform(int selection, int val);
    void setResonance(float val);
    void setCutoff(float val);
    void setAmplitude(float val);
    void setLevel(float val);
    void setDetuneSequencer(float val);
    void sendState();
    void setPan(float val);
    void beginRecord();
    void stopRecord();
    void setTime(float val);
    void setSpeed(float val);
    void play();
    void stop();
    bool button_1_state = true;
    bool button_2_state = false;
    bool button_3_state = false;
    float amplitude;
    float note_playing;
    int capacity = 0;
    void setCurrentOctave1(int octave);
    void setCurrentOctave2(int octave);
    void setCurrentOctave3(int octave);
    void sendLFOButton(int number, int value);
    bool play_status = false;
    bool sequencer_playing = false;

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OrangeJuceAudioProcessor)
    juce::OSCSender sender;
    juce::OSCMessage message;
    float freq;
    float note[50];
    float phase;
    bool recording = false;
    
    
    int currentOctave1 = 0; // Default starting octave
    int currentOctave2 = 0; // Default starting octave
    int currentOctave3 = 0; // Default starting octave
    
    

};


