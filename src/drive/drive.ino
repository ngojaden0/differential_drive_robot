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
#define LOOPTIME 100.0
#define WHEEL_RADIUS 0.0762
#define RESOLUTION 2048.0

float lval;
float rval;
float encR;
float encL;
float old_encR = 0;
float old_encL = 0;
float angular_vel_encR;
float angular_vel_encL;
float linear_vel_encR;
float linear_vel_encL;
float kp = 1;
float kd = 0;
float ki = 0;
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
  encRSetup();
  encLSetup();
}

void loop()
{
  nh.spinOnce();
  if((millis()-lastTime) >= LOOPTIME)
  {
    encR = getEncR();
    encL = getEncL();
    angular_vel_encR = abs(encR-old_encR)/LOOPTIME/RESOLUTION*1000.0*2*M_PI;
    angular_vel_encL = abs(encL-old_encL)/LOOPTIME/RESOLUTION*1000.0*2*M_PI;
    if(encR < old_encR)
      angular_vel_encR = -1*angular_vel_encR;
    if(encL < old_encL)
      angular_vel_encL = -1*angular_vel_encL;
    linear_vel_encR = WHEEL_RADIUS*angular_vel_encR;
    linear_vel_encL = WHEEL_RADIUS*angular_vel_encL;
    publishSpeed();
    old_encR = encR;
    old_encL = encL;
  }
  
}
void publishSpeed()
{
  speed_msg.header.stamp = nh.now();      //timestamp for odometry data
  speed_msg.vector.x = angular_vel_encR;
  speed_msg.vector.y = angular_vel_encL;
  speed_pub.publish(&speed_msg);
  nh.spinOnce();
  nh.loginfo("Publishing odometry");
}
