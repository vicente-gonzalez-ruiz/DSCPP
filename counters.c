/*
 * counters.c -- Implementaci'on de los contadors.
 * Ultima modificaci'on: 23-11-00.
 * gse.
 */

#include "gates.h"
#include "latches.h"
#include "flip-flops.h"
#include "counters.h"

WIRE Vcc=VCC; /* La tensi'on de alimentaci'on com'un */

void AU_4BIN_COUNTER::run(WIRE &ck, WIRE &Q0, WIRE &Q1, WIRE &Q2, WIRE &Q3) {
  f0.run(Vcc,Vcc,ck,Q0,Q0n);
  f1.run(Vcc,Vcc,Q0,Q1,Q1n);
  f2.run(Vcc,Vcc,Q1,Q2,Q2n);
  f3.run(Vcc,Vcc,Q2,Q3,Q3n);
}

void AU_8BIN_COUNTER::run(WIRE &ck,
                          WIRE &Q0, WIRE &Q1, WIRE &Q2, WIRE &Q3,
                          WIRE &Q4, WIRE &Q5, WIRE &Q6, WIRE &Q7) {
  co0.run(ck,Q0,Q1,Q2,Q3);
  co1.run(Q3,Q4,Q5,Q6,Q7);
}

void AD_4BIN_COUNTER::run(WIRE &ck, WIRE &Q0, WIRE &Q1, WIRE &Q2, WIRE &Q3) {
  f0.run(Vcc,Vcc,ck,Q0,Q0n);
  f1.run(Vcc,Vcc,Q0n,Q1,Q1n);
  f2.run(Vcc,Vcc,Q1n,Q2,Q2n);
  f3.run(Vcc,Vcc,Q2n,Q3,Q3n);
}

void AU_BCD_COUNTER::run(WIRE &ck, WIRE &Q0, WIRE &Q1, WIRE &Q2, WIRE &Q3) {
  f0.run(Vcc,Vcc,ck,Q0,Q0n);
  f1.run(Q3n,Vcc,Q0,Q1,Q1n);
  f2.run(Vcc,Vcc,Q1,Q2,Q2n);
  f3.run(Vcc,Vcc,a,Q3,Q3n);
  na0.run(b,Q0,Q3);
  na1.run(a,b,Q2n);
}

void AU_BCD_COUNTER_CLR::run(WIRE &ck, WIRE &Q0, WIRE &Q1, WIRE &Q2, WIRE &Q3) {
  f0.run(Vcc,Vcc,ck,Vcc,newCLR,Q0,Q0n);
  f1.run(Vcc,Vcc,Q0,Vcc,newCLR,Q1,Q1n);
  f2.run(Vcc,Vcc,Q1,Vcc,newCLR,Q2,Q2n);
  f3.run(Vcc,Vcc,Q2,Vcc,newCLR,Q3,Q3n);
  na.run(CLR,Q1,Q3);
  n0.run(w0,CLR); n1.run(w1,w0); an.run(newCLR,CLR,w1);
}

void BC_SU_BIN_COUNTER::run(WIRE &ck, WIRE &a, WIRE &b, WIRE &c, WIRE &d) {
  y.run(c,a,b);
  ff.run(c,c,ck,d,Qn);
}

void SU_4BIN_COUNTER::run(WIRE &ck, WIRE Q[4]) {
  c[0].run(ck,Vcc,Vcc,a[0],Q[0]);
  c[1].run(ck,a[0],Q[0],a[1],Q[1]);
  c[2].run(ck,a[1],Q[1],a[2],Q[2]);
  c[3].run(ck,a[2],Q[2],a[3],Q[3]);
}

void SU_8BIN_COUNTER::run(WIRE &ck, WIRE Q[4]) {
  c[0].run(ck,Vcc,Vcc,a[0],Q[0]);
  for(int i=1;i<8;i++)
    c[i].run(ck,a[i-1],Q[i-1],a[i],Q[i]);
}


void SU_BIN_COUNTER::create(int size) {
  this->size =size;
  c = new BC_SU_BIN_COUNTER[size];
  a = new WIRE[size];
}
void SU_BIN_COUNTER::destroy() {
  delete c;
  delete a;
}
void SU_BIN_COUNTER::run(WIRE &ck, WIRE Q[]) {
  c[0].run(ck,Vcc,Vcc,a[0],Q[0]);
  for(int i=1;i<size;i++)
    c[i].run(ck,a[i-1],Q[i-1],a[i],Q[i]);
}
