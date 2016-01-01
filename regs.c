/*
 * regs.c -- Implementaci'on de los registros.
 * Ultima modificaci'on: 23-11-00.
 * gse.
 */

#include "gates.h"
#include "latches.h"
#include "flip-flops.h"
#include "regs.h"

void REG_4::run(WIRE &ck, WIRE i[4], WIRE o[4]) {
  ff[0].run(i[0],ck,o[0],Qn[0]);
  ff[1].run(i[1],ck,o[1],Qn[1]);
  ff[2].run(i[2],ck,o[2],Qn[2]);
  ff[3].run(i[3],ck,o[3],Qn[3]);
}

void REG_4_PC::run(WIRE &ck, WIRE i[4], WIRE o[4], WIRE &preset, WIRE &clear) {
  ff[0].run(i[0],ck,o[0],Qn[0],preset,clear);
  ff[1].run(i[1],ck,o[1],Qn[1],preset,clear);
  ff[2].run(i[2],ck,o[2],Qn[2],preset,clear);
  ff[3].run(i[3],ck,o[3],Qn[3],preset,clear);
}

void REG_8::run(WIRE &ck, WIRE i[8], WIRE o[8]) {
  reg[0].run(ck,i,o);
  reg[1].run(ck,i+4,o+4);
}

void REG_16::run(WIRE &ck, WIRE i[16], WIRE o[16]) {
  reg[0].run(ck,i,o);
  reg[1].run(ck,i+8,o+8);
}

void REG_32::run(WIRE &ck, WIRE i[32], WIRE o[32]) {
  reg[0].run(ck,i,o);
  reg[1].run(ck,i+16,o+16);
}

REG::REG(int num_bits) {
  this->num_bits = num_bits;
  ff = new NEG_D_FF[num_bits];
  Qn = new WIRE[num_bits];
}

REG::~REG() {
  delete ff;
  delete Qn;
}

void REG::run(WIRE &ck, WIRE input[], WIRE output[]) {
  for(int i = 0; i < num_bits; i++)
    ff[i].run(input[i], ck, output[i], Qn[i]);
}
