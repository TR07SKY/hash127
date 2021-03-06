/* Public domain. */

#include "hash127.h"
#define CHUNK HASH127_CHUNK

/* dc: 2 52^2 51^+p2 16^*p2 16^*p2 16^*p2 16^*p2 16^*p2 16^*p2 16^*p2 15^*p */
#define alpha0 6755399441055744.0
#define alpha16 442721857769029238784.0
#define alpha32 29014219670751100192948224.0
#define alpha48 1901475900342344102245054808064.0
#define alpha64 124615124604835863084731911901282304.0
#define alpha80 8166776806102523123120990578362437074944.0
#define alpha96 535217884764734955396857238543560676143529984.0
#define alpha112 35076039295941670036888435985190792471742381031424.0
#define alpha127 1149371655649416643768760270362731887714054341637701632.0

/* dc: 100k 0.5 0.5 33^ - p 2 32^*p 2 32^*p 2 32^*p 2 31^*p */
#define beta0 0.499999999883584678173065185546875
#define beta32 2147483647.5
#define beta64 9223372034707292160.0
#define beta96 39614081247908796759917199360.0
#define beta127 85070591710427575237277567459556065280.0

/* dc: 500k 0.5 127^p */
#define twom127 0.0000000000000000000000000000000000000058774717541114375398436826861112283890933277838604376075437585313920862972736358642578125

#define two31 2147483648.0
#define two32 4294967296.0
#define two64 18446744073709551616.0
#define two96 79228162514264337593543950336.0
#define twom32 0.00000000023283064365386962890625
#define twom64 0.0000000000000000000542101086242752217003726400434970855712890625
#define twom96 0.000000000000000000000000000012621774483536188886587657044524579674771302961744368076324462890625
#define twom1 0.5

#define Q bigr->q
#define C bigr->c

static int32 load(int32 x,int flaglittle)
{
  if (flaglittle) {
    unsigned long y;
    y = 3[(unsigned char *) &x]; y <<= 8;
    y += 2[(unsigned char *) &x]; y <<= 8;
    y += 1[(unsigned char *) &x]; y <<= 8;
    y += 0[(unsigned char *) &x];
    return y;
  }

  return x;
}

