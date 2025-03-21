/*
 * Created by ArduinoGetStarted.com
 *
 * This example code is in the public domain
 *
 * Tutorial page: https://arduinogetstarted.com/tutorials/arduino-http-request
 */
#include <WiFiS3.h>
#include <A4990MotorShield.h>

A4990MotorShield motors;

const char ssid[] = "IU-Study";     // change your network SSID (name)
const char pass[] = "studieren_an_der_IU";  // change your network password (use for WPA, or use as key for WEP)

WiFiClient client;
int status = WL_IDLE_STATUS;

int HTTP_PORT = 80;
String HTTP_METHOD = "GET";  // or POST
char PROXY_HOST[] = "cors-anywhere.herokuapp.com";
String PROXY_PATH = "/https://api.jsonbin.io/v3/b/6662fa7de41b4d34e4ffd938";

String MASTER_KEY = "$2a$10$cVDLXTLgbT6IpIZ0.BIzjOEn1DB8asJ5DJPvN50x4yUaqqLVbbA9i";
String ACCESS_KEY = "$2a$10$qi7.YqseSvPSXnvmrcZEt.MTvqhYL8UQc4bYLffBfPtwIJ6x8dE66";

void connectToWiFi() {
  // Attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // Wait 10 seconds for connection:
    delay(4000);
  }

  // Print your board's IP address:
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void sendHttpRequest() {
  // Connect to web server on port 80:
  if (client.connect(PROXY_HOST, HTTP_PORT)) {
    // If connected:
    Serial.println("Connected to proxy server");

    // Make a HTTP request:
    // Send HTTP header
    client.println(HTTP_METHOD + " " + PROXY_PATH + " HTTP/1.1");
    client.println("Host: " + String(PROXY_HOST));
    client.println("Origin: http://localhost");
    client.println("Connection: close");
    client.println("X-Master-Key: " + MASTER_KEY);
    client.println("X-Access-Key: " + ACCESS_KEY);
    client.println();  // End HTTP header
    
    // String to hold the server response
    String answer = "";

    // Read response from the server and store it in the answer string
    while (client.connected()) {
      if (client.available()) {
        // Read an incoming byte from the server and print it to serial monitor:
        char c = client.read();
        // Append the character to the answer string
        answer += c;
      }
    }

    // Print the server response
    Serial.println("Server Response:");
    Serial.println(answer);

    // Check if the response contains "drive":"backwards"
    if (answer.indexOf("\"drive\":\"backwards\"") != -1) {
      Serial.println("The response contains \"drive\":\"backwards\"");
      motors.setSpeeds(0,0);
      motors.setM1Speed(-1000);
      motors.setM2Speed(-1000);
      delay(1000); // Run motors for 1 seconds
      motors.setSpeeds(0,0);
    } else {
      Serial.println("The response does not contain \"drive\":\"backwards\"");
      motors.setM1Speed(1000);
      motors.setM2Speed(1000);
      delay(1000); // Run motors for 1 seconds
      motors.setSpeeds(0,0);
    }

    // The server's disconnected, stop the client:
    client.stop();
    Serial.println();
    Serial.println("disconnected");
  } else {  // If not connected:
    Serial.println("Connection to proxy failed");
  }
}

void setup() {
  //Serial.begin(9600);
  Serial.begin(115200);
  // Check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // Don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  connectToWiFi();  // Connect to WiFi network
}

void loop() {
  sendHttpRequest();  // Send HTTP request
  delay(10000);       // Wait for 10 seconds before repeating
}
