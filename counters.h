/*
 * counters.h -- Bilbioteca de contadores.
 * Ultima modificaci'on: 6-11-00.
 * gse.
 */

/* Simbolog'ia utilizada:
 *
 * {A,S}{U,D}_??...?_COUNTER
 *  ---  ---  ------
 *   |    |      |
 *   |    |      +-- Tipo de contador: xBIN (Binario de x bits)
 *   |    |                            BCD (Binary Coded Decimal).
 *   |    +--------- Up/Down (Ascendente/Descendente)
 *   +-------------- Asynchronous/Synchronous (As'incrono/S'incrono).
 */

/*
 * Contador de 4 bits, binario, as'incrono y ascendente.
 *
 *             Q0           Q1           Q2           Q3
 *     +-----+  |   +-----+  |   +-----+  |   +-----+  |
 *   1-|J    |--+ 1-|J    |--+ 1-|J    |--+ 1-|J    |--+
 * ck-O|>    |  +--O|>    |  +--O|>    |  +--O|>    |
 *   1-|K    |O-  1-|K    |O-  1-|K    |O-  1-|K    |O-
 *     +-----+      +-----+      +-----+      +-----+
 */
class AU_4BIN_COUNTER {
  NEG_JK_FF f0,f1,f2,f3;
  WIRE Q0n,Q1n,Q2n,Q3n;
public:
  void run(WIRE &ck, WIRE &Q0, WIRE &Q1, WIRE &Q2, WIRE &Q3);
};

/* 
 * Contador de 8 bits, binario, as'incrono y ascendente,
 * formado por dos contadores de 4 bits en cascada.
 *
 *      +---------+ Q3  +---------+ Q7
 *      | AU_4BIN |--+  | AU_4BIN |---
 * ck -O| COUNTER |  +-O| COUNTER |
 *      |   co0   |     |   co1   |
 *      +---------+     +---------+
 *
 */
class AU_8BIN_COUNTER {
  AU_4BIN_COUNTER co0,co1;
 public:
  void run(WIRE &ck,
           WIRE &Q0, WIRE &Q1, WIRE &Q2, WIRE &Q3,
           WIRE &Q4, WIRE &Q5, WIRE &Q6, WIRE &Q7);
};

/*
 * Contador de 4 bits, binario, as'incrono y descendente.
 *
 *            Q0           Q1            Q2          Q3
 *     +-----+ |    +-----+ |    +-----+ |    +-----+ |
 *   1-|J    |-+  1-|J    |-+  1-|J    |-+  1-|J    |-+
 * ck-O|>    |  +--O|>    |  +--O|>    |  +--O|>    |
 *   1-|K    |O-+ 1-|K    |O-+ 1-|K    |O-+ 1-|K    |O-
 *     +-----+      +-----+      +-----+      +-----+
 */
class AD_4BIN_COUNTER {
  NEG_JK_FF f0,f1,f2,f3;
  WIRE Q0n,Q1n,Q2n,Q3n;
 public:
  void run(WIRE &ck, WIRE &Q0, WIRE &Q1, WIRE &Q2, WIRE &Q3);
};

/*
 * Contador BCD, as'incrono y ascendente.
 *
 *            Q0           Q1            Q2           Q3   +----+ 
 *             |            |            |             +---|NAND|-+
 *             +------------|------------|-------------|---|    | |
 *             |  +---------|------------|-------------|-+ +----+ |
 *     +-----+ |  | +-----+ |    +-----+ |     +-----+ | |        |
 *   1-|J    |-+  +-|J    |-+  1-|J    |-+   1-|J    |-+ |        |
 * ck-O|>    |  +--O|>    |  +--O|>    |    +-O|>    |   |        |
 *   1-|K    |O-+ 1-|K    |O-+ 1-|K    |O-+ |1-|K    |O--+        |
 *     +-----+      +-----+      +-----+  | |  +-----+            |
 *                                      +-+ |a                    |
 *                                      | +----+                  |
 *                                      | |NAND|                  |
 *                                      | +----+                  |
 *                                      |  |  |         b         |
 *                                      +--+  +-------------------+
 */
class AU_BCD_COUNTER {
  NEG_JK_FF f0,f1,f2,f3;
  NAND na0,na1;
  WIRE Q0n,Q1n,Q2n,Q3n,a,b;
 public:
  void run(WIRE &ck, WIRE &Q0, WIRE &Q1, WIRE &Q2, WIRE &Q3);
};

