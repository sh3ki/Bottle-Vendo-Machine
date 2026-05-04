#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// --- WiFi Configuration ---
const char* ssid = "HydroCharge_Vendo";
const char* password = "hydrocharge"; 

ESP8266WebServer server(80);


const int ULTRASONIC_TRIG_PIN = D8;  // GPIO15
const int ULTRASONIC_ECHO_PIN = D6;
const int RELAY_MOTOR = D5;         // GPIO14 - motor relay (avoid D0/GPIO16)
const int RELAY_QC3 = D7;           // GPIO13 - USB relay
const int RELAY_ON = HIGH;
const int RELAY_OFF = LOW;


int bottleCount = 0;
unsigned long chargingEndTime = 0;
 
// Track relay states to avoid spamming Serial and to debug activation
bool motorState = false;
bool qc3State = false;

const unsigned long BOTTLE_REWARD = 120000; // 2 minutes in milliseconds
const unsigned long MOTOR_SPIN_TIME = 3000; 
const float DETECTION_THRESHOLD_MIN = 45.0;  // Detect < 45cm or > 80cm
const float DETECTION_THRESHOLD_MAX = 80.0;

float readDistanceCm() {
  // Improved trigger timing and retry logic with diagnostics
  for (int attempt = 0; attempt < 3; ++attempt) {
    int echoState = digitalRead(ULTRASONIC_ECHO_PIN);
    Serial.print("ECHO before trigger: "); Serial.println(echoState);

    // If ECHO is already HIGH before sending a trigger, the line is stuck
    // (usually caused by improper level-shifting or wiring). Avoid calling
    // pulseIn in this case to prevent repeated timeouts.
    if (echoState == HIGH) {
      Serial.println("ECHO stuck HIGH before trigger - check wiring/level-shifter/common GND");
      return -1.0;
    }

    Serial.print("TRIG before HIGH: "); Serial.println(digitalRead(ULTRASONIC_TRIG_PIN));
    digitalWrite(ULTRASONIC_TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(ULTRASONIC_TRIG_PIN, HIGH);
    Serial.println(">>> TRIG pulse sent (HIGH)");
    delayMicroseconds(10);
    digitalWrite(ULTRASONIC_TRIG_PIN, LOW);
    Serial.println(">>> TRIG pulse end (LOW)");

    Serial.print("TRIG after LOW: "); Serial.println(digitalRead(ULTRASONIC_TRIG_PIN));

    // HC-SR04 returns a single HIGH pulse whose width encodes distance.
    unsigned long duration = pulseIn(ULTRASONIC_ECHO_PIN, HIGH, 50000UL); // 50ms timeout for HIGH pulse

    if (duration == 0) {
      Serial.print("pulseIn timeout (attempt "); Serial.print(attempt + 1); Serial.println(")");
      delay(20); // short pause before retry
      continue;
    }

    float distance = duration * 0.0343 / 2.0;
    Serial.print("pulseIn duration: "); Serial.print(duration); Serial.print(" us -> ");
    Serial.print(distance, 2); Serial.println(" cm");
    return distance;
  }

  return -1.0; // all attempts failed
}

void handleRoot() {
  unsigned long now = millis();
  long timeLeft = (chargingEndTime > now) ? (chargingEndTime - now) / 1000 : 0;
  
  String html = "<html><head><meta http-equiv='refresh' content='2' name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<style>body{font-family:sans-serif; text-align:center; background:#121212; color:white;} ";
  html += ".card{background:#1e1e1e; padding:30px; border-radius:15px; display:inline-block; margin-top:50px; border: 2px solid #3498db;}";
  html += ".timer{font-size: 48px; font-weight: bold; color: #3498db;}</style></head><body>";
  html += "<div class='card'><h1>HydroCharge Dashboard</h1>";
  html += "<h2>Bottles: <span style='color:#2ecc71;'>" + String(bottleCount) + "</span></h2>";
  html += "<div class='timer'>" + String(timeLeft) + "s</div>";
  html += "<p>Status: " + String(timeLeft > 0 ? "CHARGING" : "IDLE") + "</p></div></body></html>";
  
  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200);
  delay(500);
  
  // Pin Modes
  pinMode(ULTRASONIC_TRIG_PIN, OUTPUT);
  pinMode(ULTRASONIC_ECHO_PIN, INPUT);
  pinMode(RELAY_MOTOR, OUTPUT);
  pinMode(RELAY_QC3, OUTPUT);

  // Initialize Relays to OFF
  digitalWrite(RELAY_MOTOR, RELAY_OFF);
  digitalWrite(RELAY_QC3, RELAY_OFF);
  digitalWrite(ULTRASONIC_TRIG_PIN, LOW);

  // Start Access Point
  WiFi.softAP(ssid, password);
  server.on("/", handleRoot);
  server.begin();
  
  Serial.println("HydroCharge System Online");
}

void loop() {
  server.handleClient();
  unsigned long currentTime = millis();

  float distanceCm = readDistanceCm();

  if (distanceCm > 0) {
    Serial.print("Ultrasonic distance: ");
    Serial.print(distanceCm, 1);
    Serial.println(" cm");
  } else {
    Serial.println("Ultrasonic distance: out of range");
  }

  if (distanceCm > 0 && (distanceCm < DETECTION_THRESHOLD_MIN || distanceCm > DETECTION_THRESHOLD_MAX)) {
    delay(150);

    distanceCm = readDistanceCm();
    if (distanceCm > 0 && (distanceCm < DETECTION_THRESHOLD_MIN || distanceCm > DETECTION_THRESHOLD_MAX)) {
      bottleCount++;
      Serial.print("Bottle detected at ");
      Serial.print(distanceCm, 1);
      Serial.println(" cm. Motor ON for 3 seconds");

      digitalWrite(RELAY_MOTOR, RELAY_ON);
      if (!motorState) {
        Serial.println("RELAY_MOTOR ON");
        motorState = true;
      }
      unsigned long motorStartTime = millis();
      
      // Motor runs for exactly 3 seconds
      while (millis() - motorStartTime < MOTOR_SPIN_TIME) {
        yield();
        delay(50);
      }
      
      digitalWrite(RELAY_MOTOR, RELAY_OFF);
      if (motorState) {
        Serial.println("RELAY_MOTOR OFF");
        motorState = false;
      }
      Serial.println("Motor OFF");

      // Set charging to run for 2 minutes
      chargingEndTime = millis() + BOTTLE_REWARD;
      Serial.println("USB charging enabled for 2 minutes");

      delay(500);
    }
  }

 
  if (currentTime < chargingEndTime) {
    if (!qc3State) {
      digitalWrite(RELAY_QC3, RELAY_ON); // Enable USB Port
      qc3State = true;
      Serial.println("RELAY_QC3 ON - USB enabled");
    }
  } else {
    if (qc3State) {
      digitalWrite(RELAY_QC3, RELAY_OFF);  // Disable USB Port
      qc3State = false;
      Serial.println("RELAY_QC3 OFF - USB disabled");
    }
  }
}