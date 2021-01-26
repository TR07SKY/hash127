/* Public domain. */

#ifndef HASH127_H
#define HASH127_H "pentium"

#include "int32.h"

struct hash127 {
  double constants[23];
  double tmp[4];
  double q[9];
  double c[5][96];
} ;

extern void hash127_expand(struct hash127 *,int32 [4]);
extern void hash127(int32 [4],int32 *,unsigned int,struct hash127 *,int32 [4]);
#define hash127_little hash127

#endif
