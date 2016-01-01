/*
 * regs.h -- Registros.
 * Ultima modificaci'on: 2-11-00.
 * gse.
 */

/*
 * Un registro de 4 bits construido con 4 flip-flops D.
 * La l'inea ck provoca que el registro se carge con el contenido
 * presentado a trav'es del bus de entrada i[], cuando se produce
 * un flanco negativo.
 *
 *   i[0]        i[1]        i[2]        i[3]
 *   |           |           |           |
 *   |  +-----+  |  +-----+  |  +-----+  |  +-----+
 *   +--|D    |O +--|D    |O +--|D    |O +--|D    |O
 *  ck-O|>    |----O|>    |----O|>    |----O|>    |
 *      |     |--+  |     |--+  |     |--+  |     |--+
 *      +-----+  |  +-----+  |  +-----+  |  +-----+  |
 *               |           |           |           |
 *               o[0]        o[1]        o[2]        o[3]
 */
class REG_4 {
  NEG_D_FF ff[4];
  WIRE Qn[4];
 public:
  void run(WIRE &ck, WIRE i[4], WIRE o[4]);
};

/* Registro de 4 bits construido con 4 flip-flops D con entradas de
 * preset y clear.
 */
class REG_4_PC {
  NEG_D_FF_PC ff[4];
  WIRE Qn[4];
 public:
  void run(WIRE &ck, WIRE i[4], WIRE o[4], WIRE &preset, WIRE &clear);
};

/*
 * Un registro de 8 bits construido con 2 registros de 4 bits.
 *
 *       i[0..3]       i[4..7]
 *          |             |
 *      +-------+     +-------+
 *      | REG_4 |     | REG_4 |
 *  ck-O|       |-ck-O|       |
 *      | reg[0]|     | reg[1]|
 *      +-------+     +-------+
 *          |             |
 *       o[0..3]       o[4..7]
 *
 */
class REG_8 {
  REG_4 reg[2];
 public:
  void run(WIRE &ck, WIRE i[8], WIRE o[8]);
};

/*
 * Un registro de 16 bits construido con 2 registros de 8 bits.
 */
class REG_16 {
  REG_8 reg[2];
 public:
  void run(WIRE &ck, WIRE i[16], WIRE o[16]);
};

/*
 * Un registro de 32 bits construido con 2 registros de 16 bits.
 */
class REG_32 {
  REG_16 reg[2];
 public:
  void run(WIRE &ck, WIRE i[32], WIRE o[32]);
};

class REG {
  NEG_D_FF *ff;
  WIRE *Qn;
  int num_bits;
  
 public:
  REG(int num_bits);
  ~REG();
  void run(WIRE &ck, WIRE i[], WIRE o[]);
};
