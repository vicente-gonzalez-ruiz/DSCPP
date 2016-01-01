/*
 * decods.h -- Descodificadores para SDLC++.
 * Ultima modificaci'on: 19-10-2000.
 * gse.
 */

/* Descodificador 2:4
 *
 * in[1] in[0]   nin[1] nin[0]
 *   |     |        |      |
 *   +-----|-not[1]-+      |
 *   |     +-not[0]-|------+
 *   |     |        |      | +---+
 *   |     |        +------|-|and|___ out[0] = nin[1] & nin[0]
 *   |     |        |      +-|[0]|
 *   |     |        |      | +---+
 *   |     |        |      | +---+
 *   |     +--------|------|-|and|___ out[1] = nin[1] & in[0]
 *   |     |        +------|-|[1]|
 *   |     |        |      | +---+
 *   |     |        |      | +---+
 *   +-----|--------|------|-|and|___ out[2] = in[1] & nin[0]
 *   |     |        |      +-|[2]|
 *   |     |        |      | +---+
 *   |     |        |      | +---+
 *   +-----|--------|------|-|and|___ out[3] = in[1] & in[0]
 *   |     +--------|------|-|[3]|
 *   |     |        |      | +---+
 */

class DECOD_2_4 {
  WIRE nin[2];
  NOT no[2];
  AND y[4];
public:
 void run(WIRE in[2], WIRE out[4]);
};

/*   in[4-0]       nin[4-0]
 *  | | | | |     | | | | |
 *  | | | | +-NOT-|-|-|-|-+
 *  | | | +-|-NOT-|-|-|-+ |
 *  | | +-|-|-NOT-|-|-+ | |
 *  | +-|-|-|-NOT-|-+ | | |
 *  +-|-|-|-|-NOT-+ | | | |
 *  | | | | |     +-|-|-|-|-A
 *  | | | | |     | +-|-|-|-N
 *  | | | | |     | | +-|-|-D--- out[0]
 *  | | | | |     | | | +-|-[0
 *  | | | | |     | | | | +-]
 *  : : : : :     : : : : :
 *  | | | | |
 *  +-|-|-|-|---------------A
 *    +-|-|-|---------------N
 *      +-|-|---------------D--- out[31]
 *        +-|---------------[31
 *          +---------------]
 */
class DECOD_5_32 {
  WIRE nin[5];
  NOT no[5];
  AND y[32];
 public:
  void run(WIRE in[5], WIRE out[32]);
};

/*
 * Descodificador 2:4 con entrada de selecci'on.
 * Se trata de un descodificador normal que en cada salida tiene una AND
 * cuyas entradas son una de las salidas del descodificador normal y la
 * entrada de selecci'on. Este tipo de descodificador es muy 'util en la
 * construcci'on de memorias.
 */
class DECOD_2_4_SEL {
  DECOD_2_4 dec;
  AND y[4];
  WIRE o[4];
 public:
  void run(WIRE in[2], WIRE &select, WIRE out[4]);
};

class DECOD_5_32_SEL {
  DECOD_5_32 dec;
  AND y[32];
  WIRE o[32];
 public:
  void run(WIRE in[5], WIRE &select, WIRE out[32]);
};

/*
 * Descodificador de tama~no variable.
 */
class DECOD {
  WIRE *nin;
  NOT *no;
  AND *y;
  int num_sels; // N'umero de entradas de selecci'on (no hardware)
  int num_outs; /* N'umero de salidas del descodificador */
  WIRE *aux;    // Array auxiliar que no forma parte de la descripci'on
                // real del descodificador (no hardware). Se usa para
 public:
  DECOD();      /* Declara un descodificador pero no asigna memoria.
		   Se usa cuando es necesario declarar el
		   descodificador y en tiempo de compilaci'on no se
		   conoce su tama~no. */
  DECOD(int num_sels);  /* Declara el descodificador de tama~no
			   variable. Esta declaraci'on se usa cuando
			   su tama~no es conocido en tiempo de
			   compilaci'on. */
  ~DECOD();
  void create(int num_sels);     // Asigna memoria a los componentes internos
  void destroy();                // Libera la memoria
  void run(WIRE *in, WIRE *out); // Simula el descodificador
};
