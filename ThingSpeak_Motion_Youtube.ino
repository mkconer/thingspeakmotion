// Cheap Home Security Motion Detector
// This program uses the Nodemcu and a motion sensor HC-SR501 to alert you of motion
// using ThingSpeak and Twitter - You will need a Twiiter and ThingSpeak account
// and you will need to link them.
// Be sure to set the Delay and Sensitivity potentioometers on the motion sensor
// The motion sensor should have at least a 15 second delay to prevent updating to 
// ThingSpeak to quickly (they don't like that!)


#include "ThingSpeak.h"                             // Include ThingSpeak Library
#include <ESP8266WiFi.h>                            // Include ESP8266wifi Library
  
char ssid[] = "yourwifi";                           // Enter your WiFi SSID (router name)
char pass[] = "yourpassword";                       // Enter your WiFi router password
unsigned long myChannelNumber = 111111;             // Enter your Thingspeak Channel Number
const char * myWriteAPIKey = "your-api-TS-key";     // Enter your ThingSpeak API Key

int ledPin = D5;                                    // Choose the pin for the LED (Pin D5, GPIO 14) 
int inputPin = D6;                                  // Choose the input pin for motion sensor (Pin D6, GPIO 12)
int motionState = 0;                                // We start, assuming no motion detected
int val = 0;                                        // Variable for reading the pin status

int status = WL_IDLE_STATUS;
WiFiClient  client;

void setup() 
{   
  WiFi.begin(ssid, pass);                           // Start WiFi connection
  ThingSpeak.begin(client);                         // Start ThingSpeak connection
  Serial.begin(115200);                             // Serial Baud Rate
}

void loop() 
{

//********************* Read motion sensor and upload to ThingSpeak *****************************   

  unsigned long currentMillis = millis();

  val = digitalRead(inputPin);                                            // Read motion sensor
  if (val == HIGH)                                                        // Check if the input pin is HIGH
  {                                                                       
    digitalWrite(ledPin, HIGH);                                           // Turn LED ON
  if (motionState == 0) {                                                        
      Serial.println("Motion detected!");                                 // Motion Detected                                                       
      motionState = 1;                                                    // We only want to update on the change, not state
      ThingSpeak.writeField(myChannelNumber, 1, val, myWriteAPIKey);      // Update ThingSpeak channel the PIR sensor value '0'  
      Serial.println("Data Sent to ThinhSpeak!");  
      delay(500);                  
    }
  } else {
    digitalWrite(ledPin, LOW);                                            // Turn LED OFF
    if (motionState == 1){                                                // Motion stopped                                                      
      Serial.println("Motion ended!");                                    // We only want to print on the output change, not state                                      
      motionState = 0;
      ThingSpeak.writeField(myChannelNumber, 1, val, myWriteAPIKey);      // Update ThingSpeak channel the PIR sensor value '0' 
      Serial.println("Data Sent to ThinhSpeak!");  
      delay(500);          
    }
  }   
 
 Serial.print("Milli Seconds program running ");
 Serial.println(currentMillis / 1000);
}  



