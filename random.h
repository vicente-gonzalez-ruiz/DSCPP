/*
 * random.h -- Generador de bits aleatorios de cierta duraci'on.
 * gse. 1999.
 */

class RANDOM {
  int duracion_del_semiperiodo;
 public:
  RANDOM(int duracion_del_semiperiodo);
  void run(int iteracion_de_simulacion,
	   WIRE &ck);
};

void Random(int iterarion_de_simulacion,
            int duracion_de_la_senhal,
            WIRE &senhal);
