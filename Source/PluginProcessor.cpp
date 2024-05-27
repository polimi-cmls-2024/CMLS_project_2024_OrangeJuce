/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
OrangeJuceAudioProcessor::OrangeJuceAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
                    message("/playSequencer")
#endif
{
}

OrangeJuceAudioProcessor::~OrangeJuceAudioProcessor()
{
}

//==============================================================================
const juce::String OrangeJuceAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool OrangeJuceAudioProcessor::acceptsMidi() const
{
  #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool OrangeJuceAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool OrangeJuceAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double OrangeJuceAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int OrangeJuceAudioProcessor::getNumPrograms()
{
    return 1;
}

int OrangeJuceAudioProcessor::getCurrentProgram()
{
    return 0;
}

void OrangeJuceAudioProcessor::setCurrentProgram (int index){}

const juce::String OrangeJuceAudioProcessor::getProgramName (int index)
{
    return {};
}

void OrangeJuceAudioProcessor::changeProgramName (int index, const juce::String& newName){}

//==============================================================================
void OrangeJuceAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock) {}

void OrangeJuceAudioProcessor::releaseResources(){}

#ifndef JucePlugin_PreferredChannelConfigurations
bool OrangeJuceAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void OrangeJuceAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
  
    juce::MidiMessage m;
    int time;
    
    for (juce::MidiBuffer::Iterator i (midiMessages); i.getNextEvent (m, time);){
        if (!sequencer_playing){
            if (m.isNoteOn()) {
                freq = m.getMidiNoteInHertz(m.getNoteNumber());
                note_playing = freq;
                if (sender.connect("127.0.0.1", 57120))
                    sender.send("/setFrequency", (float) freq);
                if (sender.connect("127.0.0.1", 9000))
                    sender.send("/setFrequency", (float) freq);
                if (recording) {
                    note[capacity] = freq;
                    capacity ++;
                }
            } else if (m.isNoteOff()){
                freq = m.getMidiNoteInHertz(m.getNoteNumber());
                if (freq == note_playing){
                    if (sender.connect("127.0.0.1", 57120))
                        sender.send("/stopNote", (float) freq);
                    if (sender.connect("127.0.0.1", 9000))
                        sender.send("/stopNote", (float) freq);
                }
            }
        }
    }
}

//==============================================================================
bool OrangeJuceAudioProcessor::hasEditor() const{
    return true;
}

juce::AudioProcessorEditor* OrangeJuceAudioProcessor::createEditor(){
    return new OrangeJuceAudioProcessorEditor (*this);
}

//==============================================================================
void OrangeJuceAudioProcessor::getStateInformation (juce::MemoryBlock& destData){}

void OrangeJuceAudioProcessor::setStateInformation (const void* data, int sizeInBytes){}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new OrangeJuceAudioProcessor();
}

//==============================================================================

void OrangeJuceAudioProcessor::setPhase(int selection, float val){
    if (sender.connect("127.0.0.1", 57120))
        sender.send("/setPhase", selection, (float) val);
    if (sender.connect("127.0.0.1", 9000))
        sender.send("/setPhase", selection + 1, (float) val);
}

void OrangeJuceAudioProcessor::setAttack(int selection, float val){
    if (sender.connect("127.0.0.1", 57120))
        sender.send("/setAttack", selection, (float) val);
}

void OrangeJuceAudioProcessor::setRelease(int selection, float val){
    if (sender.connect("127.0.0.1", 57120))
        sender.send("/setRelease", selection, (float) val);
}

void OrangeJuceAudioProcessor::setSustain(int selection, float val){
    if (sender.connect("127.0.0.1", 57120))
        sender.send("/setSustain", selection, (float) val);
}

void OrangeJuceAudioProcessor::setDetune(int selection, float val){
    if (sender.connect("127.0.0.1", 57120))
        sender.send("/setDetune", selection, (float) val);
}

void OrangeJuceAudioProcessor::setFreq(int selection, float val){
    if (sender.connect("127.0.0.1", 57120))
        sender.send("/setFreq_lfo", selection, (float) val);
}

void OrangeJuceAudioProcessor::setDepth(int selection, float val){
    if (sender.connect("127.0.0.1", 57120))
        sender.send("/setDepth", selection, (float) val);
}

void OrangeJuceAudioProcessor::setCurrentOctave1(int octave)
{
    if (sender.connect("127.0.0.1", 57120))
        sender.send("/setOctave", 0, octave);
}

