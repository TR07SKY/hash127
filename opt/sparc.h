/* Public domain. */

#ifndef HASH127_H
#define HASH127_H "sparc"

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
