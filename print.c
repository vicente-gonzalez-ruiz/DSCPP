/*
 * print.c -- Funciones de Impresi'on de Bits y Buses.
 * Ultima modificaci'on: 17-10-2000.
 * gse.
 */

#include <stdio.h>
#include "defs.h"
#include "print.h"

void PrintWire(WIRE wire) {
  printf(" %3d",wire);
}

void PrintBit(WIRE bit) {
  putchar(' ');
  if(bit) putchar('1'); else putchar('0');
}

void PrintBus(WIRE bus[], int size) {
  int i;
  putchar(' ');
  for(i=size-1;i>=0;i--) {
    if(bus[i]) putchar('1'); else putchar('0');
  }
}
