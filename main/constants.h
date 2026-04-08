
#ifndef CONFIG_H
#define CONFIG_H

// ================== NODE SELECTION ==================
// Enable ONLY ONE node at a time

// #define node_1
// #define node_2
// #define node_3
// #define node_4
// #define node_5
// #define node_6
// #define node_7
#define node_8


// ================== NODE 1 ==================
#ifdef node_1

const char* ssid = "KIET_ROUTE_!";
const char* password = "DFGUG&^@#*F";

const char* ctopApi = "https://ctop.iiit.ac.in/api/nodes/create-cin/20";
const char* authToken = "a64c0e30b6ad0069f1ba920171b6ca1c";

float m_pm2 = 0.0007842824264254494;
float c_pm2 = 0.007693018993987527;
float m_pm10 = 0.0006857976315845867;
float c_pm10 = 0.009788900400601618;

float m_db = 1.0;
float c_db = 0.0;

#endif


// ================== NODE 2 ==================
#ifdef node_2

const char* ssid = "Your_WIFI";
const char* password = "PASSWORD";

const char* ctopApi = "https://ctop.iiit.ac.in/api/nodes/create-cin/16";
const char* authToken = "your_token_node_2";

float m_pm2 = 1.0;
float c_pm2 = 0.0;
float m_pm10 = 1.0;
float c_pm10 = 0.0;

float m_db = 1.0;
float c_db = 0.0;

#endif


// ================== NODE 3 ==================
#ifdef node_3

const char* ssid = "Your_WIFI";
const char* password = "PASSWORD";

const char* ctopApi = "https://ctop.iiit.ac.in/api/nodes/create-cin/17";
const char* authToken = "your_token_node_3";

float m_pm2 = 1.0;
float c_pm2 = 0.0;
float m_pm10 = 1.0;
float c_pm10 = 0.0;

float m_db = 1.0;
float c_db = 0.0;

#endif


// ================== NODE 4 ==================
#ifdef node_4

const char* ssid = "Your_WIFI";
const char* password = "PASSWORD";

const char* ctopApi = "https://ctop.iiit.ac.in/api/nodes/create-cin/18";
const char* authToken = "your_token_node_4";

float m_pm2 = 1.0;
float c_pm2 = 0.0;
float m_pm10 = 1.0;
float c_pm10 = 0.0;

float m_db = 1.0;
float c_db = 0.0;

#endif


// ================== NODE 5 ==================
#ifdef node_5

const char* ssid = "Your_WIFI";
const char* password = "PASSWORD";

const char* ctopApi = "https://ctop.iiit.ac.in/api/nodes/create-cin/19";
const char* authToken = "your_token_node_5";

float m_pm2 = 1.0;
float c_pm2 = 0.0;
float m_pm10 = 1.0;
float c_pm10 = 0.0;

float m_db = 1.0;
float c_db = 0.0;

#endif


// ================== NODE 6 ==================
#ifdef node_6

const char* ssid = "Your_WIFI";
const char* password = "PASSWORD";

const char* ctopApi = "https://ctop.iiit.ac.in/api/nodes/create-cin/385";
const char* authToken = "your_token_node_6";

float m_pm2 = 1.0;
float c_pm2 = 0.0;
float m_pm10 = 1.0;
float c_pm10 = 0.0;

float m_db = 1.0;
float c_db = 0.0;

#endif


// ================== NODE 7 ==================
#ifdef node_7

const char* ssid = "Your_WIFI";
const char* password = "PASSWORD";

const char* ctopApi = "https://ctop.iiit.ac.in/api/nodes/create-cin/386";
const char* authToken = "your_token_node_7";

float m_pm2 = 1.0;
float c_pm2 = 0.0;
float m_pm10 = 1.0;
float c_pm10 = 0.0;

float m_db = 1.0;
float c_db = 0.0;

#endif


// ================== NODE 8 ==================
#ifdef node_8

const char* ssid = "CLASSKG9";
const char* password = "kw@12345";

const char* ctopApi = "https://ctop.iiit.ac.in/api/nodes/create-cin/387";
const char* authToken = "bfb502d7b83786a95e960e7c7cd4cf70";

float m_pm2 = 1.0;
float c_pm2 = 0.0;
float m_pm10 = 1.0;
float c_pm10 = 0.0;

float m_db = 1.0;
float c_db = 0.0;

#endif


#endif // CONFIG_H
