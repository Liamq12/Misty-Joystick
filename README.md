# Misty-Joystick
Arduino code for Misty Joystick Mini-project
# Hardware and Dependencies
2 Axis Joystick <br/>
Microcontroller (This example uses the ESP32. Microcontroller must have an internet connection)<br/>
Misty 2 <br/>
Arduino IDE<br/>
WiFi Library<br/>
HTTPClient Library<br/>
# Setup
Joystick 5V to Microcontroller 5V or 3V (3V works fine in most cases)<br/>
Joystick GND to Microcontroller GND <br/>
Joystick VRy and VRx pins to Microcontroller AnalogInputs <br/>
Joystick SW Pin not connected<br/>
# Code Configuration
SSID: Your network name. This needs to be the same network as your Misty 2 <br/>
Password: Network password<br/>
MistyIP: Your Misty's IP Address. Can be found in the Misty App<br/>
JoyX: Microcontroller pin connected to VRx<br/>
JoyY: Microcontroller pin connected to VRy<br/>
AnalogMax: Max value from an analog reading. 4096 for ESP32<br/>
DeadZone: Joystick Deadzone, set to -1 for autoconfiguration<br/>
