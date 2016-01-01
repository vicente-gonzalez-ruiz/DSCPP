/*
 * gates.c -- Implementaci'on de las puertas l'ogicas.
 * Ultima modificaci'on: 23-11-00.
 * gse.
 */

#include <stdarg.h>
#include "gates.h"

/************************* N O T *******************************/

inline static WIRE Not(WIRE xxx) {
  /*
    x | NOT
    --+-------
    0 | 1
    1 | 0
  */ 
  if(xxx<127) return VCC; else return GND;
}

void NOT::run(WIRE &output, WIRE &input) {
  /* Calculamos la operaci'on l'ogica */
  WIRE tmp=Not(input);
  /* S'olo si la salida anterior coincide con el resultado
     actual, la salida ser'a igual al resultado actual */
  if(delay==tmp) output=tmp;
  /* Recordamos el resultado actual */
  delay=tmp;
}

/************************* A N D *******************************/

inline static WIRE And(WIRE x, WIRE y) {
  /*
    x y | AND
    ----+-------
    0 0 | 0
    0 1 | 0
    1 0 | 0
    1 1 | 1
  */
  if((x<127)||(y<127)) return GND; else return VCC;
}

void AND::run (WIRE &output, WIRE &input0, WIRE &input1) {
  WIRE tmp=And(input0,input1);
  if(delay==tmp) output=tmp;
  delay=tmp;
}

void AND::run(WIRE &output, short num_inputs, WIRE &input, ...) {
  va_list args;                     // Puntero a la lista de argumentos
  va_start(args,input);             // que ahora apunta al primero de ellos
  WIRE tmp=input;                   // Primera entrada
  while(--num_inputs) {             // Para el resto de entradas
    tmp=And(tmp,va_arg(args,WIRE)); // AND con la siguiente entrada
  }
  va_end(args);                     // Finalizaci'on de manipulaci'on
  if(delay==tmp) output=tmp;
  delay=tmp;
}

void AND::run(WIRE &output, short num_inputs, WIRE input[]) {
  WIRE tmp=input[0];
  for(int j=1;j<num_inputs;j++) {
    tmp=And(tmp,input[j]);
  }
  if(delay==tmp) output=tmp;
  delay=tmp;
}

/*********************** N A N D *******************************/

inline static WIRE Nand(WIRE x, WIRE y) {
  /*
    x y | NAND
    ----+-------
    0 0 | 1
    0 1 | 1
    1 0 | 1
    1 1 | 0
  */
  //if((x<127)&&(y<127)) return VCC; else return GND;
  return Not(And(x,y));
}

void NAND::run (WIRE &output, WIRE &input0, WIRE &input1) {
  WIRE tmp=Nand(input0,input1);
  if(delay==tmp) output=tmp;
  delay=tmp;
}

void NAND::run(WIRE &output, short num_inputs, WIRE &input, ...) {
  va_list args;
  va_start(args,input);
  WIRE tmp=input;
  while(--num_inputs) {
    tmp=And(tmp,va_arg(args,WIRE));
  }
  va_end(args);
  tmp=Not(tmp);
  if(delay==tmp) output=tmp;
  delay=tmp;
}

void NAND::run(WIRE &output, short num_inputs, WIRE input[]) {
  WIRE tmp=input[0];
  for(int j=1;j<num_inputs;j++) {
    tmp=And(tmp,input[j]);
  }
  tmp=Not(tmp);
  if(delay==tmp) output=tmp;
  delay=tmp;
}

/************************* O R *********************************/

inline static WIRE Or(WIRE x, WIRE y) {
  /*
    x y | OR
    ----+-------
    0 0 | 0
    0 1 | 1
    1 0 | 1
    1 1 | 1
  */
  if((x<127)&&(y<127)) return GND; else return VCC;
}

void OR::run (WIRE &output, WIRE &input0, WIRE &input1) {
  WIRE tmp=Or(input0,input1);
  if(delay==tmp) output=tmp;
  delay=tmp;
}

void OR::run(WIRE &output, short num_inputs, WIRE &input, ...) {
  va_list args;
  va_start(args,input);
  WIRE tmp=input;
  while(--num_inputs) {
    tmp=Or(tmp,va_arg(args,WIRE));
  }
  va_end(args);
  if(delay==tmp) output=tmp;
  delay=tmp;
}

