#include <LWiFi.h>
#include <WiFiClient.h>
#include "MCS.h"
#include "DHT.h"

#define DHTPIN 2 
#define DHTTYPE DHT11

// Assign AP ssid / password here
#define _SSID "AA202_2G"
#define _KEY  "iteaa202"

DHT dht(DHTPIN, DHTTYPE);
// Assign device id / key of your test device
MCSDevice mcs("DCBahTBb", "d6nEAcSQkLWL7HEQ");

// Assign channel id 
// The test device should have 2 channel
// the first channel should be "Controller" - "On/Off"
// the secord channel should be "Display" - "On/Off"
MCSDisplayFloat    Temperature("TemperatureTEST");
MCSDisplayFloat    Humidity("HumidityTEST");


void setup() {
  // setup Serial output at 9600
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));

  dht.begin();


  // setup Wifi connection
  while(WL_CONNECTED != WiFi.status())
  {
    Serial.print("WiFi.begin(");
    Serial.print(_SSID);
    Serial.print(",");
    Serial.print(_KEY);
    Serial.println(")...");
    WiFi.begin(_SSID, _KEY);
  }
  Serial.println("WiFi connected !!");

  // setup MCS connection
  mcs.addChannel(Humidity);
  mcs.addChannel(Temperature);
  
  while(!mcs.connected())
  {
    Serial.println("MCS.connect()...");
    mcs.connect();
  }
  Serial.println("MCS connected !!");
}

void loop() {
  // call process() to allow background processing, add timeout to avoid high cpu usage
  Serial.print("process(");
  Serial.print(millis());
  Serial.println(")");
  mcs.process(100);
  delay(1500);

  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  
  // updated flag will be cleared in process(), user must check it after process() call.
  
    if(!Humidity.set(h))
    {
      Serial.print("Failed to update remote");
      Serial.println(Humidity.value());
    }

    if(!Temperature.set(t))
    {
      Serial.print("Failed to update remote");
      Serial.println(Temperature.value());
    }

    
  // check if need to re-connect
  while(!mcs.connected())
  {
    Serial.println("re-connect to MCS...");
    mcs.connect();
    if(mcs.connected())
      Serial.println("MCS connected !!");
  }
}
