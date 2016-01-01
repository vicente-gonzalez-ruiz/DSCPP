/*
 * print.h -- Funciones de Impresi'on de Bits y Buses.
 * Ultima modificaci'on: 17-10-2000.
 * gse.
 */

void PrintWire(WIRE wire); // Imprime el valor de "wire"
void PrintBit(WIRE bit);   // Imprime "0" o "1" por la salida estandar
void PrintBus(WIRE bus[],  // Imprime "size" bits de golpe comenzando
              int size);   // por el m'as significativo: bus[size-1]

