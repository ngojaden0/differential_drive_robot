#include <math.h>
//#include "readPinFast.h"
//#include "enc_1.h"
#include "mot_1.h"
//#include "enc_4.h"
#include "mot_4.h"

#include <ros.h>
#include <geometry_msgs/Twist.h>

#define MAX_VEL 0.05625
#define MAX_ANALOG 255
bool on = false;

void moveForward(float a)
{
  digitalWrite(motAdir, HIGH);
  digitalWrite(motBdir, HIGH);
  analogWrite(motApwm, a);
  analogWrite(motBpwm, a);
}
void moveBackward(float a)
{
  digitalWrite(motAdir, LOW);
  digitalWrite(motBdir, LOW);
  analogWrite(motApwm, a);
  analogWrite(motBpwm, a);
}
void messageCb(const geometry_msgs::Twist& vel)
{
  float x, z;
  x = vel.linear.x;
  x = round(x);
  z = vel.angular.z;
  if(x > 0)
  {
    if(on != true)
    {
        digitalWrite(motAdir, HIGH);
        digitalWrite(motBdir, HIGH);
        analogWrite(motApwm, x);
        analogWrite(motBpwm, x);
    }
    else
    {
      on == true;
    }
  }
  else
  {
    analogWrite(motApwm, 0);
    analogWrite(motBpwm, 0);
  }
}
ros::NodeHandle  nh;
ros::Subscriber<geometry_msgs::Twist> sub("cmd_vel", messageCb);
void setup()
{
  nh.getHardware()-> setBaud(57600);
  nh.initNode();
  nh.subscribe(sub);
  motor1Setup();
  motor4Setup();
}

void loop()
{
  nh.spinOnce();
  delay(100);
}
