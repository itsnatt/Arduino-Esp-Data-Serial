#define BLYNK_TEMPLATE_ID "TMPLISfGJVCW"
#define BLYNK_DEVICE_NAME "Quickstart Device"
#define BLYNK_AUTH_TOKEN "rc8B1fRGypoGqzegOk0Iuyq04eKxdQe_"
#define BLYNK_PRINT Serial

char ssid [] = "megachan";
char pass [] = "1234567q";
char auth [] = BLYNK_AUTH_TOKEN;

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#include <ArduinoJson.h>
#define RXp2 16
#define TXp2 17

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define SW2 14
#define relaypin  13
#define relaypin2  12
double volt;
double amper;
int p;
int sw = 0;
String  st;
int maxP = 0;


void setup() 
{

 pinMode(SW2, INPUT_PULLUP);
 pinMode(relaypin, OUTPUT);
 pinMode(relaypin2, OUTPUT);
  digitalWrite(relaypin, HIGH);
  digitalWrite(relaypin2, HIGH);
    Serial.begin(115200);
    Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2);
     lcd.begin();
     lcd.setCursor(0,0);
     lcd.print("Connecting...");   
        Blynk.begin(auth, ssid, pass); 
        digitalWrite(relaypin2, LOW);
}
 
void datain()
{
  StaticJsonDocument<300> doc;
    DeserializationError err = deserializeJson(doc, Serial2);
   if (err == DeserializationError::Ok) 
    {
      amper = doc["amper"].as<double>() ;
      volt =  doc["volt"].as<double>() ;
      if(doc["volt"].as<double>() < 100)
      {
        volt = 0;
      }
    } 
    else 
    {
      Serial.println(err.c_str());
      while (Serial2.available() > 0)
        Serial2.read();
    }
}

void datakirim()
{
  Blynk.run(); 
  p = amper * 220;
   Blynk.virtualWrite(V0,volt);
    Blynk.virtualWrite(V1,amper);
     Blynk.virtualWrite(V2,p);
      Blynk.virtualWrite(V3,st);
         
}

BLYNK_WRITE(V4)
{
  sw = param.asInt(); 
}
BLYNK_WRITE(V5)
{
  maxP = param.asInt(); 
}

void param()
{

    if (digitalRead(SW2) == LOW) 
      {
        sw = !sw;
         delay(600);
         Blynk.virtualWrite(V4,sw);
      }
    if (p > maxP)
    {
      digitalWrite(relaypin2, HIGH);
       lcd.setCursor(9,1);
        lcd.print("  OL! ");
        st = "Over Load";
    }   
    if (p < maxP)
    {
      digitalWrite(relaypin2, LOW);
       lcd.setCursor(9,1);
        lcd.print("      ");
        st = "Normal";
          
    }

}

void relay()
  {
    if(sw == 1)
    {
      digitalWrite(relaypin, LOW);
      lcd.setCursor(9,0);
      lcd.print("  ON ");   
    }
    else {
    digitalWrite(relaypin, HIGH);
      lcd.setCursor(9,0);
      lcd.print("  OFF ");   
    }
  }

void hapuslcd()
{
  lcd.setCursor(0,0);
  lcd.print("         ");  
  lcd.setCursor(0,1);
  lcd.print("         ");  

}

void loop() {
  if (Serial2.available()) 
  {
    hapuslcd();
        for (int i = 0; i < 5; i++) 
          {
            datain();
            relay();
            param();
            datakirim();
              lcd.setCursor(0,0);
              lcd.print("V = ");
              lcd.print(volt);
              lcd.setCursor(0,1);
              lcd.print("I = ");
              lcd.print(amper);
            delay(1000);
        }
    hapuslcd();
        for (int y = 0; y < 5; y++) 
          {
            datain();
            relay();
            param();
            datakirim();
              lcd.setCursor(0,0);
              lcd.print("POWER");
              lcd.setCursor(0,1);
              lcd.print(p);
              lcd.setCursor(4,1);
              lcd.print(" WATT");
            delay(1000);
        }
   
  }
}
