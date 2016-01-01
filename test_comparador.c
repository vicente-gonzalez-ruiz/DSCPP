#include <iostream.h>
#include <iomanip.h>
#include "defs.h"
#include "random.h"
#include "gates.h"

void main() {
  int i;                             // Una variable contador
  WIRE A[4], B[4],                   // Buses de entrada A y B
    C[4],                            // Bus interno, salida de las puertas NXOR
    result;                          // Salida del comparador
  NXOR nxor[4];                      // Declaraci'on de las 4 puertas NXOR
  AND and;                           // Declaraci'on de la puerta AND
  for(int iter=0;iter<400;iter++) {  // 400 iteraciones de simulaci'on
    for(i=0;i<4;i++)
      Random(iter,20,A[i]);          // Generaci'on aleatoria del bus A
    for(i=0;i<4;i++)
      Random(iter,20,B[i]);          // Generaci'on aleatoria del bus B
    for(i=0;i<4;i++)
      nxor[i].run(C[i],A[i],B[i]);   // Simulaci'on de las 4 puertas NXOR
    and.run(result,4,C);             // Simulaci'on de la puerta AND
    for(i=0;i<4;i++)
      cout << setw(4) << (int)A[i];  // Salida por pantalla del bus A
    for(i=0;i<4;i++)
      cout << setw(4) << (int)B[i];  // Salida por pantalla del bus B
    cout << setw(4) <<
      (int)result << '\n';           // Salida por pantalla del resultado
  }
}
