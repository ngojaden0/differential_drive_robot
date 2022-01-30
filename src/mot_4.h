#define motBpwm 11
#define motBbrk 8
#define motBdir 13

void motor4Setup()
{
  pinMode(motBpwm, OUTPUT);
  pinMode(motBbrk, OUTPUT);
  pinMode(motBdir, OUTPUT);
}
