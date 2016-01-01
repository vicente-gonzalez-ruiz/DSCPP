#include <stdio.h>
#include "gates.h"
#include "decods.h"
#include "clock.h"

#ifdef _DECOD_2_4_
void main() {
  FILE *data=fopen("data","w");
  FILE *data_sdl=fopen("data.sdl","w");
  int iter=0;
  WIRE i[2], o[4];
  DECOD_2_4 decoder;
  fprintf(data,"i[0] i[1] o[0] o[1] o[2] o[3]");
  while(++iter<500) {
    Clock(iter,5,i[0]);
    Clock(iter,10,i[1]);
    decoder.run(i,o);
    fprintf(data_sdl,"%3d %3d %3d %3d %3d %3d\n"
	    ,i[0],i[1],o[0],o[1],o[2],o[3]);
  }
}
#endif

#ifdef _DECOD_5_32_
void main() {
  FILE *data=fopen("data","w");
  FILE *data_sdl=fopen("data.sdl","w");
  int iter=0;
  WIRE i[5], o[32];
  i[0]=i[1]=i[2]=i[3]=i[4]=0;
  DECOD_5_32 decoder;
  for(int i=0;i<5;i++) fprintf(data,"i[%d] ",i);
  for(int i=0;i<31;i++) fprintf(data,"o[%d] ",i);
  fprintf(data,"o[31]");
  while(++iter<500) {
    for(int j=0;j<5;j++) {
      Clock(iter,1<<(j+2),i[j]);
    }
    decoder.run(i,o);
    for(int j=0;j<5;j++) fprintf(data_sdl,"%3d ",i[j]);
    for(int j=0;j<31;j++) fprintf(data_sdl,"%3d ",o[j]);
    fprintf(data_sdl,"%3d\n",o[31]);
  }
}
#endif

#ifdef _DECOD_2_4_SEL_
void main() {
  FILE *data=fopen("data","w");
  FILE *data_sdl=fopen("data.sdl","w");
  int iter=200;
  WIRE i[2], o[4], sel;
  DECOD_2_4_SEL decoder;
  fprintf(data,"i[0] i[1] sel o[0] o[1] o[2] o[3]");
  while(iter--) {
    Clock(iter,5,i[0]);
    Clock(iter,11,i[1]);
    Clock(iter,23,sel);
    decoder.run(i,sel,o);
    fprintf(data_sdl,"%3d %3d %3d %3d %3d %3d %3d\n"
	    ,i[0],i[1],sel,o[0],o[1],o[2],o[3]);
  }
}
#endif

#ifdef _DECOD_5_32_SEL_
void main() {
  FILE *data=fopen("data","w");
  FILE *data_sdl=fopen("data.sdl","w");
  int iter=0;
  WIRE i[5], o[32], sel;
  DECOD_5_32_SEL decoder;
  for(int i=0;i<5;i++) fprintf(data,"i[%d] ",i);
  fprintf(data,"sel ");
  for(int i=0;i<31;i++) fprintf(data,"o[%d] ",i);
  fprintf(data,"o[31]");
   while(iter++<500) {
    int j;
    for(j=0;j<5;j++) {
      Clock(iter,1<<(j+2),i[j]);
    }
    Clock(iter,1<<(5+2),sel);
    decoder.run(i,sel,o);
    for(int j=0;j<5;j++) fprintf(data_sdl,"%3d ",i[j]);
    fprintf(data_sdl,"%3d ",sel);
    for(int j=0;j<31;j++) fprintf(data_sdl,"%3d ",o[j]);
    fprintf(data_sdl,"%3d\n",o[31]);
  }
}
#endif

#ifdef _DECOD_
#include <stdlib.h>
int main(int argc, char *argv[]) {
  FILE *data=fopen("data","w");
  FILE *data_sdl=fopen("data.sdl","w");
  int iter=0;
  if(argc<2) {
    fprintf(stderr,"%s n'umero_de_entradas_al_descodificador\n",argv[0]);
    return 1;
  }
  int N=atoi(argv[1]);
  fprintf(stderr,"N'umero de entradas: %d\n",N);
  //WIRE in[N], out[1<<N];
  WIRE *in=(WIRE *)malloc(N*sizeof(WIRE));
  WIRE *out=(WIRE *)malloc((1<<N)*sizeof(WIRE));
  for(int i=0;i<N;i++) in[i]=GND;
  DECOD decoder(N);
  for(int i=0;i<N;i++) fprintf(data,"in[%d] ",i);
  for(int i=0;i<(1<<N)-1;i++) fprintf(data,"out[%d] ",i);
  fprintf(data,"out[%d]",(1<<N)-1);
  //decoder.create(N);
  while(iter++<(1<<(N+2))) {
    int j;
    for(j=0;j<N;j++) {
      Clock(iter,1<<(j+1),in[j]);
    }
    decoder.run(in,out);
    for(j=0;j<N;j++) fprintf(data_sdl,"%3d ",in[j]);
    for(j=0;j<(1<<N)-1;j++) fprintf(data_sdl,"%3d ",out[j]);
    fprintf(data_sdl,"%3d\n",out[(1<<N)-1]);
  }
  //decoder.destroy();
}
#endif