void OrangeJuceAudioProcessor::setCurrentOctave2(int octave)
{
    if (sender.connect("127.0.0.1", 57120))
        sender.send("/setOctave", 1, octave);
}

void OrangeJuceAudioProcessor::setCurrentOctave3(int octave)
{
    if (sender.connect("127.0.0.1", 57120))
        sender.send("/setOctave", 2, octave);
}

void OrangeJuceAudioProcessor::setWaveform(int selection, int val){
    if (sender.connect("127.0.0.1", 57120))
        sender.send("/setWavetype", selection, val-1);
    if (sender.connect("127.0.0.1", 9000))
        sender.send("/setWavetype", selection, val-1);
}

void OrangeJuceAudioProcessor::setPan(float val){
    if (sender.connect("127.0.0.1", 57120))
        sender.send("/setPan", (float) val);
}

void OrangeJuceAudioProcessor::sendState(){
    int state;
    if (button_1_state == true && button_2_state == false && button_3_state == false) {
        state = 0;
    } else if (button_1_state == false && button_2_state == true && button_3_state == false) {
        state = 1;
    } else if (button_1_state == false && button_2_state == false && button_3_state == true) {
        state = 2;
    } else if (button_1_state == true && button_2_state == true && button_3_state == false) {
        state = 3;
    } else if (button_1_state == true && button_2_state == false && button_3_state == true) {
        state = 4;
    } else if (button_1_state == false && button_2_state == true && button_3_state == true) {
        state = 5;
    } else if (button_1_state == true && button_2_state == true && button_3_state == true) {
        state = 6;
    }
    if (sender.connect("127.0.0.1", 57120))
        sender.send("/setButton", state);
    if (sender.connect("127.0.0.1", 9000))
        sender.send("/setButton", state);
}

void OrangeJuceAudioProcessor::beginRecord(){
    message.clear();
    for (int x=0; x<50; x++)
        note[x]=0;
    recording = true;
    capacity = 0;
}

void OrangeJuceAudioProcessor::stopRecord(){
    message.addInt32(capacity);
    for (int x=0; x<capacity; x++)
        message.addFloat32(note[x]);
    if (sender.connect("127.0.0.1", 57120))
        sender.send(message);
    if (sender.connect("127.0.0.1", 9000))
        sender.send(message);
    recording = false;
}

void OrangeJuceAudioProcessor::setTime(float val){
    int val_int = (int) (val*1000);
    if (sender.connect("127.0.0.1", 57120))
        sender.send("/setTime", (float) val);
    if (sender.connect("127.0.0.1", 9000))
        sender.send("/setTime", val_int);
}

void OrangeJuceAudioProcessor::setSpeed(float val){
    int val_int = (int) (val*1000);
    if (sender.connect("127.0.0.1", 57120))
        sender.send("/setSpeed", (float) val);
    if (sender.connect("127.0.0.1", 9000))
        sender.send("/setSpeed", (int) val_int);
}

void OrangeJuceAudioProcessor::play(){
    if (sender.connect("127.0.0.1", 57120))
        sender.send("/play");
    if (sender.connect("127.0.0.1", 9000))
        sender.send("/play");
    sequencer_playing = true;
}

void OrangeJuceAudioProcessor::stop(){
    if (sender.connect("127.0.0.1", 57120))
        sender.send("/stop");
    if (sender.connect("127.0.0.1", 9000))
        sender.send("/stop");
    sequencer_playing = false;
}

void OrangeJuceAudioProcessor::sendLFOButton(int number, int value){
    if (sender.connect("127.0.0.1", 57120))
        sender.send("/setLFOButton", (int)number, (int) value);
}

void OrangeJuceAudioProcessor::setCutoff(float val){
    if (sender.connect("127.0.0.1", 57120))
        sender.send("/setCutoff", (float) val);
}

void OrangeJuceAudioProcessor::setResonance(float val){
    if (sender.connect("127.0.0.1", 57120))
        sender.send("/setResonance", (float) val);
}

void OrangeJuceAudioProcessor::setLevel(float val){
    if (sender.connect("127.0.0.1", 57120))
        sender.send("/setAmplitude", (float) val);
}

void OrangeJuceAudioProcessor::setDetuneSequencer(float val){
    if (sender.connect("127.0.0.1", 57120))
        sender.send("/setDetuneSequencer", (float) val);
}
