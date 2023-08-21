/*
Batterieladeregelung PV Überschuss v02
duese1981 

  - Shelly 3EM auslesen
  - JSON deserialisieren
  - Phasen saldieren
  - Steuerung digitales Poti X9C103S
  - Relais für Ladegerät netzabhängig aus- und einschalten
  - Einfacher Timer für Wiedereinschalten des Ladegeräts

*/


// Pins Poti X9C103S auf D1 Mini
//  U/D -> D4 PIN 2
//  INC -> D3 PIN 0
//  CS  -> D2 PIN 4


#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include "Arduino.h"
#include "X9C10X.h"

//Relais Shield auf Pin D1
#define RELAIS D1

// Digitales Poti
X9C10X pot(10000);  //  10 KΩ  (ALT-234)
uint8_t direction = LOW;
uint8_t step = 1;

// WLAN Zugangsdaten
const char* ssid = "ssid";
const char* password = "password";

//IP Adresse Shelly 3EM
const char* serverName0 = "http://192.168.68.107/emeter/0"; //IP Adresse des Shelly 3EM im eigenen Netzwerk (IP austauschen)
const char* serverName1 = "http://192.168.68.107/emeter/1";
const char* serverName2 = "http://192.168.68.107/emeter/2";

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
unsigned long timerDelay = 5000;

String input;             // Stream http request
int empower;              // Gesamtleistung Shelly 3EM über 3 Phasen
int potipos;              // Position Poti 0-99
int avpower;              // gleitender Leistungswert
int z = 0;                // Zähler

// Programm JSON einlesen
String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;

  // Your IP address with path or Domain name with URL path
  http.begin(client, serverName);

  // Send HTTP POST request
  int httpResponseCode = http.GET();

  String payload = "{}";

  if (httpResponseCode > 0) {
    // Serial.print("HTTP Response code: ");
    // Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}



// Programm Deserialize Stream
int mypower(const char* serverName)
{
  // Stream& input;
  input = httpGETRequest(serverName);

  StaticJsonDocument<192> doc; // https://arduinojson.org/v6/assistant

  DeserializationError error = deserializeJson(doc, input);

  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
  }

  float power = doc["power"];

  return power;
}


void setup()
{
  Serial.begin(115200);
  pinMode(RELAIS, OUTPUT); //Relais als Ausgangssignal

  // WiFi
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  // Poti
  pot.begin(0, 2, 4); // INC, U/D, CS
  delay(20);

  // Pos. Widerstandswert auf 99 stellen
  for (uint8_t i = 0; i < 200; i++)
  {
    pot.incr();
    delay(20);
  }
  pot.setPosition(99);
  delay(100);

  // Pos. Widerstandswert auf 0 stellen
  for (uint8_t i = 0; i < 100; i++)
  {
    pot.decr();
    delay(20);
  }
}


void loop()
{

  empower = 0; // Leistungsmessung rücksetzen

  // Send an HTTP POST request depending on timerDelay
  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if (WiFi.status() == WL_CONNECTED) {

      delay(1000);
      empower = mypower(serverName0) + mypower(serverName1) + mypower(serverName2); // Summe aller Phasen (negativ bei Stromüberschuss)


    }
  }


  potipos = pot.getPosition();

  if (potipos < 99 && empower + 200 < 0) {   // Poti Position erhöhen wenn empower mehr als 200W Überschuss (-)
    pot.incr();
    delay(20);
  }


  if (potipos > 0 && empower + 100 > 0) {   // Poti Position reduzieren wenn empower weniger als 100W Überschuss (-)
    pot.decr();
  }


  // Relais für Ladegerät netzabhängig aus- und einschalten
  avpower = 0.9 * avpower + 0.1 * empower; // gleitender Leistungswert

  if (avpower > -50) {
    digitalWrite(RELAIS, HIGH);    // Abschaltung aktivieren
  }


  if (digitalRead(RELAIS)==HIGH) {              // Zähler erhöhen, wenn Abschaltung aktiv
    z++;
  }


  if (z > 500 && avpower < -100) {            // Zeitverzögerte Einschaltung nach z Loops und  (Timer)
    digitalWrite(RELAIS, LOW);               // Abschaltung deaktiviert, Ladegerät wieder an
    z = 0;
  }

  if (z > 500) {                  // damit Zähler nicht unendlich groß werden kann
    z = 500;
  }



  Serial.print ("Pos: ");
  Serial.println(potipos);
  Serial.print ("Power: ");
  Serial.println(empower);
  Serial.print ("AVPower: ");
  Serial.println(avpower);
  Serial.print ("Zähler: ");
  Serial.println(z);
  Serial.println("");


}
