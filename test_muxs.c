/*
 * test_muxs.h -- Testeo de los multiplexores y desmultiplexores.
 * Ultima modificaci'on: 19-10-2000.
 * gse.
 */

#include <stdio.h>
#include "gates.h"
#include "decods.h"
#include "muxs.h"
#include "clock.h"

#ifdef _MUX_2_1_
int main() {
  FILE *data=fopen("data","w");
  FILE *data_sdl=fopen("data.sdl","w");
  int iter=0;
  WIRE i[2], sel, o;
  MUX_2_1 mux;
  fprintf(data,"i[0] i[1] sel o");
  while(iter++<300) {
    Clock(iter,10,i[0]);
    Clock(iter,20,i[1]);
    Clock(iter,40,sel);
    mux.run(i,sel,o);
    fprintf(data_sdl,"%3d %3d %3d %3d\n"
            ,i[0],i[1],sel,o);
  }
}
#endif

#ifdef _DEMUX_1_2_
int main() {
  FILE *data=fopen("data","w");
  FILE *data_sdl=fopen("data.sdl","w");
  int iter=0;
  WIRE i, sel, o[2];
  DEMUX_1_2 demux;
  fprintf(data,"i sel o[0] o[1]");
  while(iter++<300) {
    Clock(iter,3,i);
    Clock(iter,10,sel);
    demux.run(i,sel,o);
    fprintf(data_sdl,"%3d %3d %3d %3d\n"
            ,i,sel,o[0],o[1]);
  }
}
#endif

#ifdef _MUX_4_1_
int main() {
  FILE *data=fopen("data","w");
  FILE *data_sdl=fopen("data.sdl","w");
  int iter=0;
  WIRE i[4], sel[2], o;
  MUX_4_1 mux;
  fprintf(data,"i[0] i[1] i[2] i[3] sel[0] sel[1] o");
  while(iter++<500) {
    int j;
    for(j=0;j<4;j++) {
      Clock(iter,1<<(j+1),i[j]);
    }
    for(j=0;j<2;j++) {
      Clock(iter,1<<(j+5),sel[j]);
    }
    mux.run(i,sel,o);
    for(j=0;j<4;j++) {
      fprintf(data_sdl,"%3d ",i[j]);
    }
    for(j=0;j<2;j++) {
      fprintf(data_sdl,"%3d ",sel[j]);
    }
    fprintf(data_sdl,"%3d\n",o);
  }
}
#endif

#ifdef _DEMUX_1_4_
int main() {
  FILE *data=fopen("data","w");
  FILE *data_sdl=fopen("data.sdl","w");
  int iter=0;
  WIRE i, sel[2], o[4];
  DEMUX_1_4 demux;
  fprintf(data,"i sel[0] sel[1] o[0] o[1] o[2] o[3]");
  while(iter++<500) {
    int j;
    Clock(iter,3,i);
    for(j=0;j<2;j++) {
      Clock(iter,1<<(j+4),sel[j]);
    }
    demux.run(i,sel,o);
    fprintf(data_sdl,"%3d ",i);
    for(j=0;j<2;j++) {
      fprintf(data_sdl,"%3d ",sel[j]);
    }
    for(j=0;j<3;j++) {
      fprintf(data_sdl,"%3d ",o[j]);
    }
    fprintf(data_sdl,"%3d\n",o[3]);
  }
}
#endif

#ifdef _MUX_
#include <stdlib.h>
int main(int argc, char *argv[]) {
  if(argc<2) {
    fprintf(stderr,"%s entradas_selecci'on\n",argv[0]);
    return 1;
  }
  FILE *data=fopen("data","w");
  FILE *data_sdl=fopen("data.sdl","w");
  int N = atoi(argv[1]);
  WIRE *in = new WIRE[1<<N],
       *sel = new WIRE[N],
       out;
  MUX mux;
  mux.create(N);
  int iter=0;
  for(int j=0;j<(1<<N);j++) fprintf(data,"i[%d] ",j);
  for(int j=0;j<N;j++) fprintf(data,"sel[%d] ",j);
  fprintf(data,"o");
  while(iter++<(1<<(3*N))) {
    int j;
    for(j=0;j<(1<<N);j++) {
      Clock(iter,1<<j,in[j]);
    }
    for(j=0;j<N;j++) {
      Clock(iter,1<<(j+5),sel[j]);
    }
    mux.run(in,sel,out);
    for(j=0;j<(1<<N);j++) {
      fprintf(data_sdl,"%3d ",in[j]);
    }
    for(j=0;j<N;j++) {
      fprintf(data_sdl,"%3d ",sel[j]);
    }
    fprintf(data_sdl,"%3d\n",out);
  }
}
#endif

#ifdef _DEMUX_
#include <stdlib.h>
int main(int argc, char *argv[]) {
  if(argc<2) {
    fprintf(stderr,"%s entradas_selecci'on\n",argv[0]);
    return 1;
  }
  FILE *data=fopen("data","w");
  FILE *data_sdl=fopen("data.sdl","w");
  int N = atoi(argv[1]);
  WIRE in,
    *sel = new WIRE[N],
    *out = new WIRE[1<<N];
  DEMUX demux(N);
  int iter=0;
  fprintf(data,"in ");
  for(int j=0;j<N;j++) fprintf(data,"sel[%d] ",j);
  for(int j=0;j<(1<<N)-1;j++) fprintf(data,"o[%d] ",j);
  fprintf(data,"o[%d]",(1<<N)-1);
  while(iter++<(1<<(3*N))) {
    Clock(iter,10,in);
    int j;
    for(j=0;j<N;j++) {
      Clock(iter,1<<(j+5),sel[j]);
    }
    demux.run(in,sel,out);
    fprintf(data_sdl,"%3d ",in);
    for(j=0;j<N;j++) {
      fprintf(data_sdl,"%3d ",sel[j]);
    }
    for(j=0;j<(1<<N)-1;j++) {
      fprintf(data_sdl,"%3d ",out[j]);
    }
    fprintf(data_sdl,"%3d\n",out[(1<<N)-1]);
  }
}
#endif
