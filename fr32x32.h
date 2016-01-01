/*
 * fr32x32.h -- Fichero de registros de 32x32 bits (R2000).
 * Ultima modoficaci'on: 13-11-00.
 * gse.
 */

/*
 * Celda b'asica del fichero de registros, basada en un flip-flop D.
 *
 *                       sel0 sel1                  |     |
 *                         |    |              +----v-----v---+
 *            +------+  +--|---BUF-- out1      |  sel0  sel1  |
 *      in ---+ D  Q +--+  |                 --> in      out1 +->
 *            |      |  +-BUF------- out0      |    CELDA     |
 *   write --O> ck   |                       -O> write   out0 +->
 *            +------+                         |    BASICA    |
 *                                             +--------------+
 */
class FR_BIT {
  WIRE q,qn;     // Salidas del flip-flop (aunque s'olo "q" se utiliza)
  NEG_D_FF f;    // El flip-flop
  BUFFER_Z b[2]; // Los buffers triestado
 public:
  void run(WIRE &in,                // Entrada de datos
           WIRE &sel0, WIRE &sel1,  // Selectores
           WIRE &write,             // Orden de escritura sobre el flip-flop
           WIRE &out0, WIRE &out1); // Salidas de la celda
};


/*
 * Celda b'asica del registro 0.
 * El registro 0 contiene siempre un 0. Por tanto, puede ser dise~nado
 * como un array de celdas vac'ias donde s'olo las dos salidas "out0"
 * y "out1" est'an conectadas a GND.
 */
class FR0_BIT {
  //BUFFER_Z b[2]; // Los buffers triestado
 public:
  void run(WIRE &in,                // Entrada de datos
           WIRE &sel0, WIRE &sel1,  // Selectores
           WIRE &write,             // Orden de escritura sobre el flip-flop
           WIRE &out0, WIRE &out1); // Salidas de la celda
};

/*
 * Fichero de registros de 32x32 bits.
 *
 *                 write in[0]             in[1]                 in[31]
 *                    |   |       s0[0]     |                     |
 *                +---|---|-------+---------|-------+--------...--|-------+
 *          /|  /   +-|---|----+--|---------|----+--|--------...--|----+  |
 *        / 0+-+   /  |   | +--+--+--+      | +--+--+--+    s1[0] | +--+--+--+
 *     5 |  1+-       |   | | Reg 0  +---+  | | Reg 0  +---+      | | Reg 0  +---+
 *    -/-+  :| \  /   |   +-+ Bit 0  |   |  +-+ Bit 1  |   |      +-+ Bit 31 |   |
 * addr0 |  :|  \     |   | |        +-+ |  | |        +-+ |      | |        +-+ |
 *        \31+-  X    +-A | +----+---+ | |  | +----+---+ | |      | +----+---+ | |
 *          \| \  \   | N-|------+-----|-|--|------+-----|-|-...--|------+     | |
 *              /  \ +|-D |wck[0] s0[1]| |  |            | |s1[1] |            | |
 *              \   +-|---|-------+----|-|--|-------+----|-|-...--|-------+    | |
 *          /| /  +-/-|---|----+--|----|-|--|----+--|----|-|-...--|----+  |    | |
 *        / 0+-  X    |   | +--+--+--+ | |  | +--+--+--+ | |      | +--+--+--+ | |
 *     5 |  1+-+   /  |   | | Reg 1  +-|-+  | | Reg 1  +-|-+      | | Reg 1  +-|-+
 *    -/-+  :|    \   |   +-+ Bit 0  | | |  +-+ Bit 1  | | |      +-+ Bit 31 | | |
 * addr1 |  :|    /   |   | |        +-+ |  | |        +-+ |      | |        +-+ |
 *        \31+-+   \  +-A | +----+---+ | |  | +----+---+ | |      | +----+---+ | |
 *          \|  \/    | N-|------+-----|-|--|------+-----|-|-...--|------+     | |
 *               \ +\-|-D |wck[1]      | |  |            | |      |            | |
 *          w[0]/ X   :   :     s0[31] : :  :            : :      :            : :
 *               / \ +|---|-------+----|-|--|-------+----|-|-...--|-------+    | |
 *          /| //   +-|---|----+--|----|-|--|----+--|----|-|-...--|----+  |    | |
 *        / 0+-/      |   | +--+--+--+ | |  | +--+--+--+ | |s1[31]| +--+--+--+ | |
 *     5 |  1+-w[1]   |   | | Reg 31 +-|-+  | | Reg 31 +-|-+      | | Reg 31 +-|-+
 *    -/-+  :|        |   +-+ Bit 0  | | |  +-+ Bit 1  | | |      +-+ Bit 31 | | |
 * addr2 |  :|        |     |        +-+ |    |        +-+ |        |        +-+ |
 *        \31+-+      +-A   +----+---+ | |    +----+---+ | |        +----+---+ | |
 *          \|   \      N--------+-----|-|---------+-----|-|-...---------+     | |
 *                 +----D wck[31]      | |               | |                   | |
 *               w[31]           out0[0] out1[0]   out0[1] out1[1]       out0[31]
 *                                                                        out1[31]
 *                     |       |
 *                +----+-------+------+
 *              5 |    ck    write    |
 *             -/-> addr0             | 32
 *             -/-> addr1        out0 +-/->
 *             -/-> addr2             | 32
 *             32 |              out1 +-/->
 *             -/-> in   FICHERO      |
 *                |      REGISTROS    |
 *                +-------------------+
 */
class FR_32x32 {
  FR0_BIT bit0[32];    // Registro 0
  FR_BIT bit[31][32];  // Array de celdas b'asicas
  DECOD_5_32 dec0;     // Decodificador 0 de direcci'on para lectura
  DECOD_5_32 dec1;     // Decodificador 1 de direcci'on para lectura
  DECOD_5_32 decw;     // Decodificador de direcci'on para escritura
  AND y[32];         // Puertas de habilitaci'on de escritura
  WIRE
    s0[32],            // Salidas del primer decodificador de lectura
    s1[32],            // Salidas del segundo decodificador de lectura
    w[32],             // Salidas del decodificador de escritura
    wck[32];           // Salidas de las puertas de habilitaci'on de escritura

 public:
  void run(WIRE in[32], WIRE addr0[5], WIRE addr1[5], WIRE addr2[5],
           WIRE &write, WIRE out0[32], WIRE out1[32]);
};
