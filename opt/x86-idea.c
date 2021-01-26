/* Public domain. */

#include "hash127.h"
#define CHUNK HASH127_CHUNK

/* dc: 2 63^ 2 62^+p 2 26^*p 2 26^*p 2 26^*p 2 26^*p 2 23^*p */
#define alpha0 13835058055282163712.0
#define alpha26 928455029464035206174343168.0
#define alpha52 62307562302417931542365955950641152.0
#define alpha78 4181389724724491839037947176121567782371328.0
#define alpha104 280608314367533360295107487881526339773939048251392.0
#define alpha127 2353913150770005286438421033702874906038383291674012942336.0

/* dc: 500k 0.5 127^p */
#define twom127 0.0000000000000000000000000000000000000058774717541114375398436826861112283890933277838604376075437585313920862972736358642578125

/* dc: 2 63^ 2 62^+ 2 32^*p 2 32^*p 2 32^*p */
#define alpha32 59421121885698253195157962752.0
#define alpha64 255211775190703847597530955573826158592.0
#define alpha96 1096126227998177188652763624537212264741949407232.0

/* dc: 100k 0.5 0.5 33^ - p 2 32^*p 2 32^*p 2 32^*p 2 31^*p */
#define beta0 0.499999999883584678173065185546875
#define beta32 2147483647.5
#define beta64 9223372034707292160.0
#define beta96 39614081247908796759917199360.0
#define beta127 85070591710427575237277567459556065280.0

/* dc: 2 52^ 2 51^+p */
#define gamma 6755399441055744.0

#define two32 4294967296.0
#define two64 18446744073709551616.0
#define two96 79228162514264337593543950336.0
#define twom32 0.00000000023283064365386962890625
#define twom64 0.0000000000000000000542101086242752217003726400434970855712890625
#define twom96 0.000000000000000000000000000012621774483536188886587657044524579674771302961744368076324462890625
#define twom1 0.5

#define two31 2147483648.0

#define Q bigr->q
#define C bigr->c