/*
 * Contador BCD, as'incrono y ascendente, que utiliza flip-flops
 * con entrada de CLEAR para truncar la secuencia de cuenteo natural
 * al llegar al d'ecimo estado.
 *                                                      Q1+----+ 
 *                           +----------------------------|NAND|-+
 *                           |                         +--|    | |
 *                           |                         |Q3+----+ |
 *     +-----+Q0    +-----+  |   +-----+ Q2   +-----+  |         |
 *   1-|J    |--+ 1-|J    |--+ 1-|J    |--+ 1-|J    |--+         |
 * ck-O|>    |  +--O|>    |  +--O|>    |  +--O|>    |            |
 *   1-|K    |O-  1-|K    |O-  1-|K    |O-  1-|K    |O-          |
 *     +-----+      +-----+      +-----+      +-----+            |
 *        O            O            O            O               |
 *        |CLR         |CLR         |CLR         |CLR            |
 *        +------------+------------+------------+---------------+
 *
 * Fig. 9-6. Fundamentos de Sistemas Digitales.
 * T. L. Floyd.
 *
 */
class AU_BCD_COUNTER_CLR {
  NEG_JK_FF_PC f0,f1,f2,f3;
  NAND na;
  WIRE Q0n,Q1n,Q2n,Q3n,CLR;
  /* Circuito retardo para incrementar la duraci'on del pulso de borrado.
   * Sin este circuito, el contador no funciona.
   *
   * CLR +-----+ w0 +-----+ w1  
   * --+-+ NOT +----+ NOT +---+
   *   | | n0  |    | n1  |   | +-----+ newCLR
   *   | +-----+    +-----+   +-+ AND +-------
   *   +------------------------+ an  |
   *                            +-----+
   */
  NOT n0,n1;
  AND an;
  WIRE w0,w1,newCLR;
 public:
  void run(WIRE &ck, WIRE &Q0, WIRE &Q1, WIRE &Q2, WIRE &Q3);
};

/*
 * Celda b'asica del contador s'incrono binario ascendente.
 *
 * Un contador binario s'incrono ascendente puede ser construido
 * como un array de celdas b'asicas como la que a continuaci'on
 * es presentada.
 *
 *      +---+
 *  a --|AND|   +----------- c
 *  b --|and|-+-+        +-- d
 *      +---+ |          |
 *            |  +-----+ |
 *            +--|J    |-+
 * ck --------|-O|> ff |---- ck
 *            +--|K    |O
 *               +-----+ 
 *
 */
class BC_SU_BIN_COUNTER {
  NEG_JK_FF ff;
  AND y;
  WIRE Qn;
 public:
  void run(WIRE &ck, WIRE &a, WIRE &b, WIRE &c, WIRE &d);
};

/*
 * Contador s'incrono ascendente de 4 bits.
 *
 *           Q0  a[0]  Q1        Q2         Q3
 *            |+---+    |         |          |
 *    +-----+ |v+-----+ | +-----+ | +-----+  |
 * 1 -|a   c|-|-|a   c|-|-|a   c|-|-|a   c|- |
 * 1 -|b   d|-+-|b   d|-+-|b   d|-+-|b   d|--+
 *    +-----+   +-----+   +-----+   +-----+
 *       |         |         |         |
 * ck ---+---------+---------+---------+
 *
 */
class SU_4BIN_COUNTER {
  BC_SU_BIN_COUNTER c[4];
  WIRE a[4];             // Conecta las l'ineas "c" del BC_SU_BIN_COUNTER
                         // El resto de l'ineas son de entrada o de salida
 public:
  void run(WIRE &ck, WIRE Q[4]);
};

/*
 * Contador s'incrono ascendente de 8 bits.
 *
 */
class SU_8BIN_COUNTER {
  BC_SU_BIN_COUNTER c[8];
  WIRE a[8];
 public:
  void run(WIRE &ck, WIRE Q[8]);
};

/*
 * Contador s'incrono ascendente de N bits.
 */
class SU_BIN_COUNTER {
  BC_SU_BIN_COUNTER *c;         // Array de celdas b'asicas
  WIRE *a;                      // Array de conexiones internas
  int size;                     // Tama~no del contador (no hardware)
 public:
  void create(int size);        // Asigna memoria a los dispositivos internos
  void destroy();               // Libera la memoria
  void run(WIRE &ck, WIRE Q[]); // Simula el contador
}; 
