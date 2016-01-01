/*
 * R2000.h -- Microprocesador R2000.
 * gse. 1999.
 */

/*
 * Este es el interface del MIPS R2000.
 *
 *                             |
 *                             v
 *             +--------------------------------+
 *             |             Clock              |
 *             |                                |
 *         32  |                                |   32
 *      <--/---| DataInAddr         DataOutAddr |---/-->
 *             |                                |
 *             |      M I P S   R 2 0 0 0       |
 *         32  |                                |   32
 *      ---/-->| DataIn                 DataOut |---/-->
 *             |                                |
 *             |                                |
 *             |            RAMWrite            |
 *             +--------------------------------+
 *                             |
 *                             v
 *
 * Clock:       se~nal de reloj (controla la velocidad de funcionamiento).
 * DataInAddr:  direcci'on de la palabra leida en RAM.
 * DataIn:      palabra leida en RAM.
 * DataOutAddr: direcci'on de la palabra escrita en RAM.
 * DataOut:     palabra escrita en RAM.
 * RAMWrite:    orden de escritura en RAM.
 */

/*
                                                              +--------------+
                        +-----------------------------+       |              |
                        |              PC             |<------| PCWrite      |
                        +--------------v--------------+       |              |
                                       |                      |              |
             +------------+---------+  +--+-----IAddr----+    |              |
             |            |         |     |              |    |              |
             |            |       +-v-----v-+            |    |              |
             |          DAddr     | 1     0 |<-----------|----| IorDSel      |
             |            |        \ MuxID /             |    |              |
             |            |          +-v-+               |    |              |
             |            |            |                 |    |              |
             |            v        IorDAddr              |    |              |
             |                                <----------|----| RAMWrite     |
             | +--Dout->                      <----------|----| RAMRead      |
             | |                     IorD                |    |              |
             | |                       |                 |    |              |
             | |      +----------------+                 |    |              |
             | |      |                |                 |    |              |
             | |      | +----+----+----v----+----+----+  |    |              |
             | |      | | Fu : De : Rd : Rt : Rs : Op |<-|----| IRWrite      |
             | |      | +--v-+--v-+--v-+--v-+--v-+--v-+  |    |              |
             | |      |    |    |    |    |    |    |    |    |              |
             | |    +-|----+----+----+  +-+    |    +----|--->|              |
             | |    | +------+       |  | |    |         |    |              |
             +-|----|-----+  |   +---|--|-|----|---------|----| MemToReg     |
             | |    |    +v--v+  1  +v--v+|    |         |    |              |
             | |    |    |0  1|<-+  |1  0|<----|---------|----| RegDest      |
             | |    |     \  /       \  / |    |         |    |              |
             | |    |      \/         \/  |    |         |    |              |
             | |    |       |          |  |    |         |    |              |
             | |    |   +---v----------v--v----v------+  |    |              |
             | |    |   |   E       DirE DirB DirA    |  |    |              |
             | |  +-v-+ |                             |  |    |              |
             | |  |Ext| |           RegFile     Write |<-|----| RegFileWrite |
             | |  |Sig| |                             |  |    |              |
             | |  +-v-+ |     B                A      |  |    |              |
             | |    |   +-----v----------------v------+  |    |              |
             | |    +---+     |                |         |    |              |
             | +----|---|-----+                +-+     +-+    |              |
             |   +--v-+ |     |                  |     |      |              |
             |   | x4 | |     |               +--v-----v--+   |              |
             |   +--v-+ |  4  |               |  1     0  |<--| ALUSelA      |
             |      |   |  |  |                \   MuxA  /    |              |
             |    +-v---v--v--v-+                +--v--+      |              |
             |    | 11 10 01 00 |<------------------|---------| ALUSelB      |
             |     \   MuxB    /                    |         |              |
             |       +---v---+                      |         |              |
             |           |                          |         |              |
             |           +--+                    +--+         |              |
             |              |                    |            |              |
             |       +------v-----+        +-----v------+     |              |
             |        \            \      /            /      |              |
             |         \            \    /            /       |              |
             |          \            \  /            /        |              |
             |           \            \/            /         |              |
             |            \                     Op /<---------| ALUOp        |
             |             \                      /           |              |
             |              \        ALU         /            |              |
             |               \             zero /------------>| ALUzero      |
             |                \                /              |              |
             |                 +--------------+               |              |
             |                         |                      |              |
             +--------ALUOutput--------+                      |              |
             |                         |                      |              |
             |          +--------------v--------------+       |              |
             |          |            TARGET           |<------| TARGETWrite  |
             |          +-----------------------------+       |              |
             |                         |                      |              |
             +---------------+         |                      |              |
     
 */

class R2000 {
  REG_32 PC;  /* Contador de programa */
  REG_32 OPC; /* PC auxiliar ... quitar ? */
  REG_32 IR;  /* Registro de instrucci'on */
  REG_32 TARGET;  /* Registro */
  R2000_UC uc;    /* Unidad de control */
  MUX_2_1 mux_SRAM; /* Multiplexor de entrada a la SRAM */
  MUX_2_1 mux_data_in_FR;  /* Multiplexor de datos de entrada del FR */
  MUX_2_1 mux_addr_FR[5]; /* Multiplexores de direcci'on del FR */
  MUX_2_1 mux_data_out_A_FR[32]; /* Multiplexor de datos de salida A del FR */
  MUX_4_1 mux_data_out_B_FR[32]; /* Multiplexor de datos de dalida B del FR */
  ALU_CONTROL alU_ctrl;          /* Control de la ALU */
  ALU_32_RC alu;                 /* ALU */
 public:
  void run(WIRE in_ram[32],       /* Bus de datos de entrada desde la RAM */
           WIRE out_ram[32],      /* Bus de datos de salida hacia la RAM */
           WIRE addr_in_ram[32],  /* Bus de direcci'on de datos de entrada desde la RAM */
           WIRE addr_out_ram[32], /* Bus de direcci'on de datos de salida hacia la RAM */
           WIRE &ram_read,        /* Orden de lectura de la RAM */
           WIRE &ram_write,       /* Orden de escritura de la RAM */
           WIRW &clk);            /* Se~nal de reloj */
};
           
