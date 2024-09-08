#include <USBMIDI.h>

// The note numbers corresponding to the buttons in the matrix
const int addresses[4][4] = {
  {36, 37, 38, 39},
  {40, 41, 42, 43},
  {44, 45, 46, 47},
  {48, 49, 50, 51}
};

// Pins for the rows and columns of the button matrix
const int rowPins[4] = {4, 5, 6, 7};
const int colPins[4] = {15, 16, 17, 18};

// Variables to store the previous state of each button
bool previousState[4][4] = {0};

USBMIDI MIDI; // Use USBMIDI instance for MIDI communication

void setup() {
  MIDI.begin(); // Initialize USBMIDI
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
      bool currentState = digitalRead(rowPins[j]) == LOW; // Read the current button state
      if (currentState != previousState[j][i]) { // If the button state has changed
        if (currentState) { // Button pressed
          sendMIDIMessage(addresses[j][i], true);
        } else { // Button released
          sendMIDIMessage(addresses[j][i], false);
        }
        previousState[j][i] = currentState; // Update the previous state
      }
    }
    digitalWrite(colPins[i], HIGH); // Set current column pin back to high
  }
}

// Function to send MIDI messages
void sendMIDIMessage(int note, bool isNoteOn) {
  if (isNoteOn) {
    MIDI.noteOn(note, 127, 1); // Send MIDI note-on message, velocity 127, channel 1
  } else {
    MIDI.noteOff(note, 0, 1); // Send MIDI note-off message, velocity 0, channel 1
  }
}
