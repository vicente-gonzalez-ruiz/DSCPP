/*
 * alu32.h -- Unidad Aritm'etico-l'ogica de 32 bits.
 * Ultima modificaci'on: 14-11-00.
 * gse.
 */

/*
 * Sumador de 1 bit.
 *
 *             carry_in
 *                |                          carry_in a b |sum carry_out
 *                |                          -------------+-------------
 *                |  +---+                          0 0 0 |  0 0
 *    a ----+-----|--|AND|____t0                    0 0 1 |  1 0
 *          |     +--|a0 |    |                     0 1 0 |  1 0
 *          |     |  +---+    |  +---+              0 1 1 |  0 1
 *          |     |  +---+ t1 +--|OR |              1 0 0 |  1 0
 *    b ----|--+--|--|AND|__-----| o |---+          1 0 1 |  0 1
 *          |  |  +--|a1 |    +--|   |   |          1 1 0 |  0 1
 *          |  |  |  +---+    |  +---+   |          1 1 1 |  1 1
 *          |  |  |  +---+    |          |
 *          |  +--|--|AND|____|          |
 *          +--|--|--|a2 |    t2         |
 *          |  |  |  +---+               |
 *          |  |  |  +---+               |
 *          |  +--|--|XOR|____           |
 *          +-----|--|x0 | t3 | +---+    |
 *                |  +---+    +-|XOR|____|____ sum
 *                +-------------|x1 |    |
 *                              +---+    |
 *                                   carry_out
 */
class ADDER_BIT {
  WIRE t[4];
  AND a0,a1,a2;
  XOR x0,x1;
  OR o;
  
 public:
  void run(WIRE &a, WIRE &b,     // Entradas de datos
           WIRE &carry_in,       // Acarreo de entrada
           WIRE &sum,            // Resultado de la suma
           WIRE &carry_out);     // Acarreo de salida
};

/*
 * ALU de 1 bit.
 *
 *      binvert                carry_in       op
 *         |                       |          |
 * less ---|-----------------------|--+       |
 *         |                +---+  |  |       /2
 *    a ---|-------------+--|AND|__|__|__     |
 *         |          +--|--|and|  |  |  |    |
 *         |          |  |  +---+  |  |  |  +-+-
 *         |          |  |  +---+  |  |  +--|i0  \
 *         |          |  +--|OR |__|__|_____|i1   \
 *         |          +--|--| or|  |  |     |  mux +- res
 *         |          |  |  +---+  |  |  +--|i2   /
 *         |          |  |    +----+  +--|--|i3  /
 *         |          |  |    |          |  +---
 *         |          |  |  +---+        |
 *         |  +---+   |  +--| + |________|   less=i3
 *         +--|XOR|___+-----|sum|  
 *    b ------|xor|    t    +---+
 *            +---+           |
 *                        carry_out
 *  binvert op | res
 *  -----------+------
 *        0 00 | a and b
 *        0 01 | a  or b
 *        0 10 | a add b
 *        0 11 | less
 *        1 00 | a and (not b)
 *        1 01 | a  or (not b)
 *        1 10 | a sub b (si carry_in=1)
 *        1 11 | less
 */
class ALU_BIT {
  WIRE t,               // Salida de la puerta XOR
    i[4];               // Entradas del multiplexor
  XOR x;
  OR o;
  AND y;
  ADDER_BIT sum;
  MUX_4_1 mux;

public:
  void run(WIRE &a, WIRE &b,    // Bits de datos
           WIRE &binvert,       // Complemento de la l'inea "b"
           WIRE &carry_in,      // Acarreo de entrada
           WIRE op[2],          // Operaci'on de la ALU
           WIRE &less,          // Entrada directa a la salida
           WIRE &res,           // Resultado de la ALU
           WIRE &carry_out);    // Acarreo de salida
};

/*
 * ALU-MSb de 1 bit.
 *
 *      binvert                carry_in       op
 *         |                       |          |
 * less ---|-----------------------|--+       |
 *         |                +---+  |  |       /2
 *    a ---|-------------+--|AND|__|__|__     |
 *         |          +--|--|and|  |  |  |    |
 *         |          |  |  +---+  |  |  |  +-+-
 *         |          |  |  +---+  |  |  +--|i0  \
 *         |          |  +--|OR |__|__|_____|i1   \
 *         |          +--|--| or|  |  |     |  mux +-------- res
 *         |          |  |  +---+  |  |  +--|i2   /
 *         |          |  |    +----+  +--|--|i3  /
 *         |          |  |    |    |     |  +---
 *         |          |  |  +---+  |     |        +---+
 *         |  +---+   |  +--| + |__|_____+--------|XOR|_____ set
 *         +--|XOR|___+-----|sum|  |           +--| x1|
 *    b ------|xor|    t    +---+  |    +---+  |  +---+
 *            +---+           |    +----|XOR|__|____________ overflow
 *                            +---------| x0|
 *                            |         +---+
 *                        carry_out
 */
class ALU_MSBIT {
  WIRE t,               // Salida de la puerta XOR
    i[4];               // Entradas del multiplexor
  XOR x;
  OR o;
  AND y;
  ADDER_BIT sum;
  MUX_4_1 mux;
  XOR x0,x1;

public:
  void run(WIRE &a, WIRE &b,
           WIRE &binvert,
           WIRE &carry_in,
           WIRE op[2],
           WIRE &less,
           WIRE &res,
           WIRE &carry_out,
           WIRE &set,
           WIRE &overflow);
};

