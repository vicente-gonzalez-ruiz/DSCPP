/*
 * Descodificadores.
 * Ultima modificaci'on: 23-11-2000.
 * gse.
 */

#include "gates.h"
#include "decods.h"

void DECOD_2_4::run(WIRE in[2], WIRE out[4]) {
  no[0].run(nin[0],in[0]);
  no[1].run(nin[1],in[1]);
  y[0].run(out[0],nin[1],nin[0]);
  y[1].run(out[1],nin[1], in[0]);
  y[2].run(out[2], in[1],nin[0]);
  y[3].run(out[3], in[1], in[0]);
}

void DECOD_5_32::run(WIRE in[5], WIRE out[32]) {
  int i;
  for(i=0;i<5;i++) {
    no[i].run(nin[i],in[i]);
  }
  WIRE aux[5];             // Este bus temporal se usa para elegir
  for(i=0;i<32;i++) {      // las entradas correspondientes a cada
    int j;                 // puerta AND.
    for(j=0;j<5;j++) {     // Este bucle lo que hace b'asicamente es
      if((i>>j)%2) {       // generar un n'umero binario de 5 bits.
        aux[j]=in[j];      // Y en funci'on de que el bit sea 0 o 1,
      } else {             // seleccionamos nin[j] o in[j]
        aux[j]=nin[j];     // respectivamente.
      }
    }
    y[i].run(out[i],5,aux);
  }
}

void DECOD_2_4_SEL::run(WIRE in[2], WIRE &select, WIRE out[4]) {
  dec.run(in,o);                     // Simulamos el descodificador 2:4
  for(int i=0;i<4;i++)
    y[i].run(out[i],o[i],select);  // Simulamos las 4 AND
}

void DECOD_5_32_SEL::run(WIRE in[5], WIRE &select, WIRE out[32]) {
  dec.run(in,o);                     // Simulamos el descodificador 5:32
  for(int i=0;i<32;i++) {
    y[i].run(out[i],o[i],select);  // Simulamos las 32 AND
  }
}

void DECOD::create(int num_sels) {
  this->num_sels = num_sels;
  num_outs = 1<<num_sels;
  nin = new WIRE[num_sels];
  no = new NOT[num_sels];
  y = new AND[num_outs];
  aux = new WIRE[num_sels]; // Variable auxiliar
}
void DECOD::destroy() {
  delete nin;
  delete no;
  delete y;
  delete aux;
}

DECOD::DECOD(){}

DECOD::DECOD(int num_sels) {
  create(num_sels);
}

DECOD::~DECOD() {
  destroy();
}

void DECOD::run(WIRE *in, WIRE *out) {
  int i;
  //int num_outs = 1<<num_sels;
  for(i=0;i<num_sels;i++)
    no[i].run(nin[i],in[i]);
  //WIRE *aux = new WIRE[num_sels];
  //WIRE aux[num_sels];
  for(i=0;i<num_outs;i++) {
    for(int j=0;j<num_sels;j++) {
      if((i>>j)&0x01) {
        aux[j]=in[j];
      } else {
        aux[j]=nin[j];
      }
    }
    y[i].run(out[i],num_sels,aux);
  }
  //delete aux;
}
