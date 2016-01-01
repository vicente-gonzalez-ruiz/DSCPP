/*
 * gates.h -- Bilbioteca de puertas l'ogicas.
 *
 * Este software de simulaci'on digital simula los circuitos como redes de
 * puertas l'ogicas. La precisi'on del modelo y el tiempo de simulaci'on va
 * a depender de la simulaci'on de las puertas y por lo tanto, existen
 * distintos niveles de aproximaci'on.
 *
 * En el nivel m'nimo, las puertas l'ogicas no simulan los tiempos de retardo,
 * fan-out, etc. Es el modelo m'as r'apido y el que se debe utilizar en
 * simulaciones muy grandes o muy largas, o si no se desea precisi'on del
 * simulador.
 *
 * gse. 1999.
 */

class NOT {
public:
  NOT();
  void run(WIRE &i, WIRE &o);
};

class AND {
public:
  AND();
  void run(WIRE &i0, WIRE &i1, WIRE &o);
};

class NAND {
public:
  NAND();
  void run(WIRE &i0, WIRE &i1, WIRE &o);
};

class NAND3 {
public:
  NAND3();
  void run (WIRE &i0, WIRE &i1, WIRE &i2, WIRE &o);
};

class NAND4 {
public:
  NAND4();
  void run (WIRE &i0, WIRE &i1, WIRE &i2, WIRE &i3, WIRE &o);
};

class OR {
public:
  OR();
  void run(WIRE &i0, WIRE &i1, WIRE &o);
};

class NOR {
public:
  NOR();
  void run(WIRE &i0, WIRE &i1, WIRE &o);
};

class XOR {
public:
  XOR();
  void run(WIRE &i0, WIRE &i1, WIRE &o);
};

class BUFFER_Z {
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
