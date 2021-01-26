/* Public domain. */

#ifndef HASH127_H
#define HASH127_H "idea"

#include "int32.h"

#define HASH127_CHUNK 96

struct hash127 {
  double q[14];
  double c[8][HASH127_CHUNK];
} ;

extern void hash127_expand(struct hash127 *,int32 [4]);
extern void hash127(int32 [4],int32 *,unsigned int,struct hash127 *,int32 [4]);
extern void hash127_little(int32 [4],int32 *,unsigned int,struct hash127 *,int32 [4]);

#endif

/*
struct hash127 layout:

c[0][i] + c[1][i] + c[2][i] + c[3][i] + c[4][i] + c[5][i] + c[6][i] + c[7][i]
is congruent to r^(CHUNK-i) mod p for i = 0, 1, 2, ..., CHUNK-1.

q[0] + q[1] + q[2] + q[3] + q[4] + q[5] + q[6] + q[7]
is congruent to r^(CHUNK+1).
q[8] et al. are 2^(-127) times q[2] et al.

During expand, q[0] + q[1] + q[2] + q[3] is congruent to r.
*/
