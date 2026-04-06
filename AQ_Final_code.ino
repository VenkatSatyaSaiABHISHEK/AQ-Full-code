#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <SDS011.h>
#include "Adafruit_AHTX0.h"
#include <ArduinoJson.h>
#include "constants.h"

// ===== SDS011 PM Sensor =====
SoftwareSerial sdsSerial(16, 17); 
SDS011 my_sds;
float pm2_5 = 0.0, pm10 = 0.0;
int sdsErr = -1;

// ===== AHT10 Sensor =====
Adafruit_AHTX0 aht;
float temperature = 0.0, humidity = 0.0;

// ===== Sound Sensor =====
#define SoundSensorPin 36
#define VREF 3.3
float voltageValue = 0.0, dbValue = 0.0;


// ===== Callibatre variables =====

float pm2_5_cal = 0.0, pm10_cal = 0.0, dbValue_cal = 0.0;
float aqiValue = 0.0; 


// ===== LED Pins =====
#define POWER_LED     4
#define WIFI_LED      5

// ===== Timing Variables =====
unsigned long previousMillis = 0;
const unsigned long interval = 60000;  // 60 seconds

// ===== Blink LED Function =====
void blinkLed(int pin, int durationMs, int intervalMs) {
  unsigned long startTime = millis();
  while (millis() - startTime < durationMs) {
    digitalWrite(pin, HIGH);
    delay(intervalMs);
    digitalWrite(pin, LOW);
    delay(intervalMs);
  }
}

// ===== Connect to WiFi with Slow Blinking =====
void connectWiFi() {
  int maxAttempts = 5;
  int attempt = 0;

  while (attempt < maxAttempts) {
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    Serial.print("📡 MAC Address: ");
    Serial.println(WiFi.macAddress());
    unsigned long startAttempt = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startAttempt < 15000) {
      Serial.print(".");
      blinkLed(WIFI_LED, 500, 250);  // 500ms total = 250ms on + 250ms off

    }

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("\n✅ WiFi connected.");
      Serial.print("🌐 IP Address: ");
      Serial.println(WiFi.localIP());
      digitalWrite(WIFI_LED, HIGH);  // Turn ON LED for connected status
      return;  // exit the function
    } else {
      Serial.println("\n❌ WiFi attempt failed.");
      attempt++;
      delay(1000); // small wait before retry
    }
  }

  // If we reach here, all attempts failed
  Serial.println("🛑 Could not connect to WiFi after 5 attempts. Restarting ESP...");
  delay(2000);
  ESP.restart();
}

// ===== Read SDS011 =====
void readSDS() {
  sdsErr = my_sds.read(&pm2_5, &pm10);
  if (!sdsErr) {
    Serial.print("PM2.5: ");
    Serial.print(pm2_5);
    Serial.print(" µg/m³ | PM10: ");
    Serial.print(pm10);
    Serial.println(" µg/m³");
  } else {
    Serial.println("Failed to read from SDS011 sensor!");
    pm2_5 = pm10 = 0.0;
  }
}

// ===== Read AHT10 =====
void readAHT() {
  sensors_event_t humidityEvent, tempEvent;
  aht.getEvent(&humidityEvent, &tempEvent);
  if (!isnan(tempEvent.temperature) && !isnan(humidityEvent.relative_humidity)) {
    temperature = tempEvent.temperature;
    humidity = humidityEvent.relative_humidity;
    Serial.printf("Temp: %.1f °C | Humidity: %.1f%%\n", temperature, humidity);
  } else {
    Serial.println("Failed to read AHT10.");
    temperature = humidity = 0.0;
  }
}

// ===== Read Sound Sensor =====
void readSound() {
  voltageValue = analogRead(SoundSensorPin) / 4096.0 * VREF;
  dbValue = voltageValue * 50.0;
  Serial.print("Sound Level: ");
  Serial.print(dbValue, 1);
  Serial.println(" dBA");
}

// ===== Claibrated PM  & Sound values=====
void Calibrate() {
  pm2_5_cal = (m_pm2 * pm2_5) + c_pm2;
  pm10_cal = (m_pm10 * pm10) + c_pm10;
  Serial.print("PM2.5_cal: ");
  Serial.println(pm2_5_cal);
  Serial.print("PM10_cal: ");
  Serial.println(pm10_cal);

  dbValue_cal = (m_db * dbValue) + c_db;
  Serial.print("Calibrated Noise (dBA): ");
  Serial.println(dbValue_cal);
}

