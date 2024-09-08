#include <USB.h>
#include <USBMIDI.h>

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

USBMIDI MIDI; // Use USBMIDI instance for MIDI communication

void setup() {
  Serial.begin(115200); // Initialize serial communication for debugging
  USB.begin(); // Initialize USB for MIDI communication
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
      if (digitalRead(rowPins[j]) == LOW) { // If button is pressed
        MIDI.noteOn(addresses[j][i], 127, 1); // Send MIDI note-on message, velocity 127, channel 1
        delay(50); // Debouncing delay
        while (digitalRead(rowPins[j]) == LOW) {} // Wait until button is released
        MIDI.noteOff(addresses[j][i], 0, 1); // Send MIDI note-off message, velocity 0, channel 1
      }
    }
    digitalWrite(colPins[i], HIGH); // Set current column pin back to high
  }
}
