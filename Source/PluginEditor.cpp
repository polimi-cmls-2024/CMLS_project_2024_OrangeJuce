/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <juce_graphics/juce_graphics.h>


//==============================================================================
OrangeJuceAudioProcessorEditor::OrangeJuceAudioProcessorEditor (OrangeJuceAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (1200, 700);
    connect(15000);
    addListener(this, "/changeSpeed");
    addListener(this, "/changeTime");
    addListener(this, "/changeDetune_seq");
    addListener(this, "/changeFreq");
    addListener(this, "/changeDepth");
    addListener(this, "/changePhase");
    addListener(this, "/changeDetune");
    
    // first set of knobs
    
    phase1.setRange(0.0, 360.0, 1);
    phase1.setSliderStyle(juce::Slider::Rotary);
    phase1.setTextBoxStyle (juce::Slider::TextBoxAbove, false, 50, 20);
    phase1.setColour(juce::Slider::thumbColourId, juce::Colours::orange);
    phase1.addListener(this);

    detune1.setRange(-1,1,0.01);
    detune1.setSliderStyle(juce::Slider::Rotary);
    detune1.setTextBoxStyle (juce::Slider::TextBoxAbove, false, 50, 20);
    detune1.setColour(juce::Slider::thumbColourId, juce::Colours::orange);
    detune1.addListener(this);
   
    attack1.setRange(0,1,0.1);
    attack1.setValue(0.1);
    attack1.setSliderStyle(juce::Slider::Rotary);
    attack1.setTextBoxStyle (juce::Slider::TextBoxAbove, false, 50, 20);
    attack1.setColour(juce::Slider::thumbColourId, juce::Colours::orange);
    attack1.addListener(this);
   
    sustain1.setRange(0,1,0.1);
    sustain1.setValue(0.5);
    sustain1.setSliderStyle(juce::Slider::Rotary);
    sustain1.setTextBoxStyle (juce::Slider::TextBoxAbove, false, 50, 20);
    sustain1.setColour(juce::Slider::thumbColourId, juce::Colours::orange);
    sustain1.addListener(this);
    
    release1.setRange(0,10,1);
    release1.setValue(1);
    release1.setSliderStyle(juce::Slider::Rotary);
    release1.setTextBoxStyle (juce::Slider::TextBoxAbove, false, 50, 20);
    release1.setColour(juce::Slider::thumbColourId, juce::Colours::orange);
    release1.addListener(this);
   
    freq1.setRange(0.01,20,0.01);
    freq1.setValue(6);
    freq1.setSliderStyle(juce::Slider::Rotary);
    freq1.setTextBoxStyle (juce::Slider::TextBoxAbove, false, 50, 20);
    freq1.setColour(juce::Slider::thumbColourId, juce::Colours::orange);
    freq1.addListener(this);
    
    dep1.setRange(0,100,1);
    dep1.setSliderStyle(juce::Slider::Rotary);
    dep1.setTextBoxStyle (juce::Slider::TextBoxAbove, false, 50, 20);
    dep1.setColour(juce::Slider::thumbColourId, juce::Colours::orange);
    dep1.addListener(this);
    

    addAndMakeVisible(activation_lfo1);
    activation_lfo1.setToggleable(true);
    activation_lfo1.setColour(juce::TextButton::buttonOnColourId, juce::Colours::green); // It changes the colour when it is clicked
    activation_lfo1.onClick = [this] { buttonClicked(&activation_lfo1); };
    
    
    // settings of octave - first set
    
    octaveUpButton1.setButtonText("+");
    octaveUpButton1.onClick = [this] { changeOctave1(1); };
    addAndMakeVisible(octaveUpButton1);

    octaveDownButton1.setButtonText("-");
    octaveDownButton1.onClick = [this] { changeOctave1(-1); };
    addAndMakeVisible(octaveDownButton1);

    octaveLabel1.setText(juce::String(currentOctave1), juce::dontSendNotification);
    octaveLabel1.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(octaveLabel1);

    // second set of knobs
    
    phase2.setRange(0.0, 360.0, 1);
    phase2.setSliderStyle(juce::Slider::Rotary);
    phase2.setTextBoxStyle (juce::Slider::TextBoxAbove, false, 50, 20);
    phase2.setColour(juce::Slider::thumbColourId, juce::Colours::orange);
    phase2.addListener(this);
   
    
    detune2.setRange(-1,1,0.01);
    detune2.setSliderStyle(juce::Slider::Rotary);
    detune2.setTextBoxStyle (juce::Slider::TextBoxAbove, false, 50, 20);
    detune2.setColour(juce::Slider::thumbColourId, juce::Colours::orange);
    detune2.addListener(this);
   

    attack2.setRange(0,1,0.1);
    attack2.setValue(0.1);
    attack2.setSliderStyle(juce::Slider::Rotary);
    attack2.setTextBoxStyle (juce::Slider::TextBoxAbove, false, 50, 20);
    attack2.setColour(juce::Slider::thumbColourId, juce::Colours::orange);
    attack2.addListener(this);
   
    
    sustain2.setRange(0,1,0.1);
    sustain2.setValue(0.5);
    sustain2.setSliderStyle(juce::Slider::Rotary);
    sustain2.setTextBoxStyle (juce::Slider::TextBoxAbove, false, 50, 20);
    sustain2.setColour(juce::Slider::thumbColourId, juce::Colours::orange);
    sustain2.addListener(this);
    
    
    release2.setRange(0,10,1);
    release2.setValue(1);
    release2.setSliderStyle(juce::Slider::Rotary);
    release2.setTextBoxStyle (juce::Slider::TextBoxAbove, false, 50, 20);
    release2.setColour(juce::Slider::thumbColourId, juce::Colours::orange);
    release2.addListener(this);
   
    
    freq2.setRange(0.01,20,0.01);
    freq2.setValue(6);
    freq2.setSliderStyle(juce::Slider::Rotary);
    freq2.setTextBoxStyle (juce::Slider::TextBoxAbove, false, 50, 20);
    freq2.setColour(juce::Slider::thumbColourId, juce::Colours::orange);
    freq2.addListener(this);
    
    dep2.setRange(0,100,1);
    dep2.setSliderStyle(juce::Slider::Rotary);
    dep2.setTextBoxStyle (juce::Slider::TextBoxAbove, false, 50, 20);
    dep2.setColour(juce::Slider::thumbColourId, juce::Colours::orange);
    dep2.addListener(this);
    
    
    addAndMakeVisible(activation_lfo2);
    activation_lfo2.setToggleable(true);
    activation_lfo2.setColour(juce::TextButton::buttonOnColourId, juce::Colours::green); // It changes the colour when it is clicked
    activation_lfo2.onClick = [this] { buttonClicked(&activation_lfo2); };
    
    // settings of octave - second set
    
    octaveUpButton2.setButtonText("+");
    octaveUpButton2.onClick = [this] { changeOctave2(1); };
    addAndMakeVisible(octaveUpButton2);

    octaveDownButton2.setButtonText("-");
    octaveDownButton2.onClick = [this] { changeOctave2(-1); };
    addAndMakeVisible(octaveDownButton2);

    octaveLabel2.setText(juce::String(currentOctave2), juce::dontSendNotification);
    octaveLabel2.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(octaveLabel2);
    
    // third set of knobs
    
    phase3.setRange(0.0, 360.0, 1);
    phase3.setSliderStyle(juce::Slider::Rotary);
    phase3.setTextBoxStyle (juce::Slider::TextBoxAbove, false, 50, 20);
    phase3.setColour(juce::Slider::thumbColourId, juce::Colours::orange);
    phase3.addListener(this);
   
    detune3.setRange(-1,1,0.01);
    detune3.setSliderStyle(juce::Slider::Rotary);
    detune3.setTextBoxStyle (juce::Slider::TextBoxAbove, false, 50, 20);
    detune3.setColour(juce::Slider::thumbColourId, juce::Colours::orange);
    detune3.addListener(this);
   
    attack3.setRange(0,1,0.1);
    attack3.setValue(0.1);
    attack3.setSliderStyle(juce::Slider::Rotary);
    attack3.setTextBoxStyle (juce::Slider::TextBoxAbove, false, 50, 20);
    attack3.setColour(juce::Slider::thumbColourId, juce::Colours::orange);
    attack3.addListener(this);
       
    sustain3.setRange(0,1,0.1);
    sustain3.setValue(0.5);
    sustain3.setSliderStyle(juce::Slider::Rotary);
    sustain3.setTextBoxStyle (juce::Slider::TextBoxAbove, false, 50, 20);
    sustain3.setColour(juce::Slider::thumbColourId, juce::Colours::orange);
    sustain3.addListener(this);
    
    release3.setRange(0,10,1);
    release3.setValue(1);
    release3.setSliderStyle(juce::Slider::Rotary);
    release3.setTextBoxStyle (juce::Slider::TextBoxAbove, false, 50, 20);
    release3.setColour(juce::Slider::thumbColourId, juce::Colours::orange);
    release3.addListener(this);
    
    freq3.setRange(0.01,20,0.01);
    freq3.setValue(6);
    freq3.setSliderStyle(juce::Slider::Rotary);
    freq3.setTextBoxStyle (juce::Slider::TextBoxAbove, false, 50, 20);
    freq3.setColour(juce::Slider::thumbColourId, juce::Colours::orange);
    freq3.addListener(this);
    
    dep3.setRange(0,100,1);
    dep3.setSliderStyle(juce::Slider::Rotary);
    dep3.setTextBoxStyle (juce::Slider::TextBoxAbove, false, 50, 20);
    dep3.setColour(juce::Slider::thumbColourId, juce::Colours::orange);
    dep3.addListener(this);
    
    
    addAndMakeVisible(activation_lfo3);
    activation_lfo3.setToggleable(true);
    activation_lfo3.setColour(juce::TextButton::buttonOnColourId, juce::Colours::green); // It changes the colour when it is clicked
    activation_lfo3.onClick = [this] { buttonClicked(&activation_lfo3); };
    
    
   
    // settings of octave - third set
    
    octaveUpButton3.setButtonText("+");
    octaveUpButton3.onClick = [this] { changeOctave3(1); };
    addAndMakeVisible(octaveUpButton3);

    octaveDownButton3.setButtonText("-");
    octaveDownButton3.onClick = [this] { changeOctave3(-1); };
    addAndMakeVisible(octaveDownButton3);

    octaveLabel3.setText(juce::String(currentOctave3), juce::dontSendNotification);
    octaveLabel3.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(octaveLabel3);
    
    // filter controls
    
    cutoff.setRange(10000,40000,1000);
    cutoff.setValue(20000);
    cutoff.setSliderStyle(juce::Slider::Rotary);
    cutoff.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 50, 20);
    cutoff.setColour(juce::Slider::thumbColourId, juce::Colours::orange);
    cutoff.addListener(this);
    
    resonance.setRange(0,1,0.1);
    resonance.setValue(0.5);
    resonance.setSliderStyle(juce::Slider::Rotary);
    resonance.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 50, 20);
    resonance.setColour(juce::Slider::thumbColourId, juce::Colours::orange);
    resonance.addListener(this);
    
    
    // general controls
    
    pan.setRange(-1,1,0.1);
    pan.setSliderStyle(juce::Slider::Rotary);
    pan.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 50, 20);
    pan.setColour(juce::Slider::thumbColourId, juce::Colours::orange);
    pan.addListener(this);
    
    level.setRange(0,100,1);
    level.setValue(25);
    level.setSliderStyle(juce::Slider::LinearVertical);
    level.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 50, 20);
    level.setColour(juce::Slider::thumbColourId, juce::Colours::orange);
    level.addListener(this);
    
    
    // sequencer controls
    
    
    time.setRange(0,1,0.001);
    time.setSliderStyle(juce::Slider::Rotary);
    time.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 50, 20);
    time.setColour(juce::Slider::thumbColourId, juce::Colours::orange);
    time.setValue(1);
    time.addListener(this);
    
    speed.setRange(0,0.5,0.001);
    speed.setSliderStyle(juce::Slider::Rotary);
    speed.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 50, 20);
    speed.setColour(juce::Slider::thumbColourId, juce::Colours::orange);
    speed.setValue(1);
    speed.addListener(this);
    
    
    time.setRange(0,0.5,0.001);
    time.setSliderStyle(juce::Slider::Rotary);
    time.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 50, 20);
    time.setColour(juce::Slider::thumbColourId, juce::Colours::orange);
    time.addListener(this);
    
    detune_seq.setRange(-50,50,1);
    detune_seq.setSliderStyle(juce::Slider::Rotary);
    detune_seq.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 50, 20);
    detune_seq.setColour(juce::Slider::thumbColourId, juce::Colours::orange);
    detune_seq.addListener(this);
    
    record.setToggleable(true);
    
    record.setColour(juce::TextButton::buttonOnColourId, juce::Colours::red); // It changes the colour when it is clicked
    record.onClick = [this] { buttonClicked(&record); };
    
    play_pause.setToggleable(true);
    
    play_pause.setColour(juce::TextButton::buttonOnColourId, juce::Colours::greenyellow); // It changes the colour when it is clicked
    play_pause.onClick = [this] { buttonClicked(&play_pause); };
    
    
    choice_1.addItem("Sine", 1);
    choice_1.addItem("Saw", 2);
    choice_1.addItem("Square", 3);
    choice_2.addItem("Sine", 1);
    choice_2.addItem("Saw", 2);
    choice_2.addItem("Square", 3);
    choice_3.addItem("Sine", 1);
    choice_3.addItem("Saw", 2);
    choice_3.addItem("Square", 3);
    choice_1.setSelectedId(1);
    choice_2.setSelectedId(1);
    choice_3.setSelectedId(1);
    choice_1.addListener(this);
    choice_2.addListener(this);
    choice_3.addListener(this);
    
    button_1.setToggleable(true);
    button_1.setToggleState(true, juce::dontSendNotification);
    button_1.setColour(juce::TextButton::buttonOnColourId, juce::Colours::green); // It changes the colour when it is clicked
    button_1.onClick = [this] { buttonClicked(&button_1); };
    
    button_2.setToggleable(true);
    
    button_2.setColour(juce::TextButton::buttonOnColourId, juce::Colours::green); // It changes the colour when it is clicked
    button_2.onClick = [this] { buttonClicked(&button_2); };
    
    button_3.setToggleable(true);
    
    button_3.setColour(juce::TextButton::buttonOnColourId, juce::Colours::green); // It changes the colour when it is clicked
    button_3.onClick = [this] { buttonClicked(&button_3); };
    
    addAndMakeVisible(phase1);
    addAndMakeVisible(detune1);
    addAndMakeVisible(attack1);
    addAndMakeVisible(sustain1);
    addAndMakeVisible(release1);
    addAndMakeVisible(freq1);
    addAndMakeVisible(dep1);
    
    addAndMakeVisible(phase2);
    addAndMakeVisible(detune2);
    addAndMakeVisible(attack2);
    addAndMakeVisible(sustain2);
    addAndMakeVisible(release2);
    addAndMakeVisible(freq2);
    addAndMakeVisible(dep2);
    
    addAndMakeVisible(phase3);
    addAndMakeVisible(detune3);
    addAndMakeVisible(attack3);
    addAndMakeVisible(sustain3);
    addAndMakeVisible(release3);
    addAndMakeVisible(freq3);
    addAndMakeVisible(dep3);
    addAndMakeVisible(cutoff);
    addAndMakeVisible(resonance);
    
    addAndMakeVisible(pan);
    addAndMakeVisible(level);
    
    addAndMakeVisible(record);
    addAndMakeVisible(time);
    addAndMakeVisible(speed);
    addAndMakeVisible(detune_seq);
    addAndMakeVisible(play_pause);

    addAndMakeVisible(choice_1);
    addAndMakeVisible(choice_2);
    addAndMakeVisible(choice_3);
    addAndMakeVisible(button_1);
    addAndMakeVisible(button_2);
    addAndMakeVisible(button_3);
}

