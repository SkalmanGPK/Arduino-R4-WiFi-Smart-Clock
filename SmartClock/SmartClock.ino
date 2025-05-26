#include <WiFiS3.h>
#include <Arduino_LSM6DSOX.h>
#include <RTC.h>
#include "secrets.h"
#include <LEDMatrix.h>

// Wi-Fi Config
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;
const char* ntpServer = "pool.ntp.org";
const long gmtOffset = 0; // Adjust for your timezone (e.g., -5 * 3600 for EST)
const int daylightOffset = 3600;

// Hardware
LEDMatrix matrix;
#define BUTTON_PIN 12

enum DisplayMode { TIME, TEMP, ACCEL };
DisplayMode displayMode = TIME;
bool displayActive = true;
unsigned long lastMotionTime = 0;
unsigned long lastScrollTime = 0;
int scrollPosition = 0;

void setup() {
    Serial.begin(9600);
    while (!Serial); // wait for Serial Monitor

    // Initialize hardware
    matrix.begin();
    matrix.brightness(255);
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    // Initialize IMU
    if (!IMU.begin()) {
        Serial.println("IMU initialization failed!")
        while (1);
    }
    // Connect to Wi-Fi
    connectToWiFi();

    // Sync RTC with NTP
    syncTime();
}

void loop() {
    checkButton();
    checkMotion();
    controlDisplayBrightness();

    if (displayActive) {
        switch (displayMode) {
            case TIME:  displayTime(); break;
            case TEMP:  displayTemp(); break;
            case ACCEL: displayAccel(); break;
        }
        matrix.clear();
    }
}

// ---- WiFi & Time functions ----
void connectToWiFi() {
    WiFi.begin(ssid, pass);
    Serial.print("Connecting to WiFi...");
    while (WiFi.Status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".")
    }
    Serial.println("\nConnected!");
}

void syncTime() {
    WiFiClient client;
    unsigned long epochTime = WiFi.getTime();
    if (epochTime > 0) {
        RTC.setEpoch(epochTime);
        Serial.println("Time synced with NTP!");

    } else {
        Serial.println("NTP sync failed!");
    }
}

// ---- Input Handling ----
void checkButton() {
    static unsigned long lastDebounce = 0;
    if (digitalRead(BUTTON_PIN) == LOW && millis() - lastDebounce > 200) {
        displayMode = static_cast<DisplayMode>((displayMode + 1) % 3);
        matrix.clear();
        lastDebounce = millis();
        while (digitalRead(BUTTON_PIN) == LOW); // wait for release
    }
}

void checkMotion() {
    float accelX, accelY, accelz;
    if (IMU.accelerationAvailable()) {
        IMU.readAcceleration(accelX, accelY, accelZ);
        if (abs(accelX) > 1.2 || abs(accelY) > 1.2) {
            lastMotionTime= millis();
            displayActive = True;
        }
    }
}

void controlDisplayBrightness() {
    if (millis() - lastMotionTime > 60000)  { // 1 minute timout
        displayActive = false;
    }
}