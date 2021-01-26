/* Public domain. */

#include "hash127.h"
#define CHUNK HASH127_CHUNK

#define twom127 bigr->constants[0]
#define alpha26 bigr->constants[1]
#define alpha52 bigr->constants[2]
#define alpha78 bigr->constants[3]
#define alpha104 bigr->constants[4]
#define alpha127 bigr->constants[5]
#define alpha0 bigr->constants[6]
#define alpha32 bigr->constants[7]
#define alpha64 bigr->constants[8]
#define alpha96 bigr->constants[9]
#define beta0 bigr->constants[10]
#define beta32 bigr->constants[11]
#define beta64 bigr->constants[12]
#define beta96 bigr->constants[13]
#define beta127 bigr->constants[14]
#define two32 bigr->constants[15]
#define two64 bigr->constants[16]
#define two96 bigr->constants[17]
#define twom32 bigr->constants[18]
#define twom64 bigr->constants[19]
#define twom96 bigr->constants[20]
#define gamma bigr->constants[21]
#define twom1 bigr->constants[22]

#define T bigr->tmp
#define Q bigr->q
#define C bigr->c
#define VT *(volatile double *)&T
#define VQ *(volatile double *)&Q
#define VC *(volatile double *)&C

void hash127(int32 out[4],register int32 *buf,register unsigned int len,register struct hash127 *bigr,int32 pad[4])
{
  register double t0, t1, t2, t3, t4, t5, t6, t7;
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

  while (i & 3) {
    t7 = buf[i];
    t5 = VC[0][i + CHUNK];
    t5 *= t7;
    t0 += t5;
    t5 = VC[1][i + CHUNK];
    t5 *= t7;
    t1 += t5;
    t5 = VC[2][i + CHUNK];
    t5 *= t7;
    t2 += t5;
    t5 = VC[3][i + CHUNK];
    t5 *= t7;
    t3 += t5;
    t7 *= C[4][i + CHUNK];
    t4 += t7;

    i += 1;
  }

  while (i & 31) {
    t7 = buf[i];
    t5 = VC[0][i + CHUNK];
    t5 *= t7;
    t0 += t5;
    t6 = VC[1][i + CHUNK];
    t6 *= t7;
    t1 += t6;
    t5 = VC[2][i + CHUNK];
    t5 *= t7;
    t2 += t5;
    t6 = VC[3][i + CHUNK];
    t6 *= t7;
    t3 += t6;
    t7 *= C[4][i + CHUNK];
    t4 += t7;

    t7 = buf[i + 1];
    t5 = VC[0][i + 1 + CHUNK];
    t5 *= t7;
    t0 += t5;
    t6 = VC[1][i + 1 + CHUNK];
    t6 *= t7;
    t1 += t6;
    t5 = VC[2][i + 1 + CHUNK];
    t5 *= t7;
    t2 += t5;
    t6 = VC[3][i + 1 + CHUNK];
    t6 *= t7;
    t3 += t6;
    t7 *= C[4][i + 1 + CHUNK];
    t4 += t7;

    t7 = buf[i + 2];
    t5 = VC[0][i + 2 + CHUNK];
    t5 *= t7;
    t0 += t5;
    t6 = VC[1][i + 2 + CHUNK];
    t6 *= t7;
    t1 += t6;
    t5 = VC[2][i + 2 + CHUNK];
    t5 *= t7;
    t2 += t5;
    t6 = VC[3][i + 2 + CHUNK];
    t6 *= t7;
    t3 += t6;
    t7 *= C[4][i + 2 + CHUNK];
    t4 += t7;

    t7 = buf[i + 3];
    t5 = VC[0][i + 3 + CHUNK];
    t5 *= t7;
    t0 += t5;
    t6 = VC[1][i + 3 + CHUNK];
    t6 *= t7;
    t1 += t6;
    t5 = VC[2][i + 3 + CHUNK];
    t5 *= t7;
    t2 += t5;
    t6 = VC[3][i + 3 + CHUNK];
    t6 *= t7;
    t3 += t6;
    t7 *= C[4][i + 3 + CHUNK];
    t4 += t7;

    i += 4;
  }

  for (;;) {
    for (;;) {
      /* i is a multiple of 32 */
      if (i >= 0) break;

      t7 = buf[i];
      t5 = VC[0][i + CHUNK];
      t5 *= t7;
      t0 += t5;
      t5 = VC[1][i + CHUNK];
      t5 *= t7;
      t1 += t5;
      t5 = VC[2][i + CHUNK];
      t5 *= t7;
      t2 += t5;
      t5 = VC[3][i + CHUNK];
      t5 *= t7;
      t3 += t5;
      t5 = VC[4][i + CHUNK];
      t5 *= t7;
      t4 += t5;

      t7 = buf[i + 1];
      t5 = VC[0][i + 1 + CHUNK];
      t5 *= t7;
      t0 += t5;
      t5 = VC[1][i + 1 + CHUNK];
      t5 *= t7;
      t1 += t5;
      t5 = VC[2][i + 1 + CHUNK];
      t5 *= t7;
      t2 += t5;
      t5 = VC[3][i + 1 + CHUNK];
      t5 *= t7;
      t3 += t5;
      t7 *= C[4][i + 1 + CHUNK];
      t4 += t7;

      t7 = buf[i + 2];
      t5 = VC[0][i + 2 + CHUNK];
      t5 *= t7;
      t0 += t5;
      t6 = VC[1][i + 2 + CHUNK];
      t6 *= t7;
      t1 += t6;
      t5 = VC[2][i + 2 + CHUNK];
      t5 *= t7;
      t2 += t5;
      t6 = VC[3][i + 2 + CHUNK];
      t6 *= t7;
      t3 += t6;
      t7 *= C[4][i + 2 + CHUNK];
      t4 += t7;

      t7 = buf[i + 3];
      t5 = VC[0][i + 3 + CHUNK];
      t5 *= t7;
      t0 += t5;
      t6 = VC[1][i + 3 + CHUNK];
      t6 *= t7;
      t1 += t6;
      t5 = VC[2][i + 3 + CHUNK];
      t5 *= t7;
      t2 += t5;
      t6 = VC[3][i + 3 + CHUNK];
      t6 *= t7;
      t3 += t6;
      t7 *= C[4][i + 3 + CHUNK];
      t4 += t7;

      t7 = buf[i + 4];
      t5 = VC[0][i + 4 + CHUNK];
      t5 *= t7;
      t0 += t5;
      t6 = VC[1][i + 4 + CHUNK];
      t6 *= t7;
      t1 += t6;
      t5 = VC[2][i + 4 + CHUNK];
      t5 *= t7;
      t2 += t5;
      t6 = VC[3][i + 4 + CHUNK];
      t6 *= t7;
      t3 += t6;
      t7 *= C[4][i + 4 + CHUNK];
      t4 += t7;

      t7 = buf[i + 5];
      t5 = VC[0][i + 5 + CHUNK];
      t5 *= t7;
      t0 += t5;
      t6 = VC[1][i + 5 + CHUNK];
      t6 *= t7;
      t1 += t6;
      t5 = VC[2][i + 5 + CHUNK];
      t5 *= t7;
      t2 += t5;
      t6 = VC[3][i + 5 + CHUNK];
      t6 *= t7;
      t3 += t6;
      t7 *= C[4][i + 5 + CHUNK];
      t4 += t7;

      t7 = buf[i + 6];
      t5 = VC[0][i + 6 + CHUNK];
      t5 *= t7;
      t0 += t5;
      t6 = VC[1][i + 6 + CHUNK];
      t6 *= t7;
      t1 += t6;
      t5 = VC[2][i + 6 + CHUNK];
      t5 *= t7;
      t2 += t5;
      t6 = VC[3][i + 6 + CHUNK];
      t6 *= t7;
      t3 += t6;
      t7 *= C[4][i + 6 + CHUNK];
      t4 += t7;

      t7 = buf[i + 7];
      t5 = VC[0][i + 7 + CHUNK];
      t5 *= t7;
      t0 += t5;
      t6 = VC[1][i + 7 + CHUNK];
      t6 *= t7;
      t1 += t6;
      t5 = VC[2][i + 7 + CHUNK];
      t5 *= t7;
      t2 += t5;
      t6 = VC[3][i + 7 + CHUNK];
      t6 *= t7;
      t3 += t6;
      t7 *= C[4][i + 7 + CHUNK];
      t4 += t7;

      t7 = buf[i + 8];
      t5 = VC[0][i + 8 + CHUNK];
      t5 *= t7;
      t0 += t5;
      t6 = VC[1][i + 8 + CHUNK];
      t6 *= t7;
      t1 += t6;
      t5 = VC[2][i + 8 + CHUNK];
      t5 *= t7;
      t2 += t5;
      t6 = VC[3][i + 8 + CHUNK];
      t6 *= t7;
      t3 += t6;
      t7 *= C[4][i + 8 + CHUNK];
      t4 += t7;

      t7 = buf[i + 9];
      t5 = VC[0][i + 9 + CHUNK];
      t5 *= t7;
      t0 += t5;
      t6 = VC[1][i + 9 + CHUNK];
      t6 *= t7;
      t1 += t6;
      t5 = VC[2][i + 9 + CHUNK];
      t5 *= t7;
      t2 += t5;
      t6 = VC[3][i + 9 + CHUNK];
      t6 *= t7;
      t3 += t6;
      t7 *= C[4][i + 9 + CHUNK];
      t4 += t7;

      t7 = buf[i + 10];
      t5 = VC[0][i + 10 + CHUNK];
      t5 *= t7;
      t0 += t5;
      t6 = VC[1][i + 10 + CHUNK];
      t6 *= t7;
      t1 += t6;
      t5 = VC[2][i + 10 + CHUNK];
      t5 *= t7;
      t2 += t5;
      t6 = VC[3][i + 10 + CHUNK];
      t6 *= t7;
      t3 += t6;
      t7 *= C[4][i + 10 + CHUNK];
      t4 += t7;

      t7 = buf[i + 11];
      t5 = VC[0][i + 11 + CHUNK];
      t5 *= t7;
      t0 += t5;
      t6 = VC[1][i + 11 + CHUNK];
      t6 *= t7;
      t1 += t6;
      t5 = VC[2][i + 11 + CHUNK];
      t5 *= t7;
      t2 += t5;
      t6 = VC[3][i + 11 + CHUNK];
      t6 *= t7;
      t3 += t6;
      t7 *= C[4][i + 11 + CHUNK];
      t4 += t7;

      t7 = buf[i + 12];
      t5 = VC[0][i + 12 + CHUNK];
      t5 *= t7;
      t0 += t5;
      t6 = VC[1][i + 12 + CHUNK];
      t6 *= t7;
      t1 += t6;
      t5 = VC[2][i + 12 + CHUNK];
      t5 *= t7;
      t2 += t5;
      t6 = VC[3][i + 12 + CHUNK];
      t6 *= t7;
      t3 += t6;
      t7 *= C[4][i + 12 + CHUNK];
      t4 += t7;

      t7 = buf[i + 13];
      t5 = VC[0][i + 13 + CHUNK];
      t5 *= t7;
      t0 += t5;
      t6 = VC[1][i + 13 + CHUNK];
      t6 *= t7;
      t1 += t6;
      t5 = VC[2][i + 13 + CHUNK];
      t5 *= t7;
      t2 += t5;
      t6 = VC[3][i + 13 + CHUNK];
      t6 *= t7;
      t3 += t6;
      t7 *= C[4][i + 13 + CHUNK];
      t4 += t7;

      t7 = buf[i + 14];
      t5 = VC[0][i + 14 + CHUNK];
      t5 *= t7;
      t0 += t5;
      t6 = VC[1][i + 14 + CHUNK];
      t6 *= t7;
      t1 += t6;
      t5 = VC[2][i + 14 + CHUNK];
      t5 *= t7;
      t2 += t5;
      t6 = VC[3][i + 14 + CHUNK];
      t6 *= t7;
      t3 += t6;
      t7 *= C[4][i + 14 + CHUNK];
      t4 += t7;

      t7 = buf[i + 15];
      t5 = VC[0][i + 15 + CHUNK];
      t5 *= t7;
      t0 += t5;
      t6 = VC[1][i + 15 + CHUNK];
      t6 *= t7;
      t1 += t6;
      t5 = VC[2][i + 15 + CHUNK];
      t5 *= t7;
      t2 += t5;
      t6 = VC[3][i + 15 + CHUNK];
      t6 *= t7;
      t3 += t6;
      t7 *= C[4][i + 15 + CHUNK];
      t4 += t7;

      t7 = buf[i + 16];
      t5 = VC[0][i + 16 + CHUNK];
      t5 *= t7;
      t0 += t5;
      t6 = VC[1][i + 16 + CHUNK];
      t6 *= t7;
      t1 += t6;
      t5 = VC[2][i + 16 + CHUNK];
      t5 *= t7;
      t2 += t5;
      t6 = VC[3][i + 16 + CHUNK];
      t6 *= t7;
      t3 += t6;
      t7 *= C[4][i + 16 + CHUNK];
      t4 += t7;

      t7 = buf[i + 17];
      t5 = VC[0][i + 17 + CHUNK];
      t5 *= t7;
      t0 += t5;
      t6 = VC[1][i + 17 + CHUNK];
      t6 *= t7;
      t1 += t6;
      t5 = VC[2][i + 17 + CHUNK];
      t5 *= t7;
      t2 += t5;
      t6 = VC[3][i + 17 + CHUNK];
      t6 *= t7;
      t3 += t6;
      t7 *= C[4][i + 17 + CHUNK];
      t4 += t7;

      t7 = buf[i + 18];
      t5 = VC[0][i + 18 + CHUNK];
      t5 *= t7;
      t0 += t5;
      t6 = VC[1][i + 18 + CHUNK];
      t6 *= t7;
      t1 += t6;
      t5 = VC[2][i + 18 + CHUNK];
      t5 *= t7;
      t2 += t5;
      t6 = VC[3][i + 18 + CHUNK];
      t6 *= t7;
      t3 += t6;
      t7 *= C[4][i + 18 + CHUNK];
      t4 += t7;

      t7 = buf[i + 19];
      t5 = VC[0][i + 19 + CHUNK];
      t5 *= t7;
      t0 += t5;
      t6 = VC[1][i + 19 + CHUNK];
      t6 *= t7;
      t1 += t6;
      t5 = VC[2][i + 19 + CHUNK];
      t5 *= t7;
      t2 += t5;
      t6 = VC[3][i + 19 + CHUNK];
      t6 *= t7;
      t3 += t6;
      t7 *= C[4][i + 19 + CHUNK];
      t4 += t7;

      t7 = buf[i + 20];
      t5 = VC[0][i + 20 + CHUNK];
      t5 *= t7;
      t0 += t5;
      t6 = VC[1][i + 20 + CHUNK];
      t6 *= t7;
      t1 += t6;
      t5 = VC[2][i + 20 + CHUNK];
      t5 *= t7;
      t2 += t5;
      t6 = VC[3][i + 20 + CHUNK];
      t6 *= t7;
      t3 += t6;
      t7 *= C[4][i + 20 + CHUNK];
      t4 += t7;

      t7 = buf[i + 21];
      t5 = VC[0][i + 21 + CHUNK];
      t5 *= t7;
      t0 += t5;
      t6 = VC[1][i + 21 + CHUNK];
      t6 *= t7;
      t1 += t6;
      t5 = VC[2][i + 21 + CHUNK];
      t5 *= t7;
      t2 += t5;
      t6 = VC[3][i + 21 + CHUNK];
      t6 *= t7;
      t3 += t6;
      t7 *= C[4][i + 21 + CHUNK];
      t4 += t7;

      t7 = buf[i + 22];
      t5 = VC[0][i + 22 + CHUNK];
      t5 *= t7;
      t0 += t5;
      t6 = VC[1][i + 22 + CHUNK];
      t6 *= t7;
      t1 += t6;
      t5 = VC[2][i + 22 + CHUNK];
      t5 *= t7;
      t2 += t5;
      t6 = VC[3][i + 22 + CHUNK];
      t6 *= t7;
      t3 += t6;
      t7 *= C[4][i + 22 + CHUNK];
      t4 += t7;

      t7 = buf[i + 23];
      t5 = VC[0][i + 23 + CHUNK];
      t5 *= t7;
      t0 += t5;
      t6 = VC[1][i + 23 + CHUNK];
      t6 *= t7;
      t1 += t6;
      t5 = VC[2][i + 23 + CHUNK];
      t5 *= t7;
      t2 += t5;
      t6 = VC[3][i + 23 + CHUNK];
      t6 *= t7;
      t3 += t6;
      t7 *= C[4][i + 23 + CHUNK];
      t4 += t7;

      t7 = buf[i + 24];
      t5 = VC[0][i + 24 + CHUNK];
      t5 *= t7;
      t0 += t5;
      t6 = VC[1][i + 24 + CHUNK];
      t6 *= t7;
      t1 += t6;
      t5 = VC[2][i + 24 + CHUNK];
      t5 *= t7;
      t2 += t5;
      t6 = VC[3][i + 24 + CHUNK];
      t6 *= t7;
      t3 += t6;
      t7 *= C[4][i + 24 + CHUNK];
      t4 += t7;

      t7 = buf[i + 25];
      t5 = VC[0][i + 25 + CHUNK];
      t5 *= t7;
      t0 += t5;
      t6 = VC[1][i + 25 + CHUNK];
      t6 *= t7;
      t1 += t6;
      t5 = VC[2][i + 25 + CHUNK];
      t5 *= t7;
      t2 += t5;
      t6 = VC[3][i + 25 + CHUNK];
      t6 *= t7;
      t3 += t6;
      t7 *= C[4][i + 25 + CHUNK];
      t4 += t7;

      t7 = buf[i + 26];
      t5 = VC[0][i + 26 + CHUNK];
      t5 *= t7;
      t0 += t5;
      t6 = VC[1][i + 26 + CHUNK];
      t6 *= t7;
      t1 += t6;
      t5 = VC[2][i + 26 + CHUNK];
      t5 *= t7;
      t2 += t5;
      t6 = VC[3][i + 26 + CHUNK];
      t6 *= t7;
      t3 += t6;
      t7 *= C[4][i + 26 + CHUNK];
      t4 += t7;

      t7 = buf[i + 27];
      t5 = VC[0][i + 27 + CHUNK];
      t5 *= t7;
      t0 += t5;
      t6 = VC[1][i + 27 + CHUNK];
      t6 *= t7;
      t1 += t6;
      t5 = VC[2][i + 27 + CHUNK];
      t5 *= t7;
      t2 += t5;
      t6 = VC[3][i + 27 + CHUNK];
      t6 *= t7;
      t3 += t6;
      t7 *= C[4][i + 27 + CHUNK];
      t4 += t7;

      t7 = buf[i + 28];
      t5 = VC[0][i + 28 + CHUNK];
      t5 *= t7;
      t0 += t5;
      t6 = VC[1][i + 28 + CHUNK];
      t6 *= t7;
      t1 += t6;
      t5 = VC[2][i + 28 + CHUNK];
      t5 *= t7;
      t2 += t5;
      t6 = VC[3][i + 28 + CHUNK];
      t6 *= t7;
      t3 += t6;
      t7 *= C[4][i + 28 + CHUNK];
      t4 += t7;

      t7 = buf[i + 29];
      t5 = VC[0][i + 29 + CHUNK];
      t5 *= t7;
      t0 += t5;
      t6 = VC[1][i + 29 + CHUNK];
      t6 *= t7;
      t1 += t6;
      t5 = VC[2][i + 29 + CHUNK];
      t5 *= t7;
      t2 += t5;
      t6 = VC[3][i + 29 + CHUNK];
      t6 *= t7;
      t3 += t6;
      t7 *= C[4][i + 29 + CHUNK];
      t4 += t7;

      t7 = buf[i + 30];
      t5 = VC[0][i + 30 + CHUNK];
      t5 *= t7;
      t0 += t5;
      t6 = VC[1][i + 30 + CHUNK];
      t6 *= t7;
      t1 += t6;
      t5 = VC[2][i + 30 + CHUNK];
      t5 *= t7;
      t2 += t5;
      t6 = VC[3][i + 30 + CHUNK];
      t6 *= t7;
      t3 += t6;
      t7 *= C[4][i + 30 + CHUNK];
      t4 += t7;

      t7 = buf[i + 31];
      t5 = VC[0][i + 31 + CHUNK];
      t5 *= t7;
      t0 += t5;
      t6 = VC[1][i + 31 + CHUNK];
      t6 *= t7;
      t1 += t6;
      t5 = VC[2][i + 31 + CHUNK];
      t5 *= t7;
      t2 += t5;
      t6 = VC[3][i + 31 + CHUNK];
      t6 *= t7;
      t3 += t6;
      t7 *= C[4][i + 31 + CHUNK];
      t4 += t7;

      i += 32;
    }

    t7 = alpha26; t7 += t0; t7 -= alpha26; t1 += t7; t0 -= t7;
    t7 = alpha52; t7 += t1; t7 -= alpha52; t2 += t7; t1 -= t7;
    t7 = alpha78; t7 += t2; t7 -= alpha78; t3 += t7; t2 -= t7;
    t7 = alpha104; t7 += t3; t7 -= alpha104; t4 += t7; t3 -= t7;
    t7 = alpha127; t7 += t4; t7 -= alpha127; t4 -= t7; t7 *= twom127; t0 += t7;
    t7 = alpha26; t7 += t0; t7 -= alpha26; t0 -= t7; t1 += t7;

    if (!len) break;

    t0 += buf[0];

    T[1] = t1;
    T[2] = t2;
    T[3] = t3;
    T[0] = t0;

    t0 *= Q[0];
    t5 = VQ[8];
    t5 *= T[1];
    t0 += t5;
    t7 = VQ[7];
    t7 *= T[2];
    t0 += t7;
    t5 = VQ[6];
    t5 *= T[3];
    t0 += t5;
    t7 = t4;
    t7 *= Q[5];
    t0 += t7;

    t1 *= Q[0];
    t5 = VQ[1];
    t5 *= T[0];
    t1 += t5;
    t7 = VQ[8];
    t7 *= T[2];
    t1 += t7;
    t5 = VQ[7];
    t5 *= T[3];
    t1 += t5;
    t7 = t4;
    t7 *= Q[6];
    t1 += t7;

    t2 *= Q[0];
    t5 = VQ[2];
    t5 *= T[0];
    t2 += t5;
    t7 = VQ[1];
    t7 *= T[1];
    t2 += t7;
    t5 = VQ[8];
    t5 *= T[3];
    t2 += t5;
    t7 = t4;
    t7 *= Q[7];
    t2 += t7;

    t3 *= Q[0];
    t5 = VQ[3];
    t5 *= T[0];
    t3 += t5;
    t7 = VQ[2];
    t7 *= T[1];
    t3 += t7;
    t5 = VQ[1];
    t5 *= T[2];
    t3 += t5;
    t7 = t4;
    t7 *= Q[8];
    t3 += t7;

    t4 *= Q[0];
    t5 = VQ[4];
    t5 *= T[0];
    t4 += t5;
    t7 = VQ[3];
    t7 *= T[1];
    t4 += t7;
    t5 = VQ[2];
    t5 *= T[2];
    t4 += t5;
    t7 = VQ[1];
    t7 *= T[3];
    t4 += t7;

    buf += CHUNK + 1;
    len -= CHUNK + 1;
    i = -CHUNK;
  }

  t0 += t1;
  t1 = alpha32; t1 += t0; t1 -= alpha32; t2 += t1; t0 -= t1;
  t1 = t2; t2 += alpha64; t2 -= alpha64; t3 += t2; t1 -= t2;
  t2 = t3; t3 += alpha96; t3 -= alpha96; t2 -= t3; t3 += t4;

  t5 = pad[3];
  t5 *= two96;
  t3 += t5;
  t5 = pad[2];
  t5 *= two64;
  t2 += t5;
  t5 = pad[1];
  t5 *= two32;
  t1 += t5;
  t5 = pad[0];
  t0 += t5;

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

  t0 += gamma;
  t1 += gamma;
  t2 += gamma;
  t3 += gamma;

  T[0] = t0;
  T[1] = t1;
  T[2] = t2;
  T[3] = t3;

  out[0] = *(int32 *) &T[0] - 0x80000000;
  out[1] = *(int32 *) &T[1] - 0x80000000;
  out[2] = *(int32 *) &T[2] - 0x80000000;
  out[3] = *(int32 *) &T[3] - 0x80000000;
}

