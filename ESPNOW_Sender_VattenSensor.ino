/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp-now-esp8266-nodemcu-arduino-ide/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#include <ESP8266WiFi.h>
#include <espnow.h>

//#define SensorPin D0
//int LedPin = 1; // ESP01 = 1, Wimos = 2

// REPLACE WITH RECEIVER MAC Address
uint8_t broadcastAddress[] = {0xD8, 0xF1, 0x5B, 0x16, 0x75, 0x6D}; // D8:F1:5B:16:75:6D

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  char a[32];
  int b;
  float c;
  String d;
  bool e;
} struct_message;

// Create a struct_message called myData
struct_message myData;

unsigned long lastTime = 0;  
unsigned long timerDelay = 2000;  // send readings timer

// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
    //digitalWrite(LedPin, LOW); 
  }
  else{
    Serial.println("Delivery fail");
    //digitalWrite(LedPin, HIGH); 
  }
}
 
void setup() {
//  pinMode(LedPin, OUTPUT); 
  //pinMode(SensorPin, OUTPUT);
  // Init Serial Monitor
  Serial.begin(115200);
  
  Serial.print("ESP8266 Board MAC Address:  ");
  Serial.println(WiFi.macAddress());

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}
 
void loop() {
  if ((millis() - lastTime) > timerDelay) {
    // Set values to send
    strcpy(myData.a, "48:3F:DA:69:EB:33");
    myData.b = random(1,3);
    myData.c = 1.2;
    myData.d = "Hello";
    myData.e = false;
    
    // Send message via ESP-NOW
    esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
    
    lastTime = millis();
  }
}
