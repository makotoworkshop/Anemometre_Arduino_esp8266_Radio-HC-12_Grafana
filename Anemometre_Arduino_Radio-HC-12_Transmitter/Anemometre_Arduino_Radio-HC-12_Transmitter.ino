/*    Arduino Anemometer Radio Transmitter using HC-12    */
    
#include <SoftwareSerial.h>
/****************/
/* DÉCLARATIONS */
/****************/
SoftwareSerial HC12(10, 11); // HC-12 TX Pin, HC-12 RX Pin
// le capteur à effet Hall est connecté à la pin 2 = int0
unsigned long rpmVent = 0;
unsigned long vitVentKMH = 0;
unsigned long dateDernierChangementVent = 0;
unsigned long dateDernierChangementKMH = 0;
float intervalleKMH = 0;

/*********/
/* SETUP */
/*********/  
void setup() {
  Serial.begin(9600);             // Serial port to computer
  HC12.begin(9600);               // Serial port to HC12
  // Pin capteurs
  attachInterrupt(0, rpm_vent, FALLING); 
      
}

/*************/
/* PROGRAMME */
/*************/
void loop() {
  RemiseZeroVitVentKMH ();
  delay(500);
}

/*************/
/* FONCTIONS */
/*************/
void rpm_vent()   // appelée par l'interruption, Anémomètre vitesse du vent.
{ 
  unsigned long dateCourante = millis();
  intervalleKMH = (dateCourante - dateDernierChangementVent);
  Serial.print ( "intervalle en s : " );
  Serial.println (intervalleKMH/1000); // affiche l'intervalle de temps entre deux passages
  if (intervalleKMH != 0)  // attention si intervalle = 0, division par zero -> erreur
  {
    rpmVent = 60 / (intervalleKMH /1000);  
  }
  vitVentKMH = ( rpmVent + 6.174 ) / 8.367;
  Serial.print ( "vitVentKMH : " );
  Serial.println ( vitVentKMH ); // affiche les rpm  
  Serial.println ( "" );
  HC12.print(char(vitVentKMH)); 
  dateDernierChangementVent = dateCourante;
}

void RemiseZeroVitVentKMH ()
{
  unsigned long dateCouranteKMH = millis();
  if (intervalleKMH == intervalleKMH) // Si ça ne tourne plus (valeur plus mise à jour)
  {  
    float dureeKMH = (dateCouranteKMH - dateDernierChangementKMH);
    if (dureeKMH > 10000) // Si ça ne tourne plus depuis 10 secondes
    {
      Serial.print ( "dureeKMH : " );
      Serial.println ( dureeKMH ); // affiche les rpm  
      vitVentKMH = 0;  // Remsise à zero !
      dateDernierChangementKMH = dateCouranteKMH;    
    }
  }
}
