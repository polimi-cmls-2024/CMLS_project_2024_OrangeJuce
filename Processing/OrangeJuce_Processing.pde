import oscP5.*;
import netP5.*;

OscP5 oscP5;
NetAddress myRemoteLocation;

float frequency;
float amplitude;
float[] phase = {0, 0, 0};
float scaleX = 0.5;
float margin = 50; // Increased margin to create more space between waveforms
int numWaves = 3;
float[] note = new float[50];
int size;
int play[] = {0, 0, 0};
int[] waveform_type = {1, 1, 1}; // 1: sin, 2: square, 3: sawtooth
float phaseIncrement = 0.05; // Fixed increment value for phase
int button_status = 0;
color bgColor;     // Background color
color waveColor;   // Waveform color
color textColor;   // Text and line color
color titleColor;  // Title color

PFont customFont;  // Font variable
float horizontalZoom = 3.0; // Horizontal zoom factor

int currentNoteIndex = 0;
boolean isSequencerPlaying = false;
int sequencerStartTime;
int speed = 500; // Time between notes in milliseconds
int loopTime = 500; // Time to wait at the end of the sequence before restarting
boolean isResetting = false;
int resetStartTime;

void setup() {
  size(800, 600);
  oscP5 = new OscP5(this, 9000);  // Listen on port 12000
  myRemoteLocation = new NetAddress("127.0.0.1", 9000);
  amplitude = height / 10;
  bgColor = color(0x16, 0x1A, 0x1D); // Convert hex to color
  waveColor = color(0xFF, 0x92, 0x4D);
  textColor = color(0xAD, 0xA3, 0xA2); 
  titleColor = color(0xFF, 0xA5, 0x00); 

  // Load a font
  customFont = createFont("DMSerifDisplay-Regular.ttf", 16);
  textFont(customFont);
}

