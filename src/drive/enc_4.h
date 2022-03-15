#define enc4Interrupt 5    //Interrupt pin number
#define enc4PinA      18   //Arduino pin number corresponding to interrupt pin
#define enc4PinB      7   //Digital pin number // Do not use pins 0, 1, 3, 8, 9, 11, 12, 13
#define enc4Res       2048.0

volatile long enc4Ticks = 0;  // Variable used to store encoder ticks

void enc4ReadA()
{
  enc4Ticks += readPinFast(enc4PinB) ? -1:1;
}

void enc4Setup() 
{
  pinMode(enc4PinA, INPUT);      // sets Encoder-4 pin A as input
  pinMode(enc4PinB, INPUT);      // sets Encoder-4 pin B as input
  attachInterrupt(enc4Interrupt, enc4ReadA, RISING); // Executes the function 'encReadA()' at rising edge of signal A from Encoder-4
}

float getEnc4()
{
  return enc4Ticks;
}
