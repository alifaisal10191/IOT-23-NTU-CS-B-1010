/*
  Project: Button Press Detection (Short/Long Press)
  Name: Ali Faisal
  Reg No: 23-NTU-CS-10104
  Date: 23-Oct-2025
*/

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// --- Pins ---
#define BTN 34
#define LED 18
#define BUZZER 26

// --- OLED Setup ---
Adafruit_SSD1306 display(128, 64, &Wire, -1);

// --- Variables ---
bool ledOn = false;            // LED ON/OFF state
bool buttonPressed = false;    // To check if button was pressed
unsigned long pressStart = 0;  // Time when button was pressed

// --- Function to show text on OLED ---
void showText(String text) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 20);
  display.println(text);
  display.display();
}

void setup() {
  pinMode(BTN, INPUT_PULLUP);  // Button as input (pull-up resistor ON)
  pinMode(LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  showText("System Ready");
}

void loop() {
  int buttonState = digitalRead(BTN);

  // When button is pressed
  if (buttonState == LOW && !buttonPressed) {
    buttonPressed = true;
    pressStart = millis();
  }

  // When button is released
  if (buttonState == HIGH && buttonPressed) {
    buttonPressed = false;
    unsigned long pressTime = millis() - pressStart;

    // Long press (>1.5 seconds)
    if (pressTime > 1500) {
      tone(BUZZER, 1000, 500);     // Play buzzer for 0.5 sec
      showText("Long Press: Buzzer ON");
    } 
    // Short press
    else {
      ledOn = !ledOn;              // Toggle LED
      digitalWrite(LED, ledOn);
      showText("Short Press: LED Toggled");
    }
  }
}
