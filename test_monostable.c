/*
 * test_monostable.c -- Testeo del monostable
 * Ultima modificaci'on: 17-10-2000.
 * gse.
 */

#include <stdio.h>
#include "gates.h"
#include "monostable.h"

void main() {
  FILE *data=fopen("data","w");
  FILE *data_sdl=fopen("data.sdl","w");
  int iteraciones=500;
  WIRE a;
  MONOSTABLE ck(3,50);
  fprintf(data,"a");
  while(iteraciones--) {
    ck.run(a);
    fprintf(data_sdl,"%3d\n",a);
  }
}
