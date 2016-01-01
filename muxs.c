/*
 * muxs.c -- Multiplexores y desmultiplexores.
 * Ultima modificaci'on: 23-11-2000.
 * gse.
 */

#include "gates.h"
#include "decods.h"
#include "muxs.h"

void MUX_2_1::run(WIRE in[2], WIRE &sel, WIRE &out) {
  no.run(nsel,sel);
  buf[0].run(out,in[0],nsel);
  buf[1].run(out,in[1],sel);
}

void DEMUX_1_2::run(WIRE &in, WIRE &sel, WIRE out[2]) {
  no.run(nsel,sel);
  y[0].run(out[0],nsel,in);
  y[1].run(out[1],sel,in);
}

void MUX_4_1::run(WIRE in[4], WIRE sel[2], WIRE &out) {
  dec.run(sel,d);
  for(int i=0;i<4;i++)
    buf[i].run(out,in[i],d[i]);
}

void DEMUX_1_4::run(WIRE &in, WIRE sel[2], WIRE out[4]) {
  dec.run(sel,d);
  for(int i=0;i<4;i++)
    y[i].run(out[i],d[i],in);
}

void MUX::create(int num_sels) {
  decoder.create(num_sels);
  num_ins = 1<<num_sels;
  buffers = new BUFFER_Z[num_ins];
  outs_decoder = new WIRE[num_ins];
}
void MUX::destroy() {
  decoder.destroy();
  delete buffers;
  delete outs_decoder;
}
void MUX::run(WIRE in[], WIRE sel[], WIRE &out) {
  decoder.run(sel,outs_decoder);
  for(int i=0; i<num_ins; i++) {
    buffers[i].run(out,in[i],outs_decoder[i]);
  }
}

//DEMUX::DEMUX() {}
void DEMUX::create(int num_sels) {
  decoder.create(num_sels);
  num_outs = 1<< num_sels;
  y = new AND[num_outs];
  out_decoder = new WIRE[num_outs];
}
void DEMUX::destroy() {
  decoder.destroy();
  delete y;
  delete out_decoder;
}
DEMUX::DEMUX(int num_sels) {
  create(num_sels);
}
void DEMUX::run(WIRE &in, WIRE sel[], WIRE out[]) {
  decoder.run(sel,out_decoder);
  for(int i=0;i<num_outs;i++)
    y[i].run(out[i],out_decoder[i],in);
}
