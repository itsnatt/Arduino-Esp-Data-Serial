#include <ArduinoJson.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11); // RX, TX

#include "ACS712.h"
ACS712 sensor1(ACS712_30A, A0);
float amper;

double sensorValue1 = 0;
double sensorValue2 = 0;
int crosscount = 0;
int climb_flag = 0;
int val[100];
int max_v = 0;
double VmaxD = 0;
double VeffD = 0;
double Veff = 0;
double volt2;

void volt1() 
{
  for ( int i = 0; i < 100; i++ ) {
    sensorValue1 = analogRead(A3);
    
    if (analogRead(A3) > 511) {
      val[i] = sensorValue1;
      digitalWrite(13, HIGH);
    }
    else {
      val[i] = 0;
    }
    delay(1);
  }

  max_v = 0;

  for ( int i = 0; i < 100; i++ )
  {
    if ( val[i] > max_v )
    {
      max_v = val[i];
    }
    val[i] = 0;
  }
  if (max_v != 0) {
    VmaxD = max_v;
    VeffD = VmaxD / sqrt(2);
    Veff = (((VeffD - 420.76) / -90.24) * -210.2) + 210.2;
  }
  else {
    Veff = 0;
  }
  //Serial.print("Voltage: ");
  if(Veff == 0)
  {
    volt2 = 0; 
  }
  else 
  {
    volt2 = Veff + 40; 
  }
     
  VmaxD = 0;
   delay(100);
    digitalWrite(13, LOW);
}


void setup() {
  Serial.begin(115200);
  mySerial.begin(9600);
  pinMode(13, OUTPUT);
  sensor1.calibrate();
}
 
void loop() 
{
  volt1();
  amper = sensor1.getCurrentAC();
    if (amper < 0.11) {
    amper = 0;
  }
  StaticJsonDocument<200> doc;
  doc["amper"] = amper;
  doc["volt"] = volt2;
    serializeJson(doc, mySerial);
}
