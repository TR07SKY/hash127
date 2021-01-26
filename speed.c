#include "hash127.h"
#include "timing.h"

static int32 pad;
static struct hash127 h[1];
static int32 r[4];
static int32 k[4];
static int32 out[4];
static int32 buf[10000];

static timing start;
static timing_basic startb;
static timing finish;
static timing_basic finishb;

/* must start with 0 */
static int size[] = {
  0, 1, 10, 16, 20, 32, 40, 64, 65, 66, 95, 96, 97, 100, 128, 256, 480, 500, 512, 1000, 1024, 10000
};
#define SIZES ((sizeof size)/sizeof(size[0]))

#define TIMINGS 7
static timing t[SIZES][TIMINGS + 1];
static timing tlittle[SIZES][TIMINGS + 1];
static timing texpand[TIMINGS + 1];
static timing tnothing[TIMINGS + 1];

main()
{
  int i;
  int j;
  double diff;
  double diffmin;
  double diffmin0;

  for (i = 0;i < 10000;++i)
    buf[i] = i;

  timing_basic_now(&startb);
  timing_now(&start);

  timing_now(&tnothing[0]);
  for (i = 1;i <= TIMINGS;++i)
    timing_now(&tnothing[i]);

  timing_now(&texpand[0]);
  for (i = 1;i <= TIMINGS;++i) {
    hash127_expand(h,r);
    timing_now(&texpand[i]);
  }

  for (j = 0;j < SIZES;++j) {
    timing_now(&t[j][0]);
    for (i = 1;i <= TIMINGS;++i) {
      hash127(out,buf,size[j],h,k);
      timing_now(&t[j][i]);
    }
  }

  for (j = 0;j < SIZES;++j) {
    timing_now(&tlittle[j][0]);
    for (i = 1;i <= TIMINGS;++i) {
      hash127_little(out,buf,size[j],h,k);
      timing_now(&tlittle[j][i]);
    }
  }

  timing_basic_now(&finishb);
  timing_now(&finish);

  printf("Using");
#ifdef HASRDTSC
  printf(" RDTSC,");
#else
#ifdef HASGETHRTIME
  printf(" gethrtime(),");
#endif
#ifdef HASRPCC
  printf(" rpcc(),");
#endif
#endif
  printf(" opt/%s.c.\n",HASH127_H);

  printf("nothing              |");
  for (i = 0;i < TIMINGS;++i) {
    diff = timing_diff(&tnothing[i + 1],&tnothing[i]);
    printf(" %6.0f",diff);
  }
  printf("|\n");

  printf("expand               |");
  for (i = 0;i < TIMINGS;++i) {
    diff = timing_diff(&texpand[i + 1],&texpand[i]);
    printf(" %6.0f",diff);
  }
  printf("|\n");

  for (j = 0;j < SIZES;++j) {
    printf("hash127 %6d %6d|",size[j],size[j] * 4);
    for (i = 0;i < TIMINGS;++i) {
      diff = timing_diff(&t[j][i + 1],&t[j][i]);
      printf(" %6.0f",diff);
      if (!i || (diff < diffmin)) diffmin = diff;
    }
    if (size[j])
      printf("|%7.2f",(diffmin - diffmin0) / (4.0 * size[j]));
    else {
      diffmin0 = diffmin;
      printf("|");
    }
    printf("\n");
  }

  for (j = 0;j < SIZES;++j) {
    printf("little  %6d %6d|",size[j],size[j] * 4);
    for (i = 0;i < TIMINGS;++i) {
      diff = timing_diff(&tlittle[j][i + 1],&tlittle[j][i]);
      printf(" %6.0f",diff);
      if (!i || (diff < diffmin)) diffmin = diff;
    }
    if (size[j])
      printf("|%7.2f",(diffmin - diffmin0) / (4.0 * size[j]));
    else {
      diffmin0 = diffmin;
      printf("|");
    }
    printf("\n");
  }

  printf("Timings are in ticks. Nanoseconds per tick: approximately %f.\n"
    ,timing_basic_diff(&finishb,&startb) / timing_diff(&finish,&start));
  printf("Timings may be underestimates on systems without hardware tick support.\n");

  exit(0);
}
