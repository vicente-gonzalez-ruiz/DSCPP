/*
 * flip-flops.c -- Implementaci'on de los flip-flops.
 * Ultima modificaci'on: 23-11-00.
 * gse.
 */

#include "gates.h"
#include "latches.h"
#include "flip-flops.h"

void POSITIVE_DETECTOR::run(WIRE &i, WIRE &o) {
  n0.run(a0,i);
  n1.run(a1,a0);
  n2.run(a2,a1);
  an.run(o,i,a2);
}

void NEGATIVE_DETECTOR::run(WIRE &i, WIRE &o) {
  no.run(ni,i);
  pd.run(ni,o);
}

void POSITIVE_GLICH_SUPRESSOR::run(WIRE &a, WIRE &d) {
  no[0].run(b,a);
  no[1].run(c,b);
  y.run(d,a,c);
}

void NEGATIVE_GLICH_SUPRESSOR::run(WIRE &a, WIRE &d) {
  no[0].run(b,a);
  no[1].run(c,b);
  o.run(d,a,c);
}

void NEG_SR_FF::run(WIRE &S, WIRE &R, WIRE &ck, WIRE &Q, WIRE &Qn) {
  detec.run(ck,pulse);
  latch.run(S,R,pulse,Q,Qn);
}

void NEG_D_FF::run(WIRE &D, WIRE &ck, WIRE &Q, WIRE &Qn) {
  n.run(Dn,D);
  ff.run(D,Dn,ck,Q,Qn);
}

void NEG_JK_FF::run(WIRE &J, WIRE &K, WIRE &c, WIRE &Q, WIRE &Qn) {
  dec.run(c,pulse);
  latch_JK.run(J,K,pulse,Q,Qn);
}

void NEG_JK_FF_PC::run(WIRE &J, WIRE &K, WIRE &c, WIRE &PRT,
                       WIRE &CLR, WIRE &Q, WIRE &Qn) {
  dec.run(c,pulse);
  na0.run(a,4,CLR,Qn,J,pulse);
  na1.run(b,4,PRT, Q,K,pulse);
  na2.run( Q,3,Qn,PRT,a);
  na3.run(Qn,3, Q,CLR,b);
}

void NEG_D_FF_PC::run(WIRE &D, WIRE &ck, WIRE &Q, WIRE &Qn,
		      WIRE &preset, WIRE &clear) {
  no.run(Dn,D);
  ff.run(D,Dn,ck,preset,clear,Q,Qn);
}

void SR_FF_MS::run(WIRE &S, WIRE &R, WIRE &ck, WIRE &Q, WIRE &Qn) {
  n.run(nck,ck);
  master.run(S,R,ck,q,qn);
  slave.run(q,qn,nck,Q,Qn);
}

void JK_FF_MS::run (WIRE &J, WIRE &K, WIRE &ck, WIRE &Q, WIRE &Qn) {
  a0.run(a,J,Qn);
  a1.run(b,K,Q);
  ff.run(a,b,ck,Q,Qn);
}
