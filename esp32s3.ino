#if ARDUINO_USB_MODE
#warning "This sketch should be used when USB is in OTG mode"

// This section will be used when USB is in OTG mode.
// You can include the specific setup for OTG mode here if needed.
void setup() {
  // Setup code for OTG mode
}

void loop() {
  // Loop code for OTG mode
}

#else

#include "USB.h"
#include "USBMIDI.h"

const int addresses[4][4] = {
  {36, 37, 38, 39},
  {40, 41, 42, 43},
  {44, 45, 46, 47},
  {48, 49, 50, 51}
};

const int rowPins[4] = {4, 5, 6, 7};
const int colPins[4] = {15, 16, 17, 18};

bool previousState[4][4] = {0};

USBMIDI MIDI; // Use USBMIDI instance for MIDI communication

void setup() {
  MIDI.begin(); // Initialize USBMIDI
  setupButtonMatrix();
}

void loop() {
  checkButtonMatrix();
}

void setupButtonMatrix() {
  for (int i = 0; i < 4; i++) {
    pinMode(rowPins[i], INPUT_PULLUP); // Set row pins as input with pull-up resistors
    pinMode(colPins[i], OUTPUT); // Set column pins as output
    digitalWrite(colPins[i], HIGH); // Set column pins to high
  }
}

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

void sendMIDIMessage(int note, bool isNoteOn) {
  if (isNoteOn) {
    MIDI.noteOn(note, 127, 1); // Send MIDI note-on message, velocity 127, channel 1
  } else {
    MIDI.noteOff(note, 0, 1); // Send MIDI note-off message, velocity 0, channel 1
  }
}

#endif /* ARDUINO_USB_MODE */
