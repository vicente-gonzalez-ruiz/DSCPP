/*
 * gates.cc -- Bilbioteca de puertas l'ogicas.
 * gse. 1999.
 */

#include "defs.h"
#include "gates.h"

NOT::NOT() {delay=0;}
void NOT::run(WIRE &i, WIRE &o) {
  o = delay;
  delay = ~i;
}

AND::AND(){delay=0;}
void AND::run(WIRE &i0, WIRE &i1, WIRE &o) {
  o = delay;
  delay = i0 & i1;
}

NAND::NAND() {delay=0;}
void NAND::run(WIRE &i0, WIRE &i1, WIRE &o) {
  o = delay;
  delay = ~(i0 & i1);
}

OR::OR(){delay=0;}
void OR::run(WIRE &i0, WIRE &i1, WIRE &o) {
  o = delay;
  delay = i0 | i1;
}

NOR::NOR(){delay=0;}
void NOR::run(WIRE &i0, WIRE &i1, WIRE &o) {
  o = delay;
  delay = ~(i0 | i1);
}

XOR::XOR(){delay=0;}
void XOR::run(WIRE &i0, WIRE &i1, WIRE &o) {
  o = delay;
  delay = i0 ^ i1;
}

BUFFER_Z::BUFFER_Z(){delay=0;}
void BUFFER_Z::run(WIRE &i, WIRE &e, WIRE &o) {
  if (e==VCC) o = delay;
  delay = i;
}

#ifdef _TEST_
#include <iostream.h>
#include <iomanip.h>
#include "clock.h"

#ifdef _NOT_
void main() {
  int iter=30;
  WIRE a,b;
  NOT n;
  CLOCK ck(5);
  while(iter--) {
    a = ck.run(iter);
    n.run(a,b);
    cout << setw(4) << (int)a
         << setw(4) << (int)b
         << '\n';
  }
}
#endif

#ifdef _AND_
void main() {
  int iter=1000;
  WIRE a=0,b=0,c=0;
  AND and;
  CLOCK ck0(3),ck1(7);
  while(iter--) {
    a = ck0.run(iter);
    b = ck1.run(iter);
    and.run(a,b,c);
    cout << setw(4) << (int)a
         << setw(4) << (int)b
         << setw(4) << (int)c
         << '\n';
  }
}
#endif /*_AND_*/

#ifdef _AND3_
void main() {
  int iter=100;
  WIRE a=0,b=0,c=0,d=0,e=0;
  AND a0,a1;
  CLOCK ck0(8),ck1(10),ck2(13);
  while(iter--) {
    a=ck0.run(iter);
    b=ck1.run(iter);
    c=ck2.run(iter);
    a0.run(a,b,d);
    a1.run(c,d,e);
    cout << setw(4) << (int)a
         << setw(4) << (int)b
         << setw(4) << (int)c
         << setw(4) << (int)e
         << '\n';
  }
}
#endif

#ifdef _NAND4_
void main() {
  int iter=1000;
  WIRE a=0,b=0,c=0,d=0,e;
  NAND4 na;
  CLOCK ck0(8),ck1(10),ck2(13),ck3(17);
  while(iter--) {
    a=ck0.run(iter);
    b=ck1.run(iter);
    c=ck2.run(iter);
    d=ck3.run(iter);
    na.run(a,b,c,d,e);
    cout << setw(4) << (int)a
         << setw(4) << (int)b
         << setw(4) << (int)c
         << setw(4) << (int)e
         << '\n';
  }
}
#endif

#ifdef _NAND_
void main() {
  int iter=100;
  WIRE a=0,b=0,c=0,d=0,e=0;
  NAND nand0,nand1;
  CLOCK ck0(2),ck1(3),ck2(1);
  while(iter--) {
    a = ck0.run(iter);
    b = ck1.run(iter);
    c = ck2.run(iter);
    nand0.run(a,b,d);
    nand1.run(b,c,e);
    cout << setw(4) << (int)a
         << setw(4) << (int)b
         << setw(4) << (int)c
         << setw(4) << (int)d
         << setw(4) << (int)e
         << '\n';
  }
}
#endif

#ifdef _OR_
void main() {
  int iter=100;
  WIRE a=0,b=0,c=0;
  OR or;
  CLOCK ck0(8),ck1(10);
  while(iter--) {
    a = ck0.run(iter);
    b = ck1.run(iter);
    or.run(a,b,c);
    cout << setw(4) << (int)a
         << setw(4) << (int)b
         << setw(4) << (int)c
         << '\n';
  }
}
#endif

#ifdef _XOR_
void main() {
  int iter=100;
  WIRE a=0,b=0,c=0;
  XOR xor;
  CLOCK ck0(8),ck1(10);
  while(iter--) {
    a = ck0.run(iter);
    b = ck1.run(iter);
    xor.run(a,b,c);
    cout << setw(4) << (int)a
         << setw(4) << (int)b
         << setw(4) << (int)c
         << '\n';
  }
}
#endif

#ifdef _NOR_
void main() {
  int iter=100;
  WIRE a=0,b=0,c=0;
  NOR nor;
  CLOCK ck0(1),ck1(5);
  while(iter--) {
    a = ck0.run(iter);
    b = ck1.run(iter);
    nor.run(a,b,c);
    cout << setw(4) << (int)a
         << setw(4) << (int)b
         << setw(4) << (int)c
         << '\n';
  }
}
#endif

#ifdef _BUFFER_Z_
void main() {
  int iter=1000;
  WIRE i=0,e=0,o=0;
  BUFFER_Z x;
  CLOCK ck0(8),ck1(130);
  while(iter--) {
    i = ck0.run(iter);
    e = ck1.run(iter);
    x.run(i,e,o);
    cout << setw(4) << (int)i
         << setw(4) << (int)e
         << setw(4) << (int)o
         << '\n';
  }
}
#endif
#endif /* _TEST_*/
