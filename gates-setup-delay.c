#include <stdarg.h>
#include "defs.h"
#include "wire.h"
#include "gates.h"

/* N O T */

void NOT::run (WIRE &o, WIRE &i) {
  WIRE tmp = ~i;
  if(delay==tmp) o=tmp;
  delay=tmp;
  if(o<127) o=GND; else o=VCC;
}


/* A N D */

AND::AND(int num_inputs) {
  this->num_inputs=num_inputs;
}

void AND::run(WIRE &output, WIRE &input, ...) {
  int inputs=num_inputs;
  va_list args;                 // Puntero a la lista de argumentos
  va_start(args,input);         // que ahora apunta al primero de ellos
  WIRE tmp = input;             // Primera entrada
  while(--inputs) {             // Para el resto de entradas
    tmp &= va_arg(args,WIRE);   // AND con la siguiente entrada
  }
  va_end(args);
  if(delay==tmp) {              // Miramos si la salida es estable
    if(tmp<127) output=GND;     // y binarizamos
    else output=VCC;
  }
  delay=tmp;
}

#ifdef _1_
void AND::run (WIRE &o, WIRE &i0, WIRE &i1) {
  WIRE tmp = i0 & i1;
  if(delay==tmp) o=tmp;
  delay=tmp;
#ifdef _DIGITAL_
  if(o<127) o=GND; else o=VCC;
#endif
}

void AND::run(WIRE &out, int inputs, WIRE &in, ...) {
  va_list args;                 // Puntero a la lista de argumentos
  va_start(args,in);            // que ahora apunta al primero de ellos
  WIRE tmp = in;                // Primera entrada
  while(--inputs) {             // Para el resto de entradas
    tmp &= va_arg(args,WIRE);   // AND con la siguiente entrada
  }
  va_end(args);                 // Finalizaci'on de manipulaci'on
  if(delay==tmp) {              // Miramos si la salida es estable
#ifdef _DIGITAL_
    if(tmp<127) out=GND;        // y binarizamos
    else out=VCC;
#else
    out=tmp;
#endif
  }
  delay=tmp;                    // Recordamos la "posible salida"
}

void AND::run(WIRE &o, int inputs, WIRE i[]) {
  WIRE tmp = i[0];
  for(int j=1;j<inputs;j++) {
    tmp &= i[j];
  }
  if(delay==tmp) o=tmp;
  delay=tmp;
#ifdef _DIGITAL_
  if(o<127) o=GND; else o=VCC;
#endif
}

/* N A N D */

void NAND::run (WIRE &o, WIRE &i0, WIRE &i1) {
  WIRE tmp = ~(i0 & i1);
  if(delay==tmp) o=tmp;
 delay=tmp;
#ifdef _DIGITAL_
  if(o<127) o=GND; else o=VCC;
#endif
}

void NAND::run(WIRE &out, int inputs, WIRE &in, ...) {
  va_list args;                 // Puntero a la lista de argumentos
  va_start(args,in);            // que ahora apunta al primero de ellos
  WIRE tmp = in;                // Primera entrada
  while(--inputs) {             // Para el resto de entradas
    tmp &= va_arg(args,WIRE);   // AND con la siguiente entrada
  }
  va_end(args);                 // Finalizaci'on de manipulaci'on
  tmp = ~tmp;                   // Negamos la AND
  if(delay==tmp) {              // Miramos si la salida es estable
#ifdef _DIGITAL_
    if(tmp<127) out=GND;        // y binarizamos
    else out=VCC;
#else
    out=tmp;
#endif
  }
  delay=tmp;                    // Recordamos la "posible salida"
}

void NAND::run(WIRE &o, int inputs, WIRE i[]) {
  WIRE tmp = i[0];
  for(int j=1;j<inputs;j++) {
    tmp &= i[j];
  }
  tmp = ~tmp;
  if(delay==tmp) o=tmp;
  delay=tmp;
#ifdef _DIGITAL_
  if(o<127) o=GND; else o=VCC;
#endif
}


/* O R */

void OR::run(WIRE &o, WIRE &i0, WIRE &i1) {
  WIRE tmp = i0 | i1;
  if(delay==tmp) o=tmp;
  delay=tmp;
#ifdef _DIGITAL_
  if(o<127) o=GND; else o=VCC;
#endif
}

void OR::run(WIRE &out, int inputs, WIRE &in, ...) {
  va_list args;                 // Puntero a la lista de argumentos
  va_start(args,in);            // que ahora apunta al primero de ellos
  WIRE tmp = in;                // Primera entrada
  while(--inputs) {             // Para el resto de entradas
    tmp |= va_arg(args,WIRE);   // OR con la siguiente entrada
  }
  va_end(args);                 // Finalizaci'on de manipulaci'on
  if(delay==tmp) {              // Miramos si la salida es estable
#ifdef _DIGITAL_
    if(tmp<127) out=GND;        // y binarizamos
    else out=VCC;
#else
    out=tmp;
#endif
  }
  delay=tmp;                    // Recordamos la "posible salida"
}

