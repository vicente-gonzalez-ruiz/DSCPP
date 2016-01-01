/*
 * clock.h -- Generador de onda cuadrada de periodo ajustable.
 * gse. 1999.
 */

/* Como objeto */
class CLOCK {
  int duracion_del_semiperiodo;
 public:
  CLOCK(int duracion_del_semiperiodo);
  void run(int iteracion_de_simulacion,
	   WIRE &ck);
};

/* Como funci'on */
void Clock(int iteracion_de_simulacion,
           int duracion_del_semiperiodo,
           WIRE &senhal_de_reloj);