OrangeJuceAudioProcessorEditor::~OrangeJuceAudioProcessorEditor()
{
}

//==============================================================================
void OrangeJuceAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    //g.setColour (juce::Colours::white);
   
    background = juce::ImageCache::getFromMemory(BinaryData::background_png_png, BinaryData::background_png_pngSize);
    g.drawImageWithin(background, 0, 0, getWidth(), getHeight(), juce::RectanglePlacement::stretchToFit);
    g.setFont (15.0f);
}

void OrangeJuceAudioProcessorEditor::resized()
{
    
    // first set of knobs
    phase1.setBounds(252,85,82,82);
    detune1.setBounds(444,85,82,82);
    attack1.setBounds(593,85,82,82);
    sustain1.setBounds(684,85,82,82);
    release1.setBounds(775,85,82,82);
    freq1.setBounds(924,85,82,82);
    dep1.setBounds(1026,85,82,82);
    activation_lfo1.setBounds(1002, 124, 25, 25);
    
    // settings of octave - first set - bounds
    
    octaveDownButton1.setBounds(363, 122, 25, 25);
    octaveLabel1.setBounds(377, 82, 25, 25);
    octaveUpButton1.setBounds(388, 122, 25, 25);
    
    
    // second set of knobs
    
    phase2.setBounds(252,237,82,82);
    detune2.setBounds(444,237,82,82);
    attack2.setBounds(593,237,82,82);
    sustain2.setBounds(684,237,82,82);
    release2.setBounds(775,237,82,82);
    freq2.setBounds(924,237,82,82);
    dep2.setBounds(1026,237,82,82);
    activation_lfo2.setBounds(1002, 276, 25, 25);
    
    
    // settings of octave - second set - bounds
    
    octaveDownButton2.setBounds(363, 273, 25, 25);
    octaveLabel2.setBounds(377, 233, 25, 25);
    octaveUpButton2.setBounds(388, 273, 25, 25);
    
    // third set of knobs
    
    phase3.setBounds(252,390,82,82);
    detune3.setBounds(444,390,82,82);
    attack3.setBounds(593,390,82,82);
    sustain3.setBounds(684,390,82,82);
    release3.setBounds(775,390,82,82);
    freq3.setBounds(924,390,82,82);
    dep3.setBounds(1026,390,82,82);
    activation_lfo3.setBounds(1002, 430, 25, 25);

    // settings of octave - third set - bounds
    
    octaveDownButton3.setBounds(363, 424, 25, 25);
    octaveLabel3.setBounds(377, 387, 25, 25);
    octaveUpButton3.setBounds(388, 424, 25, 25);
    
    
    // filter controls
    
    cutoff.setBounds(132, 570, 82, 82);
    resonance.setBounds(295, 570, 82, 82);
    
    // general controls
    
    pan.setBounds(508, 570, 82, 82);
    level.setBounds(606, 562, 120, 120);
    
    // sequencer controls
    
    record.setBounds(803, 575, 35, 35);
    speed.setBounds(875, 570, 82, 82);
    time.setBounds(973, 570, 82, 82);
    play_pause.setBounds(803, 620, 35, 35);
    detune_seq.setBounds(1067, 570, 82, 82);
    
    //combobox for the waveforms
    
    choice_1.setBounds(185, 152, 70, 20);
    choice_2.setBounds(185, 302, 70, 20);
    choice_3.setBounds(185, 452, 70, 20);
    
    //button for the activation of the waveform
    
    button_1.setBounds(204, 100, 35, 35);
    button_2.setBounds(204, 250, 35, 35);
    button_3.setBounds(204, 400, 35, 35);
}

