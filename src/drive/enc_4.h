#define encLInterrupt 5    //Interrupt pin number
#define encLPinA      18   //Arduino pin number corresponding to interrupt pin
#define encLPinB      7   //Digital pin number // Do not use pins 0, 1, 3, 8, 9, 11, 12, 13
#define encLRes       2048.0

volatile long encLTicks = 0;  // Variable used to store encoder ticks

void encLReadA()
{
  encLTicks += readPinFast(encLPinB) ? -1:1;
}

void encLSetup() 
{
  pinMode(encLPinA, INPUT);      // sets Encoder-4 pin A as input
  pinMode(encLPinB, INPUT);      // sets Encoder-4 pin B as input
  attachInterrupt(encLInterrupt, encLReadA, RISING); // Executes the function 'encReadA()' at rising edge of signal A from Encoder-4
}

float getEncL()
{
  return encLTicks;
}
