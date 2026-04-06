# 📘 README: Air Quality Monitoring System

This project uses an **ESP32** to collect environmental data (PM2.5, PM10, Temperature, Humidity, and Sound Level) and **post it to a remote server (CTOP at IIIT Hyderabad)** via WiFi every **1 minute**.

---


### 📁 File Structure
│
├── main.ino         # Main logic for Pins, Global varibles sensor reading, WiFi, LED handling, posting
├── constants.h      # Configuration:  WiFi credentials, API URLs, Coeffient values
├── README.md        # Project Documentation



### 🧰 Hardware Components

| Component           | Description                      |
| ------------------- | -------------------------------- |
| ESP32               | Microcontroller Board            |
| SDS011              | PM2.5 & PM10 Dust Sensor         |
| AHT10 / AHT20       | Temperature & Humidity Sensor    |
| Analog Sound Sensor | For measuring noise in dBA       |
| LEDs                | Power and WiFi status indicators |
| Jumper Wires        | For connections                  |

---

### 📌 Pinout

| Pin on ESP32 | Connected To                |
| ------------ | --------------------------- |
| GPIO 16, 17  | SDS011 via SoftwareSerial   |
| GPIO 21, 22  | I2C (SCL, SDA) for AHT10    |
| GPIO 36      | Sound Sensor (Analog Input) |
| GPIO 4       | Power LED                   |
| GPIO 5       | WiFi LED                    |

---

### 📦 Libraries Used

Ensure these libraries are installed in Arduino IDE:

- `WiFi.h` – For WiFi connection
- `HTTPClient.h` – For HTTP POST
- `Wire.h` – For I2C communication
- `SoftwareSerial.h` – For SDS011
- `SDS011.h` – Library for SDS011
- `Adafruit_AHTX0.h` – For AHT10 sensor
- `ArduinoJson.h` – For JSON formatting (v6+ recommended)

---

### ⚙️ Configuration: `constants.h`

You must configure `constants.h` for your node and WiFi:

#### Example (for node 1):

```cpp
#define node_1 // Uncomment only one node

const char* ssid = "Your_WIFI";
const char* password = "PASSWORD";

const char* ctopApi = "https://ctop.iiit.ac.in/api/nodes/create-cin/15";
const char* authToken = "your_token_here";

// Calibration Coefficients (can be adjusted based on lab data)
float m_pm2 = 1.0;
float c_pm2 = 0.0;
float m_pm10 = 1.0;
float c_pm10 = 0.0;
float m_db = 1.0;
float c_db = 0.0;
```

Only **one node macro** should be active at a time.

---

### 🧠 How the Code Works (`main.ino`)

#### 🔌 `setup()`

- Initializes sensors, LEDs, and WiFi.
- Begins sensor communication and I2C.
- Attempts to connect to WiFi (5 attempts max).
- Waits 3 seconds for sensor stabilization.

#### ⭯️ `loop()`

Runs every **60 seconds**, performs:

1. **WiFi check:** reconnects if disconnected.
2. **Sensor reading:**
   - PM2.5, PM10 from SDS011.
   - Temp & Humidity from AHT10.
   - Sound Level from analog pin.
3. **Calibration:** Applies slope-intercept calibration.
4. **AQI Calculation:** Based on calibrated PM2.5 & PM10.
5. **POST to CTOP server:** Sends JSON data.
   - Retries 5 times if fails.
   - Restarts ESP if all fail.

---

### 🔄 LED Indications

| LED            | Color | Status                                                         |
| -------------- | ----- | -------------------------------------------------------------- |
| Power LED (D4) | Any   | ON when powered , Blinks if post unsuccessful                  |
| WiFi LED (D5)  | Any   | Blinks during connection & Posting Data, OFF if disconnected   |

---

### 📡 WiFi + MAC Address

MAC address is printed **before connection** – useful for identification.

```cpp
Serial.print("📱 MAC Address: ");
Serial.println(WiFi.macAddress());
```

---

### 🧪 Calibration Formula

Calibration applied as:

```cpp
pm2_5_cal = m_pm2 * pm2_5 + c_pm2;
pm10_cal  = m_pm10 * pm10 + c_pm10;
dbValue_cal = m_db * dbValue + c_db;
```

Set the values in `constants.h` based on your sensor calibration sheet.

---

### 📤 API Format (JSON Payload)

The data sent to CTOP server looks like:

```json
{
  "pm25_cal": 12.5,
  "pm10_cal": 34.2,
  "temp": 28.6,
  "humidity": 65.0,
  "Noise": 42.8,
  "AQI": 102.3
}
```

---

### 🛠️ Customization Tips

- ⏱ **Change Interval**: Edit `interval` in `main.ino` `const unsigned long interval = 60000; // 1 minute`
- 🚩 **Restart on CTOP Fail**: Automatically handled if 5 posts fail
- 💡 **Add More Nodes**: Just add more `#define node_X` blocks in `constants.h`

---

## 🚀 Steps to Run the Code

1. **Load the Code**
   - Open the code in Arduino IDE.
   - Make sure `main.ino` and `constants.h` are in the same folder.

2. **Install Required Libraries**
   - `WiFi.h`
   - `HTTPClient.h`
   - `Wire.h`
   - `SoftwareSerial.h`
   - `SDS011.h`
   - `Adafruit_AHTX0.h`
   - `ArduinoJson.h`

3. **Select Board and Port**
   - Go to **Tools > Board** → Select **ESP32 Dev Module**
   - Go to **Tools > Port** → See the correct COM port from **Device Manager**

4. **Configure `constants.h`**  
   - Enable the appropriate `#define NODE_X` (e.g., `#define NODE_1`) as per your setup.

5. **Compile and Upload**
   - Use the ✔️ button to **Compile**
   - Use the ➡️ button to **Upload**

6. **Open Serial Monitor**
   - Open Serial Monitor at **115200 baud**
   - You should see:
     - MAC Address and IP address
     - Sensor readings (PM2.5, PM10, Temperature, Humidity, Sound)
     - CTOP API response
### ✅ Example Output

```plaintext
Connecting to WiFi📱 MAC Address: 94:B5:55:1F:AD:E2
..........
✅ WiFi connected.
🌐 IP Address: 192.168.1.102
PM2.5: 18.2 µg/m³ | PM10: 45.3 µg/m³
Temp: 26.3 °C | Humidity: 60.2%
Sound Level: 41.6 dBA
PM2.5_cal: 18.2
PM10_cal: 45.3
Calibrated Noise (dBA): 41.6
AQI value is 45.3
✅ CTOP POST success: 200
```

---

### 🖒 Troubleshooting

| Issue                   | Fix                                           |
| ----------------------- | --------------------------------------------- |
| No WiFi                 | Check SSID/PWD in `constants.h`               |
| Sensor not found        | Check wiring, I2C address                     |
| CTOP post fails         | Check internet or authToken                   |
| No data every 1 minute  | Ensure interval is set to 60000 (ms)          |
| ESP restarts frequently | Look at serial logs, WiFi or CTOP post issues |