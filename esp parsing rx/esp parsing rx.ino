#define BLYNK_TEMPLATE_ID "TMPLISfGJVCW"
#define BLYNK_DEVICE_NAME "Quickstart Device"
#define BLYNK_AUTH_TOKEN "rc8B1fRGypoGqzegOk0Iuyq04eKxdQe_"
char ssid [] = "megachan";
char pass [] = "1234567q";
// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
char auth[] = BLYNK_AUTH_TOKEN;
BlynkTimer timer;
// Your WiFi credentials.
#include "ThingSpeak.h"
int xxx;

#define RXp2 16
#define TXp2 17

String dataIn;
String dt[10];
int i;
boolean parsing=false;

int V;
float I1,I2,I3;


#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

//2 Rotary encoder
#define SW2 14
#define SW 27
int counter2 = 0;
int counter1 = 0;
int sw3 = 0;
int clr = 1;
#define relay1  13
#define relay2  12
int kamar1 = 0;

///
double harga = 1352; //rupiah / kwh
double watt1_temp;
double watt_kamar_1;
int data_1 = 0;





void setup() {
  // put your setup code here, to run once:
//serial & data parsing
    Serial.begin(9600);
     Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2);
  dataIn="";
//set lcd
    lcd.begin();
    lcd.setCursor(0,0);
  lcd.print("Connecting...");
 // Set encoder pins as inputs	
  	pinMode(SW2, INPUT_PULLUP);
	    pinMode(SW, INPUT_PULLUP);
//set Relay
    pinMode(relay1, OUTPUT);
      pinMode(relay2, OUTPUT);
//blynk
Blynk.begin(auth, ssid, pass);
 timer.setInterval(1000L, myTimerEvent);

 pinMode(2, OUTPUT);

}



//mengambil data dari serial komunikasi arduino meode parsing
void parsingData()
{
int j=0;
//Serial.print("data masuk : ");
Serial.print(dataIn);
dt[j]="";

for(i=1;i<dataIn.length();i++)
  {
    if ((dataIn[i] == '#') || (dataIn[i] == ','))
      {
        j++;
        dt[j]="";       
      }
    else
      {
        dt[j] = dt[j] + dataIn[i];
      }
}

 I1 = dt[1].toFloat() - 0.09;
 I2 = dt[2].toFloat() - 0.09;
 I3= dt[3].toFloat() - 0.09;
 V  = dt[0].toInt() - 373;


}
//perintah membaca button menu
void button1()
{
  int lastStateSW;
	lastStateSW = digitalRead(SW);
  if (lastStateSW == LOW) // light the LED
  {
  counter2 ++;
  delay(600);
  }

}
//perintah membaca button relay
void button2()
{
  int lastStateSW2;
	lastStateSW2 = digitalRead(SW2);
  if (lastStateSW2 == LOW) // light the LED
  {
  counter1 ++;
  delay(600);
  }
  if (counter1 == 2)
  {
    sw3 = 1;
    
  }
  if (counter1 == 1)
  {
    sw3 = 0;
    
    
  }
  if (counter1 > 2)
  {
    counter1 = 1;
  }

}

void menu1()
{
  lcd.setCursor(0,0);
  lcd.print("Voltase : ");
  lcd.setCursor(9,0);
  lcd.print(V);
  lcd.setCursor(12,0);
  lcd.print("   V");
  lcd.setCursor(0,1);
  lcd.print("Arus    : ");
  lcd.setCursor(9,1);
  lcd.print(I3);
  lcd.setCursor(13,1);
  lcd.print("  A");
}

void menu2()
{
  int watt1 = I3 * V;
  lcd.setCursor(0,0);
  lcd.print("Daya Terpakai");
  lcd.setCursor(0,1);
  lcd.print("    ");
  lcd.setCursor(9,1);
  lcd.print(watt1);
  lcd.setCursor(12,1);
  lcd.print("WATT");
  
}
void menu3()
{
  
    lcd.setCursor(0, 0);
    lcd.print("Kamar 1 = ");
    lcd.setCursor(10, 0);
    lcd.print(I1);
    lcd.setCursor(0, 1);
    lcd.print("Kamar 2 = ");
    lcd.setCursor(10, 1);
    lcd.print(I2);

}
void menu4()
{
  button2();
  lcd.setCursor(0, 0);
    lcd.print("Kamar 1 = ");
    lcd.setCursor(10, 0);
    lcd.print(sw3);
}

void statment1()
{
 if(Serial2.available()>0)
   {
    delay(10);
    char inChar = (char)Serial2.read();
    dataIn += inChar;
    digitalWrite(2, LOW);
    if (inChar == '\n') {
    parsing = true; 
    digitalWrite(2, HIGH);
    }
  
  }
  if(parsing)
    {
      parsingData();
      parsing=false;
      dataIn="";
      
    }

    
  if (sw3 == 1)
  {
    digitalWrite(relay1, LOW);
    Blynk.virtualWrite(V0,1);
  }
  if (sw3 == 0)
  {
    digitalWrite(relay1, HIGH);
    Blynk.virtualWrite(V0,0);
  }

}



BLYNK_WRITE(V0) // Executes when the value of virtual pin 0 changes
{
  if(param.asInt() == 1)
  {
    counter1 --;   
    
  }
  else
  {
    counter1 ++;
    
  }
 
 }
  

void myTimerEvent()
{
  
  Blynk.virtualWrite(V4,V);
   int watt2 = I3 * V;
  Blynk.virtualWrite(V6,watt2);
  Blynk.virtualWrite(V2,I3);
  int watt3 = I2 * V;
   Blynk.virtualWrite(V1,watt3);
  hitungharga1();
  Blynk.virtualWrite(V3,watt_kamar_1);
}
  


void hitungharga1()
{
  double harga2 = harga / 3600; //
  int hitungwatt1;
  hitungwatt1 = ((I1 * V) / 1000) * harga2 ;
  watt1_temp = watt1_temp + hitungwatt1;
  data_1 ++;
  if (data_1 == 60);
  {
    watt_kaar_1 = watt1_temp;
    watt1_temp = 0;
  }

}





void loop() 
{
  statment1();
  button1();  
  Blynk.run();
    timer.run();
  


switch (counter2) 
{
  case 1:
    if(clr == 1)
    {
      lcd.clear();
      clr = 0;
      lcd.setCursor(0,0);
      lcd.print("menu x");
    }
   else
    {
     menu1();
    }
  break;

  case 2:
  if(clr == 0)
    {
      lcd.clear();
      clr = 1;
    }
    else 
    {
        menu2();
    }
  break;

 case 3:
   if(clr == 1)
   {
     lcd.clear();
     clr = 0;
   }
   else {
      menu3();
   }
    
  break;
 case 4:
    
    if(clr == 0)
   {
     lcd.clear();
     clr = 1;
   }
   else {
      menu4();
   }
    break;
    
  
  default:
    // if nothing else matches, do the default
    counter2 = 1;
    break;

}

    

}




