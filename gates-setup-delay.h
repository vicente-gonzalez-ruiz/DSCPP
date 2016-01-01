/*
 * gates.h -- Bilbioteca de puertas l'ogicas.
 *
 * Este software de simulaci'on digital simula los circuitos como redes de
 * puertas l'ogicas. La precisi'on del modelo y el tiempo de simulaci'on va
 * a depender de la simulaci'on de las puertas y por lo tanto, existen
 * distintos niveles de aproximaci'on.
 *
 * Considera tiempo de establecimiento y de propagaci'on.
 *
 * gse. 1999.
 */

class NOT {
  WIRE delay;
public:
  void run(WIRE &output, WIRE &input);
};

class AND {
  WIRE delay;
  short num_inputs;
public:
  AND(int num_inputs=2);
  //void run(WIRE &output, WIRE &input0, WIRE &input1);
  //void run(WIRE &output, int inputs, WIRE &inputs, ...);
  //void run(WIRE &output, int inputs, WIRE inputs[]);
  void run(WIRE &output, WIRE &input, ...);
  void run(WIRE &output, WIRE input[]);
};

#ifdef _1_
class NAND {
  WIRE delay;
public:
  void run(WIRE &output, WIRE &input0, WIRE &input1);
  void run(WIRE &output, int inputs, WIRE &inputs, ...);
  void run(WIRE &output, int inputs, WIRE inputs[]);
};

class OR {
  WIRE delay;
public:
  void run(WIRE &output, WIRE &input0, WIRE &input1);
  void run(WIRE &output, int inputs, WIRE &inputs, ...);
  void run(WIRE &output, int inputs, WIRE inputs[]);
};

class NOR {
  WIRE delay;
public:
  void run(WIRE &output, WIRE &input0, WIRE &input1);
  void run(WIRE &output, int inputs, WIRE &inputs, ...);
  void run(WIRE &output, int inputs, WIRE inputs[]);
};

class XOR {
  WIRE delay;
public:
  void run(WIRE &output, WIRE &input0, WIRE &input1);
  void run(WIRE &output, int inputs, WIRE &inputs, ...);
  void run(WIRE &output, int inputs, WIRE inputs[]);
};

class NXOR {
  WIRE delay;
 public:
  void run(WIRE &output, WIRE &input0, WIRE &input1);
  void run(WIRE &output, int inputs, WIRE &inputs, ...);
  void run(WIRE &output, int inputs, WIRE inputs[]);
};

class BUFFER_Z {
  WIRE delay[2];
  int sample;
public:
  BUFFER_Z();
  void run(WIRE &output, WIRE &input, WIRE &enable);
};
#endif
