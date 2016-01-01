/*
 * ejcs.cc -- Ejemplo de circuito secuencial.
 * gse. 1999.
 */

#include <iostream.h>
#include <iomanip.h>
#include "defs.h"
#include "gates.h"
#include "monostable.h"

void main() {
  int iteraciones=100;
  NAND na0,na1,na2;
  WIRE A=0,B=0,Y=0,c=0,d=0;
  MONOSTABLE ck0(25,100),ck1(50,100);
  while(iteraciones--) {
    ck0.run(A);
    ck1.run(B);
    na0.run(A,B,c);
    na1.run(B,Y,d);
    na2.run(c,d,Y);
    cout << setw(4) << (int)A
         << setw(4) << (int)B
         << setw(4) << (int)c
         << setw(4) << (int)d
         << setw(4) << (int)Y 
         << '\n';
  }
}
