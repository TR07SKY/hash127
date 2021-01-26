/* Public domain. */

#include "hash127.h"

void hash127(int32 out[4],register int32 *buf,register unsigned int len,register struct hash127 *bigr,int32 pad[4])
{
  register int i;

  i = len;
  while (i >= 97) i -= 97;

  buf += i;
  len -= i;
  i = -i;

  asm volatile(

"\n cmpl $-96,%0"
"\n jnz 0f"

"\n fldl 216(%1)"
"\n fldl 224(%1)"
"\n fldl 232(%1)"
"\n fldl 240(%1)"
"\n fldl 248(%1)"
"\n jmp 1f"

"\n 0:"
"\n fldl 1048(%1,%0,8)"
"\n fldl 1816(%1,%0,8)"
"\n fldl 2584(%1,%0,8)"
"\n fldl 3352(%1,%0,8)"
"\n fldl 4120(%1,%0,8)"

"\n 1:"                                                    /* t4 t3 t2 t1 t0 */
"\n testl $1,%0; jz 9f"

"\n fildl (%2,%0,4)"                                    /* t7 t4 t3 t2 t1 t0 */
"\n fldl 1056(%1,%0,8)"                              /* C0 t7 t4 t3 t2 t1 t0 */
/* pstall */
"\n fmul %%st(1),%%st"                            /* C0*t7 t7 t4 t3 t2 t1 t0 */
"\n fldl 1824(%1,%0,8)"                        /* C1 C0*t7 t7 t4 t3 t2 t1 t0 */
"\n fmul %%st(2),%%st"                      /* C1*t7 C0*t7 t7 t4 t3 t2 t1 t0 */
"\n fxch %%st(1); faddp %%st,%%st(7)"            /* C1*t7 t7 t4 t3 t2 t1 t0' */
"\n fldl 2592(%1,%0,8)"                       /* C2 C1*t7 t7 t4 t3 t2 t1 t0' */
"\n fmul %%st(2),%%st"                     /* C2*t7 C1*t7 t7 t4 t3 t2 t1 t0' */
"\n fxch %%st(1); faddp %%st,%%st(6)"           /* C2*t7 t7 t4 t3 t2 t1' t0' */
"\n fldl 3360(%1,%0,8)"                      /* C3 C2*t7 t7 t4 t3 t2 t1' t0' */
"\n fmul %%st(2),%%st"                    /* C3*t7 C2*t7 t7 t4 t3 t2 t1' t0' */
"\n fxch %%st(1); faddp %%st,%%st(5)"          /* C3*t7 t7 t4 t3 t2' t1' t0' */
"\n fxch %%st(1); fmull 4128(%1,%0,8)"      /* C4*t7 C3*t7 t4 t3 t2' t1' t0' */
"\n fxch %%st(1); faddp %%st,%%st(3)"            /* C4*t7 t4 t3' t2' t1' t0' */
"\n incl %0"
"\n faddp %%st,%%st(1)"                               /* t4' t3' t2' t1' t0' */

/* i is even */
"\n 9:"                                                    /* t4 t3 t2 t1 t0 */
"\n testl %0,%0; jge 3f"


/* Main loop starts here. i is even and nonzero. */
"\n 2:"
"\n fldz"

/* Inner loop starts here. i is even and nonzero. */
/* The inner loop takes 32 Pentium cycles for 2 words of input. */
/* 2 of those cycles are loop overhead: addl, jnz, AGI stall. */
/* The rest of the main loop, called for 97 words of input, */
/* does about 115 cycles of real work, plus about 36 stalls. */

"\n 8:"                                                  /* p t4 t3 t2 t1 t0 */
"\n fildl (%2,%0,4)"                                  /* t7 p t4 t3 t2 t1 t0 */
"\n fldl 1056(%1,%0,8)"                            /* C0 t7 p t4 t3 t2 t1 t0 */
"\n fxch %%st(2); faddp %%st,%%st(3)"                /* t7 C0 t4 t3 t2 t1 t0 */
"\n fxch %%st(1); fmul %%st(1),%%st"              /* C0*t7 t7 t4 t3 t2 t1 t0 */
"\n fldl 1824(%1,%0,8)"                        /* C1 C0*t7 t7 t4 t3 t2 t1 t0 */
"\n fmul %%st(2),%%st"                      /* C1*t7 C0*t7 t7 t4 t3 t2 t1 t0 */
"\n fxch %%st(1); faddp %%st,%%st(7)"            /* C1*t7 t7 t4 t3 t2 t1 t0' */
"\n fldl 2592(%1,%0,8)"                       /* C2 C1*t7 t7 t4 t3 t2 t1 t0' */
"\n fmul %%st(2),%%st"                     /* C2*t7 C1*t7 t7 t4 t3 t2 t1 t0' */
"\n fxch %%st(1); faddp %%st,%%st(6)"           /* C2*t7 t7 t4 t3 t2 t1' t0' */
"\n fldl 3360(%1,%0,8)"                      /* C3 C2*t7 t7 t4 t3 t2 t1' t0' */
"\n fmul %%st(2),%%st"                    /* C3*t7 C2*t7 t7 t4 t3 t2 t1' t0' */
"\n fxch %%st(1); faddp %%st,%%st(5)"          /* C3*t7 t7 t4 t3 t2' t1' t0' */
"\n fxch %%st(1); fmull 4128(%1,%0,8)"      /* C4*t7 C3*t7 t4 t3 t2' t1' t0' */
"\n fxch %%st(1); faddp %%st,%%st(3)"            /* C4*t7 t4 t3' t2' t1' t0' */
"\n fildl 4(%2,%0,4)"                        /* t7' C4*t7 t4 t3' t2' t1' t0' */
"\n fldl 1064(%1,%0,8)"                   /* C0 t7' C4*t7 t4 t3' t2' t1' t0' */
"\n fxch %%st(2); faddp %%st,%%st(3)"                /* t7 C0 t4 t3 t2 t1 t0 */
"\n fxch %%st(1); fmul %%st(1),%%st"              /* C0*t7 t7 t4 t3 t2 t1 t0 */
"\n fldl 1832(%1,%0,8)"                        /* C1 C0*t7 t7 t4 t3 t2 t1 t0 */
"\n fmul %%st(2),%%st"                      /* C1*t7 C0*t7 t7 t4 t3 t2 t1 t0 */
"\n fxch %%st(1); faddp %%st,%%st(7)"            /* C1*t7 t7 t4 t3 t2 t1 t0' */
"\n fldl 2600(%1,%0,8)"                       /* C2 C1*t7 t7 t4 t3 t2 t1 t0' */
"\n fmul %%st(2),%%st"                     /* C2*t7 C1*t7 t7 t4 t3 t2 t1 t0' */
"\n fxch %%st(1); faddp %%st,%%st(6)"           /* C2*t7 t7 t4 t3 t2 t1' t0' */
"\n fldl 3368(%1,%0,8)"                      /* C3 C2*t7 t7 t4 t3 t2 t1' t0' */
"\n fmul %%st(2),%%st"                    /* C3*t7 C2*t7 t7 t4 t3 t2 t1' t0' */
"\n fxch %%st(1); faddp %%st,%%st(5)"          /* C3*t7 t7 t4 t3 t2' t1' t0' */
"\n fxch %%st(1); fmull 4136(%1,%0,8)"      /* C4*t7 C3*t7 t4 t3 t2' t1' t0' */
"\n fxch %%st(1); faddp %%st,%%st(3)"            /* C4*t7 t4 t3' t2' t1' t0' */
"\n addl $2,%0; jnz 8b"

"\n faddp %%st,%%st(1)"                               /* t4' t3' t2' t1' t0' */


/* i is zero */
"\n 3:"                                                    /* t4 t3 t2 t1 t0 */

/* Could save some stalls here by merging carries with last inner loop. */
"\n fldl 8(%1)"                                        /* a26 t4 t3 t2 t1 t0 */
"\n fadd %%st(5),%%st"
"\n fsubl 8(%1)"
"\n fadd %%st,%%st(4)"
"\n fsubrp %%st,%%st(5)"                                 /* t4 t3 t2 t1' t0' */
"\n fldl 16(%1)"                                     /* a52 t4 t3 t2 t1' t0' */
"\n fadd %%st(4),%%st"
"\n fsubl 16(%1)"
"\n fadd %%st,%%st(3)"
"\n fsubrp %%st,%%st(4)"                               /* t4 t3 t2' t1'' t0' */
"\n fldl 24(%1)"                                   /* a78 t4 t3 t2' t1'' t0' */
"\n fadd %%st(3),%%st" 
"\n fsubl 24(%1)" 
"\n fadd %%st,%%st(2)" 
"\n fsubrp %%st,%%st(3)"                             /* t4 t3' t2'' t1'' t0' */
"\n fldl 32(%1)"                                /* a104 t4 t3' t2'' t1'' t0' */
"\n fadd %%st(2),%%st" 
"\n fsubl 32(%1)" 
"\n fadd %%st,%%st(1)" 
"\n fsubrp %%st,%%st(2)"                           /* t4' t3'' t2'' t1'' t0' */
"\n fldl 40(%1)"                              /* a127 t4' t3'' t2'' t1'' t0' */
"\n fadd %%st(1),%%st" 
"\n fsubl 40(%1)" 
"\n fsubr %%st,%%st(1)"                        /* t7 t4'' t3'' t2'' t1'' t0' */
"\n fmull (%1)"                        /* t7*twom127 t4'' t3'' t2'' t1'' t0' */
"\n faddp %%st,%%st(5)"                          /* t4'' t3'' t2'' t1'' t0'' */
"\n fldl 8(%1)" 
"\n fadd %%st(5),%%st" 
"\n fsubl 8(%1)" 
"\n fsubr %%st,%%st(5)" 
"\n faddp %%st,%%st(4)"                        /* t4'' t3'' t2'' t1''' t0''' */

/* Escape main loop if i is zero. */
"\n testl %4,%4; jz 4f" 

"\n fildl (%2)"                                     /* buf[0] t4 t3 t2 t1 t0 */
"\n fxch %%st(4); fstl 192(%1)"                     /* t1 t4 t3 t2 buf[0] t0 */
"\n fxch %%st(4); faddp %%st,%%st(5)"                      /* t4 t3 t2 t1 t0 */
"\n fxch %%st(2); fstl 200(%1)"                            /* t2 t3 t4 t1 t0 */
"\n fxch %%st(1); fstl 208(%1)"                            /* t3 t2 t4 t1 t0 */
"\n fxch %%st(4); fstl 184(%1)"                            /* t0 t2 t4 t1 t3 */

"\n fmull 216(%1)"                                    /* t0*Q[0] t2 t4 t1 t3 */
"\n fldl %%st(3)"
"\n fmull 280(%1)"                            /* t1*Q[8] t0*Q[0] t2 t4 t1 t3 */
"\n fldl %%st(2)"
"\n fmull 272(%1)"                    /* t2*Q[7] t1*Q[8] t0*Q[0] t2 t4 t1 t3 */
"\n fxch %%st(1); faddp %%st,%%st(2)" /* t2*Q[7] t1*Q[8]+t0*Q[0] t2 t4 t1 t3 */
"\n fldl %%st(5)"
"\n fmull 264(%1)"            /* t3*Q[6] t2*Q[7] t1*Q[8]+t0*Q[0] t2 t4 t1 t3 */
"\n fxch %%st(1); faddp %%st,%%st(2)"
                              /* t3*Q[6] t2*Q[7]+t1*Q[8]+t0*Q[0] t2 t4 t1 t3 */
"\n fldl %%st(3)"
"\n fmull 256(%1)"    /* t4*Q[5] t3*Q[6] t2*Q[7]+t1*Q[8]+t0*Q[0] t2 t4 t1 t3 */
"\n fxch %%st(1); faddp %%st,%%st(2)"
                      /* t4*Q[5] t3*Q[6]+t2*Q[7]+t1*Q[8]+t0*Q[0] t2 t4 t1 t3 */

"\n fxch %%st(4); fmull 216(%1)"
                 /* t1*Q[0] t3*Q[6]+t2*Q[7]+t1*Q[8]+t0*Q[0] t2 t4 t4*Q[5] t3 */
"\n fxch %%st(1); fldl 184(%1)"
"\n fmull 224(%1)"
         /* t0*Q[1] t3*Q[6]+t2*Q[7]+t1*Q[8]+t0*Q[0] t1*Q[0] t2 t4 t4*Q[5] t3 */
"\n fxch %%st(5); faddp %%st,%%st(1)"         /* u0 t1*Q[0] t2 t4 t0*Q[1] t3 */
"\n fxch %%st(5); fld %%st(2)"
"\n fmull 280(%1)"                    /* t2*Q[8] t3 t1*Q[0] t2 t4 t0*Q[1] u0 */
"\n fxch %%st(2); faddp %%st,%%st(5)" /* t3 t2*Q[8] t2 t4 t1*Q[0]+t0*Q[1] u0 */
"\n fld %%st(0)"
"\n fmull 272(%1)"            /* t3*Q[7] t3 t2*Q[8] t2 t4 t1*Q[0]+t0*Q[1] u0 */
"\n fxch %%st(2); faddp %%st,%%st(5)"
                              /* t3 t3*Q[7] t2 t4 t2*Q[8]+t1*Q[0]+t0*Q[1] u0 */
"\n fld %%st(3)"
"\n fmull 264(%1)"    /* t4*Q[6] t3 t3*Q[7] t2 t4 t2*Q[8]+t1*Q[0]+t0*Q[1] u0 */
"\n fxch %%st(2); faddp %%st,%%st(5)"
                      /* t3 t4*Q[6] t2 t4 t3*Q[7]+t2*Q[8]+t1*Q[0]+t0*Q[1] u0 */

"\n fxch %%st(2); fmull 216(%1)"
                 /* t2*Q[0] t4*Q[6] t3 t4 t3*Q[7]+t2*Q[8]+t1*Q[0]+t0*Q[1] u0 */
"\n fxch %%st(3); fldl 184(%1)"
"\n fmull 232(%1)"
         /* t0*Q[2] t4 t4*Q[6] t3 t2*Q[0] t3*Q[7]+t2*Q[8]+t1*Q[0]+t0*Q[1] u0 */
"\n fxch %%st(2); faddp %%st,%%st(5)"         /* t4 t0*Q[2] t3 t2*Q[0] u1 u0 */
"\n fldl 192(%1)"
"\n fmull 224(%1)"                    /* t1*Q[1] t4 t0*Q[2] t3 t2*Q[0] u1 u0 */
"\n fxch %%st(2); faddp %%st,%%st(4)" /* t4 t1*Q[1] t3 t0*Q[2]+t2*Q[0] u1 u0 */
"\n fldl %%st(2)"
"\n fmull 280(%1)"            /* t3*Q[8] t4 t1*Q[1] t3 t0*Q[2]+t2*Q[0] u1 u0 */
"\n fxch %%st(2); faddp %%st,%%st(4)"
                              /* t4 t3*Q[8] t3 t1*Q[1]+t0*Q[2]+t2*Q[0] u1 u0 */
"\n fld %%st(0)"
"\n fmull 272(%1)"    /* t4*Q[7] t4 t3*Q[8] t3 t1*Q[1]+t0*Q[2]+t2*Q[0] u1 u0 */
"\n fxch %%st(2); faddp %%st,%%st(4)"
                      /* t4 t4*Q[7] t3 t3*Q[8]+t1*Q[1]+t0*Q[2]+t2*Q[0] u1 u0 */

"\n fxch %%st(2); fmull 216(%1)"
                 /* t3*Q[0] t4*Q[7] t4 t3*Q[8]+t1*Q[1]+t0*Q[2]+t2*Q[0] u1 u0 */
"\n fxch %%st(2); fldl 184(%1)"
"\n fmull 240(%1)"
         /* t0*Q[3] t4 t4*Q[7] t3*Q[0] t3*Q[8]+t1*Q[1]+t0*Q[2]+t2*Q[0] u1 u0 */
"\n fxch %%st(2); faddp %%st,%%st(4)"         /* t4 t0*Q[3] t3*Q[0] u2 u1 u0 */
"\n fldl 192(%1)"
"\n fmull 232(%1)"                    /* t1*Q[2] t4 t0*Q[3] t3*Q[0] u2 u1 u0 */
"\n fxch %%st(2); faddp %%st,%%st(3)" /* t4 t1*Q[2] t0*Q[3]+t3*Q[0] u2 u1 u0 */
"\n fldl 200(%1)"
"\n fmull 224(%1)"            /* t2*Q[1] t4 t1*Q[2] t0*Q[3]+t3*Q[0] u2 u1 u0 */
"\n fxch %%st(2); faddp %%st,%%st(3)"
                              /* t4 t2*Q[1] t1*Q[2]+t0*Q[3]+t3*Q[0] u2 u1 u0 */
"\n fld %%st(0)"
"\n fmull 280(%1)"    /* t4*Q[8] t4 t2*Q[1] t1*Q[2]+t0*Q[3]+t3*Q[0] u2 u1 u0 */
"\n fxch %%st(2); faddp %%st,%%st(3)"
                      /* t4 t4*Q[8] t2*Q[1]+t1*Q[2]+t0*Q[3]+t3*Q[0] u2 u1 u0 */

"\n fmull 216(%1)"
                 /* t4*Q[0] t4*Q[8] t2*Q[1]+t1*Q[2]+t0*Q[3]+t3*Q[0] u2 u1 u0 */
"\n fldl 184(%1)"
"\n fmull 248(%1)"
         /* t0*Q[4] t4*Q[0] t4*Q[8] t2*Q[1]+t1*Q[2]+t0*Q[3]+t3*Q[0] u2 u1 u0 */
"\n fxch %%st(2); faddp %%st,%%st(3)"         /* t4*Q[0] t0*Q[4] u3 u2 u1 u0 */
"\n fldl 192(%1)"
"\n fmull 240(%1)"                    /* t1*Q[3] t4*Q[0] t0*Q[4] u3 u2 u1 u0 */
"\n fxch %%st(1); faddp %%st,%%st(2)" /* t1*Q[3] t4*Q[0]+t0*Q[4] u3 u2 u1 u0 */
"\n fldl 200(%1)"
"\n fmull 232(%1)"            /* t2*Q[2] t1*Q[3] t4*Q[0]+t0*Q[4] u3 u2 u1 u0 */
"\n fxch %%st(1); faddp %%st,%%st(2)"
                              /* t2*Q[2] t1*Q[3]+t4*Q[0]+t0*Q[4] u3 u2 u1 u0 */
"\n fldl 208(%1)"
"\n fmull 224(%1)"    /* t3*Q[1] t2*Q[2] t1*Q[3]+t4*Q[0]+t0*Q[4] u3 u2 u1 u0 */
"\n fxch %%st(1); faddp %%st,%%st(2)"
                      /* t3*Q[1] t2*Q[2]+t1*Q[3]+t4*Q[0]+t0*Q[4] u3 u2 u1 u0 */
"\n addl $388,%2"
"\n addl $-97,%4"
"\n movl $-96,%0"
"\n faddp %%st,%%st(1)"                                    /* u4 u3 u2 u1 u0 */
"\n jmp 2b"


/* Out of main loop. Have processed entire buf. */
"\n 4:"                                                    /* t4 t3 t2 t1 t0 */

"\n fxch %%st(4)"
"\n faddp %%st,%%st(3)"                                       /* t3 t2 t0 t4 */
"\n fildl 12(%5)"                                   /* t7=pad[3] t3 t2 t0 t4 */
"\n fldl 56(%1)"                                    /* t1=a32 t7 t3 t2 t0 t4 */
"\n fadd %%st(4),%%st"                                  /* t1 t7 t3 t2 t0 t4 */
"\n fildl 8(%5)"
"\n fildl 4(%5)"                    /* t5=pad[1] t6=pad[2] t1 t7 t3 t2 t0 t4 */
"\n fxch %%st(2)"
"\n fsubl 56(%1)"                                 /* t1 t6 t5 t7 t3 t2 t0 t4 */
"\n fxch %%st(3); fmull 136(%1)"                  /* t7 t6 t5 t1 t3 t2 t0 t4 */
"\n fxch %%st(5)"
"\n fadd %%st(3),%%st"                            /* t2 t6 t5 t1 t3 t7 t0 t4 */
"\n fxch %%st(6); fsubp %%st,%%st(3)"                /* t6 t5 t0 t3 t7 t2 t4 */
"\n fmull 128(%1)"                                   /* t6 t5 t0 t3 t7 t2 t4 */
"\n fld %%st(5)"                               /* t2 t6 t5 t0 t3 t7 t1=t2 t4 */
"\n faddl 64(%1)"
"\n fxch %%st(2); fmull 120(%1)"                  /* t5 t6 t2 t0 t3 t7 t1 t4 */
"\n fxch %%st(2)"
"\n fsubl 64(%1)"                                 /* t2 t6 t5 t0 t3 t7 t1 t4 */
"\n fadd %%st,%%st(4)"
"\n fsubrp %%st,%%st(6)"                             /* t6 t5 t0 t3 t7 t1 t4 */
"\n fld %%st(3)"                               /* t3 t6 t5 t0 t2=t3 t7 t1 t4 */
"\n faddl 72(%1)"
"\n fsubl 72(%1)"
"\n fsubr %%st,%%st(4)"                           /* t3 t6 t5 t0 t2 t7 t1 t4 */
"\n faddp %%st,%%st(7)"                              /* t6 t5 t0 t2 t7 t1 t3 */
"\n fxch %%st(6); fildl (%5)"                 /* pad[0] t3 t5 t0 t2 t7 t1 t6 */
"\n fxch %%st(1)"                             /* t3 pad[0] t5 t0 t2 t7 t1 t6 */
"\n faddp %%st,%%st(5)"                          /* pad[0] t5 t0 t2 t3 t1 t6 */
"\n faddp %%st,%%st(2)"                                 /* t5 t0 t2 t3 t1 t6 */
"\n fxch %%st(1); fld %%st(3)"                    /* t7=t3 t0 t5 t2 t3 t1 t6 */
"\n fmull (%1)"                                      /* t7 t0 t5 t2 t3 t1 t6 */

"\n fxch %%st(3)"
"\n faddp %%st,%%st(6)"
"\n fxch %%st(4)"
"\n faddp %%st,%%st(1)"
"\n fxch %%st(1)"
"\n faddl 176(%1)"
"\n fsubl 80(%1)"
"\n faddl 48(%1)"
"\n fsubl 48(%1)"
"\n fadd %%st(3),%%st"
"\n fsubl 88(%1)"
"\n faddl 56(%1)"
"\n fsubl 56(%1)"
"\n fadd %%st(1),%%st"
"\n fsubl 96(%1)"
"\n faddl 64(%1)"
"\n fsubl 64(%1)"
"\n fadd %%st(4),%%st"
"\n fsubl 104(%1)"
"\n faddl 72(%1)"
"\n fsubl 72(%1)"
"\n fadd %%st(2),%%st"
"\n fsubl 112(%1)"
"\n faddl 40(%1)"
"\n fsubl 40(%1)"
"\n fsubr %%st,%%st(2)"
"\n fmull (%1)"

"\n faddp %%st,%%st(3)"
"\n fld %%st(2)"
"\n fsubl 88(%1)"
"\n faddl 56(%1)"
"\n fsubl 56(%1)"
"\n fadd %%st,%%st(1)"
"\n fsubrp %%st,%%st(3)"
"\n fld %%st(0)"
"\n fsubl 96(%1)"
"\n faddl 64(%1)"
"\n fsubl 64(%1)"
"\n fadd %%st,%%st(4)"
"\n fsubrp %%st,%%st(1)"
"\n fld %%st(3)"
"\n fsubl 104(%1)"
"\n faddl 72(%1)"
"\n fsubl 72(%1)"
"\n fadd %%st,%%st(2)"
"\n fsubrp %%st,%%st(4)"
"\n fmull 144(%1)"
"\n fxch %%st(2)"
"\n faddl 168(%1)"
"\n fxch %%st(3)"
"\n fmull 152(%1)"
"\n fxch %%st(2)"
"\n faddl 168(%1)"
"\n fxch %%st(1)"
"\n fmull 160(%1)"
"\n fxch %%st(2)"
"\n faddl 168(%1)"
"\n fxch %%st(3)"
"\n fstpl 184(%1)"
"\n fxch %%st(1)"
"\n faddl 168(%1)"
"\n fxch %%st(1)"
"\n fstpl 192(%1)"
"\n fxch %%st(1)"
"\n fstpl 200(%1)"
"\n fstpl 208(%1)"

"\n movl 184(%1),%0"
"\n movl 192(%1),%4"
"\n addl $-2147483648,%0"
"\n addl $-2147483648,%4"
"\n movl %0,(%3)"
"\n movl %4,4(%3)"
"\n movl 200(%1),%0"
"\n movl 208(%1),%4"
"\n addl $-2147483648,%0"
"\n addl $-2147483648,%4"
"\n movl %0,8(%3)"
"\n movl %4,12(%3)"

    : /* XXX: actually changing i and pad */
    : "r"(i), "r"(bigr), "r"(buf), "r"(out), "r"(len), "r"(pad)
  );
}

