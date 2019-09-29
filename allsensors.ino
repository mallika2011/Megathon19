/*
  WriteMultipleFields
  
  Description: Writes values to fields 1,2,3,4 and status in a single ThingSpeak update every 20 seconds.
  
  Hardware: ESP8266 based boards
  
  !!! IMPORTANT - Modify the secrets.h file for this project with your network connection and ThingSpeak channel details. !!!
  
  Note:
  - Requires ESP8266WiFi library and ESP8622 board add-on. See https://github.com/esp8266/Arduino for details.
  - Select the target hardware from the Tools->Board menu
  - This example is written for a network using WPA encryption. For WEP or WPA, change the WiFi.begin() call accordingly.
  
  ThingSpeak ( https://www.thingspeak.com ) is an analytic IoT platform service that allows you to aggregate, visualize, and 
  analyze live data streams in the cloud. Visit https://www.thingspeak.com to sign up for a free account and create a channel.  
  
  Documentation for the ThingSpeak Communication Library for Arduino is in the README.md folder where the library was installed.
  See https://www.mathworks.com/help/thingspeak/index.html for the full ThingSpeak documentation.
  
  For licensing information, see the accompanying license file.
  
  Copyright 2018, The MathWorks, Inc.
*/
#include "ThingSpeak.h"
//#include "secrets.h"
#include <ESP8266WiFi.h>
#include <Arduino.h>
#include <Wire.h>
#include "MutichannelGasSensor.h"
#include "sensirion_common.h"
#include "sgp30.h"
#include "DHT.h"
#include "SdsDustSensor.h"
int rxPin = D5;
int txPin = D6;
SdsDustSensor sds(rxPin, txPin);
#define DHTPIN D3     // Digital pin connected to the DHT sensor
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);

#define SECRET_SSID "JioFi_20E7D6C"
#define SECRET_PASS "2wufpbqmx0"

char ssid[] = SECRET_SSID;   // your network SSID (name) 
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

unsigned long myChannelNumber1 = 873276;
const char * myWriteAPIKey1 = "2YJYTYTICND044UF";

unsigned long myChannelNumber2 = 873290;
const char * myWriteAPIKey2 = "OB8QMGU7GL5RT25X";
//unsigned long myChannelNumber3 = SECRET_CH_ID3;
//const char * myWriteAPIKey3 = SECRET_WRITE_APIKEY3;
unsigned long myChannelNumber4 = 873290;
const char * myWriteAPIKey4 = "JGB4FWJXOXOMAZEZ";

String myStatus = "";

void setup() {
  Serial.begin(115200);  // Initialize serial

  WiFi.mode(WIFI_STA); 
  
  ThingSpeak.begin(client);  // Initialize ThingSpeak
  s16 err;
    u16 scaled_ethanol_signal, scaled_h2_signal;
  Serial.println("serial start!!");
  sds.begin();

  Serial.println(sds.queryFirmwareVersion().toString()); // prints firmware version
  Serial.println(sds.setActiveReportingMode().toString()); // ensures sensor is in 'active' reporting mode
  Serial.println(sds.setContinuousWorkingPeriod().toString()); // ensures sensor has continuous working period - default but not recommended
  dht.begin();
//For wio link!/
 gas.begin(0x04);//the default I2C address of the slave is 0x04
    gas.powerOn();
  #if defined(ESP8266)
          pinMode(15,OUTPUT);
          digitalWrite(15,1);
          Serial.println("Set wio link power!");
          delay(500);
  #endif
  /*Init module,Reset all baseline,The initialization takes up to around 15 seconds, during which
all APIs measuring IAQ(Indoor air quality ) output will not change.Default value is 400(ppm) for co2,0(ppb) for tvoc*/
  while (sgp_probe() != STATUS_OK) {
         Serial.println("SGP failed");
         while(1);
    }
    //Read H2 and Ethanol signal in the way of blocking/
    err = sgp_measure_signals_blocking_read(&scaled_ethanol_signal,
                                            &scaled_h2_signal);
    if (err == STATUS_OK) {
        Serial.println("get ram signal!");
    } else {
         Serial.println("error reading signals"); 
    }
     err = sgp_iaq_init();
     //
}

