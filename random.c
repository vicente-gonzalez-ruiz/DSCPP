#include "gates.h"
#include "random.h"
#include <stdlib.h> // rand()

RANDOM::RANDOM(int duracion_del_semiperiodo) {
  this->duracion_del_semiperiodo = duracion_del_semiperiodo;
}

void RANDOM::run(int iteracion_de_simulacion, WIRE &ck) {
  if(!(iteracion_de_simulacion%duracion_del_semiperiodo)) ck = random()&0xFF;
  if(ck<127) ck=GND; else ck=VCC;
}

void Random(int iter, int duracion, WIRE &o) {
  if(!(iter%duracion)) o = random()&0xFF;
  if(o<127) o=0; else o=255;
}
