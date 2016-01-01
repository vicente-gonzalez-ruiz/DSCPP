/*
 * test_clock.c -- Testeo del reloj
 * Ultima modificaci'on: 17-10-2000.
 * gse.
 */

#include <stdio.h>
#include "gates.h"
#include "clock.h"
#include "print.h"

void main() {
  FILE *data=fopen("data","w");
  FILE *data_sdl=fopen("data.sdl","w");
  int iter=1;
  WIRE a=GND,b=GND,c=GND;
  CLOCK ck0(3),ck1(7),ck2(11);
  fprintf(data,"a b c");
  while(iter++<100) {
    ck0.run(iter,a);
    ck1.run(iter,b);
    ck2.run(iter,c);
    fprintf(data_sdl,"%3d %3d %3d\n",a,b,c);
  }
}

