/*
  Project: LED Mode Controller with OLED and Buzzer
  Name: Ali Faisal
  Reg No: 23-NTU-CS-1010
*/

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// --- Pin definitions ---
#define LED1 17           // LED 1 pin
#define LED2 18           // LED 2 pin
#define LED3 19          // LED 3 (PWM LED)
#define BTN_MODE 26      // Button for changing modes
#define BTN_RESET 25    // Button for resetting to OFF
#define BUZZER 15        // Buzzer pin

// --- OLED display object ---
Adafruit_SSD1306 display(128, 64, &Wire, -1);

// --- Variables for modes and LED control ---
int mode = 0;                     // stores current LED mode (1–4)
unsigned long prevMillis = 0;     // for timing LED blink
bool ledState = false;             // LED toggle flag

// --- Function: show message on OLED ---
void showMsg(String msg) {
  display.clearDisplay();          // clear old screen content
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 20);
  display.print("Mode: ");         // show "Mode:" label
  display.println(msg);            // print current mode
  display.display();               // update OLED screen
}

// --- Function: make a short beep sound ---
void beepBuzzer(int freq, int dur) {
  tone(BUZZER, freq, dur);         // start buzzer tone
  delay(dur + 50);                 // wait for duration
  noTone(BUZZER);                  // stop buzzer
}

void setup() {
  // --- Pin setup ---
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(BTN_MODE, INPUT_PULLUP);   // use internal pull-up
  pinMode(BTN_RESET, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);

  // --- Initialize OLED display ---
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.display();

  showMsg("Both OFF");              // show default mode at start
}

void loop() {
  // --- Check Mode button ---
  if (digitalRead(BTN_MODE) == LOW) {
    delay(200);                     // debounce delay
    mode++;                         // next mode
    if (mode > 4) mode = 1;         // wrap back to mode 1 after 4

    // --- Change behavior based on selected mode ---
    switch (mode) {
      case 1:
        // Mode 1: Both LEDs OFF
        digitalWrite(LED1, LOW);
        digitalWrite(LED2, LOW);
        showMsg("Both OFF");
        beepBuzzer(800, 120);       // short beep
        break;

      case 2:
        // Mode 2: Alternate Blink
        showMsg("Alternate Blink");
        beepBuzzer(1000, 120);
        break;

      case 3:
        // Mode 3: Both LEDs ON
        digitalWrite(LED1, HIGH);
        digitalWrite(LED2, HIGH);
        showMsg("Both ON");
        beepBuzzer(1200, 120);
        break;

      case 4:
        // Mode 4: PWM Fade
        showMsg("PWM Fade");
        beepBuzzer(1500, 120);
        break;
    }
  }

  // --- Check Reset button ---
  if (digitalRead(BTN_RESET) == LOW) {
    delay(200);
    mode = 1;                        // go back to mode 1 (OFF)
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    analogWrite(LED3, 0);
    showMsg("Reset to OFF");
    beepBuzzer(400, 200);
  }

  // --- Mode behaviors ---
  if (mode == 2) { // Alternate Blink mode
    if (millis() - prevMillis >= 500) {
      prevMillis = millis();
      ledState = !ledState;           // toggle state
      digitalWrite(LED1, ledState);
      digitalWrite(LED2, !ledState);
    }
  }

  if (mode == 4) { // PWM Fade mode
    // increase LED brightness
    for (int i = 0; i <= 255; i++) {
      analogWrite(LED3, i);
      delay(5);
    }
    // decrease LED brightness
    for (int i = 255; i >= 0; i--) {
      analogWrite(LED3, i);
      delay(5);
    }
  }
}
