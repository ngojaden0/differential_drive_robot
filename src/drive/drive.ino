#include <math.h>
#include "readPinFast.h"
#include "enc_1.h"
#include "mot_1.h"
#include "enc_4.h"
#include "mot_4.h"
#include <ros.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Vector3Stamped.h>
#include <std_msgs/String.h>
#include <ros/time.h>

#define MAX_VEL 0.05625
#define MAX_ANALOG 255

float lval;
float rval;
float enc1;
float enc4;
unsigned long lastTime = 0;
void moveRobot(float x, float z)
{
  x = abs(x);
  x = constrain(x, 0, MAX_ANALOG);
  z = constrain(z, -1*MAX_ANALOG, MAX_ANALOG);
  
  if(z > 0)
  {
    z = abs(z);
    rval = 1;
    lval = (MAX_ANALOG-z)/MAX_ANALOG;
  }
  else if(z < 0)
  {
    z = abs(z);
    rval = (MAX_ANALOG-z)/MAX_ANALOG;
    lval = 1;
  }
  else
  {
    lval = 1;
    rval = 1;
  }

  analogWrite(motApwm, x*rval);
  analogWrite(motBpwm, x*lval);
}
void twistRobot(int z)
{
  z = abs(z);
  z = constrain(z, 0, MAX_ANALOG);
  analogWrite(motApwm, z);
  analogWrite(motBpwm, z);
}
void stopRobot()
{
  analogWrite(motApwm, 0);
  analogWrite(motBpwm, 0);
}
void messageCb(const geometry_msgs::Twist& vel)
{
  float x, z;
  x = vel.linear.x;
  z = vel.angular.z;
  x = round(x);
  z = round(z);
  
  if(x > 0)
  {
    digitalWrite(motAdir, LOW);
    digitalWrite(motBdir, LOW);
    moveRobot(x,z);
  }
  else if(x < 0)
  {
    digitalWrite(motAdir, HIGH);
    digitalWrite(motBdir, HIGH);
    moveRobot(x,z);
  }
  else
  {
    if(z > 0)
    {
      digitalWrite(motAdir, LOW);
      digitalWrite(motBdir, HIGH);
      twistRobot(z);
    }
    else if(z < 0)
    {
      digitalWrite(motAdir, HIGH);
      digitalWrite(motBdir, LOW);
      twistRobot(z);
    }
    else
    stopRobot();
  }
}
ros::NodeHandle  nh;
ros::Subscriber<geometry_msgs::Twist> sub("cmd_vel", messageCb);
geometry_msgs::Vector3Stamped speed_msg;
ros::Publisher speed_pub("speed", &speed_msg);
void setup()
{
  nh.getHardware()-> setBaud(57600);
  nh.initNode();
  nh.subscribe(sub);
  nh.advertise(speed_pub);
  motor1Setup();
  motor4Setup();
  enc1Setup();
  enc4Setup();
}

void loop()
{
  nh.spinOnce();
  enc1 = getEnc1();
  enc4 = getEnc4();
  publishSpeed();
  delay(100);
}
void publishSpeed()
{
  speed_msg.header.stamp = nh.now();      //timestamp for odometry data
  speed_msg.vector.x = enc1;
  speed_msg.vector.y = enc4;
  speed_pub.publish(&speed_msg);
  nh.spinOnce();
  nh.loginfo("Publishing odometry");
}