//Active the respective function when the value of a slider changes

void OrangeJuceAudioProcessorEditor::sliderValueChanged(juce::Slider *slider){
    if (slider == &phase1){
        audioProcessor.setPhase(0, (float)phase1.getValue());
        return;
    }
    if (slider == &detune1){
        audioProcessor.setDetune(0, (float)detune1.getValue());
        return;
    }
    if (slider == &attack1){
        audioProcessor.setAttack(0, (float)attack1.getValue());
        return;
    }
    if (slider == &sustain1){
        audioProcessor.setSustain(0, (float)sustain1.getValue());
        return;
    }
    if (slider == &release1){
        audioProcessor.setRelease(0, (float)release1.getValue());
        return;
    }
    if (slider == &freq1){
        audioProcessor.setFreq(0, (float)freq1.getValue());
        return;
    }
    if (slider == &dep1){
        audioProcessor.setDepth(0, (float)dep1.getValue());
        return;
    }
    if (slider == &phase2){
        audioProcessor.setPhase(1, (float)phase2.getValue());
        return;
    }
    if (slider == &detune2){
        audioProcessor.setDetune(1, (float)detune2.getValue());
        return;
    }
    if (slider == &attack2){
        audioProcessor.setAttack(1, (float)attack2.getValue());
        return;
    }
    if (slider == &sustain2){
        audioProcessor.setSustain(1, (float)sustain2.getValue());
        return;
    }
    if (slider == &release2){
        audioProcessor.setRelease(1, (float)release2.getValue());
        return;
    }
    if (slider == &freq2){
        audioProcessor.setFreq(1, (float)freq2.getValue());
        return;
    }
    if (slider == &dep2){
        audioProcessor.setDepth(1, (float)dep2.getValue());
        return;
    }
    if (slider == &phase3){
        audioProcessor.setPhase(2, (float)phase3.getValue());
        return;
    }
    if (slider == &detune3){
        audioProcessor.setDetune(2, (float)detune3.getValue());
        return;
    }
    if (slider == &attack3){
        audioProcessor.setAttack(2, (float)attack3.getValue());
        return;
    }
    if (slider == &sustain3){
        audioProcessor.setSustain(2, (float)sustain3.getValue());
        return;
    }
    if (slider == &release3){
        audioProcessor.setRelease(2, (float)release3.getValue());
        return;
    }
    if (slider == &freq3){
        audioProcessor.setFreq(2, (float)freq3.getValue());
        return;
    }
    if (slider == &dep3){
        audioProcessor.setDepth(2, (float)dep3.getValue());
        return;
    }
    if (slider == &pan){
        audioProcessor.setPan((float)pan.getValue());
        return;
    }
    if (slider == &level){
        if (level.getValue()==0)
            audioProcessor.setLevel(0);
        else audioProcessor.setLevel((float)level.getValue() / 50);
        return;
    }
    if (slider == &time){
        audioProcessor.setTime((float)time.getValue());
        return;
    }
    if (slider == &speed){
        audioProcessor.setSpeed((float)speed.getValue());
        return;
    }
    if (slider == &resonance){
        audioProcessor.setResonance((float)resonance.getValue());
        return;
    }
    if (slider == &cutoff){
        audioProcessor.setCutoff((float)cutoff.getValue());
        return;
    }
    if (slider == &detune_seq){
        audioProcessor.setDetuneSequencer((float)detune_seq.getValue());
        return;
    }
}

