#include <string.h>

typedef struct {
  double r;
  double i;
} Cmplx;


Cmplx cmplx_add(Cmplx z1,Cmplx z2){
  Cmplx r = {.r = z1.r+z2.r,.i=z1.i+z2.i};
  return r;
}

Cmplx cmplx_sub(Cmplx z1,Cmplx z2){
  Cmplx r = {.r = z1.r-z2.r,.i=z1.i-z2.i};
  return r;
}



Cmplx cmplx_multiply(Cmplx z1,Cmplx z2){
  double a1,a2,b1,b2;

  a1 = z1.r;
  b1 = z1.i;
  a2 = z2.r;
  b2 = z2.i;

  Cmplx r = {.r = a1*a2-b1*b2, .i = a1*b2+a2*b1};
  return r;
}

Cmplx cmplx_divide(Cmplx z1,Cmplx z2){
  double a1,a2,b1,b2,sqrDiff;

  a1 = z1.r;
  b1 = z1.i;
  a2 = z2.r;
  b2 = z2.i;

  sqrDiff = a2*a2 - b2*b2;

  Cmplx r = {.r = a1*a2-b1*b2, .i = a2*b1 - a1*b2};
  return r;
}

Cmplx cmplx_sqr(Cmplx z){
  Cmplx r = {.r = z.r*z.r - z.i*z.i, .i = 2*z.r*z.i};
  return r;
}
