/*
 * test_alu32.c -- Test de las unidades aritm'etico-l'ogicas.
 * Ultima modificaci'on: 23-11-00.
 * gse.
 */

#include "gates.h"
#include "decods.h"
#include "muxs.h"
#include "alu32.h"
#include "clock.h"
#include <stdio.h>

#ifdef _ADDER_BIT_
int main() {
  FILE *data=fopen("data","w");
  FILE *data_sdl=fopen("data.sdl","w");
  int iter=0;
  WIRE a=GND,b=GND,carry_in=GND,sum,carry_out;
  ADDER_BIT adder;
  fprintf(data,"a b carry_in sum carry_out");
  while(iter++ < 200) {
    Clock(iter,10,a);
    Clock(iter,20,b);
    Clock(iter,40,carry_in);
    adder.run(a,b,carry_in,sum,carry_out);
    fprintf(data_sdl,"%4d%4d%4d%4d%4d\n",
	    a,b,carry_in,sum,carry_out);
  }
}
#endif

#ifdef _ADDER_BIT_CLA_
int main() {
  FILE *data=fopen("data","w");
  FILE *data_sdl=fopen("data.sdl","w");
  int iter=0;
  WIRE a=GND,b=GND,carry_in=GND,sum;
  ADDER_BIT_CLA adder;
  fprintf(data,"a b carry_in sum");
  while(iter++ < 200) {
    Clock(iter,10,a);
    Clock(iter,20,b);
    Clock(iter,40,carry_in);
    adder.run(a,b,carry_in,sum);
    fprintf(data_sdl,"%4d%4d%4d%4d\n",
	    a,b,carry_in,sum);
  }
}
#endif

#ifdef _ALU_BIT_
int main() {
  FILE *data=fopen("data","w");
  FILE *data_sdl=fopen("data.sdl","w");
  int iter=0;
  WIRE a=GND,b=GND,b_invert=GND,carry_in=GND,op[2]={GND,GND},
    less,res,carry_out;
  ALU_BIT alu;
  fprintf(data,"less a b carry_in op[0] op[1] b_invert res carry_out");
  while(iter++ < 700) {
    Clock(iter,5,less);
    Clock(iter,10,a);
    Clock(iter,20,b);
    Clock(iter,40,carry_in);
    Clock(iter,80,op[0]);
    Clock(iter,160,op[1]);
    Clock(iter,320,b_invert);
    alu.run(a,b,b_invert,carry_in,op,less,res,carry_out);
    fprintf(data_sdl,"%4d%4d%4d%4d%4d%4d%4d%4d%4d\n",
	    less,a,b,carry_in,op[0],op[1],b_invert,res,carry_out);
  }
}
#endif

#ifdef _ALU_BIT_CLA_
int main() {
  FILE *data=fopen("data","w");
  FILE *data_sdl=fopen("data.sdl","w");
  int iter=0;
  WIRE a=GND,b=GND,b_invert=GND,carry_in=GND,op[2]={GND,GND},
    less,res;
  ALU_BIT_CLA alu;
  fprintf(data,"less a b carry_in op[0] op[1] b_invert res");
  while(iter++ < 700) {
    Clock(iter,5,less);
    Clock(iter,10,a);
    Clock(iter,20,b);
    Clock(iter,40,carry_in);
    Clock(iter,80,op[0]);
    Clock(iter,160,op[1]);
    Clock(iter,320,b_invert);
    alu.run(a,b,b_invert,carry_in,op,less,res);
    fprintf(data_sdl,"%4d%4d%4d%4d%4d%4d%4d%4d\n",
	    less,a,b,carry_in,op[0],op[1],b_invert,res);
  }
}
#endif

