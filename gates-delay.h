/*
 * gates.h -- Bilbioteca de puertas l'ogicas.
 *
 * Este software de simulaci'on digital simula los circuitos como redes de
 * puertas l'ogicas. La precisi'on del modelo y el tiempo de simulaci'on va
 * a depender de la simulaci'on de las puertas y por lo tanto, existen
 * distintos niveles de aproximaci'on.
 *
 * Considera los tiempos de propagaci'on de las puertas l'ogicas.
 *
 * gse. 1999.
 */

class NOT {
  WIRE delay;
public:
  NOT();
  void run(WIRE &i, WIRE &o);
};

class AND {
  WIRE delay;
public:
  AND();
  void run(WIRE &i0, WIRE &i1, WIRE &o);
};

class NAND {
  WIRE delay;
public:
  NAND();
  void run(WIRE &i0, WIRE &i1, WIRE &o);
};

class NAND3 {
  WIRE delay;
public:
  NAND3();
  void run (WIRE &i0, WIRE &i1, WIRE &i2, WIRE &o);
};

class NAND4 {
  WIRE delay;
public:
  NAND4();
  void run (WIRE &i0, WIRE &i1, WIRE &i2, WIRE &i3, WIRE &o);
};

class OR {
  WIRE delay;
public:
  OR();
  void run(WIRE &i0, WIRE &i1, WIRE &o);
};

class NOR {
  WIRE delay;
public:
  NOR();
  void run(WIRE &i0, WIRE &i1, WIRE &o);
};

class XOR {
  WIRE delay;
public:
  XOR();
  void run(WIRE &i0, WIRE &i1, WIRE &o);
};

class BUFFER_Z {
  WIRE delay;
public:
  BUFFER_Z();
  void run(WIRE &i, WIRE &e, WIRE &o);
};

class AND3 {
  AND a0,a1;
  WIRE x;
public:
  void run(WIRE &i0, WIRE &i1, WIRE &i2, WIRE &o);
};

class AND4 {
  AND3 a0;
  AND a1;
  WIRE x;
public:
  void run(WIRE &i0, WIRE &i1, WIRE &i2, WIRE &i3, WIRE &o);
};
