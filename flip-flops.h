/*
 * flip-flips.h -- Bilbioteca de flip-flops.
 * Ultima modificaci'on: 30-10-00.
 * gse.
 */

/*
 * Detector de flancos positivos.
 * Genera un pulso positivo de duraci'on igual a tres puertas
 * NOT tras la llegada de un flanco positivo.
 *
 * Este dispositivo es muy usado en la construcci'on de
 * flip-flops a partir de cerrojos.
 *
 *    i   +----+ a0 +----+ a1 +----+ a2 +--
 * ---+---| n0 |----| n1 |----| n2 |----|   \  o
 *    |   +----+    +----+    +----+    | na |---
 *    +---------------------------------|   /
 *                                      +--
 */

class POSITIVE_DETECTOR {
  NOT n0,n1,n2; // Retardo + negaci'on
  AND an;
  WIRE a0,a1,a2;
 public:
  void run(WIRE &i, WIRE &o);
};

/*
 * Eliminador de "gliches" positivos.
 *
 *                            +-----+
 * a ---+---------------------|     |
 *      | +-----+   +-----+   | AND |---- d
 *      +-| NOT |---| NOT |---|     |
 *        +-----+ b +-----+ c +-----+
 *
 *          ++     +-----
 * a        ||     |
 *          ||     |
 *   -------++-----+
 *    -------++-----+
 *           ||     |
 * b         ||     |
 *           ++     +-----
 *            ++     +-----
 * c          ||     |
 *            ||     |
 *     -------++-----+
 *                    +-----
 *                    |
 * d                  |
 *      --------------+
 * La se~nal de salida est'a retarda 6 ciclos respecto de la de
 * entrada, pero no presenta "gliches" positivos.
 *
 * Este circuito no se usa para dise~nar flip-flops, aunque si debe
 * colocarse en se~nales s'incronas que presentes glitches.
 */
class POSITIVE_GLICH_SUPRESSOR {
  NOT no[2];
  AND y;
  WIRE b,c;
 public:
  void run(WIRE &a, WIRE &d);
};


/*
 * Eliminador de "gliches" negativos.
 *
 *                            +-----+
 * a ---+---------------------|     |
 *      | +-----+   +-----+   |  OR |---- d
 *      +-| NOT |---| NOT |---|     |
 *        +-----+ b +-----+ c +-----+
 *
 *   -------++-----+     
 * a        ||     |
 *          ||     |
 *          ++     +-----
 *           ++     +-----
 *           ||     |
 * b         ||     |
 *    -------++-----+-----
 *     -------++-----+-----
 * c          ||     |
 *            ||     |
 *            ++     +-----
 *      --------------+     
 *                    |
 * d                  |
 *                    +-----
 * La se~nal de salida est'a retarda 6 ciclos respecto de la de
 * entrada, pero no presenta "gliches" negativos.
 */
class NEGATIVE_GLICH_SUPRESSOR {
  NOT no[2];
  OR o;
  WIRE b,c;
 public:
  void run(WIRE &a, WIRE &d);
};


/*
 * Se construye negando la entrada a un detector de flancos positivo.
 *
 *      +-----+ ni +----------+
 *  i --| NOT |----| POSITIVE |-- o
 *      +-----+    | DETECTOR |
 *                 +----------+
 */
class NEGATIVE_DETECTOR {
  POSITIVE_DETECTOR pd;
  NOT no;
  WIRE ni;
 public:
  void run(WIRE &i, WIRE &o);
};

/*
 * Flip-flop SR disparado por flanco negativo.
 * Se trata de un cerrojo SR con entrada de habilitaci'on, que se
 * habilita s'olo durante el tiempo que dura el detector de flancos.
 *                     +-----+
 *      +----------+  -|S    |-- Q
 * ck --| NEGATIVE |---|ENAB |
 *      | DETECTOR |  -|R    |O- Qn
 *      +----------+   +-----+
 */
class NEG_SR_FF {
  NEGATIVE_DETECTOR detec;
  SR_LATCH_ENABLE latch;
  WIRE pulse;
public:
  void run(WIRE &S, WIRE &R, WIRE &ck, WIRE &Q, WIRE &Qn);
};

/*
 * Flip-flop D disparado por flanco negativo.
 *
 *          +-----+
 *  D --+---|S    |-- Q
 * ck --|--O|>    |
 *      +-O-|R    |O- Qn
 *          +-----+
 * (el circulo en medio de la l'inea representa una puerta NOT).
 */
