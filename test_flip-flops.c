/*
 * test_flip-flops.c -- Prueba de los flip-flops.
 * Ultima modificaci'on: 23-11-00.
 * gse.
 */

#include <stdio.h>
#include "gates.h"
#include "latches.h"
#include "flip-flops.h"
#include "clock.h"
#include "random.h"

#ifdef _POSITIVE_DETECTOR_
void main() {
  FILE *data=fopen("data","w");
  FILE *data_sdl=fopen("data.sdl","w");
  int iter=100;
  WIRE a=0,b=0;
  POSITIVE_DETECTOR x;
  CLOCK ck(20);
  fprintf(data,"a b");
  while(iter--) {
    ck.run(iter,a);
    x.run(a,b);
    fprintf(data_sdl,"%3d %3d\n",a,b);
  }
}
#endif

#ifdef _NEGATIVE_DETECTOR_
void main() {
  FILE *data=fopen("data","w");
  FILE *data_sdl=fopen("data.sdl","w");
  int iter=100;
  WIRE a,b;
  NEGATIVE_DETECTOR x;
  CLOCK ck(20);
  fprintf(data,"a b");
  while(iter--) {
    ck.run(iter,a);
    x.run(a,b);
    fprintf(data_sdl,"%3d %3d\n",a,b);
  }
}
#endif

#ifdef _NEGATIVE_GLICH_SUPRESSOR_
void main() {
  FILE *data=fopen("data","w");
  FILE *data_sdl=fopen("data.sdl","w");
  int iter=100;
  WIRE a,b;
  NEGATIVE_GLICH_SUPRESSOR x;
  RANDOM ck(2);
  fprintf(data,"a b");
  while(iter--) {
    ck.run(iter,a);
    x.run(a,b);
    fprintf(data_sdl,"%3d %3d\n",a,b);
  }
}
#endif

#ifdef _POSITIVE_GLICH_SUPRESSOR_
void main() {
  FILE *data=fopen("data","w");
  FILE *data_sdl=fopen("data.sdl","w");
  int iter=100;
  WIRE a,b;
  POSITIVE_GLICH_SUPRESSOR x;
  RANDOM ck(2);
  fprintf(data,"a b");
  while(iter--) {
    ck.run(iter,a);
    x.run(a,b);
    fprintf(data_sdl,"%3d %3d\n",a,b);
  }
}
#endif

#ifdef _NEG_SR_FF_
void main() {
  FILE *data=fopen("data","w");
  FILE *data_sdl=fopen("data.sdl","w");
  int iter=500;
  WIRE S=0,R=0,ck=0,Q=0,Qn=VCC;
  NEG_SR_FF l;
  CLOCK ck0(50), ck1(100), ck2(30);
  fprintf(data,"S R ck Q Qn");
  while(iter--) {
    ck0.run(iter,S);
    ck1.run(iter,R);
    ck2.run(iter,ck);
    l.run(S,R,ck,Q,Qn);
    fprintf(data_sdl,"%3d %3d %3d %3d %3d\n",S,R,ck,Q,Qn);
  }
}
#endif

#ifdef _NEG_D_FF_
void main() {
  FILE *data=fopen("data","w");
  FILE *data_sdl=fopen("data.sdl","w");
  int iter=200;
  WIRE D=0,ck=0,Q=0,Qn=VCC;
  NEG_D_FF ff;
  CLOCK ck0(31), ck1(17);
  fprintf(data,"D ck Q Qn");
  while(iter--) {
    ck0.run(iter,D);
    ck1.run(iter,ck);
    ff.run(D,ck,Q,Qn);
    fprintf(data_sdl,"%3d %3d %3d %3d\n",D,ck,Q,Qn);
  }
}
#endif

#ifdef _NEG_JK_FF_
void main() {
  FILE *data=fopen("data","w");
  FILE *data_sdl=fopen("data.sdl","w");
  int iter=2000;
  WIRE J,K,ck,Q=0,Qn=VCC;
  NEG_JK_FF l;
  CLOCK ck0(310), ck1(470), ck2(42);
  fprintf(data,"J K ck Q Qn");
  while(iter--) {
    ck0.run(iter,J);
    ck1.run(iter,K);
    ck2.run(iter,ck);
    l.run(J,K,ck,Q,Qn);
    fprintf(data_sdl,"%3d %3d %3d %3d %3d\n",J,K,ck,Q,Qn);
  }
}
#endif

