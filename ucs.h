/*
 * ucs.h -- Unidades de control.
 * gse. 1999.
 */

/*
 * La ALU que usa el microprocesador R2000 es una ALU est'andar m'as una
 * l'ogica de control adicional que a partir del c'odigo "Fu" del la
 * instrucci'on en ensamblador y del bus de control "ALUOp" que llega desde
 * la Unidad de Control (UC) principal, realiza la operaci'on adecuada.
 *
 * La tabla de verdad implementada por el control es:
 *
 *  ALUOp[] |     Fu[]    |  op[] | instrucci'on
 * ---------+-------------+-------+--------------
 *      0 0 | X X X X X X | 0 1 0 | lw/sw
 *      X 1 | X X X X X X | 1 1 0 | beq
 *      1 0 | X X 0 0 0 0 | 0 1 0 | add
 *      1 0 | X X 0 0 1 0 | 1 1 0 | sub
 *      1 0 | X X 0 1 0 0 | 0 0 0 | and
 *      1 0 | X X 0 1 0 1 | 0 0 1 | or
 *      1 0 | X X 1 0 1 0 | 1 1 1 | slt
 *
 * Que minimizada provoca la l'ogica siguiente:
 *
 * op[0]   = ALUOp[1] AND ( Fu[3] OR Fu[0] )
 * op[1]   = ALUOp[1] XOR Fu[2]
 * bnegate = ALUOp[0] OR ( ALUOp[1] AND Fu[1] )
 *
 * (pag: 256, David A. Patterson & John L. Hennessy,
 * Organizaci'on y Dise~no de Computadores.)
 */
class ALU_CONTROL {
  AND and0, and1;
  OR  or0, or1;
  NAND nand;
  WIRE a,b;
 public:
  void run(WIRE ALUOp[2], WIRE Fu[6], WIRE &bnegate, WIRE op[2]);
};