void OrangeJuceAudioProcessorEditor::changeOctave1(int delta)
{
    currentOctave1 = juce::jlimit(-2, 2, currentOctave1 + delta); // Limita l'intervallo dell'ottava tra 0 e 8
    octaveLabel1.setText(juce::String(currentOctave1), juce::dontSendNotification);
    audioProcessor.setCurrentOctave1(currentOctave1);
}

void OrangeJuceAudioProcessorEditor::changeOctave2(int delta)
{
    currentOctave2 = juce::jlimit(-2, 2, currentOctave2 + delta); // Limita l'intervallo dell'ottava tra 0 e 8
    octaveLabel2.setText(juce::String(currentOctave2), juce::dontSendNotification);
    audioProcessor.setCurrentOctave2(currentOctave2);
}

void OrangeJuceAudioProcessorEditor::changeOctave3(int delta)
{
    currentOctave3 = juce::jlimit(-2, 2, currentOctave3 + delta); // Limita l'intervallo dell'ottava tra 0 e 8
    octaveLabel3.setText(juce::String(currentOctave3), juce::dontSendNotification);
    audioProcessor.setCurrentOctave3(currentOctave3);
}


void OrangeJuceAudioProcessorEditor::comboBoxChanged(juce::ComboBox *box){
    if (box == &choice_1){
        audioProcessor.setWaveform(0, choice_1.getSelectedId());
        return;
    }
    if (box == &choice_2){
        audioProcessor.setWaveform(1, choice_2.getSelectedId());
        return;
    }
    if (box == &choice_3){
        audioProcessor.setWaveform(2, choice_3.getSelectedId());
        return;
    }
}

