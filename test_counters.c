/*
 * test_counters.c -- Test de los contadores implementados.
 * Ultima modificaci'on: 23-11-00.
 * gse. 
 */

#include "gates.h"
#include "latches.h"
#include "flip-flops.h"
#include "counters.h"
#include "clock.h"
#include <stdio.h>

#ifdef _AU_4BIN_COUNTER_
void main() {
  FILE *data=fopen("data","w");
  FILE *data_sdl=fopen("data.sdl","w");
  int iter=5000;
  WIRE c=0,Q0=0,Q1=0,Q2=0,Q3=0;
  AU_4BIN_COUNTER co;
  CLOCK clock(50);
  fprintf(data,"c Q0 Q1 Q2 Q3");
  while(iter--) {
    clock.run(iter,c);
    co.run(c,Q0,Q1,Q2,Q3);
    fprintf(data_sdl,"%3d %3d %3d %3d %3d\n",c,Q0,Q1,Q2,Q3);
  }
}
#endif
#ifdef _AU_8BIN_COUNTER_
void main() {
  FILE *data=fopen("data","w");
  FILE *data_sdl=fopen("data.sdl","w");
  int iter=2500;
  WIRE c,Q0,Q1,Q2,Q3,Q4,Q5,Q6,Q7;
  AU_8BIN_COUNTER co;
  CLOCK clock(7);
  fprintf(data,"c Q0 Q1 Q2 Q3 Q4 Q5 Q6 Q7");
  while(iter--) {
    clock.run(iter,c);
    co.run(c,Q0,Q1,Q2,Q3,Q4,Q5,Q6,Q7);
    fprintf(data_sdl,"%3d %3d %3d %3d %3d %3d %3d %3d %3d\n",
	   c,Q0,Q1,Q2,Q3,Q4,Q5,Q6,Q7);
  }
}
#endif

#ifdef _AD_4BIN_COUNTER_
void main() {
  FILE *data=fopen("data","w");
  FILE *data_sdl=fopen("data.sdl","w");
  int iter=5000;
  WIRE c=0,Q0=0,Q1=0,Q2=0,Q3=0;
  AD_4BIN_COUNTER co;
  CLOCK clock(50);
  fprintf(data,"c Q0 Q1 Q2 Q3");
  while(iter--) {
    clock.run(iter,c);
    co.run(c,Q0,Q1,Q2,Q3);
    fprintf(data_sdl,"%3d %3d %3d %3d %3d\n",c,Q0,Q1,Q2,Q3);
  }
}
#endif

#ifdef _AU_BCD_COUNTER_
void main() {
  FILE *data=fopen("data","w");
  FILE *data_sdl=fopen("data.sdl","w");
  int iter=2000;
  WIRE c=0,Q0=0,Q1=0,Q2=0,Q3=0;
  AU_BCD_COUNTER co;
  CLOCK clock(50);
  fprintf(data,"c Q0 Q1 Q2 Q3");
  while(iter--) {
    clock.run(iter,c);
    co.run(c,Q0,Q1,Q2,Q3);
    fprintf(data_sdl,"%3d %3d %3d %3d %3d\n",c,Q0,Q1,Q2,Q3);
  }
}
#endif

#ifdef _AU_BCD_COUNTER_CLR_
void main() {
  FILE *data=fopen("data","w");
  FILE *data_sdl=fopen("data.sdl","w");
  int iter=3000;
  WIRE c=0,Q0=0,Q1=0,Q2=0,Q3=0;
  AU_BCD_COUNTER_CLR co;
  CLOCK clock(50);
  fprintf(data,"c Q0 Q1 Q2 Q3");
  while(iter--) {
    clock.run(iter,c);
    co.run(c,Q0,Q1,Q2,Q3);
    fprintf(data_sdl,"%3d %3d %3d %3d %3d\n",c,Q0,Q1,Q2,Q3);
  }
}
#endif

#ifdef _SU_4BIN_COUNTER_
void main() {
  FILE *data=fopen("data","w");
  FILE *data_sdl=fopen("data.sdl","w");
  int iter=1000;
  WIRE c=0,Q[4]={0,0,0,0};
  SU_4BIN_COUNTER co;
  CLOCK clock(5);
  fprintf(data,"c Q[0] Q[1] Q[2] Q[3]");
  while(iter--) {
    clock.run(iter,c);
    co.run(c,Q);
    fprintf(data_sdl,"%3d %3d %3d %3d %3d\n",c,Q[0],Q[1],Q[2],Q[3]);
  }
}
#endif

#ifdef _SU_8BIN_COUNTER_
void main() {
  FILE *data=fopen("data","w");
  FILE *data_sdl=fopen("data.sdl","w");
  int iter=2500;
  WIRE c=0,Q[8]={0,0,0,0,0,0,0,0};
  SU_8BIN_COUNTER co;
  CLOCK clock(10);
  fprintf(data,"c Q[0] Q[1] Q[2] Q[3] Q[4] Q[5] Q[6] Q[7]");
  while(iter--) {
    clock.run(iter,c);
    co.run(c,Q);
    fprintf(data_sdl,"%3d",c);
    for(int i=0;i<8;i++) {
      fprintf(data_sdl," %3d",Q[i]);
    }
    fprintf(data_sdl,"\n");
  }
}
#endif

#ifdef _SU_BIN_COUNTER_
#include <stdlib.h> /* atoi() */
int main(int argc, char *argv[]) {
  if(argc<3) {
    fprintf(stderr,"%s tama~no_del_contador n'umero_de_iteraciones\n",argv[0]);
    return 1;
  }
  FILE *data=fopen("data","w");
  FILE *data_sdl=fopen("data.sdl","w");
  int bits = atoi(argv[1]);
  int iters = atoi(argv[2]);
  WIRE ck;                  // Reloj
  WIRE *Q = new WIRE[bits]; // Salidas del contador
  SU_BIN_COUNTER co;        // Contador de tama~no variable
  co.create(bits);
  CLOCK clock(10);
  fprintf(data,"ck");
  for(int i=0;i<bits;i++) fprintf(data," Q[%d]",i);
  while(iters--) {
    clock.run(iters,ck);
    co.run(ck,Q);
    fprintf(data_sdl,"%3d",ck); 
    for(int i=0;i<bits;i++) {
      fprintf(data_sdl," %3d",Q[i]);
    }
    fprintf(data_sdl,"\n");
  }
  co.destroy();
}
#endif

