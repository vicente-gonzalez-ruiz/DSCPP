#include <iostream.h>
#include <iomanip.h>
#include "gates.h"
#include "clock.h"

#ifdef _NOT_
void main() {
  int iter=30;
  WIRE a=GND,b;
  NOT gate;
  CLOCK ck(5);
  while(iter--) {
    ck.run(iter,a);
    gate.run(b,a);
    cout << setw(4) << (int)a
         << setw(4) << (int)b
         << '\n';
  }
}
#endif

#ifdef _AND_
void main() {
  int iter=100;
  WIRE a=GND,b=GND,c=GND;
  AND gate;
  CLOCK ck0(3), ck1(7);
  while(iter--) {
    ck0.run(iter,a);
    ck1.run(iter,b);
    gate.run(c,a,b);
    cout << setw(4) << (int)a
         << setw(4) << (int)b
         << setw(4) << (int)c
         << '\n';
  }
}
#endif

#ifdef _AND3_
void main() {
  int iter=100;
  WIRE i[3],o;
  AND gate;
  i[0] = i[1] = i[2] = GND;
  CLOCK ck0(8), ck1(10), ck2(13);
  while(iter--) {
    ck0.run(iter,i[0]);
    ck1.run(iter,i[1]);
    ck2.run(iter,i[2]);
    gate.run(o,3,i);
    cout << setw(4) << (int)i[0]
         << setw(4) << (int)i[1]
         << setw(4) << (int)i[2]
         << setw(4) << (int)o
         << '\n';
  }
}
#endif

#ifdef _AND3b_
void main() {
  int iter=100;
  WIRE i0,i1,i2,o;
  AND gate;
  CLOCK ck0(8), ck1(10), ck2(13);
  //i0 = i1 = i2 = o = GND;
  i0=134; i1=97, i2=83;
  while(iter--) {
    ck0.run(iter,i0);
    ck1.run(iter,i1);
    ck2.run(iter,i2);
    gate.run(o,3,i0,i1,i2);
    cout << setw(4) << (int)i0
         << setw(4) << (int)i1
         << setw(4) << (int)i2
         << setw(4) << (int)o
         << '\n';
  }
}
#endif

#ifdef _OR_
void main() {
  int iter=100;
  WIRE a=GND,b=GND,c=GND;
  OR gate;
  CLOCK ck0(8), ck1(10);
  while(iter--) {
    ck0.run(iter,a);
    ck1.run(iter,b);
    gate.run(c,a,b);
    cout << setw(4) << (int)a
         << setw(4) << (int)b
         << setw(4) << (int)c
         << '\n';
  }
}
#endif

#ifdef _OR3_
void main() {
  int iter=100;
  WIRE i[3],o;
  OR gate;
  CLOCK ck0(8), ck1(10), ck2(13);
  while(iter--) {
    ck0.run(iter,i[0]);
    ck1.run(iter,i[1]);
    ck2.run(iter,i[2]);
    gate.run(o,3,i);
    cout << setw(4) << (int)i[0]
         << setw(4) << (int)i[1]
         << setw(4) << (int)i[2]
         << setw(4) << (int)o
         << '\n';
  }
}
#endif

#ifdef _NAND_
void main() {
  int iter=50;
  WIRE a=GND,b=GND,c=GND;
  NAND gate;
  CLOCK ck0(3), ck1(7);
  while(iter--) {
    ck0.run(iter,a);
    ck1.run(iter,b);
    gate.run(c,a,b);
    cout << setw(4) << (int)a
         << setw(4) << (int)b
         << setw(4) << (int)c
         << '\n';
  }
  iter=50;
  CLOCK ck2(2);
  while(iter--) {
    ck0.run(iter,a);
    ck2.run(iter,b);
    gate.run(c,a,b);
    cout << setw(4) << (int)a
         << setw(4) << (int)b
         << setw(4) << (int)c
         << '\n';
  }
  iter=50;
  CLOCK ck3(1);
  while(iter--) {
    ck0.run(iter,a);
    ck3.run(iter,b);
    gate.run(c,a,b);
    cout << setw(4) << (int)a
         << setw(4) << (int)b
         << setw(4) << (int)c
         << '\n';
  }
}
#endif

