#include <WiFiS3.h>
#include <A4990MotorShield.h>
#include <ArduinoJson.h>
#include <array>

A4990MotorShield motors;
#include <stdio.h>      /* printf */
#include <math.h>  
#define PI 3.14159265
#define DEFAULT_FORWARD_SPEED 150
#define DEFAULT_TURN_SPEED 200
#define SPEED_FACTOR 0
const char ssid[] = "IU-Study";     // change your network SSID (name)
const char pass[] = "studieren_an_der_IU";  // change your network password (use for WPA, or use as key for WEP)

WiFiClient client;
int status = WL_IDLE_STATUS;

int HTTP_PORT = 80;
String HTTP_METHOD = "GET";  // or POST
char PROXY_HOST[] = "cors-anywhere.herokuapp.com";
String PROXY_PATH = "/https://api.jsonbin.io/v3/b/666b25ece41b4d34e402d580";

String MASTER_KEY = "$2a$10$cVDLXTLgbT6IpIZ0.BIzjOEn1DB8asJ5DJPvN50x4yUaqqLVbbA9i";
String ACCESS_KEY = "$2a$10$OQgJlnn2vFFKzzlf9NnylOk4lJ2/z1ElhD4c9DFWYAinDr7hszxCa";

/*void turn(String side) {

  if (side = "left") {
   motors.setSpeeds(DEFAULT_TURN_SPEED,-DEFAULT_TURN_SPEED);
    delay(330);
    motors.setSpeeds(0,0);
  } else if (side == "right") {
    motors.setSpeeds(DEFAULT_TURN_SPEED,-DEFAULT_TURN_SPEED);
    delay(330);
    motors.setSpeeds(0,0);
  }
}*/
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

std::array<int, 4> sendHttpRequest(type = "robot") {
  std::array<int, 4> position = { -1, -1, -1, -1}; // Default invalid position

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

    // Manually extract the position string
    if(type == "object") {
      int posIndex = answer.indexOf("\"positionObject\":\"[");
    } else {
      int posIndex = answer.indexOf("\"positionZumo\":\"[");
    }
    
    if (posIndex != -1) {
      int startIndex = answer.indexOf("[", posIndex);
      int endIndex = answer.indexOf("]", startIndex);
      if (startIndex != -1 && endIndex != -1) {
        String positionArray = answer.substring(startIndex, endIndex + 1);
        Serial.print("Position array: ");
        Serial.println(positionArray);

        // Parse the position string as JSON
        DynamicJsonDocument positionDoc(256);  // Smaller buffer size for position array
        DeserializationError posError = deserializeJson(positionDoc, positionArray);

        if (posError) {
          Serial.print("deserializeJson() for position failed: ");
          Serial.println(posError.c_str());
        } else {
          JsonArray positionJson = positionDoc.as<JsonArray>();
          if (positionJson.size() == 4) {  // Ensure the array has exactly 2 elements
            position[0] = positionJson[0];
            position[1] = positionJson[1];
            position[3] = positionJson[3];
            position[4] = positionJson[4];
            Serial.print("X: ");
            Serial.println(position[0]);
            Serial.print("Y: ");
            Serial.println(position[1]);
          } else {
            Serial.println("Position array does not have exactly 4 elements");
          }
        }
      } else {
        Serial.println("No valid position array found");
      }
    } else {
      Serial.println("No position key found");
    }

    // The server's disconnected, stop the client:
    client.stop();
    Serial.println();
    Serial.println("disconnected");
  } else {  // If not connected:
    Serial.println("Connection to proxy failed");
  }

  return position;
}
void turn(int angle, int angleFactor, String side){

  //double angleDistance = angle - angleFactor;
  double time = angle / angleFactor;
  int time_int = static_cast<int>(time * 1000);

  
  if (side = "left") {
    motors.setSpeeds(-DEFAULT_TURN_SPEED,DEFAULT_TURN_SPEED);
  } else if (side == "right") {
    motors.setSpeeds(DEFAULT_TURN_SPEED,-DEFAULT_TURN_SPEED);
  }
  delay(time_int); 
  motors.setSpeeds(0,0);
}

