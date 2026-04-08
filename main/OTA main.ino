
#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <SDS011.h>
#include "Adafruit_AHTX0.h"
#include <ArduinoJson.h>
#include <ArduinoOTA.h>
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

// ===== Calibration =====
float pm2_5_cal = 0.0, pm10_cal = 0.0, dbValue_cal = 0.0;
float aqiValue = 0.0; 

// ===== LED Pins =====
#define POWER_LED     4
#define WIFI_LED      5

// ===== Timing =====
unsigned long previousMillis = 0;
const unsigned long interval = 60000;

// ===== OTA =====
void setupOTA() {
  ArduinoOTA.setHostname("Node-8");

  ArduinoOTA.onStart([]() {
    Serial.println("🔄 OTA Start");
  });

  ArduinoOTA.onEnd([]() {
    Serial.println("\n✅ OTA End");
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("OTA: %u%%\r", (progress * 100) / total);
  });

  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("❌ OTA Error[%u]\n", error);
  });

  ArduinoOTA.begin();
  Serial.println("🚀 OTA Ready");
}

// ===== LED Blink =====
void blinkLed(int pin, int durationMs, int intervalMs) {
  unsigned long startTime = millis();
  while (millis() - startTime < durationMs) {
    digitalWrite(pin, HIGH);
    delay(intervalMs);
    digitalWrite(pin, LOW);
    delay(intervalMs);
  }
}

// ===== WiFi =====
void connectWiFi() {
  int attempts = 0;

  while (attempts < 5) {
    WiFi.begin(ssid, password);

    Serial.print("Connecting WiFi | MAC: ");
    Serial.println(WiFi.macAddress());

    unsigned long start = millis();

    while (WiFi.status() != WL_CONNECTED && millis() - start < 15000) {
      Serial.print(".");
      blinkLed(WIFI_LED, 500, 250);
    }

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("\n✅ Connected");
      Serial.println(WiFi.localIP());

      digitalWrite(WIFI_LED, HIGH);

      setupOTA();
      return;
    }

    attempts++;
    Serial.println("\nRetrying...");
  }

  Serial.println("❌ Restarting ESP...");
  ESP.restart();
}

// ===== Sensors =====
void readSDS() {
  sdsErr = my_sds.read(&pm2_5, &pm10);

  if (!sdsErr) {
    Serial.printf("PM2.5: %.2f | PM10: %.2f\n", pm2_5, pm10);
  } else {
    pm2_5 = pm10 = 0.0;
  }
}

void readAHT() {
  sensors_event_t hum, temp;
  aht.getEvent(&hum, &temp);

  temperature = temp.temperature;
  humidity = hum.relative_humidity;

  Serial.printf("Temp: %.1f | Hum: %.1f\n", temperature, humidity);
}

void readSound() {
  voltageValue = analogRead(SoundSensorPin) / 4096.0 * VREF;
  dbValue = voltageValue * 50.0;

  Serial.printf("Noise: %.1f dBA\n", dbValue);
}

// ===== Calibration =====
void Calibrate() {
  pm2_5_cal = (m_pm2 * pm2_5) + c_pm2;
  pm10_cal = (m_pm10 * pm10) + c_pm10;
  dbValue_cal = (m_db * dbValue) + c_db;
}

// ===== AQI =====
float calculate_AQI(float pm2, float pm10) {
  float aqi = max(pm2, pm10);
  Serial.println("AQI: " + String(aqi));
  return aqi;
}

// ===== POST (FIXED ONLY HERE 🔥) =====
void postToCTOP(float pm25, float pm10, float temp, float hum, float noise, float aqi) {

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    http.begin(ctopApi);

    http.addHeader("Content-Type", "application/json");
    http.addHeader("Authorization", String("Bearer ") + authToken);

    DynamicJsonDocument doc(256);

    // ✅ FIXED KEYS (IMPORTANT)
    doc["pm2.5"] = pm25;
    doc["pm10"] = pm10;
    doc["temperature"] = temp;
    doc["humidity"] = hum;
    doc["noise"] = noise;

    String payload;
    serializeJson(doc, payload);

    Serial.println("Sending JSON:");
    Serial.println(payload);

    int code = http.POST(payload);

    Serial.println("POST Code: " + String(code));

    if (code > 0) {
      Serial.println(http.getString());
    } else {
      Serial.println("POST Failed");
    }

    http.end();
  }
}

// ===== SETUP =====
void setup() {
  Serial.begin(115200);

  pinMode(POWER_LED, OUTPUT);
  pinMode(WIFI_LED, OUTPUT);

  digitalWrite(POWER_LED, HIGH);

  pinMode(SoundSensorPin, INPUT);
  analogReadResolution(12);

  connectWiFi();

  sdsSerial.begin(9600);
  my_sds.begin(16, 17);

  Wire.begin(21, 22);
  aht.begin();

  delay(3000);
}

// ===== LOOP =====
void loop() {

  ArduinoOTA.handle();

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    if (WiFi.status() != WL_CONNECTED) {
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