void hash127_expand(register struct hash127 *bigr,int32 r[4])
{
  register double t0, t1, t2, t3, t4, t5, t6, t7;
  register int i;

  asm volatile("fldcw %0"::"m"(0x137f));

  alpha0 = 13835058055282163712.0;
  alpha26 = 928455029464035206174343168.0;
  alpha52 = 62307562302417931542365955950641152.0;
  alpha78 = 4181389724724491839037947176121567782371328.0;
  alpha104 = 280608314367533360295107487881526339773939048251392.0;
  alpha127 = 2353913150770005286438421033702874906038383291674012942336.0;
  twom127 = 0.0000000000000000000000000000000000000058774717541114375398436826861112283890933277838604376075437585313920862972736358642578125;
  alpha32 = 59421121885698253195157962752.0;
  alpha64 = 255211775190703847597530955573826158592.0;
  alpha96 = 1096126227998177188652763624537212264741949407232.0;
  beta0 = 0.499999999883584678173065185546875;
  beta32 = 2147483647.5;
  beta64 = 9223372034707292160.0;
  beta96 = 39614081247908796759917199360.0;
  beta127 = 85070591710427575237277567459556065280.0;
  gamma = 6755399441055744.0;
  two32 = 4294967296.0;
  two64 = 18446744073709551616.0;
  two96 = 79228162514264337593543950336.0;
  twom32 = 0.00000000023283064365386962890625;
  twom64 = 0.0000000000000000000542101086242752217003726400434970855712890625;
  twom96 = 0.000000000000000000000000000012621774483536188886587657044524579674771302961744368076324462890625;
  twom1 = 0.5;

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
    t0 *= Q[0];
    t5 = Q[8];

    t5 *= t1;
    C[1][i] = t1;
    t0 += t5;
    t6 = Q[7];
    t6 *= t2;
    t0 += t6;
    t5 = Q[6];
    t5 *= t3;
    t0 += t5;
    t6 = Q[5];
    t6 *= t4;
    t0 += t6;

    t1 *= Q[0];
    t7 = t0;
    t7 += alpha26;
    t5 = VC[0][i];
    t5 *= Q[1];
    t7 -= alpha26;
    t6 = Q[8];
    t6 *= t2;
    C[2][i] = t2;
    t0 -= t7;

    t1 += t5;
    t1 += t6;
    t5 = Q[7];
    t5 *= t3;
    t1 += t5;
    t6 = Q[6];
    t6 *= t4;
    t1 += t6;

    t1 += t7;
    t7 = t1;
    t2 *= Q[0];
    t7 += alpha52;
    t7 -= alpha52;
    t1 -= t7;

    t5 = VC[0][i];
    t5 *= Q[2];
    t2 += t5;
    t6 = VC[1][i];
    t6 *= Q[1];
    t2 += t6;
    t5 = Q[8];
    t5 *= t3;
    C[3][i] = t3;
    t2 += t5;
    t6 = Q[7];
    t6 *= t4;
    t2 += t6;

    t2 += t7;
    t7 = t2;
    t7 += alpha78;
    t3 *= Q[0];
    t7 -= alpha78; 
    t2 -= t7;

    t5 = VC[0][i];
    t5 *= Q[3];
    t3 += t5;
    t6 = VC[1][i];
    t6 *= Q[2];
    t3 += t6;
    t5 = VC[2][i];
    t5 *= Q[1];
    t3 += t5;
    t6 = Q[8];
    t6 *= t4;
    C[4][i] = t4;
    t3 += t6;

    t3 += t7;
    t7 = t3;
    t7 += alpha104;
    t4 *= Q[0];
    t7 -= alpha104;
    t3 -= t7;

    t5 = VC[0][i];
    t5 *= Q[4];
    t4 += t5;
    t6 = VC[1][i];
    t6 *= Q[3];
    t4 += t6;
    t5 = VC[2][i];
    t5 *= Q[2];
    t4 += t5;
    t6 = VC[3][i];
    t6 *= Q[1];
    t4 += t6;

    t4 += t7;
    t7 = t4; t7 += alpha127; t7 -= alpha127; t4 -= t7; t7 *= twom127;
    t0 += t7;
    t7 = t0; t7 += alpha26; t7 -= alpha26; t1 += t7;
    t0 -= t7;

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