#ifdef _ALU_MSBIT_
int main() {
  FILE *data=fopen("data","w");
  FILE *data_sdl=fopen("data.sdl","w");
  int iter=0;
  WIRE a,b,b_invert,carry_in,op[2],less,res,carry_out,set,overflow;
  ALU_MSBIT alu;
  fprintf(data,"less a b carry_in op[0] op[1] b_invert res carry_out set overflow");
  while(iter++ < 700) {
    Clock(iter,5,less);
    Clock(iter,10,a);
    Clock(iter,20,b);
    Clock(iter,40,carry_in);
    Clock(iter,80,op[0]);
    Clock(iter,160,op[1]);
    Clock(iter,320,b_invert);
    alu.run(a,b,b_invert,carry_in,op,less,res,carry_out,set,overflow);
    fprintf(data_sdl,"%4d%4d%4d%4d%4d%4d%4d%4d%4d%4d%4d\n",
	    less,a,b,carry_in,op[0],op[1],b_invert,res,carry_out,set,overflow);
  }
}
#endif

#ifdef _ALU_32BIT_RC_
#include "random.h"

#define ITERS 50

int Bin2Dec(WIRE z[32]) {
  int i;
  int value = 0;
  for(i=0;i<32;i++) {
    int bit;
    if(z[i]) bit = 1; else bit = 0;
    value += bit*(1<<i);
  }
  return value;
}

int main() {
  int i,j,k;
  WIRE a[32],b[32], // Bus de datos
    bnegate,op[2],  // Operaci'on aritm'etica
    res[32],        // Resultado
    overflow,       // Overflow en la operaci'on
    carry_out,      // Acarreo aritm'etico
    zero;           // El resultado fue cero
  ALU_32BIT_RC alu;

  printf("Simulaci'on de una ALU de 32 bits (Ripple-Carry).\n\n");
  printf("Se probar'an cada una de las operaciones aritm'eticas 'utiles.\n");
  printf("Se realizar'an 10 pruebas para cada operaci'on.\n\n");

  /*************** AND *************/

  for(k=0;k<10;k++) {
    printf("Operaci'on AND: (bnegate=0, op=00).\n");
    printf("Contenido del bus a[]:\n");
    bnegate = op[1] = op[0] = GND;
    for(i=31;i>=0;i--) {
      Random(1,1,a[i]);
      printf("%4d",a[i]);
    }
    printf("\n");
    printf("Contenido del bus b[]:\n");
    for(i=31;i>=0;i--) {
      Random(1,1,b[i]);
      printf("%4d",b[i]);
    }
    printf("\n");
    printf("Igual a:\n");
    for(j=0;j<10;j++) {
      alu.run(a,b,bnegate,op,res,overflow,carry_out,zero);
      for(i=31;i>=0;i--) {
	printf("%4d",res[i]);
      }
      printf("\n");
    }
  }

  /*********** OR *************/

  for(k=0;k<10;k++) {
    printf("Operaci'on OR: (bnegate=0, op=01).\n");
    printf("Contenido del bus a[]:\n");
    bnegate = op[1] = GND; op[0] = VCC;
    for(i=31;i>=0;i--) {
      Random(1,1,a[i]);
      printf("%4d",a[i]);
    }
    printf("\n");
    printf("Contenido del bus b[]:\n");
    for(i=31;i>=0;i--) {
      Random(1,1,b[i]);
      printf("%4d",b[i]);
    }
    printf("\n");
    printf("Igual a:\n");
    for(j=0;j<10;j++) {
      alu.run(a,b,bnegate,op,res,overflow,carry_out,zero);
      for(i=31;i>=0;i--) {
	printf("%4d",(int)res[i]);
      }
      printf("\n");
    }
  }

  /*********** ADD *************/

  for(k=0;k<10;k++) {
    printf("Operaci'on ADD: (bnegate=0, op=10).\n");
    printf("Contenido del bus a[]:\n");
    bnegate = op[0] = GND; op[1] = VCC;
    for(i=31;i>=0;i--) {
      Random(1,1,a[i]);
      printf("%4d",a[i]);
    }
    printf(" = %d\n",Bin2Dec(a));
    printf("Contenido del bus b[]:\n");
    for(i=31;i>=0;i--) {
      Random(1,1,b[i]);
      printf("%4d",b[i]);
    }
    printf(" = %d\n",Bin2Dec(b));
    printf("Igual a:\n");
    for(j=0;j<ITERS;j++) {
      alu.run(a,b,bnegate,op,res,overflow,carry_out,zero);
      for(i=31;i>=0;i--) {
	printf("%4d",res[i]);
      }
      printf(" = %d\n",Bin2Dec(res));
    }
  } /* k */

  /*********** SUB *************/

  for(k=0;k<10;k++) {
    printf("Operaci'on SUB: (bnegate=1, op=10).\n");
    printf("Contenido del bus a[]:\n");
    bnegate = VCC; op[0] = GND; op[1] = VCC;
    for(i=31;i>=0;i--) {
      Random(1,1,a[i]);
      printf("%4d",a[i]);
    }
    printf(" = %d\n",Bin2Dec(a));
    printf("Contenido del bus b[]:\n");
    for(i=31;i>=0;i--) {
      Random(1,1,b[i]);
      printf("%4d",b[i]);
    }
    printf(" = %d\n",Bin2Dec(b));
    printf("Igual a:\n");
    for(j=0;j<ITERS;j++) {
      alu.run(a,b,bnegate,op,res,overflow,carry_out,zero);
      for(i=31;i>=0;i--) {
	printf("%4d",res[i]);
      }
      printf(" = %d\n",Bin2Dec(res));
    }
  } /* k */

  /*********** SLT *************/

  for(k=0;k<10;k++) {
    printf("Operaci'on SLT: (bnegate=1, op=11).\n");
    printf("Contenido del bus a[]:\n");
    bnegate = VCC; op[0] = VCC; op[1] = VCC;
    for(i=31;i>=0;i--) {
      Random(1,1,a[i]);
      printf("%4d",a[i]);
    }
    printf(" = %d\n",Bin2Dec(a));
    printf("Contenido del bus b[]:\n");
    for(i=31;i>=0;i--) {
      Random(1,1,b[i]);
      printf("%4d",b[i]);
    }
    printf(" = %d\n",Bin2Dec(b));
    printf("Igual a:\n");
    for(j=0;j<ITERS;j++) {
      alu.run(a,b,bnegate,op,res,overflow,carry_out,zero);
      for(i=31;i>=0;i--) {
	printf("%4d",res[i]);
      }
      printf(" = %d\n",Bin2Dec(res));
    }
  } /* k */
}
#endif

