/*
 * fr32x43.c -- Fichero de Registros de 32x32 bits (R2000).
 * Ultima modificaci'on: 23-11-00.
 * gse.
 */

#include "gates.h"
#include "decods.h"
#include "latches.h"
#include "flip-flops.h"
#include "fr32x32.h"

void FR_BIT::run(WIRE &in, WIRE &sel0, WIRE &sel1, WIRE &write, WIRE &out0, WIRE &out1) {
  f.run(in,write,q,qn);
  b[0].run(out0,q,sel0);
  b[1].run(out1,q,sel1);
}

void FR0_BIT::run(WIRE &in, WIRE &sel0, WIRE &sel1, WIRE &write, WIRE &out0, WIRE &out1) {
  //WIRE gnd=GND;
  //b[0].run(gnd,sel0,out0);
  //b[1].run(gnd,sel1,out1);
  out0 = out1 = GND;
}

void FR_32x32::run(WIRE in[32], WIRE addr0[5], WIRE addr1[5], WIRE addr2[5],
                   WIRE &write, WIRE out0[32], WIRE out1[32]) {
  //WIRE gnd=GND;
  int i;
  dec0.run(addr0,s0);
  dec1.run(addr1,s1);
  decw.run(addr2,w);
  for(i=0;i<32;i++) {
    y[i].run(wck[i],w[i],write);
  }
  int j;
  // El registro 0 siempre contiene 0.
  // Para conseguir esto, hacemos que su entrada siempre valga 0.
  /*
  for(j=0;j<32;j++) {
    bit[0][j].run(gnd,s0[0],s1[0],wck[0],out0[j],out1[j]);
  }
  */
  for(j=0;j<32;j++) {
    bit0[j].run(in[i],s0[0],s1[0],wck[0],out0[j],out1[j]);
  }
  for(i=1;i<32;i++) {
    for(j=0;j<32;j++) {
      bit[i-1][j].run(in[j],s0[i],s1[i],wck[i],out0[j],out1[j]);
    }
  }
}
