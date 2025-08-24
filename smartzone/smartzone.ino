#define BLYNK_TEMPLATE_ID "your_template_id7"
#define BLYNK_TEMPLATE_NAME "your-template-name"
#define BLYNK_AUTH_TOKEN "your_auth_token"

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

// WiFi credentials
#define WIFI_SSID "your-wifi-namet"
#define WIFI_PASSWORD "your-wifi-password"
#define WIFI_TIMEOUT_MS 10000 // 10 seconds connection timeout

// LCD setup
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pin definitions
#define sensorA D5
#define sensorB D6
#define relayZone1 D0
#define relayZone2 D7
#define relayZone3 D3
#define relayZone4 D8  
#define DHTPIN D4       // DHT11 on D4

// DHT Sensor Setup
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Variables
int peopleCount = 0;
bool sensorA_triggered = false;
bool sensorB_triggered = false;

// Mode Control
bool isAutoMode = true;
bool manualZone1 = false;
bool manualZone2 = false;
bool manualZone3 = false;
bool manualZone4 = false;

// Blynk State Tracking
bool blynkLedZone1 = false;
bool blynkLedZone2 = false;
bool blynkLedZone3 = false;
bool blynkLedZone4 = false;
bool lastSwitchStateZone1 = false;
bool lastSwitchStateZone2 = false;
bool lastSwitchStateZone3 = false;
bool lastSwitchStateZone4 = false;

// Data transmission optimization
unsigned long lastPeopleCountUpdate = 0;
const unsigned long PEOPLE_COUNT_UPDATE_INTERVAL = 10000; // 10 seconds
float lastSentTemp = -100;
float lastSentHumidity = -100;
const float DHT_CHANGE_THRESHOLD = 2; // Only send if change > 2°C or 2%

// WiFi Management
bool wifiConnected = false;
unsigned long lastWifiAttempt = 0;
const unsigned long WIFI_RETRY_INTERVAL = 30000; // 30 seconds between retries

BlynkTimer timer;

void setup() {
  Serial.begin(115200);
  Serial.println("System starting...");

  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Booting...");

  // Pin modes
  pinMode(sensorA, INPUT);
  pinMode(sensorB, INPUT);
  pinMode(relayZone1, OUTPUT);
  pinMode(relayZone2, OUTPUT);
  pinMode(relayZone3, OUTPUT);
  pinMode(relayZone4, OUTPUT);
  
  // Initialize DHT
  pinMode(DHTPIN, INPUT_PULLUP);
  dht.begin();

  // Initial relay states (OFF - active LOW)
  digitalWrite(relayZone1, HIGH);
  digitalWrite(relayZone2, HIGH);
  digitalWrite(relayZone3, HIGH);
  digitalWrite(relayZone4, HIGH);

  // Connect to WiFi
  connectToWiFi();

  // Timers
  timer.setInterval(5000L, checkDHTSensorData);
  timer.setInterval(1000L, checkBlynkUpdates);
}

void connectToWiFi() {
  Serial.println("Connecting to WiFi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  unsigned long startTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startTime < WIFI_TIMEOUT_MS) {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    wifiConnected = true;
    Serial.println("\nWiFi connected!");
    Blynk.config(BLYNK_AUTH_TOKEN);
    if (Blynk.connect()) {
      Serial.println("Blynk connected");
      // Sync initial states
      Blynk.virtualWrite(V1, blynkLedZone1);
      Blynk.virtualWrite(V2, blynkLedZone2);
      Blynk.virtualWrite(V3, blynkLedZone3);
      Blynk.virtualWrite(V4, blynkLedZone4);
    }
    lcd.setCursor(0, 0);
    lcd.print("Welcome to");
    lcd.setCursor(0,1);
    lcd.print("SMARTZONE");
  } else {
    wifiConnected = false;
    Serial.println("\nWiFi connection failed - working locally");
    lcd.setCursor(0, 0);
    lcd.print("Local Mode:    ");
    lcd.setCursor(0,1);
    lcd.print("SMARTZONE");
  }
  lastWifiAttempt = millis();
}

void checkDHTSensorData() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
  if (!isnan(h) && !isnan(t)) {
    if (wifiConnected && Blynk.connected()) {
      if (abs(t - lastSentTemp) > DHT_CHANGE_THRESHOLD || lastSentTemp == -100) {
        Blynk.virtualWrite(V10, t);
        lastSentTemp = t;
      }
      if (abs(h - lastSentHumidity) > DHT_CHANGE_THRESHOLD || lastSentHumidity == -100) {
        Blynk.virtualWrite(V11, h);
        lastSentHumidity = h;
      }
    }
  }
}