/*
 * Descripci'on de la unidad de control de la implementaci'on multiciclo.
 *
 * Instrucciones implementadas:
 *
 * 1. Instrucciones aritm'etico-l'ogicas (tipo R).
 *    Formato en c'odigo m'aquina:
 *
 *    +---+----+----+----+---+----+
 *    | 0 | Rs | Rt | Rd | 0 | Fu |
 *    +---+----+----+----+---+----+
 *      6    5    5    5   5    6 bits
 *
 *          Instr.        Acci'on        Campo Fu:
 *      ------------   ---------------   ---------
 *      add Rd,Rs,Rt   Rd <- Rs + Rt        0x20
 *      sub Rd,Rs,Rt   Rd <- Rs - Rt        0x22
 *      and Rd,Rs,Rt   Rd <- Rs and Rt      0x24
 *      or  Rd,Rs,Rt   Rd <- Rs or Rt       0x25
 *      slt Rd,Rs,Rt   Rd <- (Rs<Rt)        0x2A
 *      
 * 2. Instrucciones de carga/almacenamiento (tipo I).
 *    Formatos en c'odigo m'aquina:
 *
 *    lw:
 *    +------+----+----+----------+
 *    | 0x23 | Rs | Rt |   desp   |
 *    +------+----+----+----------+
 *        6     5    5      16 bits
 *
 *    sw:
 *    +------+----+----+----------+
 *    | 0x2B | Rs | Rt |   desp   |
 *    +------+----+----+----------+
 *        6     5    5      16 bits
 *
 *          Instr.            Acci'on
 *      --------------   ------------------
 *      lw Rt,desp(Rs)   Rt <- Mem[Rs+desp]
 *      sw Rt,desp(Rs)   Mem[Rs+desp] <- Rt
 *
 * 3. Instrucci'on de salto condicional.
 *    Formato en c'odigo m'aquina:
 *
 *    +------+----+----+----------+
 *    | 0x04 | Rs | Rt |   desp   |
 *    +------+----+----+----------+
 *        6     5    5      16 bits
 *
 *          Instr.                     Acci'on
 *      --------------   ------------------------------------
 *      beq Rs,Rt,desp   si (Rs==Rt) entonces PC <- PC+desp*4
 *
 * 4. Instrucci'on de salto incondicional.
 *    Formato en c'odigo m'aquina:
 *
 *    +------+--------------------+
 *    | 0x02 | direcci'on instr.  |
 *    +------+--------------------+
 *        6         26 bits
 *
 *        Instr.                     Acci'on
 *     -------------   ----------------------------------------
 *      j direcci'on   PC <- PC[31-28] concatenado direcci'on*4
 *
 * La implementaci'on expuesta a continuaci'on es muticiclo (5 ciclos). La
 * duraci'on del ciclo de reloj ser'a igual a la duraci'on de la etapa m'as larga.
 *
 * 1. Fase de b'usqueda de la instrucci'on (F).
 * ===========================================
 * Esta etapa es com'un a todas las instucciones. En ella se busca la
 * instrucci'on en memoria y se incrementa el PC, para que siempre apunte a la
 * siguiente instrucci'on a ejecutar (excepto en el caso de un salto). La
 * instrucci'on leida de la memoria de instrucciones se almacena en el registro
 * IR (Instruction Register). El registro OPC mantiene el valor del PC sin
 * incrementar porque si la siguiente instrucci'on es beq, el valor de la
 * direcci'on de salto se calcula con el PC sin incrementar.
 *
 * Micro-operaciones       Se~nales de control asertadas
 * -----------------       -----------------------------
 * IR <- Mem[PC]           MemRead <- 1
 * OPC <- PC               IRWrite <- 1
 * PC <- OPC+4             IorD <- 0
 *                         ALUSelB <- 01
 *                         ALUOp <- 00
 *                         WritePC <- 1
 *                         ALUSelA <- 0
 *                         OPCWrite <- 1
 *                         PCSource <- 00
 *
 * 2. Fase de decodificaci'on de la instrucci'on y b'usqueda de los operandos (D).
 * ==============================================================================
 * La unidad de control utiliza el c'odigo de operaci'on de la instrucci'on
 * almacenada en IR para ir asertando las correspondientes se~nales de control.
 * Hasta que no finaliza esta etapa, no sabemos de que instrucci'on se trata.
 * Por esta raz'on, realizamos acciones que son aplicables a todas las
 * instrucciones o que no son perjudiciales, aunque no sean de utilidad.
 * Este tipo de micro-operaciones las llamaremos optimistas.
 *
 * La fase de codificaci'on comienza con la instrucci'on a ejecutar en el
 * registro IR, del que podemos extraer los campos Rs y Rt para leer con ellos
 * el fichero de registros, con la esperanza de adelantar trabajo 'util a la
 * siguiente etapa (notar que el fichero de registros siempre se est'a leyendo).
 * La 'unica instrucci'on que no aprovecha esta acci'on optimista es la de
 * salto incondicional. Otra acci'on de este tipo consiste en calcular la
 * direcci'on de salto OPC+desp*4, que se almacenar'a en un registro dedicado
 * llamado "Target". Esto puede realizarse con la ALU ya que en esta etapa est'a
 * ociosa.
 *
 * Micro-operaciones                Se~nales de control asertadas
 * --------------------------       -----------------------------
 * Target <- OPC + IR[desp]*4       ALUSelB <- 11
 *                                  ALUSelA <- 0
 *                                  ALUOp <- 00
 *                                  TargetWrite <- 1
 *                                  IrWrite <- 0
 *
 * 3. Fase de ejecuci'on de las instrucciones tipo R,
 * ==================================================
 * c'alculo de la direcci'on de memoria de las instrucciones tipo I o
 * ==================================================================
 * terminaci'on de la instrucci'on de salto condicional (beq). (ALU).
 * =================================================================
 * En este instante, la acci'on a tomar depende de la instrucci'on que se est'a
 * ejecutando. En cualquier caso, utilizaremos la ALU que opera sobre los
 * operandos extraidos en la etapa enterior.
 * 
 * 3.1. Instruciones aritm'etico-l'ogicas (ALU.R).
 * ==============================================
 *       La ALU realiza la operaci'on indicada en el campo "Fu".
 *
 *       Micro-operaciones       Se~nales de control asertadas
 *       ---------------------   -----------------------------
 *       ALUOutput <- A oper B   ALUSelA <- 1
 *                               ALUSelB <- 00
 *                               ALUOp <- 10
 *
 * 3.2. Instrucciones de carga/almacenamiento (ALU.lw/sw).
 * ======================================================
 *       La ALU se usa para calcular la direcci'on del dato a leer o escribir.
 *
 *       Micro-operaciones               Se~nales de control asertadas
 *       ---------------------          ------------------------------
 *       ALUOutput <- A oper IR[desp]   ALUSelA <- 1
 *                                      ALUSelB <- 10
 *                                      ALUOp <- 00
 *
 * 3.3. Instrucci'on de salto codicional (ALU.beq).
 * =============================================== 
 *       La ALU compara dos registros. El flag "zero" de la ALU determina
 *       si hay o no salto.
 *
 *       Micro-operaciones               Se~nales de control asertadas
 *       ---------------------          ------------------------------
 *       si (A==B) PC <- Target         ALUSelA <- 1
 *                                      ALUSelB <- 00
 *                                      ALUOp <- 01
 *                                      PCSource <- 01
 *                                      (si zero==1) WritePC <- 1
 *                                      Branch <- 1
 *
 * 3.4. Instrucci'on de salto incondicional (ALU.j).
 * ================================================
 *       Escribimos en el PC la concatenaci'on de los 4 bits m'as significativos
 *       del registro OPC, junto con los 28 bits resultantes de la multiplicaci'on
 *       *4 del campo direcci'on de la instrucci'on de dalto. La instrucci'on
 *       finaliza realizado esto.
 *
 *       Micro-operaciones               Se~nales de control asertadas
 *       ---------------------          ------------------------------
 *       PC <- OPC[31-28]|(IR[25-0]<<2) WritePC <- 1
 *                                      PCSource <-10
 *
 * 4. Fase de acceso a memoria (lw/sw) o
 * =====================================
 * almacenamiento de resultado las instrucciones aritm'etico-l'ogicas (W).
 * ======================================================================
 *
 * 4.1. Instrucciones aritm'etico-l'ogicas (W.R).
 * =============================================
 *       Terminan su ejecuci'on almacenando en resultado de la operaci'on en
 *       el registro destino.
 *
 *       Micro-operaciones               Se~nales de control asertadas
 *       ---------------------          ------------------------------
 *       Reg[IR[Rd]] <- ALUOutput       RegDest <- 1
 *                                      RegWrite <- 1
 *                                      MemtoReg <- 0
 *                                      ALUSelA <- 1
 *                                      ALUSelB <- 00
 *                                      ALUOp <- 10
 *
 * 4.2. Instruci'on lw (W.lw).
 * ==========================
 *       Se extrae una palabra de la RAM de datos.
 *
 *       Micro-operaciones               Se~nales de control asertadas
 *       ---------------------          ------------------------------
 *       MemData <- Mem[ALUOutput]      IorD <- 1
 *                                      MemRead <- 1
 *                                      MemWrite <- 0
 *                                      ALUSelA <- 1
 *                                      ALUSelB <- 10
 *                                      ALUOp <- 00
 *
 * 4.3. Instrucci'on sw (W.sw).
 * ===========================
 *       Finaliza escribiendo el contenido del bus B en la memoria.
 *
 *      Micro-operaciones               Se~nales de control asertadas
 *      ---------------------          ------------------------------
 *      Mem[ALUOutput] <- B             MemRead <- 0
 *                                      MemWrite <- 1
 *                                      ALUSelA <- 1
 *                                      ALUSelB <- 10
 *                                      ALUOp <- 00
 *
 * 5. Fase de post-escritura (WB).
 * ==============================
 *  Finaliza la instrucci'on lw almacenando el dato leido de la memoria sobre el
 *  archivo de registros.
 *
 *      Micro-operaciones               Se~nales de control asertadas
 *      ---------------------          ------------------------------
 *      Reg[IR[Rt]] <- MemData         MemtoReg <- 1
 *                                     RegWrite <- 1
 *                                     RegDest <- 0
 *                                     IorD <- 1
 *                                     MemRead <- 1
 *                                     MemWrite <- 0
 *                                     ALUSelA <- 1
 *                                     ALUSelB <- 10
 *                                     ALUOp <- 00
 *
 * La l'ogica de control puede ser resumida en la siguiente tabla:
 *
 *                                           E T A P A
 *            +------------------------------------------------------------------------+
 *  CONTROL   |  F |  D | ALU.lw/sw | ALU.R | ALU.beq | ALU.j | W.lw | W.sw | W.R | WB |
 * -----------+----+----+-----------+-------+---------+-------+------+------+-----+----+
 * IorD       |  0 |  X |         X |     X |       X |     X |    1 |    X |   X |  1 |
 * MemtoReg   |  X |  X |         X |     X |       X |     X |    X |    X |   0 |  1 |
 * RegDest    |  X |  X |         X |     X |       X |     X |    X |    X |   1 |  0 |
 * ALUSelA    |  0 |  0 |         1 |     1 |       1 |     X |    1 |    1 |   1 |  1 |
 * ALUSelB    | 01 | 11 |        10 |    00 |      00 |    XX |   10 |   10 |  00 | 10 |
 * ALUOp      | 00 | 00 |        00 |    10 |      01 |    XX |   00 |   00 |  10 | 00 |
 * PCSource   | 00 | XX |        XX |    XX |      01 |    10 |   XX |   XX |  XX | XX |
 * -----------+----+----+-----------+-------+---------+-------+------+------+-----+----+
 * MemRead    |  1 |  0 |         0 |     0 |       0 |     0 |    1 |    0 |   0 |  1 |
 * MemWrite   |  0 |  0 |         0 |     0 |       0 |     0 |    0 |    1 |   0 |  0 |
 * IRWrite    |  1 |  0 |         0 |     0 |       0 |     0 |    0 |    0 |   0 |  0 |
 * RegWrite   |  0 |  0 |         0 |     0 |       0 |     0 |    0 |    0 |   1 |  1 |
 * Branch     |  0 |  0 |         0 |     0 |       1 |     0 |    0 |    0 |   0 |  0 |
 * WritePC    |  1 |  0 |         0 |     0 |       1 |     1 |    0 |    0 |   0 |  0 |
 * TargetWrite|  0 |  1 |         0 |     0 |       0 |     0 |    0 |    0 |   0 |  0 |
 * OPCWrite   |  1 |  0 |         0 |     0 |       0 |     0 |    0 |    0 |   0 |  0 |
 * -----------+----+----+-----------+-------+---------+-------+------+------+-----+----+
 *
 * El control puede construirse utilizando un m'aquina de estados finitos o un
 * secuenciador de microprograma. 
 *
 * 1. Control cableado utilizando una m'aquina de estados.
 * ======================================================
 * Se construye un aut'omata secuencial con tantos estados como configuraciones
 * diferentes de se~nales de control hay que contemplar. En cada estado, la
 * m'aquina emplea un ciclo de reloj.
 *
 * El aut'omata (de Moore) a dise~nar debe tener los siguientes estados.
 *
 *     +---+   +-------+    lw/sw    +-----------+ lw  +------+   +----+   
 * +-->| F |-->|   D   |------------>| ALU.lw/sw |---->| W.lw |-->| WB |--+
 * |   +---+   +-------+             +-----------+     +------+   +----+  |
 * |           |   |   |R +-------+   +-----+    | sw  +------+           |
 * |           |   |   +->| ALU.R |-->| W.R |--+ +---->| W.sw |--+        |
 * |           |   |      +-------+   +-----+  |       +------+  |        |
 * |           |   |beq +---------+            |                 |        |
 * |           |   +--->| ALU.beq |--+         |                 |        |
 * |           |        +---------+  |         |                 |        |
 * |           |j   +-------+        |         |                 |        |
 * |           +--->| ALU.j |--+     |         |                 |        |
 * |                +-------+  |     |         |                 |        |
 * +---------------------------+-----+---------+-----------------+--------+
 *
 * A la hora de implementar la m'aquina de estados finitos tenemos dos alternativas.
 * La primera, la cl'asica y que menos circuiter'ia genera, consiste en construir
 * un circuito secuencial de arquitectura:
 *
 *    +-------------------+
 *    |                   |     18     SE~NALES
 *    |      LOGICA       +-----/----> DE
 *    |   COMBINACIONAL   +----+       CONTROL
 *    |                   |    |
 *    +-------------------+    |       
 *         ^         ^ ESTADO  |
 *         |         | ACTUAL  | ESTADO
 *         |  +------+-----+   | SIGUIENTE
 *         |  | REG ESTADO |   |
 *         |  +------------+   |
 *         / 5       ^         / 4
 *         |         |         |
 *         |         +---------+
 *      Op[5-0]
 *
 * El registro de estado debe tener 4 biestables para diferenciar entre los 10 estados
 * distintos que tiene la unidad de control. Lo m'as com'un es utilizar biestables D.
 *
 * Para construir la l'ogica combinacional que encuentra el estado siguiente a partir
 * del estado actual y del c'odigo de operaci'on de la instrucci'on que se est'a
 * ejecutando, es necesario realizar una asignaci'on de estados y minimizar en funci'on
 * de esta, la tabla de transiciones. Ambas se exponen a continuaci'on:
 *
 *   Tabla de signaci'on de estados:
 *   -------------------------------
 *
 *   Fase     | Asignaci'on
 *   ---------+------------
 *          F | 0000
 *          D | 0001
 *  ALU.lw/sw | 0010
 *      ALU.R | 0011
 *    ALU.beq | 0100
 *      ALU.j | 0101
 *       W.lw | 0110
 *       W.sw | 0111
 *        W.R | 1000
 *         WB | 1001
 *
 *
 *
 *  Tabla de transiciones:
 *  ----------------------
 *
 *                    tipo R       lw          sw          beq          j
 *                +-----------+-----------+-----------+-----------+-----------+
 *  Estado Actual |      0x00 |      0x23 |      0x2B |      0x04 |      0x02 |
 *  --------------+-----------+-----------+-----------+-----------+-----------+
 *              F |         D |         D |         D |         D |         D |
 *              D |     ALU.R | ALU.lw/sw | ALU.lw/sw |   ALU.beq |     ALU.j |
 *      ALU.lw/sw |         - |      W.lw |      W.sw |         - |         - |
 *          ALU.R |       W.R |         - |         - |         - |         - |
 *        ALU.beq |         - |         - |         - |         F |         - |
 *          ALU.j |         - |         - |         - |         - |         F |
 *           W.lw |         - |        WB |         - |         - |         - |
 *           W.sw |         - |         - |         F |         - |         - |
 *            W.R |         F |         - |         - |         - |         - |
 *             WB |         - |         F |         - |         - |         - |
 *  --------------+-----------+-----------+-----------+-----------+-----------+
 *
 * Nosotros por simplicidad de dise~no y pensando en futuras almpliaciones vamos a
 * avandonar esta t'ecnica y utilizaremos un registro de desplazamiento que a 
 * continuaci'on se expone:
 *
 *                            Op[5-0]
 *                               |
 *                             ---+b[]+-------+     C     +-----+       D
 *                            /  0|-->| ALU.R |---------->| W.R |----------------------+
 *                           /   1|   --------+ +-------+ +-----+       E              |
 *        Reset (CLR)       /    2|------------>| ALU.j |-----------------------------+|
 *          |              /     3|             +-------+ +---------+   F             ||
 *        +-+-+ A +---+ B /      4|---------------------->| ALU.beq |----------------+||
 *    +-->| F |-->| D |-->       :|                       +---------+                |||
 *    |   +---+   +---+   \      :|                         _ d[]+------+ H +----+ J |||
 *    |                    \    23|--\   c +-----------+ G /0|-->| W.lw |-->| WB |--+|||
 *    |                     \    :|   OR-->| ALU.lw/sw |-->  |   +======+   +----+  ||||
 *    |a                     \  2B|--/     +-----------+   \1|-->| W.sw |--+        ||||
 *    |                       \  :|                         |    +------+ I|        ||||
 *    |                        ---+                       Op[3]            |        ||||
 *   O R                                                                   |        ||||
 * /_____\                                                                 |        ||||
 * | |||||                                                                 |        ||||
 * | ||||+-----------------------------------------------------------------+        ||||
 * | |||+---------------------------------------------------------------------------+|||
 * | ||+-----------------------------------------------------------------------------+||
 * | |+-------------------------------------------------------------------------------+|
 * | +---------------------------------------------------------------------------------+
 * |
 * |e    /- A
 * |   N  - B      
 * +---O  :            
 *     R  - I                
 *       \- J
 *
 * Para generar las se~nales de control a partir de este registro de desplazamiento,
 * miramos en la tabla de verdad de cada se~nal.
 *
 * IorD = Not(A)
 * MemtoReg = Not(D)
 * RegDest = Not(J)
 * ALUSelA = Or(A,B)
 * ALUSelB[1] = Or(D,G,H,I,J)
 * ALUSelB[0] = Or(A,B)
 * ALUOp[1] = Or(C,D)
 * ALUOp[0] = F
 * PCSource[1] = E
 * PCSource[0] = F
 *
 * MemRead = Or(A,H,J)
 * MemWrite = I
 * IRWrite = A
 * RegWrite = Or(D,J)
 * Branch = F
 * WritePC = Or(A,F,E)
 * TargetWrite = B
 * OPCWrite = A
 *
 */

class R2000_CONTROL {
  NEG_D_FF Ff, Df, ALU_R, ALU_j, ALU_beq, ALU_lw_sw, W_R, W_lw, W_sw, WB;
  OR or;
  NOR nor;
  DEMUX demux6(6);
  DEMUX_1_2 demux1;
  WIRE A,B,C,D,E,F,G,H,I,J,a,b[64],c,d[2],e;
  WIRE An,Bn,Cn,Dn,En,Fn,Gn,Hn,In,Jn;
public:
  void run(WIRE &ck,
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
           WIRE &OPCWrite);
};
