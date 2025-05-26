/*
    Arduino Uno R4 WiFi Smart clock
    Features: WiFi time sync, temp monitoring, IMU interaction, LED matrix display
    Hardware: Uses only onboard compontents (LED matrix, IMU, button, WiFi)
*/


// ---- Libraries ----
#include <WiFiS3.h>        // WiFi connectivity
#include <RTC.h>           // Real-Time Clock control
#include <Arduino_LSM6DSOX.h>  // IMU (accelerometer/gyro)
#include <LEDMatrix.h>     // 12x8 LED matrix control
#include "secrets.h"       // Wi-Fi credentials (keep private!)

// ---- Configuration ----
// Wi-Fi Settings
char ssid[] = SECRET_SSID;     // Network SSID from secrets.h
char pass[] = SECRET_PASS;     // Network password from secrets.h
const char* ntpServer = "pool.ntp.org";  // NTP server for time sync
const long gmtOffset = -5 * 3600;  // UTC-5 (EST) in seconds
const int daylightOffset = 3600;   // Daylight savings offset

// --- Hardware Definitions ---
LEDMatrix matrix;           // LED matrix controller object
#define BUTTON_PIN 12       // Onboard user button pin

// --- State Management ---
enum DisplayMode { TIME, TEMP, ACCEL };  // Display modes
DisplayMode displayMode = TIME;          // Current mode
bool displayActive = true;               // Screen on/off state
unsigned long lastMotionTime = 0;        // Last motion detection timestamp
unsigned long lastScrollTime = 0;        // For scrolling animations
int scrollPosition = 0;                  // Text scroll position

// === SETUP ===
void setup() {
    Serial.begin(9600);        // Start serial communication
    while (!Serial);           // Wait for serial port (debugging)
    // Hardware initialization
    matrix.begin();            // Start LED matrix
    matrix.brightness(255);    // Max brightness (0-255)
    pinMode(BUTTON_PIN, INPUT_PULLUP);  // Configure button

    // IMU initialization check
    if (!IMU.begin()) {
        Serial.println("IMU initialization failed!");
        while (1);  // Halt if IMU fails
    }
    // Network connection
    connectToWiFi();  // Connect to Wi-Fi
    syncTime();       // Sync RTC with NTP server
}

// === MAIN LOOP ===
void loop() {
    checkButton();                // Handle mode switching
    checkMotion();                // Detect movement
    controlDisplayBrightness();   // Auto-dim logic
    // Update display if active
    if (displayActive) {
        switch (displayMode) {
            case TIME:  displayTime(); break;  // Show HH:MM
            case TEMP:  displayTemp(); break;  // Show temperature
            case ACCEL: displayAccel(); break; // Show accelerometer vis
        }
        matrix.clear(); // Turn off display
    }
}

// ---- WiFi & Time functions ----
// Connect to WiFi network
void connectToWiFi() {
    WiFi.begin(ssid, pass);
    Serial.print("Connecting to WiFi...");
    while (WiFi.Status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".") // Progress dots
    }
    Serial.println("\nConnected!");
}
// Sync RTC with NTP server
void syncTime() {
    WiFiClient client;
    unsigned long epochTime = WiFi.getTime();
    if (epochTime > 0) {
        RTC.setEpoch(epochTime); // Update RTC
        Serial.println("Time synced with NTP!");

    } else {
        Serial.println("NTP sync failed!");
    }
}

// --- Display Modes ---
// Show current time (HH:MM format)
void displayTime() {
    RTCTime currentTime;
    RTC.getTime(currentTime); // Get RTC data
    char timeStr[6];
    sprintf(timeStr, "%02d:%02d", currentTime.getHour(), currentTime.getMinute());
    matrix.drawText(timeStr, 0, 0); // Static display
}

// Show internal temperature (°C)
void displayTemp() {
    float temp = analogReadTemp(); // built-in sensor
    char tempStr[8];
    sprintf(tempStr, "%1.fC", temp); // format to 1 decimal
    matrix.drawText(tempStr, 0, 0);
}
void displayAccel() {
    float accelX, accelY, accelZ;
    if (IMU.accelerationAvailable()) {
        IMU.readAcceleration(accelX, accelY, accelZ);
        // Map accelerometer values to LED positions
        int ballX = map(accelX * 100, -100, 100, 0, 11); // X-axis (0-11 cols)
        int ballY = map(accelY * 100, -100, 100, 0, 7); // Y-axis (0-7 cols)
        matrix.clear();
        matrix.drawPixel(ballX, ballY, 1); // Draw moving dot
    }
}
// ---- Input Handling ----
// Debounced button press detection
void checkButton() {
    static unsigned long lastDebounce = 0;
    if (digitalRead(BUTTON_PIN) == LOW && millis() - lastDebounce > 200) {
        displayMode = static_cast<DisplayMode>((displayMode + 1) % 3); // Cycle modes
        matrix.clear();             // Clear previous display
        lastDebounce = millis();    // Reset debounce timer
        while (digitalRead(BUTTON_PIN) == LOW); // wait for release
    }
}

// Motion detection via accelerometer
void checkMotion() {
    float accelX, accelY, accelz;
    if (IMU.accelerationAvailable()) {
        IMU.readAcceleration(accelX, accelY, accelZ);
        // Trigger on significant movement
        if (abs(accelX) > 1.2 || abs(accelY) > 1.2) {
            lastMotionTime= millis(); // Update last activity
            displayActive = True;     // Wake display
        }
    }
}

// Auto-dim after 1 minute inactivity
void controlDisplayBrightness() {
    if (millis() - lastMotionTime > 60000)  { // 1 minute timout
        displayActive = false; // Turn off display
    }
}