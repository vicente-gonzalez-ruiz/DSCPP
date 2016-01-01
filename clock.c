/*
 * clock.c -- Generador de onda cuadrada de periodo ajustable.
 * Ultima modificaci'on: 17-10-2000.
 * gse.
 */

#include "gates.h"
#include "clock.h"

CLOCK::CLOCK(int semiperiodo) {
  duracion_del_semiperiodo = semiperiodo;
}

void CLOCK::run(int iteracion, WIRE &ck) {
  //if(ck<127) ck=GND; else ck=VCC;
  if(!(iteracion%duracion_del_semiperiodo)) {
    if(ck>127) ck=GND;
    else ck=VCC;
    //ck = ~ck;
  }
}

void Clock(int iter, int duracion, WIRE &io) {
  //if(io<127) io=GND; else io=VCC;
  if(!(iter%duracion)) {
    if(io>127) io=GND;
    else io=VCC;
    //io = ~io;
  }
}