// ===== calculate AQI =====
float calculate_AQI(float pm2_cal, float pm10_cal) {
    float pm25_subIndex;
    if (pm2_cal <= 30) pm25_subIndex = pm2_cal * 50 / 30;
    else if (pm2_cal <= 60) pm25_subIndex = 50 + (pm2_cal - 30) * 50 / 30;
    else if (pm2_cal <= 90) pm25_subIndex = 100 + (pm2_cal - 60) * 100 / 30;
    else if (pm2_cal <= 120) pm25_subIndex = 200 + (pm2_cal - 90) * 100 / 30;
    else if (pm2_cal <= 250) pm25_subIndex = 300 + (pm2_cal - 120) * 100 / 130;
    else pm25_subIndex = 400 + (pm2_cal - 250) * 100 / 130;

    float pm10_subIndex;
    if (pm10_cal <= 50) pm10_subIndex = pm10_cal;
    else if (pm10_cal <= 100) pm10_subIndex = pm10_cal;
    else if (pm10_cal <= 250) pm10_subIndex = 100 + (pm10_cal - 100) * 100 / 150;
    else if (pm10_cal <= 350) pm10_subIndex = 200 + (pm10_cal - 250);
    else if (pm10_cal <= 430) pm10_subIndex = 300 + (pm10_cal - 350) * 100 / 80;
    else pm10_subIndex = 400 + (pm10_cal - 430) * 100 / 80;

    float maxIndex = max(pm25_subIndex, pm10_subIndex);
    Serial.println("AQI value is " + String(maxIndex));
    return maxIndex;
}


// ===== Post Data to CTOP with Fast Blink =====
void postToCTOP(float pm25_cal, float pm10_cal, float temp, float hum, float noise, float aqi) {
  int maxRetries = 5;
  int retryCount = 0;
  bool success = false;

  while (retryCount < maxRetries) {
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      http.begin(ctopApi);
      http.addHeader("Content-Type", "application/json");
      http.addHeader("X-Auth-Token", authToken);

      // Prepare JSON payload
      String payload = "{\"pm25_cal\":" + String(pm25_cal) +
                       ",\"pm10_cal\":" + String(pm10_cal) +
                       ",\"temp\":" + String(temp) +
                       ",\"humidity\":" + String(hum) +
                       ",\"Noise\":" + String(noise) +
                       ",\"AQI\":" + String(aqi) + "}";

      int httpResponseCode = http.POST(payload);

      if (httpResponseCode > 0) {
        Serial.println("✅ CTOP POST success: " + String(httpResponseCode));
        success = true;
        http.end();
        break;
      } else {
        Serial.println("❌ CTOP POST failed, code: " + String(httpResponseCode));
      }

      http.end();
    } else {
      Serial.println("📡 WiFi not connected. Skipping CTOP POST.");
    }

    retryCount++;
    delay(2000); // Wait before retry
  }

  if (!success) {
    Serial.println("🛑 CTOP POST failed 5 times. Restarting ESP...");
    delay(3000);
    ESP.restart();
  }
}


// ===== Setup =====
void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(POWER_LED, OUTPUT);
  pinMode(WIFI_LED, OUTPUT);
  digitalWrite(POWER_LED, HIGH);
  digitalWrite(WIFI_LED, LOW);

  pinMode(SoundSensorPin, INPUT);
  analogReadResolution(12);

  connectWiFi();

  sdsSerial.begin(9600);
  my_sds.begin(16, 17);
  Serial.println("SDS011 Sensor Initialized.");

  Wire.begin(21, 22);
  if (!aht.begin()) {
    Serial.println("AHT10 sensor not found!");
  } else {
    Serial.println("AHT10 initialized.");
  }

  Serial.println("Stabilizing sensors...");
  delay(3000);
}

// ===== Loop =====
void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("WiFi disconnected. Reconnecting...");
      connectWiFi();
    }

    readSDS();
    readAHT();
    readSound();
    Calibrate();
    aqiValue = calculate_AQI(pm2_5_cal, pm10_cal);
    postToCTOP(pm2_5_cal, pm10_cal, temperature, humidity, dbValue_cal, aqiValue);
  }
}