#ifdef _NAND3_
void main() {
  int iter=200;
  WIRE i[3],o;
  NAND gate;
  CLOCK ck0(8), ck1(10), ck2(13);
  i[0]=i[1]=i[2]=GND; o = GND;
  while(iter--) {
    ck0.run(iter,i[0]);
    ck1.run(iter,i[1]);
    ck2.run(iter,i[2]);
    gate.run(o,3,i);
    cout << setw(4) << (int)i[0]
         << setw(4) << (int)i[1]
         << setw(4) << (int)i[2]
         << setw(4) << (int)o
         << '\n';
  }
}
#endif

#ifdef _NAND3b_
void main() {
  int iter=200;
  WIRE i0,i1,i2,o;
  i0=i1=i2=GND; o = GND;
  NAND gate;
  CLOCK ck0(8), ck1(10), ck2(13);
  while(iter--) {
    ck0.run(iter,i0);
    ck1.run(iter,i1);
    ck2.run(iter,i2);
    gate.run(o,3,i0,i1,i2);
    cout << setw(4) << (int)i0
         << setw(4) << (int)i1
         << setw(4) << (int)i2
         << setw(4) << (int)o
         << '\n';
  }
}
#endif

#ifdef _NOR_
void main() {
  int iter=100;
  WIRE a=GND,b=GND,c=GND;
  NOR gate;
  CLOCK ck0(1), ck1(5);
  while(iter--) {
    ck0.run(iter,a);
    ck1.run(iter,b);
    gate.run(c,a,b);
    cout << setw(4) << (int)a
         << setw(4) << (int)b
         << setw(4) << (int)c
         << '\n';
  }
}
#endif

#ifdef _NOR3_
void main() {
  int iter=1000;
  WIRE i[3],o;
  NOR gate;
  CLOCK ck0(8), ck1(10), ck2(13);
  while(iter--) {
    ck0.run(iter,i[0]);
    ck1.run(iter,i[1]);
    ck2.run(iter,i[2]);
    gate.run(o,3,i);
    cout << setw(4) << (int)i[0]
         << setw(4) << (int)i[1]
         << setw(4) << (int)i[2]
         << setw(4) << (int)o
         << '\n';
  }
}
#endif

#ifdef _XOR_
void main() {
  int iter=100;
  WIRE a=GND,b=GND,c=GND;
  XOR gate;
  CLOCK ck0(8), ck1(10);
  while(iter--) {
    ck0.run(iter,a);
    ck1.run(iter,b);
    gate.run(c,a,b);
    cout << setw(4) << (int)a
         << setw(4) << (int)b
         << setw(4) << (int)c
         << '\n';
  }
}
#endif

#ifdef _XOR3_
void main() {
  int iter=1000;
  WIRE i[3],o;
  XOR gate;
  CLOCK ck0(8), ck1(10), ck2(13);
  while(iter--) {
    ck0.run(iter,i[0]);
    ck1.run(iter,i[1]);
    ck2.run(iter,i[2]);
    gate.run(o,3,i);
    cout << setw(4) << (int)i[0]
         << setw(4) << (int)i[1]
         << setw(4) << (int)i[2]
         << setw(4) << (int)o
         << '\n';
  }
}
#endif

#ifdef _NXOR_
void main() {
  int iter=100;
  WIRE a=GND,b=GND,c=GND;
  NXOR gate;
  CLOCK ck0(8), ck1(10);
  while(iter--) {
    ck0.run(iter,a);
    ck1.run(iter,b);
    gate.run(a,b,c);
    cout << setw(4) << (int)a
         << setw(4) << (int)b
         << setw(4) << (int)c
         << '\n';
  }
}
#endif

#ifdef _NXOR3_
void main() {
  int iter=1000;
  WIRE i[3],o;
  NXOR gate;
  CLOCK ck0(8), ck1(10), ck2(13);
  while(iter--) {
    ck0.run(iter,i[0]);
    ck1.run(iter,i[1]);
    ck2.run(iter,i[2]);
    gate.run(o,3,i);
    cout << setw(4) << (int)i[0]
         << setw(4) << (int)i[1]
         << setw(4) << (int)i[2]
         << setw(4) << (int)o
         << '\n';
  }
}
#endif

#ifdef _BUFFER_Z_
void main() {
  int iter=1000;
  WIRE i=GND,e=GND,o=GND;
  BUFFER_Z x;
  CLOCK ck0(8), ck1(130);
  while(iter--) {
    ck0.run(iter,i);
    ck1.run(iter,e);
    x.run(o,i,e);
    cout << setw(4) << (int)i
         << setw(4) << (int)e
         << setw(4) << (int)o
         << '\n';
  }
}
#endif