static void doit(int32 out[4],int32 *buf,unsigned int len,struct hash127 *bigr,int32 pad[4],int flaglittle)
{
  double t0, t1, t2, t3, t4, t5, t6, t7;
  double u0, u2, u4, u6;
  int i;

  /* r^(len+1) + buf[0] * r^len + buf[1] * r^(len-1) + ... + buf[len-1] * r */

  i = len;
  while (i >= CHUNK + 1) i -= CHUNK + 1;

  buf += i;
  len -= i;
  i = -i;

  if (!(CHUNK + i)) {
    t0 = Q[0];
    t1 = Q[1];
    t2 = Q[2];
    t3 = Q[3];
    t4 = Q[4];
    t5 = Q[5];
    t6 = Q[6];
    t7 = Q[7];
  }
  else {
    t0 = C[0][CHUNK - 1 + i];
    t1 = C[1][CHUNK - 1 + i];
    t2 = C[2][CHUNK - 1 + i];
    t3 = C[3][CHUNK - 1 + i];
    t4 = C[4][CHUNK - 1 + i];
    t5 = C[5][CHUNK - 1 + i];
    t6 = C[6][CHUNK - 1 + i];
    t7 = C[7][CHUNK - 1 + i];
  }

  /* len is a multiple of CHUNK + 1 */
  /* i <= 0 */
  /* want (t+u+buf[0]) * r^len + buf[1] * r^(len-1) + ... + buf[len-1] * r */
  /* where t = t0 + t1 + t2 + t3 + t4 */
  /* and u = buf[i] * r^(-i) + ... + buf[-1] * r */

  for (;;) {
    while (i < 0) {
      u0 = load(buf[i],flaglittle);
  
      t0 += C[0][CHUNK + i] * u0;
      t1 += C[1][CHUNK + i] * u0;
      t2 += C[2][CHUNK + i] * u0;
      t3 += C[3][CHUNK + i] * u0;
      t4 += C[4][CHUNK + i] * u0;
      t5 += C[5][CHUNK + i] * u0;
      t6 += C[6][CHUNK + i] * u0;
      t7 += C[7][CHUNK + i] * u0;
  
      ++i;
    }

    u0 = t0; u0 += alpha16; u0 -= alpha16; t0 -= u0; t1 += u0;
    u0 = t1; u0 += alpha32; u0 -= alpha32; t1 -= u0; t2 += u0;
    u0 = t2; u0 += alpha48; u0 -= alpha48; t2 -= u0; t3 += u0;
    u0 = t3; u0 += alpha64; u0 -= alpha64; t3 -= u0; t4 += u0;
    u0 = t4; u0 += alpha80; u0 -= alpha80; t4 -= u0; t5 += u0;
    u0 = t5; u0 += alpha96; u0 -= alpha96; t5 -= u0; t6 += u0;
    u0 = t6; u0 += alpha112; u0 -= alpha112; t6 -= u0; t7 += u0;
    u0 = t7; u0 += alpha127; u0 -= alpha127; t7 -= u0; u0 *= twom127; t0 += u0;
    u0 = t0; u0 += alpha16; u0 -= alpha16; t0 -= u0; t1 += u0;
    u0 = t1; u0 += alpha32; u0 -= alpha32; t1 -= u0; t2 += u0;

    u0 = t0 + t1;
    u2 = t2 + t3;
    u4 = t4 + t5;
    u6 = t6 + t7;

    if (!len) break;

    u0 += load(buf[0],flaglittle);

    t0 = u0 * Q[0] + u2 * Q[12] + u4 * Q[10] + u6 * Q[8];
    t1 = u0 * Q[1] + u2 * Q[13] + u4 * Q[11] + u6 * Q[9];
    t2 = u0 * Q[2] + u2 * Q[0] + u4 * Q[12] + u6 * Q[10];
    t3 = u0 * Q[3] + u2 * Q[1] + u4 * Q[13] + u6 * Q[11];
    t4 = u0 * Q[4] + u2 * Q[2] + u4 * Q[0] + u6 * Q[12];
    t5 = u0 * Q[5] + u2 * Q[3] + u4 * Q[1] + u6 * Q[13];
    t6 = u0 * Q[6] + u2 * Q[4] + u4 * Q[2] + u6 * Q[0];
    t7 = u0 * Q[7] + u2 * Q[5] + u4 * Q[3] + u6 * Q[1];

    buf += CHUNK + 1;
    len -= CHUNK + 1;
    i = -CHUNK;
  }

  t0 = u0 + pad[0];
  t1 = u2 + pad[1] * two32;
  t2 = u4 + pad[2] * two64;
  t3 = u6 + pad[3] * two96;

  t7 = t3 * twom127;
  t7 += twom1;

  /* now t = t0 + t1 + t2 + t3; terms not too big */
  /* t0 in Z, t1 in 2^32 Z, t2 in 2^64 Z, t3 in 2^96 Z */
  /* t7 = 1/2 + t3 / 2^127 */
  /* floor(t/p) = floor(2^(-127) (t0+t1+t2+t3+t7)) */

  t7 -= beta0; t7 += alpha0; t7 -= alpha0;
  t7 += t0; t7 -= beta32; t7 += alpha32; t7 -= alpha32;
  t7 += t1; t7 -= beta64; t7 += alpha64; t7 -= alpha64;
  t7 += t2; t7 -= beta96; t7 += alpha96; t7 -= alpha96;
  t7 += t3; t7 -= beta127; t7 += alpha127; t7 -= alpha127;

  /* now floor(t/p) = 2^(-127) t7 so t mod p = t - t7 + 2^(-127) t7 */

  t3 -= t7;
  t7 *= twom127;
  t0 += t7;

  t7 = t0; t7 -= beta32; t7 += alpha32; t7 -= alpha32; t1 += t7; t0 -= t7;
  t7 = t1; t7 -= beta64; t7 += alpha64; t7 -= alpha64; t2 += t7; t1 -= t7;
  t7 = t2; t7 -= beta96; t7 += alpha96; t7 -= alpha96; t3 += t7; t2 -= t7;

  t1 *= twom32;
  t2 *= twom64;
  t3 *= twom96;

  t0 -= two31;
  t1 -= two31;
  t2 -= two31;
  t3 -= two31;

  out[0] = (int32) t0;
  out[1] = (int32) t1;
  out[2] = (int32) t2;
  out[3] = (int32) t3;
}