/*
 * ALU de 32 bits ripple-carry.
 *
 *      bnegate                                     op[2]
 *         |                                          |
 *         +------+--------------+           +--------+
 *         |      |              |           |        |
 *         | +----+--------------+-----------+------+ |
 *         | | binvert      carry_in        op      | |
 *         | |                                      | |
 *       +---+ less                                 | |
 *       | | |                ALU[0]                | |
 *  a[0]-----+ a                                res +----+------ res[0]
 *       | | |                                      | |  |
 *  b[0]-----+ b                                    | |  |
 *       | | |               carry_out              | |  |
 *       | | +-------------------+------------------+ |  |
 *       | |                     |                    |  |
 *       | +------+              | c[0]      +--------+  |
 *       | |      |              |           |        |  |
 *       | | +----+--------------+-----------+------+ |  |
 *       | | | binvert      carry_in        op      | |  |
 *       | | |                                      | |  |
 *    0 -----+ less                                 | |  |
 *       | | |                ALU[1]                | |  |
 *  a[1]-----+ a                                res +-----+----- res[1]
 *       | | |                                      | |  ||
 *  b[1]-----+ b                                    | |  ||
 *       | | |               carry_out              | |  ||
 *       | | +-------------------+------------------+ |  ||
 *       : :                     : c[1]               :  ::
 *       | +------+              |           +--------+  ||
 *       |        |              | c[30]     |           ||            
 *       |   +----+--------------+-----------+------+    ||
 *       |   | binvert      carry_in        op      |    ||
 *       |   |                                      |    ||
 *    0 -----+ less                             set +-+  ||
 *       |   |                ALU[31]               | |  ||
 * a[31]-----+ a                                res +--------+-- res[31]
 *       |   |                                      | |  ||..|
 * b[31]-----+ b                           overflow +----------- overflow
 *       |   |               carry_out              | |  ||..|
 *       |   +-------------------+------------------+ |  ||..|
 *       |less                   |                    |  ||..|
 *       +--------------------------------------------+  ||..|
 *                               |                       ||..|
 *                           carry_out                 +-++..+-+
 *                                                     |  NOR  |
 *                                                     |  zero |
 *                                                     +---+---+
 *                                                         |
 *                                                       zero
 *        bnegate oper | output
 *        -------------+----------
 *            * 0   00 | A and B
 *            * 0   01 | A or B
 *            * 0   10 | A add B
 *              0   11 | 000...0A donde A = overflow XOR sum[MSb]
 *              1   00 | A and (not B)
 *              1   01 | A or (not B)
 *            * 1   10 | A sub B = A add ((not B) + 1)
 *            * 1   11 | (A<B)
 *
 *            * -> Operaciones normalmente 'utiles
 */

class ALU_32BIT_RC {
  WIRE c[31];                // Bits de acarreo entre ALU's de 1 bit
  WIRE less;                 // Bit de realimentaci'on del MSb al LSb
  ALU_BIT alu[31];           // ALU's menos significativas
  ALU_MSBIT alu31;           // La ALU m'as significativa
  NOR no;                    // C'alculo del zero
 public:
  void run(WIRE a[32], WIRE b[32],    // Buses de datos de entrada
           WIRE &bnegate, WIRE op[2], // Entradas de control de operaci'on
           WIRE res[32],              // Bus de datos de salida
           WIRE &overflow,            // Overflow en la operaci'on
           WIRE &carry_out,           // Acarreo de salida de la ALU
           WIRE &zero);               // El resultado es cero
};

/*******************************************************************
 * Jose Domingo Alvarez Hervas.
 * Laboratorio de Arquitectura: 1999-2000.
 * ALU de 32 bits con Carry-Look-Ahead.
 */

class ADDER_BIT_CLA {
  WIRE t;
  XOR x0,x1;
  
 public:
  void run(WIRE &a, WIRE &b,     // Entradas de datos
           WIRE &carry_in,       // Acarreo de entrada
           WIRE &sum);           // Resultado de la suma
};

class ALU_BIT_CLA {
  WIRE t,               // Salida de la puerta XOR
    i[4];               // Entradas del multiplexor
  XOR x;
  OR o;
  AND y;
  ADDER_BIT_CLA sum;
  MUX_4_1 mux;

public:
  void run(WIRE &a, WIRE &b,    // Bits de datos
           WIRE &binvert,       // Complemento de la l'inea "b"
           WIRE &carry_in,      // Acarreo de entrada
           WIRE op[2],          // Operaci'on de la ALU
           WIRE &less,          // Entrada directa a la salida
           WIRE &res);          // Resultado de la ALU
};

class ALU_32BIT_CLA {
  XOR xoraux[31];            // Con esto se si utilizo b o b negada para calcular los gi y pi
  WIRE salida[31];           // Salida de las puestas anteriores
  AND andg[31];              // Para calcular los gi
  WIRE g[31];                // Las lineas que llevan los gi    
  OR orp[31];                // Para calcular los pi
  WIRE p[31];                // Las lineas que llevan los pi
  AND andaux[496];           // Puertas AND auxiliares para calcular los carry
  WIRE aux[496];             // Salidas de las puertas AND
  OR oraux[31];              // Puertas OR auxiliares para calcular los carry
  WIRE c[31];                // Lineas de salida de las puertas OR anteriores
  WIRE less;                 // Bit de realimentaci'on del MSb al LSb
  ALU_BIT_CLA alu[31];       // ALU's menos significativas
  ALU_MSBIT alu31;           // La ALU m'as significativa
  NOR no;                    // C'alculo del zero

 public:
  void run(WIRE a[32], WIRE b[32],    // Buses de datos de entrada
           WIRE &bnegate, WIRE op[2], // Entradas de control de operaci'on
           WIRE res[32],              // Bus de datos de salida
           WIRE &overflow,            // Overflow en la operaci'on
           WIRE &carry_out,           // Acarreo de salida de la ALU
           WIRE &zero);               // El resultado es cero

};
