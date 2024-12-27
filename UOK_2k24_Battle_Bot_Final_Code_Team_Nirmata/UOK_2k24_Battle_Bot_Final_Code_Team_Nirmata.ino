#include <WiFi.h>
#include <WebServer.h>

// Replace these with your network credentials
const char* ssid = "Wi-Fi Name";
const char* password = "Wi-Fi Password";

// Motor control pins
//Motor 1

const int motor1Pin1 = 12; // Motor 1 Forward
const int motor1Pin2 = 14; // Motor 1 Backward

//Motor 2
const int motor2Pin1 = 27; // Motor 2 Forward
const int motor2Pin2 = 26; // Motor 2 Backward

//Motor 3
const int motor3Pin1 = 5; // Motor 3 Forward
const int motor3Pin2 = 18; // Motor 3 Backward

//Motor 4
const int motor4Pin1 = 19; // Motor 4 Forward
const int motor4Pin2 = 21; // Motor 4 Backward

WebServer server(80);

// Function to move the car forward
void moveForward() {
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW);
    digitalWrite(motor3Pin1, HIGH);
    digitalWrite(motor3Pin2, LOW);
    digitalWrite(motor4Pin1, HIGH);
    digitalWrite(motor4Pin2, LOW);
}

// Function to move the car backward
void moveBackward() {
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, HIGH);
    digitalWrite(motor3Pin1, LOW);
    digitalWrite(motor3Pin2, HIGH);
    digitalWrite(motor4Pin1, LOW);
    digitalWrite(motor4Pin2, HIGH);
}

// Function to stop the car
void stopCar() {
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, LOW);
    digitalWrite(motor3Pin1, LOW);
    digitalWrite(motor3Pin2, LOW);
    digitalWrite(motor4Pin1, LOW);
    digitalWrite(motor4Pin2, LOW);
}

// Function to turn left
void turnLeft() {
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH);
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW);    
    digitalWrite(motor3Pin1, LOW);
    digitalWrite(motor3Pin2, HIGH);
    digitalWrite(motor4Pin1, HIGH);
    digitalWrite(motor4Pin2, LOW);
}

// Function to turn right
void turnRight() {
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, HIGH);
    digitalWrite(motor3Pin1, HIGH);
    digitalWrite(motor3Pin2, LOW);
    digitalWrite(motor4Pin1, LOW);
    digitalWrite(motor4Pin2, HIGH);
}

String webPage = "<html>\
                  <head>\
                    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\
                    <style>\
                      body {\
                        text-align: center; \
                        font-family: Arial, sans-serif; \
                        margin: 0; \
                        padding: 0; \
                        background-color: #f4f4f4; \
                      }\
                      h1 { color: #333; margin-bottom: 20px; }\
                      .button-container {\
                        display: flex; \
                        flex-direction: column; \
                        align-items: center; \
                      }\
                      .row {\
                        display: flex; \
                        justify-content: center; \
                        margin: 10px 0; \
                      }\
                      button {\
                        width: 100px; \
                        height: 100px; \
                        font-size: 40px; \
                        margin: 5px; \
                        background-color: #4CAF50; \
                        color: white; \
                        border: none; \
                        border-radius: 5px; \
                        cursor: pointer; \
                      }\
                      button:active { background-color: #45a049; }\
                      @media (max-width: 600px) {\
                        button {\
                          width: 120px; \
                          height: 120px; \
                          font-size: 50px; \
                        }\
                      }\
                    </style>\
                    <script>\
                      function sendCommand(command) {\
                        var xhr = new XMLHttpRequest();\
                        xhr.open('GET', command, true);\
                        xhr.send();\
                      }\
                      function holdCommand(command) {\
                        sendCommand(command);\
                        this.interval = setInterval(function() { sendCommand(command); }, 200);\
                      }\
                      function releaseCommand() {\
                        clearInterval(this.interval);\
                        sendCommand('/stop');\
                      }\
                    </script>\
                  </head>\
                  <body>\
                    <h1>Car Control</h1>\
                    <div class=\"button-container\">\
                      <div class=\"row\">\
                        <button ontouchstart=\"holdCommand('/forward')\" ontouchend=\"releaseCommand()\">&#9650;</button>\
                      </div>\
                      <div class=\"row\">\
                        <button ontouchstart=\"holdCommand('/left')\" ontouchend=\"releaseCommand()\">&#9664;</button>\
                        <button ontouchstart=\"holdCommand('/stop')\" ontouchend=\"releaseCommand()\">&#10073;&#10073;</button>\
                        <button ontouchstart=\"holdCommand('/right')\" ontouchend=\"releaseCommand()\">&#9654;</button>\
                      </div>\
                      <div class=\"row\">\
                        <button ontouchstart=\"holdCommand('/backward')\" ontouchend=\"releaseCommand()\">&#9660;</button>\
                      </div>\
                    </div>\
                  </body>\
                </html>";

// Handles the commands
void handleRoot() {
  server.send(200, "text/html", webPage);
}

void handleForward() {
  moveForward();
  server.send(200, "text/html", webPage);
}

void handleBackward() {
  moveBackward();
  server.send(200, "text/html", webPage);
}

void handleLeft() {
  turnLeft();
  server.send(200, "text/html", webPage);
}

void handleRight() {
  turnRight();
  server.send(200, "text/html", webPage);
}

void handleStop() {
  stopCar();
  server.send(200, "text/html", webPage);
}

void setup() {
  Serial.begin(115200);

  // Initialize motor control pins
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  pinMode(motor3Pin1, OUTPUT);
  pinMode(motor3Pin2, OUTPUT);
  pinMode(motor4Pin1, OUTPUT);
  pinMode(motor4Pin2, OUTPUT);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());


  // Start the web server
  server.on("/", handleRoot);
  server.on("/forward", handleForward);
  server.on("/backward", handleBackward);
  server.on("/left", handleLeft);
  server.on("/right", handleRight);
  server.on("/stop", handleStop);

  server.begin();
  Serial.println("Web server started");
}

void loop() {
  server.handleClient();
}