void OrangeJuceAudioProcessorEditor::buttonClicked(juce::Button *button){
    
    int value;
    
    if (button == &button_1){
        if (!(button_1.getToggleState() == true && button_2.getToggleState() == false && button_3.getToggleState() == false)){
            button_1.setToggleState(!button_1.getToggleState(), juce::dontSendNotification);
            audioProcessor.button_1_state = button_1.getToggleState();
            audioProcessor.sendState();
            return;
        }
    }
    
    if (button == &button_2){
        if (!(button_1.getToggleState() == false && button_2.getToggleState() == true && button_3.getToggleState() == false)){
            button_2.setToggleState(!button_2.getToggleState(), juce::dontSendNotification);
            audioProcessor.button_2_state = button_2.getToggleState();
            audioProcessor.sendState();
            return;
        }
    }
    
    if (button == &button_3){
        if (!(button_1.getToggleState() == false && button_2.getToggleState() == false && button_3.getToggleState() == true)){
            button_3.setToggleState(!button_3.getToggleState(), juce::dontSendNotification);
            audioProcessor.button_3_state = button_3.getToggleState();
            audioProcessor.sendState();
            return;
        }
    }
    
    if (button == &record){
        if (record.getToggleState() == false && !audioProcessor.play_status){
            record.setToggleState(true, juce::dontSendNotification);
            audioProcessor.beginRecord();
            return;
        } else {
            record.setToggleState(false, juce::dontSendNotification);
            audioProcessor.stopRecord();
            return;
        }
    }
    
    if (button == &play_pause){
        if (play_pause.getToggleState() == false && audioProcessor.capacity!=0){
            play_pause.setToggleState(true, juce::dontSendNotification);
            audioProcessor.play_status = true;
            audioProcessor.play();
            return;
        } else {
            play_pause.setToggleState(false, juce::dontSendNotification);
            audioProcessor.play_status = false;
            audioProcessor.stop();
            return;
        }
    }
    
    if (button == &activation_lfo1){
            value = activation_lfo1.getToggleState();
            activation_lfo1.setToggleState(!value, juce::dontSendNotification);
            audioProcessor.sendLFOButton(1, !value);
    }
    
    if (button == &activation_lfo2){
            value = activation_lfo2.getToggleState();
            activation_lfo2.setToggleState(!value, juce::dontSendNotification);
            audioProcessor.sendLFOButton(2, !value);
    }
    
    if (button == &activation_lfo3){
            value = activation_lfo3.getToggleState();
            activation_lfo3.setToggleState(!value, juce::dontSendNotification);
            audioProcessor.sendLFOButton(3, !value);
    }
       
}


