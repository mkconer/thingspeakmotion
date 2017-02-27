// Cheap Home Security Motion Detector
// This program uses the Nodemcu and a motion sensor HC-SR501 to alert you of motion
// using ThingSpeak and Twitter - You will need a Twiiter and ThingSpeak account
// and you will need to link them.
// Be sure to set the Delay and Sensitivity potentioometers on the motion sensor
// The motion sensor should have at least a 15 second delay to prevent updating to 
// ThingSpeak to quickly (they don't like that!)

#include "ThingSpeak.h"                                 // Include ThingSpeak Library
#include <ESP8266WiFi.h>                                // Include ESP8266wifi Library
  
char ssid[] = "yourssid";                               // Enter your WiFi SSID (router name)
char pass[] = "yourpasswo";                             // Enter your WiFi router password
unsigned long myChannelNumber = 111111;                 // Enter your Thingspeak Channel Number
const char * myWriteAPIKey = "xxxxxxxxxxxxxxxx";        // Enter your ThingSpeak API Key

int ledPin = D5;                                        // Choose the pin for the LED (Pin D5, GPIO 14) 
int inputPin = D6;                                      // Choose the input pin for motion sensor (Pin D6, GPIO 12)
int motionState = 0;                                    // We start, assuming no motion detected
int val = 0;                                            // Variable for reading the pin status

unsigned long currentMillis = millis();
unsigned long previousMillis = 0;
unsigned long resetMillis = 0;
const long interval = 90000;                            // Time delay interval (90 seconds) to prevent false triggers when setting up device 
                                                        
int status = WL_IDLE_STATUS;
WiFiClient  client;

void setup() 
{   
  WiFi.begin(ssid, pass);                               // Start WiFi connection
  ThingSpeak.begin(client);                             // Start ThingSpeak connection
  Serial.begin(115200);                                 // Serial Baud Rate
  pinMode(ledPin, OUTPUT);                              // Set the LED pin as an Output
  pinMode(inputPin, INPUT);                             // Set the Motion Sensor pin as an Input

  Serial.println();
  Serial.print("Connecting to ");
  Serial.print(ssid); 
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");  
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  
  Serial.println("Motion Sensor Alarm");

  ThingSpeak.writeField(myChannelNumber, 1, val, myWriteAPIKey);           // Update ThingSpeak channel the PIR sensor value '0'
 
}

void loop() 
{
//********************* Read motion sensor and upload to ThingSpeak ***************************** 
  
currentMillis = millis();                                                 // Set the currentMillis equal to the millis function

val = digitalRead(inputPin);                                              // Read motion sensor
  
if (val == HIGH)                                                          // Check if the input pin is HIGH
  {                                                                                                  
  if ((motionState == 0) && (currentMillis >= interval))
    {                                                        
      digitalWrite(ledPin, HIGH);                                         // Turn LED ON
      Serial.println("Motion detected!");                                 // Motion Detected                                                       
      motionState = 1;                                                    // We only want to update on the change, not state
      ThingSpeak.writeField(myChannelNumber, 1, val, myWriteAPIKey);      // Update ThingSpeak channel the PIR sensor value '0'  
      Serial.println("Data Sent to ThingSpeak!");  
      delay(500);                  
    }
  } else {
    digitalWrite(ledPin, LOW);                                            // Turn LED OFF
    if (motionState == 1)
    {                                                                     // Motion stopped                                                      
      Serial.println("Motion ended!");                                                                          
      motionState = 0;                                                    // We only want to print on the output change, not state
      ThingSpeak.writeField(myChannelNumber, 1, val, myWriteAPIKey);      // Update ThingSpeak channel the PIR sensor value '0' 
      Serial.println("Data Sent to ThingSpeak!");  
      delay(500);  
                   
    }
  }    
 
 if (currentMillis - previousMillis >= 10000)                             // Update Serial Monitor every ten seconds (10000 milliseonds)
 {                                                                        // as to how long the program has been running
 Serial.print("Program running for ");
 Serial.print((currentMillis/1000)/60);
 Serial.println(" Minutes"); 
 previousMillis = currentMillis;
 yield();                                                                 // Yield to prevent the WDT from crashing the NodeMCU
 }
}  