void hash127(int32 out[4],int32 *buf,unsigned int len,struct hash127 *bigr,int32 pad[4])
{
  doit(out,buf,len,bigr,pad,0);
}

void hash127_little(int32 out[4],int32 *buf,unsigned int len,struct hash127 *bigr,int32 pad[4])
{
  doit(out,buf,len,bigr,pad,1);
}

void hash127_expand(struct hash127 *bigr,int32 r[4])
{
  double t0, t1, t2, t3, t4, t5, t6, t7;
  double u0, u1, u2, u3, u4, u5, u6, u7;
  int i;

  t0 = r[0];
  t2 = r[1] * two32;
  t4 = r[2] * two64;
  t6 = r[3] * two96;

  Q[0] = t0;
  Q[1] = t2; Q[4] = t2 * twom127;
  Q[2] = t4; Q[5] = t4 * twom127;
  Q[3] = t6; Q[6] = t6 * twom127;

  t1 = t0; t1 += alpha16; t1 -= alpha16; t0 -= t1;
  t3 = t2; t3 += alpha48; t3 -= alpha48; t2 -= t3;
  t5 = t4; t5 += alpha80; t5 -= alpha80; t4 -= t5;
  t7 = t6; t7 += alpha112; t7 -= alpha112; t6 -= t7;

  i = CHUNK - 1;
  do {
    C[0][i] = t0;
    C[1][i] = t1;
    C[2][i] = t2;
    C[3][i] = t3;
    C[4][i] = t4;
    C[5][i] = t5;
    C[6][i] = t6;
    C[7][i] = t7;

    u0 = t0;
    u2 = t2;
    u4 = t4;
    u6 = t6;

    t0 = u0 * Q[0] + u2 * Q[6] + u4 * Q[5] + u6 * Q[4];
    t2 = u0 * Q[1] + u2 * Q[0] + u4 * Q[6] + u6 * Q[5];
    t4 = u0 * Q[2] + u2 * Q[1] + u4 * Q[0] + u6 * Q[6];
    t6 = u0 * Q[3] + u2 * Q[2] + u4 * Q[1] + u6 * Q[0];

    u1 = t1;
    u3 = t3;
    u5 = t5;
    u7 = t7;

    t1 = u1 * Q[0] + u3 * Q[6] + u5 * Q[5] + u7 * Q[4];
    t3 = u1 * Q[1] + u3 * Q[0] + u5 * Q[6] + u7 * Q[5];
    t5 = u1 * Q[2] + u3 * Q[1] + u5 * Q[0] + u7 * Q[6];
    t7 = u1 * Q[3] + u3 * Q[2] + u5 * Q[1] + u7 * Q[0];

    u0 = t0; u0 += alpha16; u0 -= alpha16; t0 -= u0; t1 += u0;
    u0 = t1; u0 += alpha32; u0 -= alpha32; t1 -= u0; t2 += u0;
    u0 = t2; u0 += alpha48; u0 -= alpha48; t2 -= u0; t3 += u0;
    u0 = t3; u0 += alpha64; u0 -= alpha64; t3 -= u0; t4 += u0;
    u0 = t4; u0 += alpha80; u0 -= alpha80; t4 -= u0; t5 += u0;
    u0 = t5; u0 += alpha96; u0 -= alpha96; t5 -= u0; t6 += u0;
    u0 = t6; u0 += alpha112; u0 -= alpha112; t6 -= u0; t7 += u0;
    u0 = t7; u0 += alpha127; u0 -= alpha127; t7 -= u0; u0 *= twom127; t0 += u0;
    u0 = t0; u0 += alpha16; u0 -= alpha16; t0 -= u0; t1 += u0;
    u0 = t1; u0 += alpha32; u0 -= alpha32; t1 -= u0; t2 += u0;

    --i;
  } while(i >= 0);

  Q[0] = t0;
  Q[1] = t1;
  Q[2] = t2; Q[8] = t2 * twom127;
  Q[3] = t3; Q[9] = t3 * twom127;
  Q[4] = t4; Q[10] = t4 * twom127;
  Q[5] = t5; Q[11] = t5 * twom127;
  Q[6] = t6; Q[12] = t6 * twom127;
  Q[7] = t7; Q[13] = t7 * twom127;
}