void OR::run(WIRE &o, int inputs, WIRE i[]) {
  WIRE tmp = i[0];
  for(int j=1;j<inputs;j++) {
    tmp |= i[j];
  }
  if(delay==tmp) o=tmp;
  delay=tmp;
#ifdef _DIGITAL_
  if(o<127) o=GND; else o=VCC;
#endif
}


/* N O R */

void NOR::run (WIRE &o, WIRE &i0, WIRE &i1) {
  WIRE tmp = ~(i0 | i1);
  if(delay==tmp) o=tmp;
  delay=tmp;
#ifdef _DIGITAL_
  if(o<127) o=GND; else o=VCC;
#endif
}

void NOR::run(WIRE &out, int inputs, WIRE &in, ...) {
  va_list args;                 // Puntero a la lista de argumentos
  va_start(args,in);            // que ahora apunta al primero de ellos
  WIRE tmp = in;                // Primera entrada
  while(--inputs) {             // Para el resto de entradas
    tmp |= va_arg(args,WIRE);   // OR con la siguiente entrada
  }
  va_end(args);                 // Finalizaci'on de manipulaci'on
  tmp = ~tmp;                   // Negamos la OR
  if(delay==tmp) {              // Miramos si la salida es estable
#ifdef _DIGITAL_
    if(tmp<127) out=GND;        // y binarizamos
    else out=VCC;
#else
    out=tmp;
#endif
  }
  delay=tmp;                    // Recordamos la "posible salida"
}

void NOR::run(WIRE &o, int inputs, WIRE i[]) {
  WIRE tmp = i[0];
  for(int j=1;j<inputs;j++) {
    tmp |= i[j];
  }
  tmp = ~tmp;
  if(delay==tmp) o=tmp;
  delay=tmp;
#ifdef _DIGITAL_
  if(o<127) o=GND; else o=VCC;
#endif
}


/* X O R */

void XOR::run (WIRE &o, WIRE &i0, WIRE &i1) {
  WIRE tmp = i0 ^ i1;
  if(delay==tmp) o=tmp;
  delay=tmp;
#ifdef _DIGITAL_
  if(o<127) o=GND; else o=VCC;
#endif
}

void XOR::run(WIRE &out, int inputs, WIRE &in, ...) {
  va_list args;                 // Puntero a la lista de argumentos
  va_start(args,in);            // que ahora apunta al primero de ellos
  WIRE tmp = in;                // Primera entrada
  while(--inputs) {             // Para el resto de entradas
    tmp ^= va_arg(args,WIRE);   // XOR con la siguiente entrada
  }
  va_end(args);                 // Finalizaci'on de manipulaci'on
  if(delay==tmp) {              // Miramos si la salida es estable
#ifdef _DIGITAL_
    if(tmp<127) out=GND;        // y binarizamos
    else out=VCC;
#else
    out=tmp;
#endif
  }
  delay=tmp;                    // Recordamos la "posible salida"
}

void XOR::run(WIRE &o, int inputs, WIRE i[]) {
  WIRE tmp = i[0];
  for(int j=1;j<inputs;j++) {
    tmp ^= i[j];
  }
  if(delay==tmp) o=tmp;
  delay=tmp;
#ifdef _DIGITAL_
  if(o<127) o=GND; else o=VCC;
#endif
}


/* N X O R */

void NXOR::run (WIRE &o, WIRE &i0, WIRE &i1) {
  WIRE tmp = ~(i0 ^ i1);
  if(delay==tmp) o=tmp;
  delay=tmp;
#ifdef _DIGITAL_
  if(o<127) o=GND; else o=VCC;
#endif
}

void NXOR::run(WIRE &out, int inputs, WIRE &in, ...) {
  va_list args;                 // Puntero a la lista de argumentos
  va_start(args,in);            // que ahora apunta al primero de ellos
  WIRE tmp = in;                // Primera entrada
  while(--inputs) {             // Para el resto de entradas
    tmp ^= va_arg(args,WIRE);   // XOR con la siguiente entrada
  }
  va_end(args);                 // Finalizaci'on de manipulaci'on
  tmp = ~tmp;                   // Negamos la XOR
  if(delay==tmp) {              // Miramos si la salida es estable
#ifdef _DIGITAL_
    if(tmp<127) out=GND;        // y binarizamos
    else out=VCC;
#else
    out=tmp;
#endif
  }
  delay=tmp;                    // Recordamos la "posible salida"
}

void NXOR::run(WIRE &o, int inputs, WIRE i[]) {
  WIRE tmp = i[0];
  for(int j=1;j<inputs;j++) {
    tmp ^= i[j];
  }
  tmp = ~tmp;
  if(delay==tmp) o=tmp;
  delay=tmp;
#ifdef _DIGITAL_
  if(o<127) o=GND; else o=VCC;
#endif
}

/* B U F F E R _ Z */

BUFFER_Z::BUFFER_Z() {
  sample=0;
}

void BUFFER_Z::run(WIRE &o, WIRE &i, WIRE &e) {
  delay[sample] = i;
  sample = (sample+1) % 2;
  if(delay[0]!=delay[1]) return;
  if(e==VCC) o=delay[0];
}

#endif /*_1_*/
