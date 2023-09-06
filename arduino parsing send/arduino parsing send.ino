
#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11); // RX, TX
String volt;
String strI1, strI2, strI3;

#include "ACS712.h"
ACS712 sensor1(ACS712_30A, A0);
  ACS712 sensor2(ACS712_30A, A1);
    ACS712 sensor3(ACS712_30A, A2);
float I1;
  float I2;
    float I3;

double sensorValue1 = 0;
double sensorValue2 = 0;
int crosscount = 0;
int climb_flag = 0;
int val[100];
int max_v = 0;
double VmaxD = 0;
double VeffD = 0;
double Veff = 0;
int volt2;


void volt1() {

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
  volt2 = Veff + 63;
 
  VmaxD = 0;

  delay(100);
  digitalWrite(13, LOW);
}


void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
   mySerial.begin(9600);
   pinMode(13, OUTPUT);

  sensor1.calibrate();
    sensor2.calibrate();
     sensor3.calibrate();

}

void loop() { // run over and over


volt1();
I1 = sensor1.getCurrentAC();
  I2 = sensor2.getCurrentAC();
    I3 = sensor3.getCurrentAC();
  if (I1 < 0.09) {
    I1 = 0;
  }
  if (I2 < 0.09) {
    I2 = 0;
  }
  if (I3 < 0.09) {
    I3 = 0;
  }
volt  =String (volt2);
strI1 =String(I1);
strI2 =String(I2);
strI3 =String(I3);
Serial.println('*'+volt+','+strI1+','+strI2+','+strI3+'#');
mySerial.println('*'+volt+','+strI1+','+strI2+','+strI3+'#');
}