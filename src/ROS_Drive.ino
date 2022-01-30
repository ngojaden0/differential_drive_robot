
#include <math.h>
#include "readPinFast.h"
#include "enc_1.h"
#include "mot_1.h"
#include "enc_4.h"
#include "mot_4.h"

float qR;
float qL;
int count;
void stopAll()
{
  digitalWrite(motAdir, LOW);
  digitalWrite(motBdir, LOW);
  analogWrite(motApwm, 0);
  analogWrite(motBpwm, 0); 
}

void setup()
{
  Serial.begin(115200);
  enc1Setup();
  enc4Setup();
  motor1Setup();
  motor4Setup();
  stopAll();  
}
void loop()
{
    if(Serial.available() > 0)
    {
       char a = Serial.read(); // characters received through serial, one at a time.
        if(a == 'w')
	{
		digitalWrite(motAdir, LOW);
		digitalWrite(motBdir, LOW);
		analogWrite(motApwm, 255);
		analogWrite(motBpwm, 255);
	}
	else if(a == 'a')
        {
		digitalWrite(motAdir, LOW);
		digitalWrite(motBdir, HIGH);
                analogWrite(motApwm, 255);
                analogWrite(motBpwm, 255);
        }
	else if(a == 's')
        {
                digitalWrite(motAdir, HIGH);
                digitalWrite(motBdir, HIGH);
                analogWrite(motApwm, 255);
                analogWrite(motBpwm, 255);
        }
	else if(a == 'd')
        {
		digitalWrite(motAdir, HIGH);
                digitalWrite(motBdir, LOW);
                analogWrite(motApwm, 255);
                analogWrite(motBpwm, 255);
        }
	else if(a == ' ')
		stopAll();
        
    }
    //count++;
  //  if(count%100 == 0)
//	Serial.println("qL[deg]          qR[deg]");
    //qR = getEnc4()*180/M_PI;
    //qL = getEnc1()*180/M_PI;
    //Serial.print(qL);
    //Serial.print("          ");
    //Serial.println(qR);
    //delay(50);
}