void OR::run(WIRE &output, short num_inputs, WIRE input[]) {
  WIRE tmp=input[0];
  for(int j=1;j<num_inputs;j++) {
    tmp=Or(tmp,input[j]);
  }
  if(delay==tmp) output=tmp;
  delay=tmp;
}

/************************* N O R *******************************/

inline static WIRE Nor(WIRE x, WIRE y) {
  /*
    x y | NOT
    ----+-------
    0 0 | 1
    0 1 | 0
    1 0 | 0
    1 1 | 0
  */
  //if((x<127)&&(y<127)) return VCC; else return GND;
  return Not(Or(x,y));
}

void NOR::run (WIRE &output, WIRE &input0, WIRE &input1) {
  WIRE tmp=Nor(input0,input1);
  if(delay==tmp) output=tmp;
  delay=tmp;
}

void NOR::run(WIRE &output, short num_inputs, WIRE &input, ...) {
  va_list args;
  va_start(args,input);
  WIRE tmp=input;
  while(--num_inputs) {
    tmp=Or(tmp,va_arg(args,WIRE));
  }
  va_end(args);
  tmp=Not(tmp);
  if(delay==tmp) output=tmp;
  delay=tmp;
}

void NOR::run(WIRE &output, short num_inputs, WIRE input[]) {
  WIRE tmp=input[0];
  for(int j=1;j<num_inputs;j++) {
    tmp=Or(tmp,input[j]);
  }
  tmp=Not(tmp);
  if(delay==tmp) output=tmp;
  delay=tmp;
}

/************************* X O R *******************************/

inline static WIRE Xor(WIRE x, WIRE y) {
  /*
    x y | NOT
    ----+-------
    0 0 | 0
    0 1 | 1
    1 0 | 1
    1 1 | 0
  */
  WIRE output=x^y;
  if(output<127) return GND; else return VCC;
}

void XOR::run (WIRE &output, WIRE &input0, WIRE &input1) {
  WIRE tmp=Xor(input0,input1);
  if(delay==tmp) output=tmp;
  delay=tmp;
}

void XOR::run(WIRE &output, short num_inputs, WIRE &input, ...) {
  va_list args;
  va_start(args,input);
  WIRE tmp=input;
  while(--num_inputs) {
    tmp=Xor(tmp,va_arg(args,WIRE));
  }
  va_end(args);
  if(delay==tmp) output=tmp;
  delay=tmp;
}

void XOR::run(WIRE &output, short num_inputs, WIRE input[]) {
  WIRE tmp=input[0];
  for(int j=1;j<num_inputs;j++) {
    tmp=Xor(tmp,input[j]);
  }
  if(delay==tmp) output=tmp;
  delay=tmp;
}

/************************* N X O R *****************************/

inline static WIRE Nxor(WIRE x, WIRE y) {
  /*
    x y | NOT
    ----+-------
    0 0 | 1
    0 1 | 0
    1 0 | 0
    1 1 | 1
  */
  //WIRE output=~(x^y);
  //if(output<127) return GND; else return VCC;
  return Not(Xor(x,y));
}

void NXOR::run (WIRE &output, WIRE &input0, WIRE &input1) {
  WIRE tmp=Nxor(input0,input1);
  if(delay==tmp) output=tmp;
  delay=tmp;
}

void NXOR::run(WIRE &output, short num_inputs, WIRE &input, ...) {
  va_list args;
  va_start(args,input);
  WIRE tmp=input;
  while(--num_inputs) {
    tmp=Xor(tmp,va_arg(args,WIRE));
  }
  va_end(args);
  tmp=Not(tmp);
  if(delay==tmp) output=tmp;
  delay=tmp;
}

void NXOR::run(WIRE &output, short num_inputs, WIRE input[]) {
  WIRE tmp=input[0];
  for(int j=1;j<num_inputs;j++) {
    tmp=Xor(tmp,input[j]);
  }
  tmp=Not(tmp);
  if(delay==tmp) output=tmp;
  delay=tmp;
}

/******************** B U F F E R _ Z **************************/

BUFFER_Z::BUFFER_Z() {
  sample=0;
}

void BUFFER_Z::run(WIRE &output, WIRE &input, WIRE &enable) {
  delay[sample] = input;
  sample = (sample+1) % 2;
  if(delay[0]!=delay[1]) return;
  if(enable==VCC) output=delay[0];
}
