/* Public domain. */

#ifndef HASH127_H
#define HASH127_H "x86-idea"

#include "int32.h"

#define HASH127_CHUNK 96

struct hash127 {
  double q[9];
  double c[5][HASH127_CHUNK];
} ;

extern void hash127_expand(struct hash127 *,int32 [4]);
extern void hash127(int32 [4],int32 *,unsigned int,struct hash127 *,int32 [4]);
#define hash127_little hash127

#endif

/*
struct hash127 layout:

c[0][i] + c[1][i] + c[2][i] + c[3][i] + c[4][i]
is congruent to r^(CHUNK-i) mod p for i = 0, 1, 2, ... CHUNK - 1.

q[0] + q[1] + q[2] + q[3] + q[4] is congruent to r^(CHUNK+1), except
during the main loop in hash127_init(), when it is congruent to r.

q[5], q[6], q[7], q[8] are 2^(-127) times q[1], q[2], q[3], q[4].
*/
