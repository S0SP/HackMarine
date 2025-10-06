/*************************************************************
   Define Blynk settings
*************************************************************/
#define BLYNK_TEMPLATE_ID "TMPL347rOm1yU"    // Your Template ID
#define BLYNK_TEMPLATE_NAME "gasplusldr"     // Your Template Name
#define BLYNK_AUTH_TOKEN "U0RJrf4qjZ3nCjRxjH_qXMQeVKAavnuB"   // Your Device Auth Token

// Enable Blynk debug prints (comment out to disable)
#define BLYNK_PRINT Serial

/*************************************************************/

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// WiFi credentials
char ssid[] = "marine";
char pass[] = "marineHack";

// Sensor pins (use ADC pins on ESP32)
#define GAS_SENSOR 34   // MQ135 -> GPIO34
#define LDR_SENSOR 35   // LDR -> GPIO35

// Threshold values for detection
#define GAS_THRESHOLD 1000    // Adjust based on your sensor calibration
#define LDR_THRESHOLD 2000    // Adjust based on your LDR setup (lower = darker)

BlynkTimer timer;

// Variables for connection monitoring
bool wifiConnected = false;
bool blynkConnected = false;
unsigned long lastConnectionCheck = 0;
const unsigned long CONNECTION_CHECK_INTERVAL = 5000; // Check every 5 seconds

// Function to check WiFi connection
void checkWiFiConnection() {
  if (WiFi.status() == WL_CONNECTED) {
    if (!wifiConnected) {
      Serial.println("✓ WiFi Connected!");
      Serial.print("IP Address: ");
      Serial.println(WiFi.localIP());
      wifiConnected = true;
    }
  } else {
    if (wifiConnected) {
      Serial.println("✗ WiFi Disconnected!");
      wifiConnected = false;
      blynkConnected = false;
    }
  }
}

// Function to check Blynk connection
void checkBlynkConnection() {
  if (Blynk.connected()) {
    if (!blynkConnected) {
      Serial.println("✓ Blynk Connected!");
      blynkConnected = true;
    }
  } else {
    if (blynkConnected) {
      Serial.println("✗ Blynk Disconnected!");
      blynkConnected = false;
    }
  }
}

// Function to analyze sensor readings
void analyzeSensorData(int gasValue, int ldrValue) {
  Serial.println("==========================================");
  Serial.println("         SENSOR READINGS");
  Serial.println("==========================================");
  
  // Gas Sensor Analysis
  Serial.print("Gas Sensor Value: ");
  Serial.println(gasValue);
  if (gasValue > GAS_THRESHOLD) {
    Serial.println("🚨 GAS DETECTED! - Air quality poor");
  } else {
    Serial.println("✓ Air quality normal");
  }
  
  // LDR Analysis
  Serial.print("Light Sensor Value: ");
  Serial.println(ldrValue);
  if (ldrValue < LDR_THRESHOLD) {
    Serial.println("🌙 LIGHT BLOCKED - Dark environment");
  } else {
    Serial.println("☀️ Light detected - Bright environment");
  }
  
  // Connection Status
  Serial.println("------------------------------------------");
  Serial.print("WiFi Status: ");
  Serial.println(wifiConnected ? "Connected" : "Disconnected");
  Serial.print("Blynk Status: ");
  Serial.println(blynkConnected ? "Connected" : "Disconnected");
  Serial.println("==========================================\n");
}

// Function to send sensor data to Blynk
void sendSensorData() {
  int gasValue = analogRead(GAS_SENSOR);
  int ldrValue = analogRead(LDR_SENSOR);
  
  // Always show data on serial monitor (offline fallback)
  analyzeSensorData(gasValue, ldrValue);
  
  // Try to send to Blynk if connected
  if (blynkConnected) {
    Serial.println("📤 Sending data to Blynk...");
    
    // Send to Blynk Virtual Pins
    Blynk.virtualWrite(V1, gasValue);  // Gas Sensor
    Blynk.virtualWrite(V2, ldrValue);  // LDR
    
    // Send status flags for easier monitoring in Blynk app
    Blynk.virtualWrite(V3, gasValue > GAS_THRESHOLD ? 1 : 0);  // Gas Alert
    Blynk.virtualWrite(V4, ldrValue < LDR_THRESHOLD ? 1 : 0);  // Light Blocked Alert
    
    Serial.println("✓ Data sent to Blynk successfully!");
  } else {
    Serial.println("⚠️ Blynk not connected - Running in offline mode");
    Serial.println("💾 Data logged locally only");
  }
}

// Function to attempt reconnection
void attemptReconnection() {
  if (!wifiConnected && WiFi.status() != WL_CONNECTED) {
    Serial.println("🔄 Attempting WiFi reconnection...");
    WiFi.disconnect();
    WiFi.begin(ssid, pass);
    delay(1000);
  }
  
  if (wifiConnected && !blynkConnected) {
    Serial.println("🔄 Attempting Blynk reconnection...");
    Blynk.connect();
  }
}

// Blynk connection event handlers
BLYNK_CONNECTED() {
  Serial.println("✅ Blynk connection established!");
  blynkConnected = true;
}

BLYNK_DISCONNECTED() {
  Serial.println("❌ Blynk connection lost!");
  blynkConnected = false;
}

void setup() {
  Serial.begin(115200);
  Serial.println("\n==========================================");
  Serial.println("    ESP32 Gas & Light Sensor Monitor");
  Serial.println("         with Blynk Integration");
  Serial.println("==========================================\n");
  
  // Initialize WiFi connection
  Serial.print("🔌 Connecting to WiFi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  
  // Wait for WiFi connection with timeout
  int wifiTimeout = 0;
  while (WiFi.status() != WL_CONNECTED && wifiTimeout < 20) {
    delay(500);
    Serial.print(".");
    wifiTimeout++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    wifiConnected = true;
    Serial.println("\n✓ WiFi Connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    
    // Initialize Blynk connection
    Serial.println("🔗 Connecting to Blynk...");
    Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  } else {
    Serial.println("\n✗ WiFi Connection Failed!");
    Serial.println("⚠️ Running in offline mode - Serial monitor only");
  }
  
  // Configure sensor pins
  pinMode(GAS_SENSOR, INPUT);
  pinMode(LDR_SENSOR, INPUT);
  
  Serial.println("\n📊 Sensor Configuration:");
  Serial.println("Gas Sensor: GPIO34 (MQ135)");
  Serial.println("Light Sensor: GPIO35 (LDR)");
  Serial.print("Gas Alert Threshold: ");
  Serial.println(GAS_THRESHOLD);
  Serial.print("Light Block Threshold: ");
  Serial.println(LDR_THRESHOLD);
  Serial.println("\n🚀 System Ready! Starting sensor monitoring...\n");
  
  // Send sensor data every 3 seconds (increased interval for better stability)
  timer.setInterval(3000L, sendSensorData);
  
  // Check connections every 5 seconds
  timer.setInterval(CONNECTION_CHECK_INTERVAL, checkBlynkConnection);
  timer.setInterval(CONNECTION_CHECK_INTERVAL, checkWiFiConnection);
  
  // Attempt reconnection every 30 seconds if disconnected
  timer.setInterval(30000L, attemptReconnection);
}

void loop() {
  // Run Blynk only if WiFi is connected
  if (wifiConnected) {
    Blynk.run();
  }
  
  timer.run();
  
  // Handle WiFi disconnection
  if (WiFi.status() != WL_CONNECTED && wifiConnected) {
    wifiConnected = false;
    blynkConnected = false;
    Serial.println("⚠️ WiFi connection lost! Switching to offline mode...");
  }
}