#include "ESPinfluxdb.h"
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

/****************/
/* DÉCLARATIONS */
/****************/
const char *INFLUXDB_HOST = "domain.org";
const uint16_t INFLUXDB_PORT = 8086;

const char *DATABASE = "DBname";
const char *DB_USER = "user";
const char *DB_PASSWORD = "password";

int incomingChar;
int compteur = 1;

SoftwareSerial HC12(D5, D6); // HC-12 TX Pin, HC-12 RX Pin

ESP8266WiFiMulti WiFiMulti;

Influxdb influxdb(INFLUXDB_HOST, INFLUXDB_PORT);

/*********/
/* SETUP */
/*********/ 
void setup() {
  Serial.begin(115200);
  HC12.begin(9600);               // Serial port to HC12
        
  WiFiMulti.addAP("ssid", "password");
  while (WiFiMulti.run() != WL_CONNECTED) {
    delay(100);
  }
  Serial.println("Ready");

  //influxdb.opendb(DATABASE, DB_USER, DB_PASSWORD);
  // Connexion à la base InfluxDB - Connect to InfluxDB database
  while (influxdb.opendb(DATABASE, DB_USER, DB_PASSWORD)!=DB_SUCCESS) {
    Serial.println("Open database failed");
    delay(10000);
  }
}

/*************/
/* FONCTIONS */
/*************/
void ReceiveDataFromRadio() {
  while (HC12.available()) {      // If HC-12 has data
    incomingChar = HC12.read();
//    Serial.print ( "buffer : " );
//    Serial.println (incomingChar); 
  }
}

void SendDataToInfluxdbServer() {
  //Writing data with influxdb HTTP API: https://docs.influxdata.com/influxdb/v1.5/guides/writing_data/
  //Querying Data: https://docs.influxdata.com/influxdb/v1.5/query_language/
 
  // Envoi la vitesse du vent
  dbMeasurement rowAnemo("VitesseVent");
  rowAnemo.addField("VitesseVent", incomingChar);
  Serial.println(influxdb.write(rowAnemo) == DB_SUCCESS ? " - Object write success" : " - Writing failed");
  
  // Vide les données - Empty field object.
  rowAnemo.empty();
}

/*************/
/* PROGRAMME */
/*************/
void loop() {
  ReceiveDataFromRadio();

  if (compteur > 500) { // envoie la data toutes les 5 secondes (10 ms + 500)
    SendDataToInfluxdbServer();  
    compteur = 1;
  }
  else {
    compteur++;
  }
  delay(10);  // refresh la data sur l'afficheur toutes les 10 ms
//  Serial.print ( "compteur = " );
//  Serial.println(compteur);
}






/*********/
/* Notes */
/*********/
// Envoi la puissance 2nd panneau - Send power of the 2nd solar panel
//  dbMeasurement rowb("85x115");
//  rowb.addField("voltage", busvoltageb);
//  rowb.addField("Current", current_mAb); 
//  rowb.addField("power", busvoltageb * current_mAb);
//  Serial.println(influxdb.write(rowb) == DB_SUCCESS ? " - Object write success" : " - Writing failed"); 

