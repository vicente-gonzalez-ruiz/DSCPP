/*
 * test_latches.c -- Testeo de la biblioteca de cerrojos.
 * Ultima modificaci'on: 23-11-2000.
 * gse.
 */

#include <stdio.h>
#include "gates.h"
#include "latches.h"
#include "clock.h"

#ifdef _SR_LATCH_NOR_
int main() {
  FILE *data=fopen("data","w");
  FILE *data_sdl=fopen("data.sdl","w");
  int iter=0;
  WIRE S=GND,R=GND,Q=GND,Qn=VCC;
  SR_LATCH_NOR l;
  CLOCK ck0(50), ck1(100);
  fprintf(data,"S R Q Qn");
  while(iter++<1000) {
    ck0.run(iter,S);
    ck1.run(iter,R);
    l.run(S,R,Q,Qn);
    fprintf(data_sdl,"%3d %3d %3d %3d\n",S,R,Q,Qn);
  }
}
#endif

#ifdef _SR_LATCH_NAND_
int main() {
  FILE *data=fopen("data","w");
  FILE *data_sdl=fopen("data.sdl","w");
  int iter=500;
  WIRE S=GND,R=GND,Q=GND,Qn=VCC;
  SR_LATCH_NAND l;
  CLOCK ck0(25), ck1(50);
  fprintf(data,"S R Q Qn");
  while(iter--) {
    ck0.run(iter,S);
    ck1.run(iter,R);
    l.run(S,R,Q,Qn);
    fprintf(data_sdl,"%3d %3d %3d %3d\n",S,R,Q,Qn);
  }
}
#endif

#ifdef _D_LATCH_
int main() {
  FILE *data=fopen("data","w");
  FILE *data_sdl=fopen("data.sdl","w");
  int iter=400;
  WIRE D=GND,Q=GND,Qn=VCC;
  D_LATCH l;
  CLOCK ck(50);
  fprintf(data,"D Q Qn");
  while(iter--) {
    ck.run(iter,D);
    l.run(D,Q,Qn);
    fprintf(data_sdl,"%3d %3d %3d\n",D,Q,Qn);
  }
}
#endif

#ifdef _JK_LATCH_
int main() {
  FILE *data=fopen("data","w");
  FILE *data_sdl=fopen("data.sdl","w");
  int iter=0;
  WIRE J=GND,K=GND,Q=GND,Qn=VCC;
  JK_LATCH l;
  CLOCK ck0(50), ck1(70);
  fprintf(data,"J K Q Qn");
  while(iter++<1000) {
    ck0.run(iter,J);
    ck1.run(iter,K);
    l.run(J,K,Q,Qn);
    fprintf(data_sdl,"%3d %3d %3d %3d\n",J,K,Q,Qn);
  }
}
#endif

#ifdef _JK_LATCH_ENABLE_
int main() {
  FILE *data=fopen("data","w");
  FILE *data_sdl=fopen("data.sdl","w");
  int iter=200;
  WIRE J=GND,K=GND,E=GND,Q=GND,Qn=VCC;
  JK_LATCH_ENABLE l;
  CLOCK ck0(17), ck1(23), ck2(100);
  fprintf(data,"J K E Q Qn");
  while(iter--) {
    ck0.run(iter,J);
    ck1.run(iter,K);
    ck2.run(iter,E);
    l.run(J,K,E,Q,Qn);
    fprintf(data_sdl,"%3d %3d %3d %3d %3d\n",J,K,E,Q,Qn);
  }
}
#endif

#ifdef _SR_LATCH_ENABLE_
int main() {
  FILE *data=fopen("data","w");
  FILE *data_sdl=fopen("data.sdl","w");
  int iter=600;
  WIRE S=GND,R=GND,E=GND,Q=GND,Qn=VCC;
  SR_LATCH_ENABLE l;
  CLOCK ck0(25), ck1(50), ck2(100);
  fprintf(data,"S R E Q Qn");
  while(iter--) {
    ck0.run(iter,S);
    ck1.run(iter,R);
    ck2.run(iter,E);
    l.run(S,R,E,Q,Qn);
    fprintf(data_sdl,"%3d %3d %3d %3d %3d\n",S,R,E,Q,Qn);
  }
}
#endif
