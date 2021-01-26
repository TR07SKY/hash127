#include <signal.h>

void nope()
{
  exit(1);
}

main()
{
  unsigned long x[5];

  signal(SIGILL,nope);

  x[0] = 0;
  x[1] = 0;
  x[2] = 0;
  x[3] = 0;
  x[4] = 0;

  asm volatile("rpcc %0" : "r="(x[0]));
  asm volatile("rpcc %0" : "r="(x[1]));
  asm volatile("rpcc %0" : "r="(x[2]));
  asm volatile("rpcc %0" : "r="(x[3]));
  asm volatile("rpcc %0" : "r="(x[4]));

  if (x[0] == x[4]) nope();

  return 0;
}