//OSC message receiver
void OrangeJuceAudioProcessorEditor::oscMessageReceived(const juce::OSCMessage& message){
            juce::String address = message.getAddressPattern().toString();
            if (address == "/changeSpeed") {
                speed.setValue(message[0].getFloat32());
            } else if (address == "/changeTime") {
                time.setValue(message[0].getFloat32());
            } else if (address == "/changeDetune_seq") {
                detune_seq.setValue(message[0].getFloat32());
            } else if (address == "/changeFreq") {
                switch (message[0].getInt32()){
                    case 1:
                        freq1.setValue(message[1].getFloat32());
                        break;
                    case 2:
                        freq2.setValue(message[1].getFloat32());
                        break;
                    case 3:
                        freq3.setValue(message[1].getFloat32());
                        break;
                }
            } else if (address == "/changeDepth") {
                switch (message[0].getInt32()){
                    case 1:
                        dep1.setValue(message[1].getFloat32());
                        break;
                    case 2:
                        dep2.setValue(message[1].getFloat32());
                        break;
                    case 3:
                        dep3.setValue(message[1].getFloat32());
                        break;
                }
            } else if (address == "/changePhase") {
                switch (message[0].getInt32()){
                    case 1:
                        phase1.setValue(message[1].getFloat32());
                        break;
                    case 2:
                        phase2.setValue(message[1].getFloat32());
                        break;
                    case 3:
                        phase3.setValue(message[1].getFloat32());
                        break;
                }
            } else if (address == "/changeDetune") {
                switch (message[0].getInt32()){
                    case 1:
                        detune1.setValue(message[1].getFloat32());
                        break;
                    case 2:
                        detune2.setValue(message[1].getFloat32());
                        break;
                    case 3:
                        detune3.setValue(message[1].getFloat32());
                        break;
                }
            }
}
