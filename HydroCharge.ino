#include <WiFi.h>
#include <WebServer.h>

// --- WiFi Configuration ---
const char* ssid = "HydroCharge_Vendo";
const char* password = "hydrocharge"; 

WebServer server(80);


const int SENSOR_PIN = 14; // D5 on NodeMCU  
const int RELAY_MOTOR = 5; // D1 on NodeMCU
const int RELAY_QC3 = 4; // D2 on NodeMCU


int bottleCount = 0;
unsigned long chargingEndTime = 0;
const unsigned long BOTTLE_REWARD = 120000; // 2 minutes in milliseconds
const unsigned long MOTOR_SPIN_TIME = 3000; 

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
  
  // Pin Modes
  pinMode(SENSOR_PIN, INPUT_PULLUP); 
  pinMode(RELAY_MOTOR, OUTPUT);
  pinMode(RELAY_QC3, OUTPUT);

  // Initialize Relays to OFF
  digitalWrite(RELAY_MOTOR, LOW);
  digitalWrite(RELAY_QC3, LOW);

  // Start Access Point
  WiFi.softAP(ssid, password);
  server.on("/", handleRoot);
  server.begin();
  
  Serial.println("HydroCharge System Online - LED Pins Disabled");
}

void loop() {
  server.handleClient();
  unsigned long currentTime = millis();


  if (digitalRead(SENSOR_PIN) == LOW) {
    
 
    delay(150); 
    if (digitalRead(SENSOR_PIN) == LOW) {
      bottleCount++;
      Serial.println("Bottle Detected! Starting Motor...");
      

      digitalWrite(RELAY_MOTOR, HIGH);  
      delay(MOTOR_SPIN_TIME);           
      digitalWrite(RELAY_MOTOR, LOW);   

      if (currentTime < chargingEndTime) {
        chargingEndTime += BOTTLE_REWARD;
      } else {
        chargingEndTime = currentTime + BOTTLE_REWARD;
      }
    
      while(digitalRead(SENSOR_PIN) == LOW) {
        yield(); 
        delay(10);
      }
      delay(500); 
    }
  }

 
  if (currentTime < chargingEndTime) {
    digitalWrite(RELAY_QC3, HIGH); // Enable USB Port
  } else {
    digitalWrite(RELAY_QC3, LOW);  // Disable USB Port
  }
}