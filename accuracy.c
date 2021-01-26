#include "hash127.h"

int32 r[4] = { 1234567890, 1234567891, 1234567892, 1234567893 };

int32 out[4];
int32 pad[4] = { 314159265, 358979323, 846264338, 327950288 };

int32 buf[1000];

struct hash127 h[1];

main()
{
  int i;

  hash127_expand(h,r);

  for (i = 0;i < 1000;++i)
    buf[i] = -2147483648UL;

  for (i = 0;i < 1000;++i) {
    hash127(out,buf,i,h,pad);
    printf("%d: %lu %lu %lu %lu\n"
      ,i
      ,2147483648UL + (4294967295UL & (unsigned long) out[0])
      ,2147483648UL + (4294967295UL & (unsigned long) out[1])
      ,2147483648UL + (4294967295UL & (unsigned long) out[2])
      ,2147483648UL + (4294967295UL & (unsigned long) out[3])
      );
  }

  for (i = 0;i < 4000;++i)
    i[(unsigned char *) buf] = i;

  for (i = 0;i < 1000;++i) {
    hash127_little(out,buf,i,h,pad);
    printf("%d: %lu %lu %lu %lu\n"
      ,i
      ,2147483648UL + (4294967295UL & (unsigned long) out[0])
      ,2147483648UL + (4294967295UL & (unsigned long) out[1])
      ,2147483648UL + (4294967295UL & (unsigned long) out[2])
      ,2147483648UL + (4294967295UL & (unsigned long) out[3])
      );
  }

  exit(0);
}