void draw() {
  background(bgColor); // Set the background color
  stroke(waveColor);   // Set the waveform color
  noFill();
  
  if (isSequencerPlaying) {
    updateSequencer();
  }
  
  // Draw the three waves
  for (int i = 0; i < numWaves; i++) {
    if (play[i] == 1)
      drawWave(i + 1, waveform_type[i]);
    phase[i] += phaseIncrement; // Increase the phase with a fixed value
  }

  // Draw the line and the text above each waveform
  for (int i = 0; i < numWaves; i++) {
    float yPos = margin + (i + 1) * amplitude * 2 + i * margin; // Add extra margin between waveforms
    fill(textColor);
    textAlign(CENTER, BOTTOM);
    textSize(20);
    text("WAVEFORM " + (i + 1), width / 2, yPos - amplitude - 15); // Position the text above the line
    stroke(textColor);  // Color of text and line
    strokeWeight(2); // Set the thickness to 2 pixels
    line(0, yPos - amplitude - 10, width, yPos - amplitude - 10); // Position the line above the waveform
    
    strokeWeight(1); // Reset to default thickness
    stroke(#BA181B); // Red color for the middle line

    // Draw the new line at the middle of the waveform
    line(0, yPos, width, yPos); 

    // Revert the stroke color back to the original
    stroke(textColor); 
  }
  // Add a title at the top
  fill(titleColor);
  textAlign(CENTER, TOP);
  textSize(42);
  text("ORANGE JUCE", width / 2, 20); // Position the title at the top
}

void drawWave(int number, int type) {
  // Draw the waveform
  beginShape();
  for (float x = 0; x < width; x++) {
    float y = getY(number, type, x);
    vertex(x, y);
  }
  endShape();
}

float getY(int number, int type, float x) {
  float yPos = margin + number * amplitude * 2 + (number - 1) * margin; // Add extra margin between waveforms
  float scaledX = x / horizontalZoom; // Scale the x-coordinate
  switch (type) {
    case 1: // Sinusoid
      return yPos + sin(phase[number - 1] + scaledX * frequency * scaleX * 0.5) * amplitude;
    case 2: // Saw
      return yPos + (((phase[number - 1] * 0.25) + ((width - scaledX) * frequency * scaleX * 0.5) / width) % 1.0) * 2 * amplitude - amplitude;
    case 3: // Square
      return yPos + ((sin(phase[number - 1] + scaledX * frequency * scaleX * 0.5) > 0) ? amplitude : -amplitude);
  }
  return 0;
}

void resetPlay(){
  for (int i = 0; i<3; i++)
      play[i] = 0;
}

void setPlay(){
switch (button_status){
      case 0: { play[0] = 1; return; }
      case 1: { play[1] = 1; return; }
      case 2: { play[2] = 1; return; }
      case 3: { play[0] = 1; play[1] = 1; return; }
      case 4: { play[0] = 1; play[2] = 1; return; }
      case 5: { play[1] = 1; play[2] = 1; return; }
      case 6: { play[0] = 1; play[1] = 1; play[2] = 1; return; }
    }
}

void startSequencer() {
  currentNoteIndex = 0;
  sequencerStartTime = millis();
  isSequencerPlaying = true;
  isResetting = false;
}

void stopSequencer() {
  isSequencerPlaying = false;
  resetPlay();
}


void updateSequencer() {
  int currentTime = millis();
  int elapsedTime = currentTime - sequencerStartTime;

  if (isResetting) {
    if (currentTime - resetStartTime >= loopTime) {
      isResetting = false;
      currentNoteIndex = 0;
      sequencerStartTime = millis();
    }
    return;
  }

  if (currentNoteIndex < size) {
    if (elapsedTime >= currentNoteIndex * speed) {
      frequency = note[currentNoteIndex];
      setPlay();
      currentNoteIndex++;
    }
  } else {
    if (elapsedTime >= size * speed) {
      resetPlay();
      isResetting = true;
      resetStartTime = millis();
    }
  }
}

void oscEvent(OscMessage theOscMessage) {
  int number, type;
  float value;
  println("Ricevuto messaggio OSC: " + theOscMessage.addrPattern());
  if (theOscMessage.addrPattern().equals("/setPhase")) {
    number = theOscMessage.get(0).intValue();
    number--;
    value = theOscMessage.get(1).floatValue();
    println("Number: " + number);
    println("Valore: " + value);
    phase[number] = value;
  }
  if (theOscMessage.addrPattern().equals("/setWavetype")) {
    number = theOscMessage.get(0).intValue();
    type = theOscMessage.get(1).intValue();
    type++;
    println("Number: " + number);
    println("Type: " + type);
    waveform_type[number] = type;
  }
  if (theOscMessage.addrPattern().equals("/setFrequency")) {
    value = theOscMessage.get(0).floatValue();
    println("Frequency: " + value);
    frequency = value;
    setPlay();
  }
  if (theOscMessage.addrPattern().equals("/stopNote")) {
    println("Stop");
    resetPlay();
  }
  if (theOscMessage.addrPattern().equals("/setButton")) {
    number = theOscMessage.get(0).intValue();
    button_status = number;
    println("Button status: " + number);
  }
  if (theOscMessage.addrPattern().equals("/playSequencer")) {
    size = theOscMessage.get(0).intValue();
    for (int i=0; i<size; i++)
      note[i] = theOscMessage.get(i+1).floatValue();
    println(size);
  }
  if (theOscMessage.addrPattern().equals("/play")) {
    startSequencer();
    println("Play sequencer");
  }
  if (theOscMessage.addrPattern().equals("/stop")) {
    stopSequencer();
    println("Stop sequencer");
  }
  if (theOscMessage.addrPattern().equals("/setSpeed")) {
    speed = theOscMessage.get(0).intValue();
    println("Speed: " + speed);
  }
  if (theOscMessage.addrPattern().equals("/setTime")) {
    loopTime = theOscMessage.get(0).intValue();
    println("Time: " + loopTime);
  }
}