void driveForward(double distance, double distanceFactor) {

  double time = distance / distanceFactor;
  int time_int = static_cast<int>(time * 1000.0);
  
  motors.setSpeeds(DEFAULT_FORWARD_SPEED,DEFAULT_FORWARD_SPEED);
  delay(time_int);
  motors.setSpeeds(0,0);
}

  double calibrate() {
  delay(10000);
  Serial.println("Starting to calibrate:");
  std::array<int, 4> pos = sendHttpRequest();
  Serial.print("Calibrated Position - X: ");
  int x1 = pos[0];
  int y1 = pos[1];
  Serial.println(x1);
  Serial.println(y1);
  //int x1 = 76;
  //int y1 = 207;
  //motors.setM1Speed(150);
  //motors.setM2Speed(150);
  motors.setSpeeds(DEFAULT_FORWARD_SPEED,DEFAULT_FORWARD_SPEED);
  delay(1000); // Run motors for 1 second
  motors.setSpeeds(0,0);
  delay(10000);


  Serial.println("Second request");
  std::array<int, 4> pos2 = sendHttpRequest();

  int x2 = pos2[0];
  int y2 = pos2[1];
  //int x2 = 540;
  //int y2 = 240;
  double ab = sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
  //int ab = x2 - x1;
  //double bc = ab;
  double einheitPerSecond = ab;
  SPEED_FACTOR = einheitPerSecond;
  Serial.println("Einheit Per Second");
  Serial.println(einheitPerSecond);

  Serial.println("Turns");
  //Turn
  //motors.setM1Speed(-200);
  //motors.setM2Speed(200);
  motors.setSpeeds(200,-200);
  delay(1000); // Run motors for 1 second
  motors.setSpeeds(0,0);
  delay(10000);

  Serial.println("Drives second time forward");
  //Drive forward again
  //motors.setM1Speed(150);
  //motors.setM2Speed(150);
  motors.setSpeeds(DEFAULT_FORWARD_SPEED,DEFAULT_FORWARD_SPEED);
  delay(1000); // Run motors for 1 second
  motors.setSpeeds(0,0);
  delay(10000);

  Serial.println("Third request - turns back");
  std::array<int, 4> pos3 = sendHttpRequest();

  int x3 = pos3[0];
  int y3 = pos3[1];
  
  //int x3 = 419;
  //int y3 = 574;
  double bc = sqrt((x3-x2)*(x3-x2) + (y3-y2)*(y3-y2));
  double ac = sqrt((x3-x1)*(x3-x1) + (y3-y1)*(y3-y1));
  Serial.println("this is ac");
  Serial.println(ac);
  //find angle 
  //double cosB = 1 - static_cast<double>(ac * ac) / (2 * ab * bc);
  //double cosB = (ab*ab + bc*bc - ac*ac) / (2* ab * bc);
  double cosB = (ac*ac + ab*ab - bc*bc) / (2* ac * ab);
  //double mc = ac/2;
  //double cosB = 360-cos(mc/bc)*2;
  
  Serial.println("this is cos"); 
  Serial.println(cosB);
  //double B = acos (cosB) * 180.0 / PI;
  double B = degrees(acos(cosB));
  Serial.println("Angle of B");
  Serial.println(B);

  return B;
}

void setup() {
  Serial.begin(115200);
  //Serial.begin(9600);
  connectToWiFi();
  double data = calibrate();
  Serial.println("Data from calibration");
  Serial.println(data);
  turn(180,data, left);
  //Start working and pushing
  std::array<int, 4> pos = sendHttpRequest();
  Serial.print("New Robot Position ");
  int x1 = pos[0];
  int y1 = pos[1];
  std::array<int, 4> obj = sendHttpRequest("object");
  int objX = obj[0];
  int objY = obj[1];
  int width = obj[3];

  double RobotObjectDistance = sqrt((objX-x1)*(objX-x1) + (objY-y1)*(objY-y1));
  double ROD = RobotObjectDistance;

  int helpY = y1;
  int helpX = ROD + x1;

  double RobotHelperDistance = sqrt((helpX-x1)*(helpX-x1) + (helpY-y1)*(helpY-y1));
  double RHD = RobotHelperDistance;

  double HelperObjectDistance = sqrt((objX-helpX)*(objX-helpX) + (objY-helpY)*(objY-helpY));
  double HOD = HelperObjectDistance;

  double cosRobot = (ROD*ROD + RHD*RHD - HOD*HOD) / (2* ROD * RHD); 
  
  Serial.println("this is cos"); 
  Serial.println(cosRobot);
  //double B = acos (cosB) * 180.0 / PI;
  double angleToObject = degrees(acos(cosRobot));

  turn(angleToObject, data, right);

  driveForward((ROD/100)*90, SPEED_FACTOR);
  turn(90, data, "left");
  driveForward(width + (width/100)*10, SPEED_FACTOR);
  turn(90, data, "right");
  driveForward(width + (width/100)*10, SPEED_FACTOR);
  turn(90, data, "right");
  driveForward(width, SPEED_FACTOR);
  turn(90, data, "right");
  driveForward(ROD, SPEED_FACTOR);
}

void loop() {
}