void hash127(int32 out[4],register int32 *buf,register unsigned int len,register struct hash127 *bigr,int32 pad[4])
{
  long double t0, t1, t2, t3, t4, t7;
  long double u0, u1, u2, u3, u4;
  register int i;

  /* r^(len+1) + buf[0] * r^len + buf[1] * r^(len-1) + ... + buf[len-1] * r */

  i = len;
  while (i >= CHUNK + 1) i -= CHUNK + 1;

  buf += i;
  len -= i;
  i = -i;

  if (!(i + CHUNK)) {
    t0 = Q[0];
    t1 = Q[1];
    t2 = Q[2];
    t3 = Q[3];
    t4 = Q[4];
  }
  else {
    t0 = C[0][i + CHUNK - 1];
    t1 = C[1][i + CHUNK - 1];
    t2 = C[2][i + CHUNK - 1];
    t3 = C[3][i + CHUNK - 1];
    t4 = C[4][i + CHUNK - 1];
  }

  /* len is a multiple of CHUNK + 1 */
  /* want (t+u+buf[0]) * r^len + buf[1] * r^(len-1) + ... + buf[len-1] * r */
  /* where t = t0 + t1 + t2 + t3 + t4 */
  /* and u = buf[i] * r^(-i) + ... + buf[-1] * r */

  for (;;) {
    while (i < 0) {
      t7 = buf[i];

      t0 += C[0][i + CHUNK] * t7;
      t1 += C[1][i + CHUNK] * t7;
      t2 += C[2][i + CHUNK] * t7;
      t3 += C[3][i + CHUNK] * t7;
      t4 += C[4][i + CHUNK] * t7;

      ++i;
    }

    t7 = alpha26; t7 += t0; t7 -= alpha26; t1 += t7; t0 -= t7;
    t7 = alpha52; t7 += t1; t7 -= alpha52; t2 += t7; t1 -= t7;
    t7 = alpha78; t7 += t2; t7 -= alpha78; t3 += t7; t2 -= t7;
    t7 = alpha104; t7 += t3; t7 -= alpha104; t4 += t7; t3 -= t7;
    t7 = alpha127; t7 += t4; t7 -= alpha127; t4 -= t7; t7 *= twom127; t0 += t7;
    t7 = alpha26; t7 += t0; t7 -= alpha26; t0 -= t7; t1 += t7;

    if (!len) break;

    t0 += buf[0];

    u0 = t0;
    u1 = t1;
    u2 = t2;
    u3 = t3;
    u4 = t4;

    t0 = u0 * Q[0] + u1 * Q[8] + u2 * Q[7] + u3 * Q[6] + u4 * Q[5];
    t1 = u0 * Q[1] + u1 * Q[0] + u2 * Q[8] + u3 * Q[7] + u4 * Q[6];
    t2 = u0 * Q[2] + u1 * Q[1] + u2 * Q[0] + u3 * Q[8] + u4 * Q[7];
    t3 = u0 * Q[3] + u1 * Q[2] + u2 * Q[1] + u3 * Q[0] + u4 * Q[8];
    t4 = u0 * Q[4] + u1 * Q[3] + u2 * Q[2] + u3 * Q[1] + u4 * Q[0];

    buf += CHUNK + 1;
    len -= CHUNK + 1;
    i = -CHUNK;
  }

  t0 += t1;
  t1 = alpha32; t1 += t0; t1 -= alpha32; t2 += t1; t0 -= t1;
  t1 = t2; t2 += alpha64; t2 -= alpha64; t3 += t2; t1 -= t2;
  t2 = t3; t3 += alpha96; t3 -= alpha96; t2 -= t3; t3 += t4;

  t3 += pad[3] * two96;
  t2 += pad[2] * two64;
  t1 += pad[1] * two32;
  t0 += pad[0];

  t7 = t3;
  t7 *= twom127;
  t7 += twom1;

  /* now t = t0 + t1 + t2 + t3; |t0+t1+t2| < 2^100 */
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

void hash127_expand(register struct hash127 *bigr,int32 r[4])
{
  long double t0, t1, t2, t3, t4, t5, t6, t7;
  register int i;

  asm volatile("fldcw %0"::"m"(0x137f));

  t0 = r[0];
  t1 = r[1] * two32;
  t2 = r[2] * two64;
  t3 = r[3] * two96;
  t4 = 0.0;

  t7 = t0; t7 += alpha26; t7 -= alpha26; t1 += t7; t0 -= t7;
  t7 = t1; t7 += alpha52; t7 -= alpha52; t2 += t7; t1 -= t7;
  t7 = t2; t7 += alpha78; t7 -= alpha78; t3 += t7; t2 -= t7;
  t7 = t3; t7 += alpha104; t7 -= alpha104; t4 += t7; t3 -= t4;
  t7 = t4; t7 += alpha127; t7 -= alpha127; t4 -= t7; t7 *= twom127; t0 += t7;

  Q[0] = t0;
  Q[1] = t1;
  Q[2] = t2;
  Q[3] = t3;
  Q[4] = t4;
  Q[5] = t1 * twom127;
  Q[6] = t2 * twom127;
  Q[7] = t3 * twom127;
  Q[8] = t4 * twom127;

  i = CHUNK - 1;
  do {
    C[0][i] = t0;
    C[1][i] = t1;
    C[2][i] = t2;
    C[3][i] = t3;
    C[4][i] = t4;

    t0 = t0 * Q[0] + t1 * Q[8] + t2 * Q[7] + t3 * Q[6] + t4 * Q[5];
    t1 = C[0][i] * Q[1] + t1 * Q[0] + t2 * Q[8] + t3 * Q[7] + t4 * Q[6];
    t2 = C[0][i] * Q[2] + C[1][i] * Q[1] + t2 * Q[0] + t3 * Q[8] + t4 * Q[7];
    t3 = C[0][i] * Q[3] + C[1][i] * Q[2] + C[2][i] * Q[1] + t3 * Q[0] + t4 * Q[8];
    t4 = C[0][i] * Q[4] + C[1][i] * Q[3] + C[2][i] * Q[2] + C[3][i] * Q[1] + t4 * Q[0];

    t7 = t0; t7 += alpha26; t7 -= alpha26; t0 -= t7; t1 += t7;
    t7 = t1; t7 += alpha52; t7 -= alpha52; t1 -= t7; t2 += t7;
    t7 = t2; t7 += alpha78; t7 -= alpha78; t2 -= t7; t3 += t7;
    t7 = t3; t7 += alpha104; t7 -= alpha104; t3 -= t7; t4 += t7;
    t7 = t4; t7 += alpha127; t7 -= alpha127; t4 -= t7; t7 *= twom127; t0 += t7;
    t7 = t0; t7 += alpha26; t7 -= alpha26; t1 += t7; t0 -= t7;

    --i;
  } while(i >= 0);

  Q[0] = t0;
  Q[1] = t1;
  Q[2] = t2;
  Q[3] = t3;
  Q[4] = t4;
  Q[5] = t1 * twom127;
  Q[6] = t2 * twom127;
  Q[7] = t3 * twom127;
  Q[8] = t4 * twom127;
}
