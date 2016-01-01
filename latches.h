/*
 * latches.h -- Bilbioteca de cerrojos.
 * Ultima modificaci'on: 25-10-2000.
 * gse.
 */

/*
 * Dos puertas NOR realimentadas de la siguiente forma,
 * implementan un cerrojo SR:
 *
 *           +------+
 *     R ----+      |
 *           | NOR  +--+-- Q        
 *         +-+  n0  |  |         S R | Q Qn
 *         | +------+ /         -----+-----
 *          \       /            0 0 | Q Qn (estado anterior)
 *            \   /              0 1 | 0 1  (reset)
 *              X                1 0 | 1 0  (set)
 *            /   \              1 1 | 0 0  (estado indeseable)
 *          /       \
 *         | +------+ \
 *         +-+      |  |
 *           | NOR  +--+-- Qn
 *     S ----+  n1  |
 *           +------+
 */
class SR_LATCH_NOR {
  NOR n0,n1;
public:
  void run(WIRE &S, WIRE &R, WIRE &Q, WIRE &Qn);
};

/*
 * Tambi'en es posible dise~nar un cerrojo SR a partir de
 * puertas NAND.
 *
 *           +------+
 *     R ----+      |
 *           | NAND +--+-- Q
 *         +-+  n0  |  |         S R | Q Qn
 *         | +------+ /         -----+-----
 *          \       /            0 0 | 1 1  (estado indeseable)
 *            \   /              0 1 | 0 1  (reset)
 *              X                1 0 | 1 0  (set)
 *            /   \              1 1 | Q Qn (estado anterior)
 *          /       \
 *         | +------+ \
 *         +-+      |  |
 *           | NAND +--+-- Qn
 *     S ----+  n1  |
 *           +------+
 */
class SR_LATCH_NAND {
  NAND n0,n1;
public:
  void run(WIRE &S, WIRE &R, WIRE &Q, WIRE &Qn);
};

/*
 * Se implementa mediante un cerrojo SR-NOR haciendo S=D y R=~D.
 * Este circuito se utiliza muchas veces, adem'as de como elemento
 * de memoria, como circuito de retardo (delay), puesto que Q=D
 * un cierto tiempo despu'es.
 */
class D_LATCH {
  NOT n;
  SR_LATCH_NOR l;
  WIRE Dn;
public:
  void run(WIRE &D, WIRE &Q, WIRE &Qn);
};

/*
 * Cerrojo JK.
 * Se implementa con 4 puertas NAND colocadas de la siguiente forma:
 *
 *               a
 * J ---- NAND ---- NAND -+-- Q    J K | Q Qn
 *        | n0        \   |        ----+-----
 *        +------------\ /         0 0 | Q Qn
 *                      X          0 1 | 0 1
 *        +------------/ \         1 0 | 1 0
 *        | n1        /   |        1 1 | 1 1
 * K ---- NAND ---- NAND -+-- Qn
 *               b
 */
 class JK_LATCH {
  NAND na0,na1,na2,na3;
  WIRE a,b;
public:
  void run(WIRE &J, WIRE &K, WIRE &Q, WIRE &Qn);
};

/*
 * Cerrojo JK con entrada de habilitaci'on.
 * Id'entico al anterior salvo porque las puertas de entrada para J y K
 * poseen una tercera entrada a la que entra la se~nal de
 * habilitaci'on E.
 */
class JK_LATCH_ENABLE {
  NAND na0,na1;
  NAND na2,na3;
  WIRE a,b;
public:
  void run(WIRE &J, WIRE &K, WIRE &E, WIRE &Q, WIRE &Qn);
};

/*
 * Cerrojo SR con entrada de habilitaci'on.
 *
 *             a +------+
 *  S -- NAND ---|R     |---- Q
 *       |       |      |
 *  E ---+       |      |
 *       |     b |      |
 *  R -- NAND ---|S     |O--- Q'
 *               +------+
 */
class SR_LATCH_ENABLE {
  SR_LATCH_NAND latch;
  NAND na0,na1;
  WIRE a,b;
public:
  void run(WIRE &S, WIRE &R, WIRE &E, WIRE &Q, WIRE &Qn);
};
