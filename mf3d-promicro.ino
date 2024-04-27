#include <MIDIUSB.h>

// The note numbers corresponding to the buttons in the matrix
const int addresses[4][4] = {
  {36, 37, 38, 39},
  {40, 41, 42, 43},
  {44, 45, 46, 47},
  {48, 49, 50, 51}
};

// Pins for the rows and columns of the button matrix
const int rowPins[4] = {2, 3, 4, 5};
const int colPins[4] = {6, 7, 8, 9};

void setup() {
  MidiUSB.flush(); // Initialize the MIDIUSB library
  setupButtonMatrix();
}

void loop() {
  checkButtonMatrix();
}

// Function to set up the button matrix
void setupButtonMatrix() {
  for (int i = 0; i < 4; i++) {
    pinMode(rowPins[i], INPUT_PULLUP); // Set row pins as input with pull-up resistors
    pinMode(colPins[i], OUTPUT); // Set column pins as output
    digitalWrite(colPins[i], HIGH); // Set column pins to high
  }
}

// Function to check the state of the buttons in the matrix
void checkButtonMatrix() {
  for (int i = 0; i < 4; i++) {
    digitalWrite(colPins[i], LOW); // Set current column pin to low
    for (int j = 0; j < 4; j++) {
      if (digitalRead(rowPins[j]) == LOW) { // If button is pressed
        midiEventPacket_t noteOn = {0x09, 0x90 | 0, addresses[j][i], 127}; // MIDI note-on message
        MidiUSB.sendMIDI(noteOn); // Send MIDI message
        delay(50); // Debouncing delay
        while (digitalRead(rowPins[j]) == LOW) {} // Wait until button is released
        midiEventPacket_t noteOff = {0x08, 0x80 | 0, addresses[j][i], 0}; // MIDI note-off message
        MidiUSB.sendMIDI(noteOff); // Send MIDI message
      }
    }
    digitalWrite(colPins[i], HIGH); // Set current column pin back to high
  }
}
