/*
 * gates.h -- Bilbioteca de puertas l'ogicas.
 * Ultima modificaci'on: 23-11-00.
 * gse. 1999.
 */

#define WIRE unsigned
#define VCC 255
#define GND 0

class NOT {
  WIRE delay;
public:
  void run(WIRE &output, WIRE &input);
};

class AND {
  WIRE delay;
public:
  void run(WIRE &output, WIRE &input0, WIRE &input1);
  void run(WIRE &output, short num_inputs, WIRE &input, ...);
  void run(WIRE &output, short num_inputs, WIRE input[]);
};

class NAND {
  WIRE delay;
public:
  void run(WIRE &output, WIRE &input0, WIRE &input1);
  void run(WIRE &output, short num_inputs, WIRE &input, ...);
  void run(WIRE &output, short num_inputs, WIRE input[]);
};

class OR {
  WIRE delay;
public:
  void run(WIRE &output, WIRE &input0, WIRE &input1);
  void run(WIRE &output, short num_inputs, WIRE &input, ...);
  void run(WIRE &output, short num_inputs, WIRE input[]);
};

class NOR {
  WIRE delay;
public:
  void run(WIRE &output, WIRE &input0, WIRE &input1);
  void run(WIRE &output, short num_inputs, WIRE &input, ...);
  void run(WIRE &output, short num_inputs, WIRE input[]);
};

class XOR {
  WIRE delay;
public:
  void run(WIRE &output, WIRE &input0, WIRE &input1);
  void run(WIRE &output, short num_inputs, WIRE &input, ...);
  void run(WIRE &output, short num_inputs, WIRE input[]);
};

class NXOR {
  WIRE delay;
 public:
  void run(WIRE &output, WIRE &input0, WIRE &input1);
  void run(WIRE &output, short num_inputs, WIRE &input, ...);
  void run(WIRE &output, short num_inputs, WIRE input[]);
};

class BUFFER_Z {
  WIRE delay[2];
  int sample;
public:
  BUFFER_Z();
  void run(WIRE &output, WIRE &input, WIRE &enable);
};