class NEG_D_FF {
  NEG_SR_FF ff;
  NOT n;
  WIRE Dn;
 public:
  void run(WIRE &D, WIRE &ck, WIRE &Q, WIRE &Qn);
};

/*
 * Flip-flop JK disparado por flanco negativo.
 *
 *                     +-----+
 *      +----------+  -|J    |-- Q
 * ck --| NEGATIVE |---|ENAB |
 *      | DETECTOR |  -|K    |O- Qn
 *      +----------+   +-----+
 */
class NEG_JK_FF {
  NEGATIVE_DETECTOR dec;
  WIRE pulse;
  JK_LATCH_ENABLE latch_JK;
public:
  void run(WIRE &J, WIRE &K, WIRE &ck, WIRE &Q, WIRE &Qn);
};

/*
 * Flip-flop JK disparado por flanco negativo, con entradas de PReseT y
 * CLeaR activas en bajo.
 *
 *                         +-----------------------------------------+
 *                         |                PRT                      |
 *                         |+------------+   |                       |
 *                         ||  +------+  |   |   +------+            |
 *                         |+--|      |  | a +---|      |            |
 *                         +---| NAND |O-|---|---| NAND |O--+-+- Q   |
 *  J -------------------------| na0  |  |   / +-| na2  |  /  |      |
 *        +----------+      +--|      |  \  /  | +------+ /   |      |
 * ck ____| NEGATIVE |______|  +------+   \/  +|----------    |      |
 *        | DETECTOR | pulse|  +------+   /\  |+----------    |      |
 *        +----------+      +--|      |  /  \ |  +------+ \   |      |
 *  K -------------------------| NAND |  | b \+--|      |  \  |      |
 *                         +---| na1  |O-|---|---| NAND |O--+-|- Qn  |
 *                         |+--|      |  |   +---| na3  |   | |      |
 *                         ||  +------+  |   |   +------+   +-|------+
 *                         |+------------+   |                |
 *                         |                CLR               |
 *                         +----------------------------------+
 *
 * fig 8-26. Fundamentos de Sistemas Digitales.
 * T. L. Floyd.
 */
class NEG_JK_FF_PC {
  NEGATIVE_DETECTOR dec;
  NAND na0,na1,na2,na3;
  WIRE pulse,a,b;
public:
  void run(WIRE &J, WIRE &K, WIRE &ck, WIRE &PRT,
           WIRE &CLR, WIRE &Q, WIRE &Qn);
};

/*
 * Flip-flop D D, disparado por flanco y activo por flanco negativo.
 * Ha sido implementado usando un NEG_JK_FF_PC.
 *
 *                        preset
 *                          |
 *                          O
 *                       +-----+
 *       D--+------------|J    |-- Q
 *          |  +---+ ck-O|>    |
 *          +--|NOT|-----|K    |O- Qn
 *             +---+  Dn +-----+
 *                          O
 *                          |
 *                        clear
 */
class NEG_D_FF_PC {
  NEG_JK_FF_PC ff;
  NOT no;
  WIRE Dn;
 public:
  void run(WIRE &D, WIRE &ck, WIRE &Q, WIRE &Qn, WIRE &preset, WIRE &clear);
};

/*
 * Flip-flip SR maestro-esclavo, disparado por flanco negativo.
 *
 *           master     slave
 *          +-----+    +-----+
 *    S-----|S    |----|S    |-- Q
 *        +-|ENAB |  +-|ENAB |
 *    R---|-|R    |O-|-|R    |O- Qn
 *        | +-----+  | +-----+
 *        | +-----+  |
 *   ck---+-| NOT |--+
 *          +-----+
 */
class SR_FF_MS {
  SR_LATCH_ENABLE master,slave;
  NOT n;
  WIRE nck,q,qn;
public:
  void run(WIRE &S, WIRE &R, WIRE &ck, WIRE &Q, WIRE &Qn);
};

/* 
 * Flip-flop JK maestro-esclavo, disparado por flanco negativo.
 *
 *   +-----------------------+
 *   | +----+                |
 *   +-| AND| a +-------+    |
 *  J--|  a0|---|S    -+|--+-|- Q
 *     +----+   |      ||  | |
 *  ck----------|       |  | |
 *     +----+ b |     -+|  | |
 *  K--| AND|---|R     ||O-|-+- Qn
 *   +-|  A1|   +-------+  |
 *   | +----+              |
 *   +---------------------+
 */
class JK_FF_MS {
  SR_FF_MS ff;
  AND a0,a1;
  WIRE a,b;
public:
  void run (WIRE &J, WIRE &K, WIRE &ck, WIRE &Q, WIRE &Qn);
};
