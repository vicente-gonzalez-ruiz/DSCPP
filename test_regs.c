/*
 * test_regs.c -- Prueba de los registros.
 * Ultima modificaci'on: 23-11-00.
 * gse.
 */

#include <stdio.h>
#include "gates.h"
#include "latches.h"
#include "flip-flops.h"
#include "regs.h"
#include "clock.h"

#ifdef _REG_8_
void main() {
  FILE *data=fopen("data","w");
  FILE *data_sdl=fopen("data.sdl","w");
  int iter=3000,j;
  WIRE i[8],o[8],ck;
  REG_8 reg;
  fprintf(data,"ck");
  for(j=0;j<8;j++) {
    i[j]=GND;
    fprintf(data," i[%d]",j);
  }
  for(j=0;j<8;j++) {
    o[j]=GND;
    fprintf(data," o[%d]",j);
  }
  //  fprintf(data,"\n");
  while(iter--) {
    Clock(iter,17,ck);
    for(j=0;j<8;j++) {
      Clock(iter,1<<(j+5),i[j]);
    }
    reg.run(ck,i,o);
    fprintf(data_sdl," %3d",ck);
    for(j=0;j<8;j++) {
      fprintf(data_sdl," %3d",i[j]);
    }
    for(j=0;j<8;j++) {
      fprintf(data_sdl," %3d",o[j]);
    }
    fprintf(data_sdl,"\n");
  }
}
#endif

