#include <stdio.h>
#include "gates.h"
#include "random.h"

int main() {
  FILE *data=fopen("data","w");
  FILE *data_sdl=fopen("data.sdl","w");
  int iter=100;
  WIRE a,b,c;
  RANDOM r0(3), r1(7), r2(11);
  fprintf(data,"a b c");
  while(iter--) {
    r0.run(iter,a);
    r1.run(iter,b);
    r2.run(iter,c);
    fprintf(data_sdl,"%3d %3d %3d\n",a,b,c);
  }
}

