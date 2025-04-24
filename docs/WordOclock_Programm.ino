#include <ESP32Time.h> // Bibliothek, um Die zeit aus dem Arduino abfragen
ESP32Time rtc; 

#include <FastLED.h> // LED Bibliothek
#define NUM_LEDS 114 
#define DATA_PIN 5
CRGB leds[NUM_LEDS];

#include <WiFi.h> // Wifi Bibliothek
#include "time.h"
const char* ssid     = ""; 
const char* password = "";
const char* ntpServer = "0.de.pool.ntp.org";  //  ,pool.ntp.org,ptbtime1.ptb,
const long  gmtOffset_sec = 3600+0; // Zeit in sekunden eingeben um die Angabe aus dem NTP-Server zu anpassen
const int   daylightOffset_sec = 0; // am besten so lassen;

#include <DHT.h> // Temperatur und Luftfeuchtigkeit Bibliothek
DHT dht11(8, DHT11);

#include <HTTPClient.h> // HTTP Bibliothek
#include <Arduino_JSON.h> // Bibliothek, um JSON Dateien zu ablesen
String openWeatherMapApiKey = ""; // ApiKey aus der Webseite reinkopieren
String city = "Karlsruhe"; // Standort eingeben
String countryCode = "DE"; // Land in Kurzschreibweise eingeben
String jsonBuffer;
// globale Variablen bzw. in jeder Funktion verfügbar
 int g, b, r; // Farbtöne
 int sekunden,minuten,stunden,temp,hum; // wichtige werte für die WordOclock eingaben
 int x,y,v,f,th,frb; // für Funkionen genutzt um werte zu speichern
 bool gedrueckt=true; // Notwendig für ein Button

