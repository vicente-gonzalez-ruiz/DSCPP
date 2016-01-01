#include "defs.h"
#include "gates.h"
#include "decods.h"
#include "muxs.h"
#include "latches.h"
#include "flip-flops.h"
#include "ucs.h"

void ALU_CONTROL::run(WIRE ALUOp[2], WIRE Fu[6], WIRE &bnegate, WIRE op[2]) {
  or0.run(Fu[3],Fu[0],a);
  and0.run(ALUOp[1],a,op[0]);
  nand.run(ALUOp[1],Fu[2],op[1]);
  and1.run(ALUOp[1],Fu[1],b);
  or1.run(ALUOp[0],b,bnegate);
}

void R2000_CONTROL::run(WIRE &ck,
                        WIRE Op[6],
                        WIRE &IorD,
                        WIRE &MemtoReg,
                        WIRE &RegDest,
                        WIRE &ALUSelA,
                        WIRE ALUSelB[2],
                        WIRE ALUOp[2],
                        WIRE PCSource[2],
                        WIRE &MemRead,
                        WIRE &MemWrite,
                        WIRE &IRWrite,
                        WIRE &RegWrite,
                        WIRE &Branch,
                        WIRE &WritePC,
                        WIRE &TargetWrite,
                        WIRE &OPCWrite) {
  Ff.run(a,ck,A,An);
  Df.run(A,ck,B,Bn);
  demux6.run(B,Op,b);
  ALU_R.run(b[0],ck,C,Cn);
  ALU_j.run(b[2],ck,E,En);
  ALU_beq.run(b[4],ck,F,Fn);
  or.run(b[0x23],b[0x2B],c);
  ALU_lw_sw.run(c,ck,G,Gn);
  W_R.run(C,ck,D,Dn);
  demux1.run(G,Op[3],d);
  W_lw.run(d[0],ck,H,Hn);
  W_sw.run(d[1],ck,I,In);
  WB.run(H,ck,J,Jn);
  or.run(a,6,e,D,E,F,I,J);
  nor.run(e,10,A,B,C,D,E,F,G,H,I,J);
}


#ifdef _TEST_
#include <iostream.h>
#include <iomanip.h>
#include "clock.h"
#include "random.h"

int Logical(WIRE x) {
  if(x==255) return 1;
  else return 0;
}


#ifdef _ALU_CONTROL_
void main() {
  WIRE
    ALUOp[2],
    Fu[6],
    bnegate,
    op[2];
  ALU_CONTROL c;
  cout << "Simulaci'on del control de la ALU para MIPS R2000.\n\n"
       << " ALUOp[] |     Fu[]    | op[]\n"
       << "---------+-------------+------\n";
  int i;
  for(i=0;i<256;i++) {
    ALUOp[1] = ((i&0x80)>>7)*VCC;
    ALUOp[0] = ((i&0x40)>>6)*VCC;
    Fu[5]    = ((i&0x20)>>5)*VCC;
    Fu[4]    = ((i&0x10)>>4)*VCC;
    Fu[3]    = ((i&0x08)>>3)*VCC;
    Fu[2]    = ((i&0x04)>>2)*VCC;
    Fu[1]    = ((i&0x02)>>1)*VCC;
    Fu[0]    = (i&0x01)*VCC;
    int j;
    for(j=0;j<5;j++) {
      c.run(ALUOp,Fu,bnegate,op);
      cout << "     "
           << Logical(ALUOp[1])
           << ' '
           << Logical(ALUOp[0])
           << " | "
           << Logical(Fu[5])
           << ' '
           << Logical(Fu[4])
           << ' '
           << Logical(Fu[3])
           << ' '
           << Logical(Fu[2])
           << ' '
           << Logical(Fu[1])
           << ' '
           << Logical(Fu[0])
           << " | "
           << Logical(bnegate)
           << ' '
           << Logical(op[1])
           << ' '
           << Logical(op[0])
           << '\n';
    }
    cout << "---------+-------------+------\n";
  }
}
#endif