void loop() {
 s16 err=0;
  u16 tvoc_ppb, co2_eq_ppm;
  err = sgp_measure_iaq_blocking_read(&tvoc_ppb, &co2_eq_ppm);
        if (err == STATUS_OK) {
             Serial.print("tVOC  Concentration:");
             Serial.print(tvoc_ppb);
             Serial.println("ppb");
             
              Serial.print("CO2eq Concentration:");
              Serial.print(co2_eq_ppm);
               Serial.println("ppm");
        } else {
             Serial.println("error reading IAQ values\n"); 
        }
  
  // Wait a few seconds between measurements.

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));
  float nh3,co,no2,c;

    nh3 = gas.measure_NH3();
    Serial.print("The concentration of NH3 is ");
    if(c>=0) Serial.print(c);
    else Serial.print("invalid");
    Serial.println(" ppm");

    co = gas.measure_CO();
    Serial.print("The concentration of CO is ");
    if(c>=0) Serial.print(c);
    else Serial.print("invalid");
    Serial.println(" ppm");

    no2 = gas.measure_NO2();
    Serial.print("The concentration of NO2 is ");
    if(c>=0) Serial.print(c);
    else Serial.print("invalid");
    Serial.println(" ppm");

    c = gas.measure_C3H8();
    Serial.print("The concentration of C3H8 is ");
    if(c>=0) Serial.print(c);
    else Serial.print("invalid");
    Serial.println(" ppm");

    c = gas.measure_C4H10();
    Serial.print("The concentration of C4H10 is ");
    if(c>=0) Serial.print(c);
    else Serial.print("invalid");
    Serial.println(" ppm");

    c = gas.measure_CH4();
    Serial.print("The concentration of CH4 is ");
    if(c>=0) Serial.print(c);
    else Serial.print("invalid");
    Serial.println(" ppm");

    c = gas.measure_H2();
    Serial.print("The concentration of H2 is ");
    if(c>=0) Serial.print(c);
    else Serial.print("invalid");
    Serial.println(" ppm");

    c = gas.measure_C2H5OH();
    Serial.print("The concentration of C2H5OH is ");
    if(c>=0) Serial.print(c);
    else Serial.print("invalid");
    Serial.println(" ppm");
    PmResult pm = sds.readPm();
  if (pm.isOk()) {
    Serial.print("PM2.5 = ");
    Serial.print(pm.pm25);
    Serial.print(", PM10 = ");
    Serial.println(pm.pm10);

    // if you want to just print the measured values, you can use toString() method as well
    Serial.println(pm.toString());
  } else {
    // notice that loop delay is set to 0.5s and some reads are not available
    Serial.print("Could not read values from sensor, reason: ");
    Serial.println(pm.statusToString());
  }
  Serial.println("");
  Serial.println("");
  Serial.println("");
  Serial.println("");
  Serial.println("");

   
  // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }

  // set the fields with the values
  ThingSpeak.setField(1, String(tvoc_ppb));
  ThingSpeak.setField(2, String(co2_eq_ppm));;
  
  // write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(myChannelNumber2, myWriteAPIKey2);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  

 ThingSpeak.setField(1, String(h));
  ThingSpeak.setField(2, String(t));
  ThingSpeak.setField(3, String(hic));
  
  // write to the ThingSpeak channel
   x = ThingSpeak.writeFields(myChannelNumber1, myWriteAPIKey1);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }

//   ThingSpeak.setField(1, String(pm.pm25));
//  ThingSpeak.setField(2, String(pm.pm10));
//  
//  // write to the ThingSpeak channel
//  int x = ThingSpeak.writeFields(myChannelNumber3, myWriteAPIKey3);
//  if(x == 200){
//    Serial.println("Channel update successful.");
//  }
//  else{
//    Serial.println("Problem updating channel. HTTP error code " + String(x));
//  }
    ThingSpeak.setField(1, String(nh3));
  ThingSpeak.setField(2, String(co));
  ThingSpeak.setField(3, String(no2));
  // write to the ThingSpeak channel
   x = ThingSpeak.writeFields(myChannelNumber4, myWriteAPIKey4);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  delay(20000); // Wait 20 seconds to update the channel again
}
