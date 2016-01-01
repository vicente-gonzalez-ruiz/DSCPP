/*
 * monostable.cc -- Monoestable programable.
 * gse. 1999.
 */
#include "gates.h"
#include "monostable.h"

MONOSTABLE::MONOSTABLE(int t_inestable, int periodo) {
  time=0;
  this->t_inestable=t_inestable;
  this->periodo=periodo;
}

void MONOSTABLE::run(WIRE &io) {
  if(time<t_inestable) {io=VCC;}
  else {io=GND;}
  if(time>periodo) {
    io=VCC;
    time=0;
  }
  time++;
}