#ifdef _R2000_CONTROL_
void main() {
  R2000_CONTROL uc;
  WIRE ck;
  WIRE Op[6];
  WIRE IorD;
  WIRE MemtoReg;
  WIRE RegDest;
  WIRE ALUSelA;
  WIRE ALUSelB[2];
  WIRE ALUOp[2];
  WIRE PCSource[2];
  WIRE MemRead;
  WIRE MemWrite;
  WIRE IRWrite;
  WIRE RegWrite;
  WIRE Branch;
  WIRE WritePC;
  WIRE TargetWrite;
  WIRE OPCWrite;

  cout << "Simulaci'on de la unidad de control del MIPS R2000.\n\n"
       << "La unidad tiene dos entradas:\n"
       << " 1. ck: se~nal de reloj\n"
       << " 2. Op[6]: c'odigo de operaci'on de la instrucci'on ejecutada\n"
       << "y 19 salidas:\n\n"
       << "IorD: Controla la direcci'on de lectura de la SRAM\n"
       << "      Un 0 indica que direccionamos con el PC (instrucci'on)\n"
       << "      Un 1 indica que direccionamos con la ALU (dato)\n\n"
       << "MemtoReg: Controla el bus de entrada de datos del fichero de registros\n"
       << "          Un 0 indica que escribimos un dato desde la SRAM\n"
       << "          Un 1 indica que escribimos un dato desde la ALU\n\n"
       << "RegDest: Controla el bus de direcci'on del registro escrito en el "
       << "fichero de registros\n"
       << "         Un 0 indica que el campo \"Rt\" de la instrucci'on "
       << "es usado como direcci'on\n"
       << "         Un 1 indica que el campo \"Rd\" de la instrucci'on "
       << "es usado como direcci'on\n\n"
       << "ALUSelA: Controla el bus de entrada A de la ALU\n"
       << "         Un 0, el contenido de OPC se escribe en el bus A\n"
       << "         Un 1, el contenido de la salida 0 del fichero de "
       << "registros se escribe en el bus A\n\n"
       << "ALUSelB[2]: Controla el bus de entrada B de la ALU\n"
       << "            Con 00, la salida 1 del fichero de registros es usada\n"
       << "            Con 01, el valor constante 4 es volcado\n"
       << "            Con 10, la extensi'on a 32 bits del campo \"desp\" de la "
       << "instrucci'on es usada\n"
       << "            Con 11, la extensi'on a 32 bits del campo \"desp\" de la "
       << "instrucci'on, multiplicada por 4 es usada\n\n"
       << "ALUOp[2]: Controla la operaci'on realizada por la ALU\n"
       << "·         Con 00, la ALU suma (instrucci'on lw/sw)\n"
       << "          Con 01, la ALU resta (instrucci'on beq)\n"
       << "          Con 10, la ALU realiza la operaci'on indicada en el campo Fu[]\n"
       << "          Con 11, la ALU realiza la operaci'on de comparaci'on \"slt\"\n\n"
       << "PCSource[2]: Controla el valor escrito sobre el registro PC\n"
       << "             Con 00, la salida de la ALU es usada\n"
       << "             Con 01, la salida del registro Target es usado\n"
       << "             Con 10, el campo \"Dir\" de la instrucci'on concatenado con los "
       << "4 bits m'as significativos del PC y todo esto multiplicado por 4 es usado\n\n"
       << "MemRead: Asertada (1), indica al sistema de memoria que se desea leer un "
       << "dato o una instrucci'on\n\n"
       << "MemWrite: Asertada (1), indica al sistema de memoria qu ese desea escribir "
       << "un dato o una instrucci'on\n\n"
       << "IRWrite: Asertada (1), indica que el registro de instrucci'on (IR) debe ser "
       << "escrito con el contenido del bus de salida de la memoria\n\n"
       << "RegWrite: Asertada (1), indica que el fichero de registros (FR) debe ser "
       << "escrito\n\n"
       << "Branch: Asertada (1) junto con el flag \"zero\" de la ALU indica si hay que "
       << "saltar (cargar el PC)\n\n"
       << "WritePC: Asertada (1) indica que el contador de programa (PC) debe se escrito\n\n"
       << "TargetWrite: Asertada (1) indica que el registro \"Target\" debe ser escrito\n\n"
       << "OPCWrite: Asertada (1) indica que el registro \"OPC\" debe ser escrito\n\n";

  cout << "La implementaci'on actual del R2000 s'olo ejecuta las instrucciones\n\n"
       << "  instrucci'on m'aquina | C'odigo de operaci'on (Op[6])\n"
       << "------------------------+------------------------------\n"
       << "                     lw | 0x23\n"
       << "                     sw | 0x2B\n"
       << "                    beq | 0x04\n"
       << "                      j | 0x02\n"
       << " add, sub, and, or, slt | 0x00\n\n";

  cout << "Testearemos en este orden cada una de las instrucciones\n\n";

  cout << "Instrucci'on lw\n\n";
  Op[5] = VCC; Op[4] = GND; Op[3] = GND; Op[2] = GND; Op[1] = VCC; Op[0] = VCC;
  cout << "Contenido de Op[6]="
       << Logical(Op[5])
       << Logical(Op[4])
       << Logical(Op[3])
       << Logical(Op[2])
       << Logical(Op[1])
       << Logical(Op[0])
       << "\n";
  cout << "Definimos un reloj de 32 ciclos de periodo\n\n";
  int i;
  cout << "Fase de b'usqueda de la instrucci'on\n";
  cout << "ciclo ck Op[2] IorD MemtoReg RegDest ALUSelA ALUSelB ALUOp PCSource MemRead "
       << "MemWrite IRWrite RegWrite Branch WritePC TargetWrite OPCWrite\n";
  for(i=0;i<32;i++) {
    Clock(i,16,ck);

    uc.run(ck,Op,IorD,MemtoReg,RegDest,ALUSelA,ALUSelB,ALUOp,PCSource,
           MemRead,MemWrite,IRWrite,RegWrite,Branch,WritePC,TargetWrite,OPCWrite);
    cout
      << setw(5) << i
      << setw(3) << Logical(ck)
      << setw(5) << Logical(Op[1]) << Logical(Op[0])
      << setw(5) << Logical(IorD)
      << setw(9) << Logical(MemtoReg)
      << setw(9) << Logical(RegDest)
      << setw(8) << Logical(ALUSelA)
      << setw(6) << Logical(ALUSelB[1]) << Logical(ALUSelB[0])
      << setw(5) << Logical(ALUOp[1]) << Logical(ALUOp[0])
      << setw(8) << Logical(PCSource[1]) << Logical(PCSource[0])
      << setw(8) << Logical(MemRead)
      << setw(9) << Logical(MemWrite)
      << setw(8) << Logical(IRWrite)
      << setw(9) << Logical(RegWrite)
      << setw(7) << Logical(Branch)
      << setw(8) << Logical(WritePC)
      << setw(12) << Logical(TargetWrite)
      << setw(9) << Logical(OPCWrite)
      << '\n';
  }
}
#endif
#endif