void setup() {
   Serial.begin(115200); 
   dht11.begin(); // DHT Sensor Anschalten
   pinMode( 3, INPUT_PULLUP); // Button für Temperatur und Luftfeuchtigkeit definieren
   pinMode( 4 ,  INPUT_PULLUP); // Button für Farbwechsel definieren
   pinMode( 5 ,  INPUT_PULLUP); // Button für LED Helligkeit definieren
   FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS); // LED Ketten Anschalten
   f=1; 
   v=45; 
   farbauswahl(f);
}
void loop() {
  //Temperatur und Luftfeuchtigkeit mit einem Button anzeigen Lassen
   while(digitalRead(3)==1){
   hum = (int)dht11.readHumidity();
   temp = (int)dht11.readTemperature();
   Serial.println(hum);
   Serial.println(temp);
   FastLED.clear();
   ziffern(temp/10,0);
   ziffern(temp%10,1);
   celsius(true);
   delay(5000);
   FastLED.clear();
   ziffern(hum/10,0);
   ziffern(hum%10,1);
   celsius(false);
   delay(5000);
   FastLED.clear();
   TempHum();
   ziffern(temp/10,0);
   ziffern(temp%10,1);
   celsius(true);
   delay(5000);
   FastLED.clear();
   ziffern(hum/10,0);
   ziffern(hum%10,1);
   celsius(false);
   delay(5000);
   FastLED.clear();
   th=1;
   }
  //farbe mit einem Button ändern
   if(digitalRead(4)==1 && gedrueckt == true ){
      f++;
      Serial.println(f);
      farbauswahl(f);
      gedrueckt=false; // verhindert wiederholen des ifs beim gedrückthalten des Buttons
     }
   if(digitalRead(4)==0 && gedrueckt == false ){
      gedrueckt=true;
     } 
   if(f>8){
     f=1;
     farbauswahl(f);
     }
  //helligkeit mit button ändern
   if(digitalRead(5)==1 ){
      v++;
      Serial.println(analogRead(A0));
     }
    FastLED.setBrightness(v/5+analogRead(A0)/22); 
    FastLED.show();
   if(v/5>69){
    v=1;
   }
  // WordUhr
   minuten  = rtc.getMinute();
   if(minuten != x || f!=frb || th == 1){ 
   WordOclock();
   x = minuten;
   frb=f;
   }
}
void WordOclock(){
    FastLED.clear();
    stunden  = rtc.getHour();
    // vergleich mit ntp
     if(stunden =! y ){ 
     ltime(); // zeit aus dem NTP server Holen bzw. aktualisieren
     y=stunden;
     }
    stunden  = rtc.getHour();
    // minuten 1-4
     for(int i=0; i<minuten%5; i++){ 
     leds[110+i].setRGB(g, r, b);
     FastLED.show();
     }  
    // es ist
     dioden(1,1,2,1,0); // es
     dioden(1,4,6,1,0); // ist
    minute();
    // vor oder nach
     if (minuten/30==1){
     dioden(4,8,10,1,0); // vor
     }
     else if (minuten/30==0 && minuten!=0){
     dioden(4,2,5,1,0); // nach
     }
     if(minuten >= 25 && minuten < 40){
      diodenAus(4,8,10,1,0);// vor aus
      diodenAus(4,2,5,1,0);  // nach aus
     }
     if(minuten/5 == 5){
     dioden(4,8,10,1,0); // vor
     }
     else if(minuten/5 == 7){
     dioden(4,2,5,1,0); // nach 
     }
     
    hour();
    // uhr
     if(minuten/5==0){
     dioden(10,8,10,1,0); 
     }
    th=0;
} 
void frgb(int a, int d, int c){
   r=a; g=d; b=c;
}
void dioden( int reihe, int anfang, int ende, int hor, int vert){
   for (int i=0;i<=ende-anfang;i++){
   leds[(i*vert+(reihe-1))*11+abs((anfang-1-10*((i*vert+reihe-1)%2))+i*hor)].setRGB(g, r, b);
   FastLED.show();
   }
} 
void farbauswahl(int i){
  switch (i){
    case 1: // weiss
    frgb(255,255,255);
    break;
    case 2: // rot
    frgb(255,0,0);
    break;
    case 3: // grün
    frgb(0,255,0);
    break;
    case 4: // blau
    frgb(0,0,255);
    break;
    case 5: // gelb
    frgb(255,155,0);
    break;
    case 6: // türkies
    frgb(0, 255, 255); 
    break;
    case 7: // violet
    frgb(138,43,226);
    break;
    case 8: // lila
    frgb(255,0,255);
   }
}
void diodenAus( int reihe, int anfang, int ende, int hor, int vert){
   for (int i=0;i<=ende-anfang;i++){
   leds[(i*vert+(reihe-1))*11+abs((anfang-1-10*((i*vert+reihe-1)%2))+i*hor)].setRGB(0,0,0);
   FastLED.show();
   }
} 
void hour(){
  stunden  = rtc.getHour();
  int gesm= stunden*60+minuten;
  int anf=85;
  int takt=60;
  Serial.println(gesm);
  if(gesm>=anf && gesm < anf+takt){ //zwei
    dioden(6,1,4,1,0);
    Serial.println(2);   
   }
  else if(gesm>=anf+takt && gesm < anf+takt*2){ //drei
   dioden(7,2,5,1,0);
   } 
  else if(gesm>=anf+takt*2 && gesm < anf+takt*3){ //vier
   dioden(9,8,11,1,0);
   }
  else if(gesm>=anf+takt*3 && gesm < anf+takt*4){ //fünf
   dioden(7,7,10,1,0);
   }
  else if(gesm>=anf+takt*4 && gesm < anf+takt*5){ //sechs
   dioden(10,2,6,1,0);
   }  
  else if(gesm>=anf+takt*5 && gesm < anf+takt*6){ //sieben
   dioden(6,6,11,1,0);
   }
  else if(gesm>=anf+takt*6 && gesm < anf+takt*7){ //acht
   dioden(8,2,5,1,0);
   }
  else if(gesm>=anf+takt*7 && gesm < anf+takt*8){ //neun
   dioden(9,4,7,1,0);
   }  
  else if(gesm>=anf+takt*8 && gesm < anf+takt*9){ //zehn
   dioden(8,6,9,1,0);
   } 
  else if(gesm>=anf+takt*9 && gesm < anf+takt*10){ //elf
   dioden(9,1,3,1,0);
   }
  else if(gesm>=anf+takt*10 && gesm < anf+takt*11){//zwölf
   dioden(5,7,11,1,0);
   }
  else if(gesm/5==12){ //ein
   dioden(6,3,5,1,0);
   }
  else {             //eins
    dioden(6,3,6,1,0);        
   }
}  
void minute(){
   switch(minuten/5){
   case 1: case 11: 
   dioden(1,8,11,1,0); // fünf 5-9 und 55-59
   break;
   case 5: case 7:
   dioden(1,8,11,1,0); // fünf 35-39 und 25-29
   dioden(5,1,4,1,0); //halb 35-39 und 25-29
   break;
   case 2: case 10:
   dioden(2,1,4,1,0); // zehn 10-14 und 50-54
   break;
   case 3: case 9:
   dioden(2,5,11,1,0); // viertel 15-19 und 45-49
   break;
   case 4: case 8:
   dioden(3,2,8,1,0); // zwanzig 20-24 und 40-44
   break;
   case 6:
   dioden(5,1,4,1,0); //halb 30-34
    }
}
void ziffern(int i, int block){
  switch(i){
    case 1:
    dioden(1,4+block*4,7+block*4,0,1);
    dioden(5,3+block*4,5+block*4,1,0);
    dioden(2,3+block*4,3+block*4,1,0);
    break;
    case 2:
    dioden(2,3+block*4,3+block*4,0,1);
    dioden(5,3+block*4,5+block*4,1,0);
    dioden(1,3+block*4,5+block*4,1,0);
    dioden(2,5+block*4,6+block*4,0,1);
    dioden(4,4+block*4,4+block*4,0,1);
    break;
    case 4:
    dioden(1,3+block*4,5+block*4,0,1);
    dioden(3,4+block*4,4+block*4,0,1);
    dioden(1,5+block*4,9+block*4,0,1);
    break;
    case 7:
    dioden(1,3+block*4,4+block*4,1,0);
    dioden(1,5+block*4,6+block*4,0,1);
    dioden(3,4+block*4,6+block*4,0,1);
    break;
    case 8:
    acht(block);
    break;
    case 3:
    acht(block);
    diodenAus(2,3+block*4,5+block*4,0,1);
    break;
    case 5:
    acht(block);
    diodenAus(2,5+block*4,6+block*4,0,1);
    diodenAus(4,3+block*4,3+block*4,0,1);
    break;
    case 6:
    acht(block);
    diodenAus(2,5+block*4,5+block*4,0,1);
    break;
    case 9:
    acht(block);
    diodenAus(4,3+block*4,3+block*4,0,1);
    break;
    case 0:
    acht(block);
    diodenAus(3,4+block*4,4+block*4,0,1);
   }
}
void acht(int block){
    dioden(1,3+block*4,7+block*4,0,1);
    dioden(1,4+block*4,8+block*4,0,1);
    dioden(1,5+block*4,9+block*4,0,1);
    diodenAus(2,4+block*4,4+block*4,0,1);
    diodenAus(4,4+block*4,4+block*4,0,1);
}
void ltime(){
   WiFi.begin(ssid, password);
   int n=0;
    while (WiFi.status() != WL_CONNECTED){ // warten bis sich das Arduino mit dem WLAN verbindet
     leds[n].setRGB(n+g-255, n*2+r-255, b-n*2);
     FastLED.show();
     delay(50);
     FastLED.clear();
     n++;
     if(n==NUM_LEDS-4){
      n=0;
     }
     if(f>8){
      f=1;
     }
     }
   FastLED.clear();
   configTime(gmtOffset_sec, daylightOffset_sec, ntpServer); // zeit aus dem NTP server Holen
   
   struct tm timeinfo;
   while(!getLocalTime(&timeinfo));
   char timeHour[3];
   strftime(timeHour,3, "%H", &timeinfo);
   char timeMinute[3];
   strftime(timeMinute,3, "%M", &timeinfo);
   char timeSecond[3];
   strftime(timeSecond,3, "%S", &timeinfo);

   rtc.setTime(atoi(timeSecond), atoi(timeMinute), atoi(timeHour), 14, 10, 2024);
   
   WiFi.disconnect(true); // sich vom WLAN trennen
   WiFi.mode(WIFI_OFF); 
}
void celsius(bool yes){
  dioden(7,3,4,1,0);
  dioden(8,3,4,1,0);

  if(yes==true){
    dioden(8,6,7,1,0);
    dioden(9,6,7,1,0);
    dioden(7,7,9,1,0);
    dioden(10,7,9,1,0);
  }
  else{
    dioden(10,5,8,1,-1);
    dioden(10,4,7,1,-1);
    dioden(9,8,9,1,0);
    dioden(10,8,9,1,0);
  }
}
void TempHum(){
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED);

    if(WiFi.status()== WL_CONNECTED){
      String serverPath = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "," + countryCode + "&units=metric" + "&APPID=" + openWeatherMapApiKey;
      
      jsonBuffer = httpGETRequest(serverPath.c_str());
      JSONVar myObject = JSON.parse(jsonBuffer);

      if (JSON.typeof(myObject) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
      }
      temp=(int)myObject["main"]["temp"];
      hum=(int)myObject["main"]["humidity"];
    }
   WiFi.disconnect(true);
   WiFi.mode(WIFI_OFF);
}
String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;
   
  http.begin(client, serverName);// Your Domain name with URL path or IP address with path
  int httpResponseCode = http.GET(); // Send HTTP POST request
  String payload = "{}";
 
  if (httpResponseCode>0) {
    payload = http.getString();
  }
  else {
    Serial.print("Error ");
  }
  http.end();
  return payload;
}
 
