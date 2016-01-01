/*
 * srams.c -- Implementaci'on de las SRAMs.
 * Ultima modificaci'on: 23-11-00.
 * gse.
 */

#include "gates.h"
#include "decods.h"
#include "muxs.h"
#include "latches.h"
#include "srams.h"

void SRAM_BIT::run(WIRE &in, WIRE &select, WIRE &write, WIRE &out) {
  n.run(nin,in);
  a0.run(r,3,write,nin,select);
  a1.run(s,3,write,in,select);
  f.run(s,r,q,qn);
  b.run(out,q,select);
}

void BIT_4x4_Array::run(WIRE in[4], WIRE &blksel,
                        WIRE sel[4], WIRE &write, WIRE out[4]) {
  int i,r,c;
  for(i=0;i<4;i++)
    a[i].run(s[i],blksel,sel[i]);
  for(r=0;r<4;r++)
    for(c=0;c<4;c++)
      b[r][c].run(in[c],s[r],write,out[c]);
}

void SRAM_16x4::run(WIRE &select, WIRE &write, WIRE addr[4],
                    WIRE in[4], WIRE out[4]) {
  dec_sel.run(addr+2,select,blksel);
  dec.run(addr,sel);
  for(int i=0;i<4;i++) {
    blk[i].run(in,blksel[i],sel,write,out);
  }
}

void SRAM_16x32::run(WIRE &select, WIRE &write, WIRE addr[4],
                     WIRE in[32], WIRE out[32]) {
  for(int i=0;i<8;i++) {
    blk[i].run(select,write,addr,in+i*4,out+i*4);
  }
}

void SRAM_64x32::run(WIRE &select, WIRE &write, WIRE addr[6],
                     WIRE in[32], WIRE out[32]) {
  dec.run(addr+4,select,this->select);
  for(int i=0;i<4;i++) {
    blk[i].run(this->select[i],write,addr,in,out);
  }
}