void hash127_expand(register struct hash127 *bigr,int32 r[4])
{
  asm volatile("fldcw %0"::"m"(0x137f));

  asm volatile(

/* code from x86-sched.c under gcc 2.6.3; no additional hand optimization */
"\n movl $0,48(%0)"
"\n movl $1139277824,52(%0)"
"\n movl $0,8(%0)"
"\n movl $1166540800,12(%0)"
"\n movl $0,16(%0)"
"\n movl $1193803776,20(%0)"
"\n movl $0,24(%0)"
"\n movl $1221066752,28(%0)"
"\n movl $0,32(%0)"
"\n movl $1248329728,36(%0)"
"\n movl $0,40(%0)"
"\n movl $1272446976,44(%0)"
"\n movl $0,(%0)"
"\n movl $939524096,4(%0)"
"\n movl $0,56(%0)"
"\n movl $1172832256,60(%0)"
"\n movl $0,64(%0)"
"\n movl $1206386688,68(%0)"
"\n movl $0,72(%0)"
"\n movl $1239941120,76(%0)"
"\n movl $-2097152,80(%0)"
"\n movl $1071644671,84(%0)"
"\n movl $-2097152,88(%0)"
"\n movl $1105199103,92(%0)"
"\n movl $-2097152,96(%0)"
"\n movl $1138753535,100(%0)"
"\n movl $-2097152,104(%0)"
"\n movl $1172307967,108(%0)"
"\n movl $-2097152,112(%0)"
"\n movl $1204813823,116(%0)"
"\n movl $0,168(%0)"
"\n movl $1127743488,172(%0)"
"\n movl $0,120(%0)"
"\n movl $1106247680,124(%0)"
"\n movl $0,128(%0)"
"\n movl $1139802112,132(%0)"
"\n movl $0,136(%0)"
"\n movl $1173356544,140(%0)"
"\n movl $0,144(%0)"
"\n movl $1039138816,148(%0)"
"\n movl $0,152(%0)"
"\n movl $1005584384,156(%0)"
"\n movl $0,160(%0)"
"\n movl $972029952,164(%0)"
"\n movl $0,176(%0)"
"\n movl $1071644672,180(%0)"
"\n fildl (%1)"
"\n fldl 120(%0)"
"\n fimull 4(%1)"
"\n fld %%st(1)"
"\n faddl 8(%0)"
"\n fldl 128(%0)"
"\n fimull 8(%1)"
"\n fxch %%st(1)"
"\n fsubl 8(%0)"
"\n fldl 136(%0)"
"\n fimull 12(%1)"
"\n fxch %%st(3)"
"\n fadd %%st(1),%%st"
"\n fxch %%st(4)"
"\n fsubp %%st,%%st(1)"
"\n fld %%st(3)"
"\n faddl 16(%0)"
"\n fsubl 16(%0)"
"\n fadd %%st,%%st(2)"
"\n fsubrp %%st,%%st(4)"
"\n fld %%st(1)"
"\n faddl 24(%0)"
"\n fxch %%st(4)"
"\n fstl 224(%0)"
"\n fxch %%st(4)"
"\n fsubl 24(%0)"
"\n fxch %%st(4)"
"\n fmull (%0)"
"\n fxch %%st(3)"
"\n fadd %%st(4),%%st"
"\n fxch %%st(2)"
"\n fsubp %%st,%%st(4)"
"\n fxch %%st(2)"
"\n fstpl 256(%0)"
"\n fld %%st(0)"
"\n faddl 32(%0)"
"\n fxch %%st(3)"
"\n fstl 232(%0)"
"\n fxch %%st(3)"
"\n fsubl 32(%0)"
"\n fxch %%st(3)"
"\n fmull (%0)"
"\n fxch %%st(1)"
"\n fsub %%st(3),%%st"
"\n fld %%st(3)"
"\n faddl 40(%0)"
"\n fxch %%st(1)"
"\n fstl 240(%0)"
"\n fxch %%st(1)"
"\n fsubl 40(%0)"
"\n fxch %%st(1)"
"\n fmull (%0)"
"\n fxch %%st(2)"
"\n fstpl 264(%0)"
"\n fsubr %%st,%%st(3)"
"\n fmull (%0)"
"\n fxch %%st(1)"
"\n fstpl 272(%0)"
"\n faddp %%st,%%st(1)"
"\n fxch %%st(1)"
"\n fstl 248(%0)"
"\n fmull (%0)"
"\n fxch %%st(1)"
"\n fstl 216(%0)"
"\n fxch %%st(1)"
"\n fstpl 280(%0)"
"\n fldl 224(%0)"
"\n fldl 232(%0)"
"\n fldl 240(%0)"
"\n fldl 248(%0)"
"\n movl $95,%1"
"\n fxch %%st(4)"
"\n fxch %%st(1)"
"\n fxch %%st(3)"

"\n 0:"
"\n fxch %%st(3)"
"\n fxch %%st(1)"
"\n fstl 288(%0,%1,8)"
"\n fxch %%st(3)"
"\n fstl 1056(%0,%1,8)"
"\n fxch %%st(2)"
"\n fstl 1824(%0,%1,8)"
"\n fxch %%st(1)"
"\n fstl 2592(%0,%1,8)"
"\n fxch %%st(4)"
"\n fstl 3360(%0,%1,8)"
"\n fxch %%st(3)"
"\n fmull 216(%0)"
"\n fld %%st(2)"
"\n fmull 280(%0)"
"\n fld %%st(2)"
"\n fmull 272(%0)"
"\n fxch %%st(2)"
"\n faddp %%st,%%st(1)"
"\n fld %%st(5)"
"\n fmull 264(%0)"
"\n fxch %%st(1)"
"\n faddp %%st,%%st(2)"
"\n fld %%st(4)"
"\n fmull 256(%0)"
"\n fxch %%st(2)"
"\n faddp %%st,%%st(1)"
"\n fxch %%st(3)"
"\n fmull 216(%0)"
"\n fldl 288(%0,%1,8)"
"\n fxch %%st(4)"
"\n faddp %%st,%%st(2)"
"\n fxch %%st(3)"
"\n fmull 224(%0)"
"\n fld %%st(1)"
"\n faddl 8(%0)"
"\n fld %%st(3)"
"\n fmull 280(%0)"
"\n fxch %%st(5)"
"\n faddp %%st,%%st(2)"
"\n fsubl 8(%0)"
"\n fld %%st(6)"
"\n fmull 272(%0)"
"\n fxch %%st(3)"
"\n fsub %%st(1),%%st"
"\n fxch %%st(2)"
"\n faddp %%st,%%st(5)"
"\n fld %%st(5)"
"\n fmull 264(%0)"
"\n fxch %%st(5)"
"\n faddp %%st,%%st(3)"
"\n fxch %%st(3)"
"\n fmull 216(%0)"
"\n fldl 288(%0,%1,8)"
"\n fxch %%st(3)"
"\n faddp %%st,%%st(5)"
"\n fxch %%st(2)"
"\n fmull 232(%0)"
"\n fldl 1056(%0,%1,8)"
"\n fxch %%st(5)"
"\n faddp %%st,%%st(4)"
"\n fxch %%st(4)"
"\n fmull 224(%0)"
"\n fxch %%st(2)"
"\n faddp %%st,%%st(4)"
"\n fld %%st(2)"
"\n faddl 16(%0)"
"\n fld %%st(6)"
"\n fmull 280(%0)"
"\n fxch %%st(1)"
"\n fsubl 16(%0)"
"\n fxch %%st(5)"
"\n faddp %%st,%%st(3)"
"\n fld %%st(5)"
"\n fmull 272(%0)"
"\n fxch %%st(4)"
"\n fsub %%st(5),%%st"
"\n fxch %%st(3)"
"\n faddp %%st,%%st(1)"
"\n fxch %%st(6)"
"\n fmull 216(%0)"
"\n fldl 288(%0,%1,8)"
"\n fxch %%st(7)"
"\n faddp %%st,%%st(4)"
"\n fxch %%st(6)"
"\n fmull 240(%0)"
"\n fldl 1056(%0,%1,8)"
"\n fxch %%st(4)"
"\n faddp %%st,%%st(5)"
"\n fxch %%st(3)"
"\n fmull 232(%0)"
"\n fxch %%st(6)"
"\n faddp %%st,%%st(3)"
"\n fld %%st(3)"
"\n faddl 24(%0)"
"\n fldl 1824(%0,%1,8)"
"\n fmull 224(%0)"
"\n fxch %%st(4)"
"\n faddp %%st,%%st(7)"
"\n fsubl 24(%0)"
"\n fld %%st(5)"
"\n fmull 280(%0)"
"\n fxch %%st(7)"
"\n faddp %%st,%%st(4)"
"\n fsubr %%st,%%st(4)"
"\n fxch %%st(5)"
"\n fmull 216(%0)"
"\n fxch %%st(3)"
"\n faddp %%st,%%st(6)"
"\n fldl 288(%0,%1,8)"
"\n fmull 248(%0)"
"\n fxch %%st(6)"
"\n faddp %%st,%%st(5)"
"\n fldl 1056(%0,%1,8)"
"\n fmull 240(%0)"
"\n fld %%st(5)"
"\n faddl 32(%0)"
"\n fxch %%st(4)"
"\n faddp %%st,%%st(7)"
"\n fldl 1824(%0,%1,8)"
"\n fmull 232(%0)"
"\n fxch %%st(4)"
"\n fsubl 32(%0)"
"\n fxch %%st(7)"
"\n faddp %%st,%%st(1)"
"\n fldl 2592(%0,%1,8)"
"\n fxch %%st(6)"
"\n fsub %%st(7),%%st"
"\n fxch %%st(6)"
"\n fmull 224(%0)"
"\n fxch %%st(1)"
"\n faddp %%st,%%st(7)"
"\n fxch %%st(6)"
"\n faddp %%st,%%st(3)"
"\n fxch %%st(2)"
"\n faddp %%st,%%st(5)"
"\n fld %%st(4)"
"\n faddl 40(%0)"
"\n fsubl 40(%0)"
"\n fsubr %%st,%%st(5)"
"\n fmull (%0)"
"\n faddp %%st,%%st(2)"
"\n fld %%st(1)"
"\n faddl 8(%0)"
"\n fsubl 8(%0)"
"\n fadd %%st,%%st(1)"
"\n fsubrp %%st,%%st(2)"
"\n decl %1"
"\n jns 0b"

"\n fxch %%st(4)"
"\n fstl 248(%0)"
"\n fmull (%0)"
"\n fxch %%st(3)"
"\n fstl 240(%0)"
"\n fmull (%0)"
"\n fxch %%st(2)"
"\n fstl 232(%0)"
"\n fmull (%0)"
"\n fxch %%st(4)"
"\n fstl 224(%0)"
"\n fmull (%0)"
"\n fxch %%st(1)"
"\n fstpl 216(%0)"
"\n fxch %%st(2)"
"\n fstpl 280(%0)"
"\n fstpl 272(%0)"
"\n fxch %%st(1)"
"\n fstpl 264(%0)"
"\n fstpl 256(%0)"

    : /* XXX: actually changing bigr and r */
    : "r"(bigr), "r"(r)
  );
}
