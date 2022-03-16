#define encRInterrupt 2    //Interrupt pin number
#define encRPinA      21   //Arduino pin number corresponding to interrupt pin
#define encRPinB      31   //Digital pin number // Do not use pins 0, 1, 3, 8, 9, 11, 12, 13
#define encRRes       2048.0

volatile long encRTicks = 0;  // Variable used to store encoder ticks

void encRReadA()
{
  encRTicks += readPinFast(encRPinB) ? -1:1;
}

void encRSetup() 
{
  pinMode(encRPinA, INPUT);      // sets Encoder-1 pin A as input
  pinMode(encRPinB, INPUT);      // sets Encoder-1 pin B as input
  attachInterrupt(encRInterrupt, encRReadA, RISING); // Executes the function 'enc1ReadA()' at rising edge of signal A from Encoder-1
}

float getEncR()
{
  return -1*encRTicks;
}