void readSensors() {
  bool A = digitalRead(sensorA) == LOW;
  bool B = digitalRead(sensorB) == LOW;

  if (A && !sensorA_triggered) sensorA_triggered = true;
  if (B && !sensorB_triggered) sensorB_triggered = true;

  if (sensorA_triggered && sensorB_triggered) {
    if (A) peopleCount++;
    else if (B) peopleCount = (peopleCount > 0) ? peopleCount - 1 : 0;
    
    delay(500); // Debounce
    sensorA_triggered = false;
    sensorB_triggered = false;
    updateLCD();
  }
}

void updateLCD() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("People: ");
  lcd.print(peopleCount);
  lcd.print(" ");
  lcd.print(isAutoMode ? "Auto" : "Man");
  
  // Show active zones on second line
  lcd.setCursor(0, 1);
  if (digitalRead(relayZone1) == LOW) lcd.print("Z1 ");
  if (digitalRead(relayZone2) == LOW) lcd.print("Z2 ");
  if (digitalRead(relayZone3) == LOW) lcd.print("Z3 ");
  if (digitalRead(relayZone4) == LOW) lcd.print("Z4 ");
}

void controlRelays() {
  int zone1State = (isAutoMode) ? (peopleCount >= 1) ? LOW : HIGH : manualZone1 ? LOW : HIGH;
  int zone2State = (isAutoMode) ? (peopleCount >= 10) ? LOW : HIGH : manualZone2 ? LOW : HIGH;
  int zone3State = (isAutoMode) ? (peopleCount >= 20) ? LOW : HIGH : manualZone3 ? LOW : HIGH;
  int zone4State = (isAutoMode) ? (peopleCount >= 25) ? LOW : HIGH : manualZone4 ? LOW : HIGH;

  updateRelay(relayZone1, zone1State, blynkLedZone1, V1);
  updateRelay(relayZone2, zone2State, blynkLedZone2, V2);
  updateRelay(relayZone3, zone3State, blynkLedZone3, V3);
  updateRelay(relayZone4, zone4State, blynkLedZone4, V4);
}

void updateRelay(int relayPin, int newState, bool &ledState, int virtualPin) {
  if (digitalRead(relayPin) != newState) {
    digitalWrite(relayPin, newState);
    bool newLedState = (newState == LOW);
    if (ledState != newLedState && wifiConnected && Blynk.connected()) {
      ledState = newLedState;
      Blynk.virtualWrite(virtualPin, ledState);
    }
  }
}

void checkBlynkUpdates() {
  static int lastPeopleCount = -1;
  static unsigned long lastUpdateTime = 0;
  
  if (peopleCount != lastPeopleCount && 
      (millis() - lastPeopleCountUpdate >= PEOPLE_COUNT_UPDATE_INTERVAL || 
       peopleCount == 0)) {
    if (wifiConnected && Blynk.connected()) {
      Blynk.virtualWrite(V0, peopleCount);
    }
    lastPeopleCount = peopleCount;
    lastPeopleCountUpdate = millis();
  }
  
  if (millis() - lastUpdateTime >= 1000) {
    updateLCD();
    lastUpdateTime = millis();
  }
}

// Blynk button handlers
BLYNK_WRITE(V5) { 
  bool newState = param.asInt();
  if (newState != lastSwitchStateZone1) {
    manualZone1 = newState;
    lastSwitchStateZone1 = newState;
    if (!isAutoMode) controlRelays();
  }
}

BLYNK_WRITE(V6) { 
  bool newState = param.asInt();
  if (newState != lastSwitchStateZone2) {
    manualZone2 = newState;
    lastSwitchStateZone2 = newState;
    if (!isAutoMode) controlRelays();
  }
}

BLYNK_WRITE(V7) { 
  bool newState = param.asInt();
  if (newState != lastSwitchStateZone3) {
    manualZone3 = newState;
    lastSwitchStateZone3 = newState;
    if (!isAutoMode) controlRelays();
  }
}

BLYNK_WRITE(V9) { 
  bool newState = param.asInt();
  if (newState != lastSwitchStateZone4) {
    manualZone4 = newState;
    lastSwitchStateZone4 = newState;
    if (!isAutoMode) controlRelays();
  }
}

BLYNK_WRITE(V8) { 
  bool newState = param.asInt();
  if (newState != isAutoMode) {
    isAutoMode = newState;
    updateLCD();
  }
}

void loop() {
  // Handle WiFi reconnection
  if (!wifiConnected && millis() - lastWifiAttempt > WIFI_RETRY_INTERVAL) {
    connectToWiFi();
  }

  // Run Blynk if connected
  if (wifiConnected) {
    Blynk.run();
  }
  
  timer.run();

  if (isAutoMode) readSensors();
  controlRelays();
}