#ifdef _NEG_JK_FF_PC_
void main() {
  FILE *data=fopen("data","w");
  FILE *data_sdl=fopen("data.sdl","w");
  int iter=8800;
  WIRE J,K,ck,Q=0,Qn=VCC,PRT=VCC,CLR=VCC;
  NEG_JK_FF_PC l;
  CLOCK ck0(500), ck1(1000), ck2(42), ck3(1200), ck4(4400);
  fprintf(data,"J K ck PRT CLR Q Qn");
  while(iter--) {
    ck0.run(iter,J);
    ck1.run(iter,K);
    ck2.run(iter,ck);
    ck3.run(iter,PRT);
    ck4.run(iter,CLR);
    l.run(J,K,ck,PRT,CLR,Q,Qn);
    fprintf(data_sdl,"%3d %3d %3d %3d %3d %3d %3d\n",J,K,ck,PRT,CLR,Q,Qn);
  }
}
#endif

#ifdef _NEG_D_FF_PC_
void main() {
  FILE *data=fopen("data","w");
  FILE *data_sdl=fopen("data.sdl","w");
  int iter=8800;
  WIRE D,ck,Q=0,Qn=VCC,PRT=VCC,CLR=VCC;
  NEG_D_FF_PC l;
  CLOCK ck0(500), ck2(42), ck3(1200), ck4(4400);
  fprintf(data,"D ck PRT CLR Q Qn");
  while(iter--) {
    ck0.run(iter,D);
    ck2.run(iter,ck);
    ck3.run(iter,PRT);
    ck4.run(iter,CLR);
    l.run(D,ck,Q,Qn,PRT,CLR);
    fprintf(data_sdl,"%3d %3d %3d %3d %3d %3d\n",D,ck,PRT,CLR,Q,Qn);
  }
}
#endif

#ifdef _PACO_
void main() {
  FILE *data=fopen("data","w");
  FILE *data_sdl=fopen("data.sdl","w");
  int iter=1000;
  WIRE D=0,Q=0,Qn=VCC,Dn=0,a=0,b=0,c=0,d=0,e=0,f=0;
  NEG_SR_FF l;
  NOT n,m0,m1,m2,m3,m4,m5;
  CLOCK ck(40);
  fprintf(data,"D Dn f Q Qn");
  while(iter--) {
    ck.run(iter,D);
    n.run(Dn,D);
    m0.run(a,D);
    m1.run(b,a);
    m2.run(c,b);
    m3.run(d,c);
    m4.run(e,d);
    m5.run(f,e);
    l.run(D,Dn,f,Q,Qn);
    fprintf(data_sdl,"%3d %3d %3d %3d %3d\n",D,Dn,f,Q,Qn);
  }
}
#endif

#ifdef _SR_FF_MS_
void main() {
  FILE *data=fopen("data","w");
  FILE *data_sdl=fopen("data.sdl","w");
  int iter=200;
  WIRE S=0,R=0,ck=0,Q=0,Qn=VCC;
  SR_FF_MS l;
  CLOCK ck0(31), ck1(47), ck2(17);
  fprintf(data,"S R ck Q Qn");
  while(iter--) {
    ck0.run(iter,S);
    ck1.run(iter,R);
    ck2.run(iter,ck);
    l.run(S,R,ck,Q,Qn);
    fprintf(data_sdl,"%3d %3d %3d %3d %3d\n",S,R,ck,Q,Qn);
  }
}
#endif

#ifdef _JK_FF_MS_
void main() {
  FILE *data=fopen("data","w");
  FILE *data_sdl=fopen("data.sdl","w");
  int iter=800;
  WIRE J,K,ck,Q=0,Qn;
  JK_FF_MS l;
  CLOCK ck0(91), ck1(147), ck2(7);
  fprintf(data,"J K ck Q Qn");
  while(iter--) {
    ck0.run(iter,J);
    ck1.run(iter,K);
    ck2.run(iter,ck);
    l.run(J,K,ck,Q,Qn);
    fprintf(data_sdl,"%3d %3d %3d %3d %3d\n",J,K,ck,Q,Qn);
  }
}
#endif

