/*
 * test_srams.c -- Test de las memorias RAM est'aticas.
 * Ultima modificaci'on: 23-11-00.
 * gse.
 */

#include "gates.h"
#include "decods.h"
#include "muxs.h"
#include "latches.h"
#include "srams.h"
#include <stdio.h>

#ifdef _SRAM_BIT_
#include "clock.h"
void main() {
  FILE *data=fopen("data","w");
  FILE *data_sdl=fopen("data.sdl","w");
   int iters = 500;
  WIRE
    in = GND,
    select = GND,
    write = GND,
    out = GND;
  SRAM_BIT sbit;
  fprintf(data,"in write select out");
  while(iters--) {
    Clock(iters,7,in);
    Clock(iters,200,write);
    Clock(iters,33,select);
    sbit.run(in,select,write,out);
    fprintf(data_sdl,"%3d %3d %3d %3d\n",in,write,select,out);
  }
}
#endif

#ifdef _SRAM_16x4_
#include "clock.h"
#include "random.h"
void main() {
  FILE *data=fopen("data","w");
  FILE *data_sdl=fopen("data.sdl","w");
  int
    iters = 1000,
    i;
  WIRE
    select  = GND,
    write   = GND,
    addr[4] = {GND,GND,GND,GND},
    in[4]   = {GND,GND,GND,GND},
    out[4]  = {GND,GND,GND,GND};
    SRAM_16x4 sram;
  fprintf(data,"select write addr[0] addr[1] addr[2] addr[3] in[0] \
in[1] in[2] in[3] out[0] out[1] out[2] out[3]");
  while(iters--) {
    Clock(iters,256,select);
    Clock(iters,128,write);
    for(i=0;i<4;i++) Clock(iters,1<<(i+5),addr[i]);
    for(i=0;i<4;i++) Random(iters,15,in[i]);
    sram.run(select,write,addr,in,out);
    fprintf(data_sdl,"%4d%4d",select,write);
    for(i=0;i<4;i++) fprintf(data_sdl,"%4d",addr[i]);
    for(i=0;i<4;i++) fprintf(data_sdl,"%4d",in[i]);
    for(i=0;i<4;i++) fprintf(data_sdl,"%4d",out[i]);
    fprintf(data_sdl,"\n");
  }
}
#endif
#ifdef _SRAM_16x4_B_
void main() {
  int
    iters = 1000,
    i;
  WIRE
    select,
    write,
    addr[4],
    in[4],
    out[4];
  SRAM_16x4 sram;
  select=VCC; // Habilitamos la memoria para ser leida o escrita
  write =GND; // Vamos a leer la memoria, no a escribir
  // Direccionamos la posici'on 15
  addr[3] = VCC; addr[2] = VCC; addr[1] = VCC; addr[0] = VCC;
  printf("Simulaci'on de una memoria SRAM con 16 palabras de 4 bits.\n");
  printf("La memoria ha sido configurada para ser leida (select=1 y write=0).\n");
  printf("Van a ser mostrados 25 iteraciones de simulaci'on.\n");
  printf("La direcci'on de memoria seleccionada es la 15.\n");
  printf("El contenido de esta direcci'on es (memoria no inicializada):\n");
  for(i=0;i<25;i++) {
    printf("%5d",i);
    sram.run(select,write,addr,in,out);
    for(int j=3;j>=0;j--)
      printf("%4d",out[j]);
    printf("\n");
  }
  printf("Como se aprecia, la lectura de la memoria se produce en ... qu'e ciclo?\n");
  printf("Ahora vamos a escribir sobre la direcci'on 15 la palabra 1010.\n");
  printf("Usaremos 25 iteraciones de simulaci'on para asegurarnos ");
  printf("que la escritura se produce.\n");
  printf("Como la memoria siempre se est'a leyendo pues est'a selecionada, ");
  printf("presentaremos tambi'en el contenido del bus de salida.\n");
  printf("Cuando este valga lo mismo que hay a la entrada, diremos que la ");
  printf("memoria se ha escrito ...\n Cu'antos ciclos tarda en escribirse?\n");
  write = VCC;
  in[3] = VCC; in[2] = GND; in[1] = VCC; in[0] = GND;
  for(i=0;i<50;i++) {
    printf("%5d",i);
    sram.run(select,write,addr,in,out);
    for(int j=3;j>=0;j--)
      printf("%4d",in[j]);
    printf(" |");
    for(int j=3;j>=0;j--)
      printf("%4d",out[j]);
    printf("\n");
  }
  printf("Ahora vamos a tratar de modificar la palabra de la direcci'on \
de memoria\n");
  printf("15, escribiendo ahora 0000. Sin embargo, haremos que la \
entrada \"write\"=0\n");
  printf("para que la memoria no se habilitada para escritura.\n");
  printf("Es de esperar en este caso el valor almacenado (1010) \
permanezca inalterable.\n");
  printf("Esperamemos a ver si se produce la escritura durante 50 iteraciones.\n");
  write = GND;
  in[3] = GND; in[2] = GND; in[1] = GND; in[0] = GND;
  for(i=0;i<50;i++) {
    printf("%5d",i);
    sram.run(select,write,addr,in,out);
    for(int j=3;j>=0;j--)
      printf("%4d",in[j]);
    printf(" |");
    for(int j=3;j>=0;j--)
      printf("%4d",out[j]);
    printf("\n");
  }
  printf("Simulaci'on finalizada.\n");
}
#endif
