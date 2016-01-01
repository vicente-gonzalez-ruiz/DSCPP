/*
 * monostable.h -- Monoestable programable.
 * gse. 1999.
 */

class MONOSTABLE {
  int time;         /* Para contar el tiempo */
  int periodo;      /* Perido del monoestable */
  int t_inestable;  /* Duraci'on del nivel inestable */
 public:
  MONOSTABLE(int t_inestable, int periodo);
  void run(WIRE &io);
};
