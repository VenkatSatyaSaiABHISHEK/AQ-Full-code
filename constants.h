// Node Configuration

#define node_1         
// #define node_2
// #define node_3      
// #define node_4
// #define node_5


#ifdef node_1

// ===== WiFi Credentials =====
const char* ssid = "KIET_ROUTE_!";
const char* password = "DFGUG&^@#*F";

// ===== CTOP API Config =====
const char* ctopApi = "https://ctop.iiit.ac.in/api/nodes/create-cin/20";
const char* authToken = "a64c0e30b6ad0069f1ba920171b6ca1c";

// ===== calibration coeffients =====
float m_pm2 = 0.0007842824264254494;
float c_pm2 = 0.007693018993987527;
float m_pm10 = 0.0006857976315845867;
float c_pm10 = 0.009788900400601618;

float m_db = 1.0;
float c_db = 0.0;

#endif

#ifdef node_2

// ===== WiFi Credentials =====
const char* ssid = "Your_WIFI";
const char* password = "PASSWORD";

// ===== CTOP API Config =====
const char* ctopApi = "https://ctop.iiit.ac.in/api/nodes/create-cin/16";
const char* authToken = "your_token_node_2";

// ===== calibration coeffients =====
float m_pm2 = 1.0;
float c_pm2 = 0.0;
float m_pm10 = 1.0;
float c_pm10 = 0.0;

float m_db = 1.0;
float c_db = 0.0;

#endif

#ifdef node_3

// ===== WiFi Credentials =====
const char* ssid = "Your_WIFI";
const char* password = "PASSWORD";

// ===== CTOP API Config =====
const char* ctopApi = "https://ctop.iiit.ac.in/api/nodes/create-cin/17";
const char* authToken = "your_token_node_3";

// ===== calibration coeffients =====
float m_pm2 = 1.0;
float c_pm2 = 0.0;
float m_pm10 = 1.0;
float c_pm10 = 0.0;

float m_db = 1.0;
float c_db = 0.0;

#endif

#ifdef node_4

// ===== WiFi Credentials =====
const char* ssid = "Your_WIFI";
const char* password = "PASSWORD";

// ===== CTOP API Config =====
const char* ctopApi = "https://ctop.iiit.ac.in/api/nodes/create-cin/18";
const char* authToken = "your_token_node_4";

// ===== calibration coeffients =====
float m_pm2 = 1.0;
float c_pm2 = 0.0;
float m_pm10 = 1.0;
float c_pm10 = 0.0;

float m_db = 1.0;
float c_db = 0.0;

#endif

#ifdef node_5

// ===== WiFi Credentials =====
const char* ssid = "Your_WIFI";
const char* password = "PASSWORD";

// ===== CTOP API Config =====
const char* ctopApi = "https://ctop.iiit.ac.in/api/nodes/create-cin/19";
const char* authToken = "your_token_node_5";

// ===== calibration coeffients =====
float m_pm2 = 1.0;
float c_pm2 = 0.0;
float m_pm10 = 1.0;
float c_pm10 = 0.0;

float m_db = 1.0;
float c_db = 0.0;

#endif

