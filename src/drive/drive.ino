
#include <math.h>
#include "readPinFast.h"
#include "enc_1.h"
#include "mot_1.h"
#include "enc_4.h"
#include "mot_4.h"

#include <ros.h>
#include <geometry_msgs/Twist.h>

ros::NodeHandle  nh;
float x;
void messageCb(const geometry_msgs::Twist& vel){
  digitalWrite(13, HIGH-digitalRead(13));
  x = vel.linear.x - 0.5;
  
}

ros::Subscriber<geometry_msgs::Twist> sub("cmd_vel", messageCb);

void setup()
{
  pinMode(13, OUTPUT);
  nh.initNode();
  nh.subscribe(sub);
}

void loop()
{
  nh.spinOnce();
  delay(100);
}