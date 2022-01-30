#define motApwm 3
#define motAbrk 9
#define motAdir 12

void motor1Setup()
{
  pinMode(motApwm, OUTPUT);
  pinMode(motAbrk, OUTPUT);
  pinMode(motAdir, OUTPUT);
}
