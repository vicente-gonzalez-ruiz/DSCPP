/*
 * muxs.h -- Multiplexores y desmultiplexores.
 * Ultima modificaci'on: 19-10-2000.
 * gse.
 */

/*
 * Multiplexor de 2:1
 * Implementado mediante un decodificador de 1:2 y dos buffers
 * triestado.
 *
 *           sel
 *            |
 *          +---+
 *          |  NOT
 *          |   |
 *  in[0]---|-- B--+
 *          |      +-- out
 *  in[1]---B------+
 */
class MUX_2_1 {
  NOT no;
  BUFFER_Z buf[2];
  WIRE nsel;
 public:
  void run(WIRE in[2], WIRE &sel, WIRE &out);
};

/*
 * Desmultiplexor de 1:2
 *
 *     sel
 *      |
 *      +--NOT--A
 *      |       N-- out[0]
 *      |    +--D
 *  in--|----+
 *      |    +--A
 *      |       N-- out[1]
 *      +-------D
 */
class DEMUX_1_2 {
  NOT no;
  AND y[2];
  WIRE nsel;
 public:
  void run(WIRE &in, WIRE &sel, WIRE out[2]);
};

/* Mux 4:1
 *
 *          sel[2]
 *            |
 *        +-------+
 *      / DECOD_2_4 \ dec
 *     +-------------+
 *   d[4]|  |  |  |d[0]
 *       |  |  |  +  
 *       |  |  | | \ buf[4]
 *    -----------+  +---+
 *       |  |  | | /    |
 *       |  |  +  +     |
 *       |  | | \       |
 *    --------+  +------+
 *       |  | | /       |
 * in[4] |  +  +        +--- out
 *       | | \          |
 *    -----+  +---------+
 *       | | /          |
 *       +  +           |
 *      | \             |
 *    --+  +------------+
 *      | /
 *       +
 */

class MUX_4_1 {
  DECOD_2_4 dec;
  BUFFER_Z buf[4];
  WIRE d[4];

public:
  void run(WIRE in[4], WIRE sel[2], WIRE &out);
};

/*
 * Un desmultiplexor de 4:1 es un circuito similar en el que existe un
 * descodificador 2:4 y 4 puertas AND colocadas en las 4 salidas.
 * La entrada al desmultiplexor entra a cada una de las puertas.
 * Las 4 salidas del desmultiplexor son las 4 puertas AND.
 */
class DEMUX_1_4 {
  DECOD_2_4 dec;
  AND y[4];
  WIRE d[4];

 public:
  void run(WIRE &in, WIRE sel[2], WIRE out[4]);
};

/*
 * Multiplexor de tama~no variable.
 */
class MUX{
  DECOD decoder;
  BUFFER_Z *buffers;
  WIRE *outs_decoder;
  int num_ins;         // N'umero de entradas (no hardware)
 public:
  void create(int size);
  void destroy();
  void run(WIRE in[], WIRE sel[], WIRE &out);
};

/*
 * Desmultiplexor de tama~no variable.
 */
class DEMUX {
  DECOD decoder;
  AND *y;
  WIRE *out_decoder;
  int num_outs;

 public:
  DEMUX();
  DEMUX(int num_sels);
  void create(int num_sels);
  void destroy();
  void run(WIRE &in, WIRE sel[], WIRE out[]);
};
