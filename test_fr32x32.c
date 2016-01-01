/*
 * test_fr32x32.c -- Testeo del Fichero de Registros de 32x32 bits
 * (R2000).
 * Ultima modificaci'on: 23-11-00.
 * gse.
 */

#include <stdio.h>
#include "gates.h"
#include "decods.h"
#include "latches.h"
#include "flip-flops.h"
#include "fr32x32.h"
#include "clock.h"

#ifdef _FR_BIT_
void main() {
  FILE *data=fopen("data","w");
  FILE *data_sdl=fopen("data.sdl","w");
  int iters = 1000;
  WIRE
    in = GND,
    sel0 = GND,
    sel1 = GND,
    write = GND,
    out0 = GND,
    out1 = GND;
  FR_BIT bit;
  fprintf(data,"in write sel0 sel1 out0 out1");
  while(iters--) {
    Clock(iters,33,in);
    Clock(iters,47,write);
    Clock(iters,100,sel0);
    Clock(iters,200,sel1);
    bit.run(in,sel0,sel1,write,out0,out1);
    fprintf(data_sdl,"%3d %3d %3d %3d %3d %3d\n",
	    in,write,sel0,sel1,out0,out1);
  }
}
#endif

#ifdef _FR_32x32_
void main() {
  int i,j,k;
  FR_32x32 fr;
  WIRE
    in[32],   // Bus de datos de entrada
    addr0[5], // Direcci'on del primer registro leido
    addr1[5], // Direcci'on del segundo registro leido
    addr2[5], // Direcci'on del registro escrito
    write,    // Se~nal de habilitaci'on de escritura sobre un registro
    out0[32], // Contenido del primer registro leido
    out1[32]; // Contenido del segundo registro leido

  printf("Simulaci'on del fichero de registros del MIPS R2000\n");
  printf("(32 registros de 32 bits, reg[0] siempre a \"0\").\n\n");

  write = VCC; // A "1" para escribir cuando se produzca el flanco negativo

  printf("La primera operaci'on que vamos a realizar es leer el contenido\n");
  printf("del registro 0, que seg'un las especificaci'ones de MIPS debiera ser\n");
  printf("siempre cero.\nVamos a leer el contenido del este registro por el bus\n");
  printf("de salida \"out0\".\n");
  printf("Para ello, hacemos \"addr0\"=00000.\nPresentamos el contenido del bus\n");
  printf("\"out0\" durante 25 iteraciones de simulaci'on.\n");

  addr0[0] = addr0[1] = addr0[2] = addr0[3] = addr0[4] = GND;
  for(i=0;i<25;i++) {
    fr.run(in,addr0,addr1,addr2,write,out0,out1);
    printf("%3d",i);
    for(j=31;j>=0;j--) {
      printf("%4d",out0[j]);
    }
    printf("\n");
  }

  printf("A continuaci'on, lo que vamos a realizar es escribir sobre el registro 1\n");
  printf("el valor \"0010 0011 0100 0101 0110 0111 1000 1001\".\n");
  printf("Para hacer esto haremos que \"write\" (que durante todo este tiempo\n");
  printf("ha estado a \"1\") describa un flanco de bajada (poni'endose a \"0\"),\n");
  printf("que el bus de direcci'on \"addr2\" sea igual a 00001 ");
  printf("y que el bus \"in\" tenga el contenido anteriormente indicado.\n");
  printf("Para saber cuando el registro ha sido escrito y puede ser leido,\n");
  printf("mostraremos el contenido del registro 1 a trav'es del bus de salida\n");
  printf("\"out1\".\nPor tanto, haremos que \"addr1\" sea tambi'en igual a 00001.\n");
  printf("En primer lugar presentaremos el contenido del bus \"out1\" durante 5\n");
  printf("iteraciones, para mostrar que su contenido es estable.\n");

  addr1[0] = VCC; addr1[1] = addr1[2] = addr1[3] = addr1[4] = GND;
  addr2[0] = VCC; addr2[1] = addr2[2] = addr2[3] = addr2[4] = GND;
  in[31] = GND; in[30] = GND; in[29] = VCC; in[28] = GND;
  in[27] = GND; in[26] = GND; in[25] = VCC; in[24] = VCC;
  in[23] = GND; in[22] = VCC; in[21] = GND; in[20] = GND;
  in[19] = GND; in[18] = VCC; in[17] = GND; in[16] = VCC;
  in[15] = GND; in[14] = VCC; in[13] = VCC; in[12] = GND;
  in[11] = GND; in[10] = VCC; in[ 9] = VCC; in[ 8] = VCC;
  in[ 7] = VCC; in[ 6] = GND; in[ 5] = GND; in[ 4] = GND;
  in[ 3] = VCC; in[ 2] = GND; in[ 1] = GND; in[ 0] = VCC;
  for(i=0;i<5;i++) {
    fr.run(in,addr0,addr1,addr2,write,out0,out1);
    printf("%3d",i);
    for(j=31;j>=0;j--) {
      printf("%4d",out1[j]);
    }
    printf("\n");
  }
  
  printf("En este instante hacemos que \"write\" sea \"0\", con lo que da\n");
  printf("la orden de escritura.\n");
  
  write = GND;
  
  printf("Ahora presentaremos el contenido del bus \"out1\" durante\n");
  printf("25 iteraciones.\n");

  for(i=0;i<25;i++) {
    fr.run(in,addr0,addr1,addr2,write,out0,out1);
    printf("%3d",i);
    for(j=31;j>=0;j--) {
      printf("%4d",out1[j]);
    }
    printf("\n");
  }

  printf("Como puede verse, el fichero de registros emplea 7 ciclos de simulaci'on\n");
  printf("en presentar a la salida el contenido de un registro que acaba de ser\n");
  printf("modificado.\n");

  printf("Ahora vamos a tratar de modificar el contenido del registro 0.\n");
  printf("Para ello, hacemos \"addr2\"=\"00000\" y provocaremos flancos de bajada\n");
  printf("sobre \"write\", conectando esta entrada a un reloj.\n");
  printf("Haremos adem'as que \"addr0\"=\"00000\" para ver el resultado a trav'es\n");
  printf("del bus \"out0\".\n");
  printf("Ser'an presentadas 100 iteraciones de simulaci'on.\n");
  
  for(i=0;i<5;i++) addr0[i] = addr2[i] = GND;
  for(i=0;i<100;i++) {
    Clock(i,10,write);
    fr.run(in,addr0,addr1,addr2,write,out0,out1);
    printf("%4d",i);
    printf("%4d",write);
    for(j=31;j>=0;j--) {
      printf("%4d",out0[j]);
    }
    printf("\n");
  }
  
  printf("Sin embargo, si elegimos otro registro (por ejemplo, el registro 2)\n");
  printf("y hacemos el test anterior, si debemos obtener alg'un cambio a la salida.\n");
  printf("Presentamos 50 iteraciones.\n");

  addr0[1] = addr2[1] = VCC; // El resto valen GND
  for(i=0;i<50;i++) {
    Clock(i,10,write);
    fr.run(in,addr0,addr1,addr2,write,out0,out1);
    printf("%4d",i);
    printf("%4d",write);
    for(j=31;j>=0;j--) {
      printf("%4d",out0[j]);
    }
    printf("\n");
  }

  printf("Por 'ultimo, mostramos el contenido de cada uno de los registros\n");
  printf("a trav'es del bus de salida out0[], durante\n");
  printf("10 iteraciones de simulaci'on:\n");
  for(j=0;j<32;j++) {
    printf("Registro %2d\n",j);
    addr0[0] = (j&1)*255;
    addr0[1] = ((j>>1)&1)*255;
    addr0[2] = ((j>>2)&1)*255;
    addr0[3] = ((j>>3)&1)*255;
    addr0[4] = ((j>>4)&1)*255;
    for(i=0;i<10;i++) {
      fr.run(in,addr0,addr1,addr2,write,out0,out1);
      printf("%4d",i);
      for(k=31;k>=0;k--) {
	printf("%4d",out0[k]);
      }
      printf("\n");
    }
  }
}
#endif
