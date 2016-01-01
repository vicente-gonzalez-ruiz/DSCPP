/*
 * latches.c -- Biblioteca de cerrojos.
 * Ultima modificaci'on: 23-11-2000.
 * gse.
 */

#include "gates.h"
#include "latches.h"

void SR_LATCH_NOR::run(WIRE &S, WIRE &R, WIRE &Q, WIRE &Qn) {
  n0.run(Q,R,Qn);
  n1.run(Qn,S,Q);
}

void SR_LATCH_NAND::run(WIRE &S, WIRE &R, WIRE &Q, WIRE &Qn) {
  n0.run(Q,R,Qn);
  n1.run(Qn,S,Q);
}

void D_LATCH::run(WIRE &D, WIRE &Q, WIRE &Qn) {
  n.run(Dn,D);
  l.run(D,Dn,Q,Qn);
}

void JK_LATCH::run(WIRE &J, WIRE &K, WIRE &Q, WIRE &Qn) {
  na0.run(a,J,Qn);
  na1.run(b,K,Q);
  na2.run(Q,a,Qn);
  na3.run(Qn,b,Q);
}

void JK_LATCH_ENABLE::run(WIRE &J, WIRE &K, WIRE &E, WIRE &Q, WIRE &Qn) {
  na0.run(a,3,J,E,Qn);
  na1.run(b,3,K,E,Q);
  na2.run(Q,a,Qn);
  na3.run(Qn,b,Q);
}

void SR_LATCH_ENABLE::run(WIRE &S, WIRE &R, WIRE &E, WIRE &Q, WIRE &Qn) {
  na0.run(a,S,E);
  na1.run(b,R,E);
  latch.run(b,a,Q,Qn);
}

