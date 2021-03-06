/*
  Liam Homburger
  This is the code for the Misty Joystick Project

  https://github.com/Liamq12
*/
#include <WiFi.h>
#include <HTTPClient.h>

//Your Network Creds
const char* ssid = "";
const char* password = "";

//Misty IP (Can find on the misty app)
String MistyIP = "";

//X and Y Analog Pins
#define JoyX 39
#define JoyY 36

//Max Analog Reading (ESP32 is 4096, Some boards are 1024)
#define AnalogMax 4096

//Ignore readings within this threshold. Reccomended at 5% of AnalogMax. Set to -1 for an autogenerated deadzone
int DeadZone = -1;

//Prorgam Globals
int ShiftX = 0;
int ShiftY = 0;

int XAverage = 0;
int YAverage = 0;

void setup() {
  Serial.begin(115200);

  //Auto set DeadZone to 5% of AnalogMax if not set. 
  if (DeadZone == -1) {
    DeadZone = AnalogMax * 0.05;
  }

  int CalibrationX[100];
  int CalibrationY[100];
  
  //Take 100 readings super fast to get the average resting values
  for (int i = 0; i < 100; i++) {
    CalibrationX[i] = analogRead(JoyX);
    CalibrationY[i] = analogRead(JoyY);
    delay(10);
  }

  int XSum = 0;
  int YSum = 0;

  //Average resting values
  for (int i = 0; i < 100; i++) {
    XSum += CalibrationX[i];
    YSum += CalibrationY[i];
  }

  XAverage = XSum / 100;
  YAverage = YSum / 100;

  //Calculate shifts for each axis
  ShiftX = (AnalogMax / 2) - XAverage;
  ShiftY = (AnalogMax / 2) - YAverage;

  //Set Averages to True average plus Shift (About AnalogMax/2
  XAverage = XAverage + ShiftX;
  YAverage = YAverage + ShiftY;

  //Connect to Wifi
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  //Get raw X and Y value and add shift
  int XValue = analogRead(JoyX) + ShiftX;
  int YValue = analogRead(JoyY) + ShiftY;

  int LinearVelocity = 0;
  int AngularVelocity = 0;

  //Calculate Linear and Angular Velocity on gradient
  if (YValue > (YAverage + DeadZone)) {
    int Range = (AnalogMax + DeadZone) - (YAverage + DeadZone);
    double Rate = (double) Range / 100;
    LinearVelocity = (YValue - (YAverage + DeadZone)) / Rate;
  }
  if (YValue < (YAverage - DeadZone)) {
    int Range = (AnalogMax + DeadZone) - (YAverage + DeadZone);
    double Rate = (double) Range / 100;
    LinearVelocity = (YValue - (YAverage + DeadZone)) / Rate;
  }
  if (XValue > (XAverage + DeadZone)) {
    int Range = (AnalogMax + DeadZone) - (XAverage + DeadZone);
    double Rate = (double) Range / 100;
    AngularVelocity = (-1) * ((XValue - (XAverage + DeadZone)) / Rate);
  }
  if (XValue < (XAverage - DeadZone)) {
    int Range = (AnalogMax + DeadZone) - (XAverage + DeadZone);
    double Rate = (double) Range / 100;
    AngularVelocity = (-1) * ((XValue - (XAverage + DeadZone)) / Rate);
  }

  //Send HTTP POST Request to Misty with Angular and Linear Velocity
  String DriveURL = "http://" + MistyIP + "/api/drive?linearVelocity=" + (String) LinearVelocity + "&angularVelocity=" + (String) AngularVelocity;
  String HTTPRequest = httpPOSTRequest((DriveURL));
  delay(100);
}

String httpPOSTRequest(String serverName) {
  HTTPClient http;

  // IP Adress of server with endpoint
  http.begin(serverName);

  // Send HTTP POST request to server
  int httpResponseCode = http.POST("");

  // Use empty payload
  String payload = "--";

  //Report response and any error
  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}
