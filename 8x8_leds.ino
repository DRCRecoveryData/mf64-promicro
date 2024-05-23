#include <MIDIUSB.h>
#include <Adafruit_NeoPixel.h>

#define NUM_LEDS 64
#define LED_PIN 18 // Using A0 pin for LEDs

// The note numbers corresponding to the buttons in the matrix
const int addresses[8][8] = {
  {36, 37, 38, 39, 40, 41, 42, 43},
  {44, 45, 46, 47, 48, 49, 50, 51},
  {52, 53, 54, 55, 56, 57, 58, 59},
  {60, 61, 62, 63, 64, 65, 66, 67},
  {68, 69, 70, 71, 72, 73, 74, 75},
  {76, 77, 78, 79, 80, 81, 82, 83},
  {84, 85, 86, 87, 88, 89, 90, 91},
  {92, 93, 94, 95, 96, 97, 98, 99}
};

// Pins for the rows and columns of the button matrix
const int rowPins[8] = {2, 3, 4, 5, 10, 11, 12, 13};
const int colPins[8] = {6, 7, 8, 9, 14, 15, 16, 17};

// Initialize the NeoPixel strip
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  MidiUSB.flush(); // Initialize the MIDIUSB library
  setupButtonMatrix();
  strip.begin(); // Initialize the NeoPixel library

  // Welcome animation
  colorWipe(strip.Color(255, 0, 0), 50); // Red
  colorWipe(strip.Color(0, 255, 0), 50); // Green
  colorWipe(strip.Color(0, 0, 255), 50); // Blue
  
  initializeLEDs(); // Set all LEDs to dim greyish color
}

void loop() {
  checkButtonMatrix();
}

// Function to set up the button matrix
void setupButtonMatrix() {
  for (int i = 0; i < 8; i++) {
    pinMode(rowPins[i], INPUT_PULLUP); // Set row pins as input with pull-up resistors
    pinMode(colPins[i], OUTPUT); // Set column pins as output
    digitalWrite(colPins[i], HIGH); // Set column pins to high
  }
}

// Function to check the state of the buttons in the matrix
void checkButtonMatrix() {
  for (int i = 0; i < 8; i++) {
    digitalWrite(colPins[i], LOW); // Set current column pin to low
    for (int j = 0; j < 8; j++) {
      if (digitalRead(rowPins[j]) == LOW) { // If button is pressed
        int ledIndex = j * 8 + i; // Calculate the corresponding LED index
        midiEventPacket_t noteOn = {0x09, 0x90 | 0, addresses[j][i], 127}; // MIDI note-on message
        MidiUSB.sendMIDI(noteOn); // Send MIDI message
        strip.setPixelColor(ledIndex, strip.Color(0, 0, 255)); // Set LED to bright blue
        strip.show(); // Update the strip
        delay(50); // Debouncing delay
        while (digitalRead(rowPins[j]) == LOW) {} // Wait until button is released
        midiEventPacket_t noteOff = {0x08, 0x80 | 0, addresses[j][i], 0}; // MIDI note-off message
        MidiUSB.sendMIDI(noteOff); // Send MIDI message
        strip.setPixelColor(ledIndex, strip.Color(50, 50, 50)); // Set LED back to dim greyish
        strip.show(); // Update the strip
      }
    }
    digitalWrite(colPins[i], HIGH); // Set current column pin back to high
  }
}

// Function to wipe color across all LEDs one at a time
void colorWipe(uint32_t color, int wait) {
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, color);
    strip.show();
    delay(wait);
  }
}

// Function to set all LEDs to dim greyish color
void initializeLEDs() {
  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, strip.Color(50, 50, 50)); // Set LED to dim greyish color
  }
  strip.show(); // Update the strip
}
