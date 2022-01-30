#define enc1Interrupt 2    //Interrupt pin number
#define enc1PinA      21   //Arduino pin number corresponding to interrupt pin
#define enc1PinB      30   //Digital pin number // Do not use pins 0, 1, 3, 8, 9, 11, 12, 13
#define enc1Res       2048.0

volatile long enc1Ticks = 0;  // Variable used to store encoder ticks

void enc1ReadA()
{
  enc1Ticks += readPinFast(enc1PinB) ? -1:1;
}

void enc1Setup() 
{
  pinMode(enc1PinA, INPUT);      // sets Encoder-1 pin A as input
  pinMode(enc1PinB, INPUT);      // sets Encoder-1 pin B as input
  attachInterrupt(enc1Interrupt, enc1ReadA, RISING); // Executes the function 'enc1ReadA()' at rising edge of signal A from Encoder-1
}

double getEnc1()
{
  return enc1Ticks*2*M_PI/enc1Res;
}