#ifdef _ALU_32BIT_CLA_                     
#include "random.h"

#define ITERS 50

int Bin2Dec(WIRE z[32]) {
  int i;
  int value = 0;
  for(i=0;i<32;i++) {
    int bit;
    if(z[i]) bit = 1; else bit = 0;
    value += bit*(1<<i);
  }
  return value;
}

int main() {
  int i,j,k;
  WIRE a[32],b[32], // Bus de datos
    bnegate,op[2],  // Operaci'on aritm'etica
    res[32],        // Resultado
    overflow,       // Overflow en la operaci'on
    carry_out,      // Acarreo aritm'etico
    zero;           // El resultado fue cero
  ALU_32BIT_CLA alu;

  printf("Simulaci'on de una ALU de 32 bits (Carry-Look Ahead).\n\n");
  printf("Se probar'an cada una de las operaciones aritm'eticas 'utiles.\n");
  printf("Se realizar'an 10 pruebas para cada operaci'on.\n\n");

  /*************** AND *************/

  for(k=0;k<10;k++) {
    printf("Operaci'on AND: (bnegate=0, op=00).\n");
    printf("Contenido del bus a[]:\n");
    bnegate = op[1] = op[0] = GND;
    for(i=31;i>=0;i--) {
      Random(1,1,a[i]);
      printf("%4d",a[i]);
    }
    printf("\n");
    printf("Contenido del bus b[]:\n");
    for(i=31;i>=0;i--) {
      Random(1,1,b[i]);
      printf("%4d",b[i]);
    }
    printf("\n");
    printf("Igual a:\n");
    for(j=0;j<10;j++) {
      alu.run(a,b,bnegate,op,res,overflow,carry_out,zero);
      for(i=31;i>=0;i--) {
	printf("%4d",res[i]);
      }
      printf("\n");
    }
  }

  /*********** OR *************/

  for(k=0;k<10;k++) {
    printf("Operaci'on OR: (bnegate=0, op=01).\n");
    printf("Contenido del bus a[]:\n");
    bnegate = op[1] = GND; op[0] = VCC;
    for(i=31;i>=0;i--) {
      Random(1,1,a[i]);
      printf("%4d",a[i]);
    }
    printf("\n");
    printf("Contenido del bus b[]:\n");
    for(i=31;i>=0;i--) {
      Random(1,1,b[i]);
      printf("%4d",b[i]);
    }
    printf("\n");
    printf("Igual a:\n");
    for(j=0;j<10;j++) {
      alu.run(a,b,bnegate,op,res,overflow,carry_out,zero);
      for(i=31;i>=0;i--) {
	printf("%4d",(int)res[i]);
      }
      printf("\n");
    }
  }

  /*********** ADD *************/

  for(k=0;k<10;k++) {
    printf("Operaci'on ADD: (bnegate=0, op=10).\n");
    printf("Contenido del bus a[]:\n");
    bnegate = op[0] = GND; op[1] = VCC;
    for(i=31;i>=0;i--) {
      Random(1,1,a[i]);
      printf("%4d",a[i]);
    }
    printf(" = %d\n",Bin2Dec(a));
    printf("Contenido del bus b[]:\n");
    for(i=31;i>=0;i--) {
      Random(1,1,b[i]);
      printf("%4d",b[i]);
    }
    printf(" = %d\n",Bin2Dec(b));
    printf("Igual a:\n");
    for(j=0;j<ITERS;j++) {
      alu.run(a,b,bnegate,op,res,overflow,carry_out,zero);
      for(i=31;i>=0;i--) {
	printf("%4d",res[i]);
      }
      printf(" = %d\n",Bin2Dec(res));
    }
  } /* k */

  /*********** SUB *************/

  for(k=0;k<10;k++) {
    printf("Operaci'on SUB: (bnegate=1, op=10).\n");
    printf("Contenido del bus a[]:\n");
    bnegate = VCC; op[0] = GND; op[1] = VCC;
    for(i=31;i>=0;i--) {
      Random(1,1,a[i]);
      printf("%4d",a[i]);
    }
    printf(" = %d\n",Bin2Dec(a));
    printf("Contenido del bus b[]:\n");
    for(i=31;i>=0;i--) {
      Random(1,1,b[i]);
      printf("%4d",b[i]);
    }
    printf(" = %d\n",Bin2Dec(b));
    printf("Igual a:\n");
    for(j=0;j<ITERS;j++) {
      alu.run(a,b,bnegate,op,res,overflow,carry_out,zero);
      for(i=31;i>=0;i--) {
	printf("%4d",res[i]);
      }
      printf(" = %d\n",Bin2Dec(res));
    }
  } /* k */

  /*********** SLT *************/

  for(k=0;k<10;k++) {
    printf("Operaci'on SLT: (bnegate=1, op=11).\n");
    printf("Contenido del bus a[]:\n");
    bnegate = VCC; op[0] = VCC; op[1] = VCC;
    for(i=31;i>=0;i--) {
      Random(1,1,a[i]);
      printf("%4d",a[i]);
    }
    printf(" = %d\n",Bin2Dec(a));
    printf("Contenido del bus b[]:\n");
    for(i=31;i>=0;i--) {
      Random(1,1,b[i]);
      printf("%4d",b[i]);
    }
    printf(" = %d\n",Bin2Dec(b));
    printf("Igual a:\n");
    for(j=0;j<ITERS;j++) {
      alu.run(a,b,bnegate,op,res,overflow,carry_out,zero);
      for(i=31;i>=0;i--) {
	printf("%4d",res[i]);
      }
      printf(" = %d\n",Bin2Dec(res));
    }
  } /* k */
}
#endif
