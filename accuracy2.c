#include <stdio.h>
#include "hash127.h"

int32 r[4];
int32 pad[4];
int32 in[1000];
int32 out[1000];
struct hash127 h[1];

void scramble()
{
  int i;
  int j;
  int32 x = 12345UL;
  int32 sum = 67890UL;

  for (j = 0;j < 10;++j) {
    sum += 2654435769UL;
    for (i = 0;i < 1000;++i) {
      x = x + sum;
      x = (x << 7) | (127 & (x >> 25));
      x = x ^ out[i];
      out[i] = x;
    }
  }
}

main()
{
  int i;
  int loop;

  for (i = 0;i < 1000;++i) out[i] = 0;

  for (loop = 0;loop <= 1048576;++loop) {
    scramble();

    for (i = 0;i < 4;++i) r[i] = out[i + 996];
    for (i = 0;i < 4;++i) pad[i] = out[i + 992];

    if (!(loop & (loop - 1))) {
      printf("%d: %lu %lu %lu %lu\n"
        ,loop
        ,2147483648UL + (4294967295UL & (unsigned long) r[0])
        ,2147483648UL + (4294967295UL & (unsigned long) r[1])
        ,2147483648UL + (4294967295UL & (unsigned long) r[2])
        ,2147483648UL + (4294967295UL & (unsigned long) r[3])
        );
      fflush(stdout);
    }

    hash127_expand(h,r);

    for (i = 0;i < 250;++i)
      hash127(out + i * 4,out,i,h,pad);
  }

  exit(0);
}
