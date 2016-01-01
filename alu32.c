/*
 * alu32.c -- Implementaci'on de una ALU de 32 bits.
 * Ultima modificaci'on: 23-11-00.
 * gse.
 */

#include "gates.h"
#include "decods.h"
#include "muxs.h"
#include "alu32.h"

void ADDER_BIT::run(WIRE &a, WIRE &b, WIRE &carry_in,
		    WIRE &sum, WIRE &carry_out) {
  a0.run(t[0],a,carry_in);
  a1.run(t[1],b,carry_in);
  a2.run(t[2],a,b);
  o.run(carry_out,3,t);
  x0.run(t[3],a,b);
  x1.run(sum,t[3],carry_in);
}

void ALU_BIT::run(WIRE &a, WIRE &b,
                  WIRE &binvert, WIRE &carry_in, WIRE op[2],
                  WIRE &less, WIRE &res, WIRE &carry_out) {
  x.run(t,binvert,b);
  y.run(i[0],a,t);
  o.run(i[1],a,t);
  sum.run(a,t,carry_in,i[2],carry_out);
  i[3]=less;
  mux.run(i,op,res);
}

void ALU_MSBIT::run(WIRE &a, WIRE &b,
                    WIRE &binvert,
                    WIRE &carry_in,
                    WIRE op[2],
                    WIRE &less,
                    WIRE &res,
                    WIRE &carry_out,
                    WIRE &set,
                    WIRE &overflow) {
  x.run(t,binvert,b);
  y.run(i[0],a,t);
  o.run(i[1],a,t);
  sum.run(a,t,carry_in,i[2],carry_out);
  i[3]=less;
  mux.run(i,op,res);
  x0.run(overflow,carry_in,carry_out);
  x1.run(set,i[2],overflow);
}

void ALU_32BIT_RC::run(WIRE a[32], WIRE b[32],    // Buses de datos de entrada
                       WIRE &bnegate, WIRE op[2], // Entradas de control de operaci'on
                       WIRE res[32],              // Bus de datos de salida
                       WIRE &carry_out,           // Acarreo de salida de la ALU
                       WIRE &overflow,            // Overflow en la operaci'on
                       WIRE &zero) {              // El resultado es cero
  WIRE z;
  z=0;
  alu[0].run(a[0],b[0],bnegate,bnegate,op,less,res[0],c[0]);
  for(int j=1;j<31;j++) {
    alu[j].run(a[j],b[j],bnegate,c[j-1],op,z,res[j],c[j]);
  }
  alu31.run(a[31],b[31],bnegate,c[30],op,z,res[31],carry_out,less,overflow);
  no.run(zero,32,res);
}

/*******************************************************************
 * Jose Domingo Alvarez Hervas.
 * Laboratorio de Arquitectura: 1999-2000.
 * ALU de 32 bits con Carry-Look-Ahead.
 */

void ADDER_BIT_CLA::run(WIRE &a, WIRE &b, WIRE &carry_in, WIRE &sum) {
  x0.run(t,a,b);
  x1.run(sum,t,carry_in);
}

void ALU_BIT_CLA::run(WIRE &a, WIRE &b,
                  WIRE &binvert, WIRE &carry_in, WIRE op[2],
                  WIRE &less, WIRE &res) {
  x.run(t,binvert,b);
  y.run(i[0],a,t);
  o.run(i[1],a,t);
  sum.run(a,t,carry_in,i[2]);
  i[3]=less;
  mux.run(i,op,res);
}

void ALU_32BIT_CLA::run(WIRE a[32], WIRE b[32],    // Buses de datos de entrada
                        WIRE &bnegate, WIRE op[2], // Entradas de control de operaci'on
                        WIRE res[32],              // Bus de datos de salida
			WIRE &overflow,            // Overflow en la operaci'on
			WIRE &carry_out,           // Acarreo de salida de la ALU
                        WIRE &zero) {              // El resultado es cero
  WIRE z;	//Es la entrada less de las ALU´s de 1 bit que vale 0 menos para la primera
  WIRE entradas[31][32];

  // Miramos si tenemos que coger la b o la b negada
  for(int i = 0; i < 31; i++)
    xoraux[i].run(salida[i],b[i],bnegate);
    	
  // Inicializamos las lineas gi y pi
  for(int i = 0; i < 31; i++) {
    andg[i].run(g[i],a[i],salida[i]);
    orp[i].run(p[i],a[i],salida[i]);
  }

  // Inicializamos entradas
  entradas[0][0]=bnegate;
  for(int i = 1; i < 32; i++) {
    entradas[0][i]=p[i-1];
  }
  for(int i = 1; i < 31; i++) {
    entradas[i][0]=g[i-1];
    for(int k = 1; k < (32- i); k++) {
      entradas[i][k]=p[i-1+k];
    }
  }

  // Ahora sacamos los carry
  // Carry para la ALU de 1 bit numero 1
  andaux[0].run(aux[0],2,entradas[0]);
  oraux[0].run(c[0],2,aux[0],g[0]);

  // Carry para la ALU de 1 bit numero 2
  andaux[1].run(aux[1],3,entradas[0]);
  andaux[2].run(aux[2],2,entradas[1]);
  oraux[1].run(c[1],3,aux[1],aux[2],g[1]);

  // Carry para la ALU de 1 bit numero 3
  andaux[3].run(aux[3],4,entradas[0]);
  andaux[4].run(aux[4],3,entradas[1]);
  andaux[5].run(aux[5],2,entradas[2]);
  oraux[2].run(c[2],4,aux[3],aux[4],aux[5],g[2]);

  // Carry para la ALU de 1 bit numero 4
  andaux[6].run(aux[6],5,entradas[0]);
  andaux[7].run(aux[7],4,entradas[1]);
  andaux[8].run(aux[8],3,entradas[2]);
  andaux[9].run(aux[9],2,entradas[3]);
  oraux[3].run(c[3],5,aux[6],aux[7],aux[8],aux[9],g[3]);

  // Carry para la ALU de 1 bit numero 5
  andaux[10].run(aux[10],6,entradas[0]);
  andaux[11].run(aux[11],5,entradas[1]);
  andaux[12].run(aux[12],4,entradas[2]);
  andaux[13].run(aux[13],3,entradas[3]);
  andaux[14].run(aux[14],2,entradas[4]);
  oraux[4].run(c[4],6,aux[10],aux[11],aux[12],aux[13],aux[14],g[4]);

  // Carry para la ALU de 1 bit numero 6
  andaux[15].run(aux[15],7,entradas[0]);
  andaux[16].run(aux[16],6,entradas[1]);
  andaux[17].run(aux[17],5,entradas[2]);
  andaux[18].run(aux[18],4,entradas[3]);
  andaux[19].run(aux[19],3,entradas[4]);
  andaux[20].run(aux[20],2,entradas[5]);
  oraux[5].run(c[5],7,aux[15],aux[16],aux[17],aux[18],aux[19],aux[20],g[5]);

  // Carry para la ALU de 1 bit numero 7
  andaux[21].run(aux[21],8,entradas[0]);
  andaux[22].run(aux[22],7,entradas[1]);
  andaux[23].run(aux[23],6,entradas[2]);
  andaux[24].run(aux[24],5,entradas[3]);
  andaux[25].run(aux[25],4,entradas[4]);
  andaux[26].run(aux[26],3,entradas[5]);
  andaux[27].run(aux[27],2,entradas[6]);
  oraux[6].run(c[6],8,aux[21],aux[22],aux[23],aux[24],aux[25],aux[26],aux[27],g[6]);

  // Carry para la ALU de 1 bit numero 8
  andaux[28].run(aux[28],9,entradas[0]);
  andaux[29].run(aux[29],8,entradas[1]);
  andaux[30].run(aux[30],7,entradas[2]);
  andaux[31].run(aux[31],6,entradas[3]);
  andaux[32].run(aux[32],5,entradas[4]);
  andaux[33].run(aux[33],4,entradas[5]);
  andaux[34].run(aux[34],3,entradas[6]);
  andaux[35].run(aux[35],2,entradas[7]);
  oraux[7].run(c[7],9,aux[28],aux[29],aux[30],aux[31],aux[32],aux[33],aux[34],aux[35],g[7]);

  // Carry para la ALU de 1 bit numero 9
  andaux[36].run(aux[36],10,entradas[0]);
  andaux[37].run(aux[37],9,entradas[1]);
  andaux[38].run(aux[38],8,entradas[2]);
  andaux[39].run(aux[39],7,entradas[3]);
  andaux[40].run(aux[40],6,entradas[4]);
  andaux[41].run(aux[41],5,entradas[5]);
  andaux[42].run(aux[42],4,entradas[6]);
  andaux[43].run(aux[43],3,entradas[7]);
  andaux[44].run(aux[44],2,entradas[8]);
  oraux[8].run(c[8],10,aux[36],aux[37],aux[38],aux[39],aux[40],aux[41],aux[42],aux[43],aux[44],g[8]);

  // Carry para la ALU de 1 bit numero 10
  andaux[45].run(aux[45],11,entradas[0]);
  andaux[46].run(aux[46],10,entradas[1]);
  andaux[47].run(aux[47],9,entradas[2]);
  andaux[48].run(aux[48],8,entradas[3]);
  andaux[49].run(aux[49],7,entradas[4]);
  andaux[50].run(aux[50],6,entradas[5]);
  andaux[51].run(aux[51],5,entradas[6]);
  andaux[52].run(aux[52],4,entradas[7]);
  andaux[53].run(aux[53],3,entradas[8]);
  andaux[54].run(aux[54],2,entradas[9]);
  oraux[9].run(c[9],11,aux[45],aux[46],aux[47],aux[48],aux[49],aux[50],aux[51],aux[52],aux[53],
		       aux[54],g[9]);

  // Carry para la ALU de 1 bit numero 11
  andaux[55].run(aux[55],12,entradas[0]);
  andaux[56].run(aux[56],11,entradas[1]);
  andaux[57].run(aux[57],10,entradas[2]);
  andaux[58].run(aux[58],9,entradas[3]);
  andaux[59].run(aux[59],8,entradas[4]);
  andaux[60].run(aux[60],7,entradas[5]);
  andaux[61].run(aux[61],6,entradas[6]);
  andaux[62].run(aux[62],5,entradas[7]);
  andaux[63].run(aux[63],4,entradas[8]);
  andaux[64].run(aux[64],3,entradas[9]);
  andaux[65].run(aux[65],2,entradas[10]);
  oraux[10].run(c[10],12,aux[55],aux[56],aux[57],aux[58],aux[59],aux[60],aux[61],aux[62],aux[63],
		         aux[64],aux[65],g[10]);

  // Carry para la ALU de 1 bit numero 12
  andaux[66].run(aux[66],13,entradas[0]);
  andaux[67].run(aux[67],12,entradas[1]);
  andaux[68].run(aux[68],11,entradas[2]);
  andaux[69].run(aux[69],10,entradas[3]);
  andaux[70].run(aux[70],9,entradas[4]);
  andaux[71].run(aux[71],8,entradas[5]);
  andaux[72].run(aux[72],7,entradas[6]);
  andaux[73].run(aux[73],6,entradas[7]);
  andaux[74].run(aux[74],5,entradas[8]);
  andaux[75].run(aux[75],4,entradas[9]);
  andaux[76].run(aux[76],3,entradas[10]);
  andaux[77].run(aux[77],2,entradas[11]);
  oraux[11].run(c[11],13,aux[66],aux[67],aux[68],aux[69],aux[70],aux[71],aux[72],aux[73],aux[74],
			 aux[75],aux[76],aux[77],g[11]);

  // Carry para la ALU de 1 bit numero 13
  andaux[78].run(aux[78],14,entradas[0]);
  andaux[79].run(aux[79],13,entradas[1]);
  andaux[80].run(aux[80],12,entradas[2]);
  andaux[81].run(aux[81],11,entradas[3]);
  andaux[82].run(aux[82],10,entradas[4]);
  andaux[83].run(aux[83],9,entradas[5]);
  andaux[84].run(aux[84],8,entradas[6]);
  andaux[85].run(aux[85],7,entradas[7]);
  andaux[86].run(aux[86],6,entradas[8]);
  andaux[87].run(aux[87],5,entradas[9]);
  andaux[88].run(aux[88],4,entradas[10]);
  andaux[89].run(aux[89],3,entradas[11]);
  andaux[90].run(aux[90],2,entradas[12]);
  oraux[12].run(c[12],14,aux[78],aux[79],aux[80],aux[81],aux[82],aux[83],aux[84],aux[85],aux[86],
			 aux[87],aux[88],aux[89],aux[90],g[12]);

  // Carry para la ALU de 1 bit numero 14
  andaux[91].run(aux[91],15,entradas[0]);
  andaux[92].run(aux[92],14,entradas[1]);
  andaux[93].run(aux[93],13,entradas[2]);
  andaux[94].run(aux[94],12,entradas[3]);
  andaux[95].run(aux[95],11,entradas[4]);
  andaux[96].run(aux[96],10,entradas[5]);
  andaux[97].run(aux[97],9,entradas[6]);
  andaux[98].run(aux[98],8,entradas[7]);
  andaux[99].run(aux[99],7,entradas[8]);
  andaux[100].run(aux[100],6,entradas[9]);
  andaux[101].run(aux[101],5,entradas[10]);
  andaux[102].run(aux[102],4,entradas[11]);
  andaux[103].run(aux[103],3,entradas[12]);
  andaux[104].run(aux[104],2,entradas[13]);
  oraux[13].run(c[13],15,aux[91],aux[92],aux[93],aux[94],aux[95],aux[96],aux[97],aux[98],aux[99],
			 aux[100],aux[101],aux[102],aux[103],aux[104],g[13]);

  // Carry para la ALU de 1 bit numero 15
  andaux[105].run(aux[105],16,entradas[0]);
  andaux[106].run(aux[106],15,entradas[1]);
  andaux[107].run(aux[107],14,entradas[2]);
  andaux[108].run(aux[108],13,entradas[3]);
  andaux[109].run(aux[109],12,entradas[4]);
  andaux[110].run(aux[110],11,entradas[5]);
  andaux[111].run(aux[111],10,entradas[6]);
  andaux[112].run(aux[112],9,entradas[7]);
  andaux[113].run(aux[113],8,entradas[8]);
  andaux[114].run(aux[114],7,entradas[9]);
  andaux[115].run(aux[115],6,entradas[10]);
  andaux[116].run(aux[116],5,entradas[11]);
  andaux[117].run(aux[117],4,entradas[12]);
  andaux[118].run(aux[118],3,entradas[13]);
  andaux[119].run(aux[119],2,entradas[14]);
  oraux[14].run(c[14],16,aux[105],aux[106],aux[107],aux[108],aux[109],aux[110],aux[111],aux[112],
			 aux[113],aux[114],aux[115],aux[116],aux[117],aux[118],aux[119],g[14]);

  // Carry para la ALU de 1 bit numero 16
  andaux[120].run(aux[120],17,entradas[0]);
  andaux[121].run(aux[121],16,entradas[1]);
  andaux[122].run(aux[122],15,entradas[2]);
  andaux[123].run(aux[123],14,entradas[3]);
  andaux[124].run(aux[124],13,entradas[4]);
  andaux[125].run(aux[125],12,entradas[5]);
  andaux[126].run(aux[126],11,entradas[6]);
  andaux[127].run(aux[127],10,entradas[7]);
  andaux[128].run(aux[128],9,entradas[8]);
  andaux[129].run(aux[129],8,entradas[9]);
  andaux[130].run(aux[130],7,entradas[10]);
  andaux[131].run(aux[131],6,entradas[11]);
  andaux[132].run(aux[132],5,entradas[12]);
  andaux[133].run(aux[133],4,entradas[13]);
  andaux[134].run(aux[134],3,entradas[14]);
  andaux[135].run(aux[135],2,entradas[15]);
  oraux[15].run(c[15],17,aux[120],aux[121],aux[122],aux[123],aux[124],aux[125],aux[126],aux[127],
			 aux[128],aux[129],aux[130],aux[131],aux[132],aux[133],aux[134],aux[135],
			 g[15]);

  // Carry para la ALU de 1 bit numero 17
  andaux[136].run(aux[136],18,entradas[0]);
  andaux[137].run(aux[137],17,entradas[1]);
  andaux[138].run(aux[138],16,entradas[2]);
  andaux[139].run(aux[139],15,entradas[3]);
  andaux[140].run(aux[140],14,entradas[4]);
  andaux[141].run(aux[141],13,entradas[5]);
  andaux[142].run(aux[142],12,entradas[6]);
  andaux[143].run(aux[143],11,entradas[7]);
  andaux[144].run(aux[144],10,entradas[8]);
  andaux[145].run(aux[145],9,entradas[9]);
  andaux[146].run(aux[146],8,entradas[10]);
  andaux[147].run(aux[147],7,entradas[11]);
  andaux[148].run(aux[148],6,entradas[12]);
  andaux[149].run(aux[149],5,entradas[13]);
  andaux[150].run(aux[150],4,entradas[14]);
  andaux[151].run(aux[151],3,entradas[15]);
  andaux[152].run(aux[152],2,entradas[16]);
  oraux[16].run(c[16],18,aux[136],aux[137],aux[138],aux[139],aux[140],aux[141],aux[142],aux[143],
			 aux[144],aux[145],aux[146],aux[147],aux[148],aux[149],aux[150],aux[151],
			 aux[152],g[16]);

  // Carry para la ALU de 1 bit numero 18
  andaux[153].run(aux[153],19,entradas[0]);
  andaux[154].run(aux[154],18,entradas[1]);
  andaux[155].run(aux[155],17,entradas[2]);
  andaux[156].run(aux[156],16,entradas[3]);
  andaux[157].run(aux[157],15,entradas[4]);
  andaux[158].run(aux[158],14,entradas[5]);
  andaux[159].run(aux[159],13,entradas[6]);
  andaux[160].run(aux[160],12,entradas[7]);
  andaux[161].run(aux[161],11,entradas[8]);
  andaux[162].run(aux[162],10,entradas[9]);
  andaux[163].run(aux[163],9,entradas[10]);
  andaux[164].run(aux[164],8,entradas[11]);
  andaux[165].run(aux[165],7,entradas[12]);
  andaux[166].run(aux[166],6,entradas[13]);
  andaux[167].run(aux[167],5,entradas[14]);
  andaux[168].run(aux[168],4,entradas[15]);
  andaux[169].run(aux[169],3,entradas[16]);
  andaux[170].run(aux[170],2,entradas[17]);
  oraux[17].run(c[17],19,aux[153],aux[154],aux[155],aux[156],aux[157],aux[158],aux[159],aux[160],
			 aux[161],aux[162],aux[163],aux[164],aux[165],aux[166],aux[167],aux[168],
			 aux[169],aux[170],g[17]);

  // Carry para la ALU de 1 bit numero 19
  andaux[171].run(aux[171],20,entradas[0]);
  andaux[172].run(aux[172],19,entradas[1]);
  andaux[173].run(aux[173],18,entradas[2]);
  andaux[174].run(aux[174],17,entradas[3]);
  andaux[175].run(aux[175],16,entradas[4]);
  andaux[176].run(aux[176],15,entradas[5]);
  andaux[177].run(aux[177],14,entradas[6]);
  andaux[178].run(aux[178],13,entradas[7]);
  andaux[179].run(aux[179],12,entradas[8]);
  andaux[180].run(aux[180],11,entradas[9]);
  andaux[181].run(aux[181],10,entradas[10]);
  andaux[182].run(aux[182],9,entradas[11]);
  andaux[183].run(aux[183],8,entradas[12]);
  andaux[184].run(aux[184],7,entradas[13]);
  andaux[185].run(aux[185],6,entradas[14]);
  andaux[186].run(aux[186],5,entradas[15]);
  andaux[187].run(aux[187],4,entradas[16]);
  andaux[188].run(aux[188],3,entradas[17]);
  andaux[189].run(aux[189],2,entradas[18]);
  oraux[18].run(c[18],20,aux[171],aux[172],aux[173],aux[174],aux[175],aux[176],aux[177],aux[178],
			 aux[179],aux[180],aux[181],aux[182],aux[183],aux[184],aux[185],aux[186],
			 aux[187],aux[188],aux[189],g[18]);

  // Carry para la ALU de 1 bit numero 20
  andaux[190].run(aux[190],21,entradas[0]);
  andaux[191].run(aux[191],20,entradas[1]);
  andaux[192].run(aux[192],19,entradas[2]);
  andaux[193].run(aux[193],18,entradas[3]);
  andaux[194].run(aux[194],17,entradas[4]);
  andaux[195].run(aux[195],16,entradas[5]);
  andaux[196].run(aux[196],15,entradas[6]);
  andaux[197].run(aux[197],14,entradas[7]);
  andaux[198].run(aux[198],13,entradas[8]);
  andaux[199].run(aux[199],12,entradas[9]);
  andaux[200].run(aux[200],11,entradas[10]);
  andaux[201].run(aux[201],10,entradas[11]);
  andaux[202].run(aux[202],9,entradas[12]);
  andaux[203].run(aux[203],8,entradas[13]);
  andaux[204].run(aux[204],7,entradas[14]);
  andaux[205].run(aux[205],6,entradas[15]);
  andaux[206].run(aux[206],5,entradas[16]);
  andaux[207].run(aux[207],4,entradas[17]);
  andaux[208].run(aux[208],3,entradas[18]);
  andaux[209].run(aux[209],2,entradas[19]);
  oraux[19].run(c[19],21,aux[190],aux[191],aux[192],aux[193],aux[194],aux[195],aux[196],aux[197],
			 aux[198],aux[199],aux[200],aux[201],aux[202],aux[203],aux[204],aux[205],
			 aux[206],aux[207],aux[208],aux[209],g[19]);


  // Carry para la ALU de 1 bit numero 21
  andaux[210].run(aux[210],22,entradas[0]);
  andaux[211].run(aux[211],21,entradas[1]);
  andaux[212].run(aux[212],20,entradas[2]);
  andaux[213].run(aux[213],19,entradas[3]);
  andaux[214].run(aux[214],18,entradas[4]);
  andaux[215].run(aux[215],17,entradas[5]);
  andaux[216].run(aux[216],16,entradas[6]);
  andaux[217].run(aux[217],15,entradas[7]);
  andaux[218].run(aux[218],14,entradas[8]);
  andaux[219].run(aux[219],13,entradas[9]);
  andaux[220].run(aux[220],12,entradas[10]);
  andaux[221].run(aux[221],11,entradas[11]);
  andaux[222].run(aux[222],10,entradas[12]);
  andaux[223].run(aux[223],9,entradas[13]);
  andaux[224].run(aux[224],8,entradas[14]);
  andaux[225].run(aux[225],7,entradas[15]);
  andaux[226].run(aux[226],6,entradas[16]);
  andaux[227].run(aux[227],5,entradas[17]);
  andaux[228].run(aux[228],4,entradas[18]);
  andaux[229].run(aux[229],3,entradas[19]);
  andaux[230].run(aux[230],2,entradas[20]);
  oraux[20].run(c[20],22,aux[210],aux[211],aux[212],aux[213],aux[214],aux[215],aux[216],aux[217],
			 aux[218],aux[219],aux[220],aux[221],aux[222],aux[223],aux[224],aux[225],
			 aux[226],aux[227],aux[228],aux[229],aux[230],g[20]);

  // Carry para la ALU de 1 bit numero 22
  andaux[231].run(aux[231],23,entradas[0]);
  andaux[232].run(aux[232],22,entradas[1]);
  andaux[233].run(aux[233],21,entradas[2]);
  andaux[234].run(aux[234],20,entradas[3]);
  andaux[235].run(aux[235],19,entradas[4]);
  andaux[236].run(aux[236],18,entradas[5]);
  andaux[237].run(aux[237],17,entradas[6]);
  andaux[238].run(aux[238],16,entradas[7]);
  andaux[239].run(aux[239],15,entradas[8]);
  andaux[240].run(aux[240],14,entradas[9]);
  andaux[241].run(aux[241],13,entradas[10]);
  andaux[242].run(aux[242],12,entradas[11]);
  andaux[243].run(aux[243],11,entradas[12]);
  andaux[244].run(aux[244],10,entradas[13]);
  andaux[245].run(aux[245],9,entradas[14]);
  andaux[246].run(aux[246],8,entradas[15]);
  andaux[247].run(aux[247],7,entradas[16]);
  andaux[248].run(aux[248],6,entradas[17]);
  andaux[249].run(aux[249],5,entradas[18]);
  andaux[250].run(aux[250],4,entradas[19]);
  andaux[251].run(aux[251],3,entradas[20]);
  andaux[252].run(aux[252],2,entradas[21]);
  oraux[21].run(c[21],23,aux[231],aux[232],aux[233],aux[234],aux[235],aux[236],aux[237],aux[238],
			 aux[239],aux[240],aux[241],aux[242],aux[243],aux[244],aux[245],aux[246],
			 aux[247],aux[248],aux[249],aux[250],aux[251],aux[252],g[21]);

  // Carry para la ALU de 1 bit numero 23
  andaux[253].run(aux[253],24,entradas[0]);
  andaux[254].run(aux[254],23,entradas[1]);
  andaux[255].run(aux[255],22,entradas[2]);
  andaux[256].run(aux[256],21,entradas[3]);
  andaux[257].run(aux[257],20,entradas[4]);
  andaux[258].run(aux[258],19,entradas[5]);
  andaux[259].run(aux[259],18,entradas[6]);
  andaux[260].run(aux[260],17,entradas[7]);
  andaux[261].run(aux[261],16,entradas[8]);
  andaux[262].run(aux[262],15,entradas[9]);
  andaux[263].run(aux[263],14,entradas[10]);
  andaux[264].run(aux[264],13,entradas[11]);
  andaux[265].run(aux[265],12,entradas[12]);
  andaux[266].run(aux[266],11,entradas[13]);
  andaux[267].run(aux[267],10,entradas[14]);
  andaux[268].run(aux[268],9,entradas[15]);
  andaux[269].run(aux[269],8,entradas[16]);
  andaux[270].run(aux[270],7,entradas[17]);
  andaux[271].run(aux[271],6,entradas[18]);
  andaux[272].run(aux[272],5,entradas[19]);
  andaux[273].run(aux[273],4,entradas[20]);
  andaux[274].run(aux[274],3,entradas[21]);
  andaux[275].run(aux[275],2,entradas[22]);
  oraux[22].run(c[22],24,aux[253],aux[254],aux[255],aux[256],aux[257],aux[258],aux[259],aux[260],
			 aux[261],aux[262],aux[263],aux[264],aux[265],aux[266],aux[267],aux[268],
			 aux[269],aux[270],aux[271],aux[272],aux[273],aux[274],aux[275],g[22]);


  // Carry para la ALU de 1 bit numero 24
  andaux[276].run(aux[276],25,entradas[0]);
  andaux[277].run(aux[277],24,entradas[1]);
  andaux[278].run(aux[278],23,entradas[2]);
  andaux[279].run(aux[279],22,entradas[3]);
  andaux[280].run(aux[280],21,entradas[4]);
  andaux[281].run(aux[281],20,entradas[5]);
  andaux[282].run(aux[282],19,entradas[6]);
  andaux[283].run(aux[283],18,entradas[7]);
  andaux[284].run(aux[284],17,entradas[8]);
  andaux[285].run(aux[285],16,entradas[9]);
  andaux[286].run(aux[286],15,entradas[10]);
  andaux[287].run(aux[287],14,entradas[11]);
  andaux[288].run(aux[288],13,entradas[12]);
  andaux[289].run(aux[289],12,entradas[13]);
  andaux[290].run(aux[290],11,entradas[14]);
  andaux[291].run(aux[291],10,entradas[15]);
  andaux[292].run(aux[292],9,entradas[16]);
  andaux[293].run(aux[293],8,entradas[17]);
  andaux[294].run(aux[294],7,entradas[18]);
  andaux[295].run(aux[295],6,entradas[19]);
  andaux[296].run(aux[296],5,entradas[20]);
  andaux[297].run(aux[297],4,entradas[21]);
  andaux[298].run(aux[298],3,entradas[22]);
  andaux[299].run(aux[299],2,entradas[23]);
  oraux[23].run(c[23],25,aux[276],aux[277],aux[278],aux[279],aux[280],aux[281],aux[282],aux[283],
			 aux[284],aux[285],aux[286],aux[287],aux[288],aux[289],aux[290],aux[291],
			 aux[292],aux[293],aux[294],aux[295],aux[296],aux[297],aux[298],aux[299],
			 g[23]);

  // Carry para la ALU de 1 bit numero 25
  andaux[300].run(aux[300],26,entradas[0]);
  andaux[301].run(aux[301],25,entradas[1]);
  andaux[302].run(aux[302],24,entradas[2]);
  andaux[303].run(aux[303],23,entradas[3]);
  andaux[304].run(aux[304],22,entradas[4]);
  andaux[305].run(aux[305],21,entradas[5]);
  andaux[306].run(aux[306],20,entradas[6]);
  andaux[307].run(aux[307],19,entradas[7]);
  andaux[308].run(aux[308],18,entradas[8]);
  andaux[309].run(aux[309],17,entradas[9]);
  andaux[310].run(aux[310],16,entradas[10]);
  andaux[311].run(aux[311],15,entradas[11]);
  andaux[312].run(aux[312],14,entradas[12]);
  andaux[313].run(aux[313],13,entradas[13]);
  andaux[314].run(aux[314],12,entradas[14]);
  andaux[315].run(aux[315],11,entradas[15]);
  andaux[316].run(aux[316],10,entradas[16]);
  andaux[317].run(aux[317],9,entradas[17]);
  andaux[318].run(aux[318],8,entradas[18]);
  andaux[319].run(aux[319],7,entradas[19]);
  andaux[320].run(aux[320],6,entradas[20]);
  andaux[321].run(aux[321],5,entradas[21]);
  andaux[322].run(aux[322],4,entradas[22]);
  andaux[323].run(aux[323],3,entradas[23]);
  andaux[324].run(aux[324],2,entradas[24]);
  oraux[24].run(c[24],26,aux[300],aux[301],aux[302],aux[303],aux[304],aux[305],aux[306],aux[307],
			 aux[308],aux[309],aux[310],aux[311],aux[312],aux[313],aux[314],aux[315],
			 aux[316],aux[317],aux[318],aux[319],aux[320],aux[321],aux[322],aux[323],
			 aux[324],g[24]);

  // Carry para la ALU de 1 bit numero 26
  andaux[325].run(aux[325],27,entradas[0]);
  andaux[326].run(aux[326],26,entradas[1]);
  andaux[327].run(aux[327],25,entradas[2]);
  andaux[328].run(aux[328],24,entradas[3]);
  andaux[329].run(aux[329],23,entradas[4]);
  andaux[333].run(aux[330],22,entradas[5]);
  andaux[331].run(aux[331],21,entradas[6]);
  andaux[332].run(aux[332],20,entradas[7]);
  andaux[333].run(aux[333],19,entradas[8]);
  andaux[334].run(aux[334],18,entradas[9]);
  andaux[335].run(aux[335],17,entradas[10]);
  andaux[336].run(aux[336],16,entradas[11]);
  andaux[337].run(aux[337],15,entradas[12]);
  andaux[338].run(aux[338],14,entradas[13]);
  andaux[339].run(aux[339],13,entradas[14]);
  andaux[340].run(aux[340],12,entradas[15]);
  andaux[341].run(aux[341],11,entradas[16]);
  andaux[342].run(aux[342],10,entradas[17]);
  andaux[343].run(aux[343],9,entradas[18]);
  andaux[344].run(aux[344],8,entradas[19]);
  andaux[345].run(aux[345],7,entradas[20]);
  andaux[346].run(aux[346],6,entradas[21]);
  andaux[347].run(aux[347],5,entradas[22]);
  andaux[348].run(aux[348],4,entradas[23]);
  andaux[349].run(aux[349],3,entradas[24]);
  andaux[350].run(aux[350],2,entradas[25]);
  oraux[25].run(c[25],27,aux[325],aux[326],aux[327],aux[328],aux[329],aux[330],aux[331],aux[332],
			 aux[333],aux[334],aux[335],aux[336],aux[337],aux[338],aux[339],aux[340],
			 aux[341],aux[342],aux[343],aux[344],aux[345],aux[346],aux[347],aux[348],
			 aux[349],aux[350],g[25]);

  // Carry para la ALU de 1 bit numero 27
  andaux[351].run(aux[351],28,entradas[0]);
  andaux[352].run(aux[352],27,entradas[1]);
  andaux[353].run(aux[353],26,entradas[2]);
  andaux[354].run(aux[354],25,entradas[3]);
  andaux[355].run(aux[355],24,entradas[4]);
  andaux[356].run(aux[356],23,entradas[5]);
  andaux[357].run(aux[357],22,entradas[6]);
  andaux[358].run(aux[358],21,entradas[7]);
  andaux[359].run(aux[359],20,entradas[8]);
  andaux[360].run(aux[360],19,entradas[9]);
  andaux[361].run(aux[361],18,entradas[10]);
  andaux[362].run(aux[362],17,entradas[11]);
  andaux[363].run(aux[363],16,entradas[12]);
  andaux[364].run(aux[364],15,entradas[13]);
  andaux[365].run(aux[365],14,entradas[14]);
  andaux[366].run(aux[366],13,entradas[15]);
  andaux[367].run(aux[367],12,entradas[16]);
  andaux[368].run(aux[368],11,entradas[17]);
  andaux[369].run(aux[369],10,entradas[18]);
  andaux[370].run(aux[370],9,entradas[19]);
  andaux[371].run(aux[371],8,entradas[20]);
  andaux[372].run(aux[372],7,entradas[21]);
  andaux[373].run(aux[373],6,entradas[22]);
  andaux[374].run(aux[374],5,entradas[23]);
  andaux[375].run(aux[375],4,entradas[24]);
  andaux[376].run(aux[376],3,entradas[25]);
  andaux[377].run(aux[377],2,entradas[26]);
  oraux[26].run(c[26],28,aux[351],aux[352],aux[353],aux[354],aux[355],aux[356],aux[357],aux[358],
			 aux[359],aux[360],aux[361],aux[362],aux[363],aux[364],aux[365],aux[366],
			 aux[367],aux[368],aux[369],aux[370],aux[371],aux[372],aux[373],aux[374],
			 aux[375],aux[376],aux[377],g[26]);

  // Carry para la ALU de 1 bit numero 28
  andaux[378].run(aux[378],29,entradas[0]);
  andaux[379].run(aux[379],28,entradas[1]);
  andaux[380].run(aux[380],27,entradas[2]);
  andaux[381].run(aux[381],26,entradas[3]);
  andaux[382].run(aux[382],25,entradas[4]);
  andaux[383].run(aux[383],24,entradas[5]);
  andaux[384].run(aux[384],23,entradas[6]);
  andaux[385].run(aux[385],22,entradas[7]);
  andaux[386].run(aux[386],21,entradas[8]);
  andaux[387].run(aux[387],20,entradas[9]);
  andaux[388].run(aux[388],19,entradas[10]);
  andaux[389].run(aux[389],18,entradas[11]);
  andaux[390].run(aux[390],17,entradas[12]);
  andaux[391].run(aux[391],16,entradas[13]);
  andaux[392].run(aux[392],15,entradas[14]);
  andaux[393].run(aux[393],14,entradas[15]);
  andaux[394].run(aux[394],13,entradas[16]);
  andaux[395].run(aux[395],12,entradas[17]);
  andaux[396].run(aux[396],11,entradas[18]);
  andaux[397].run(aux[397],10,entradas[19]);
  andaux[398].run(aux[398],9,entradas[20]);
  andaux[399].run(aux[399],8,entradas[21]);
  andaux[400].run(aux[400],7,entradas[22]);
  andaux[401].run(aux[401],6,entradas[23]);
  andaux[402].run(aux[402],5,entradas[24]);
  andaux[403].run(aux[403],4,entradas[25]);
  andaux[404].run(aux[404],3,entradas[26]);
  andaux[405].run(aux[405],2,entradas[27]);
  oraux[27].run(c[27],29,aux[378],aux[379],aux[380],aux[381],aux[382],aux[383],aux[384],aux[385],
			 aux[386],aux[387],aux[388],aux[389],aux[390],aux[391],aux[392],aux[393],
			 aux[394],aux[395],aux[396],aux[397],aux[398],aux[399],aux[400],aux[401],
			 aux[402],aux[403],aux[404],aux[405],g[27]);

  // Carry para la ALU de 1 bit numero 29
  andaux[406].run(aux[406],30,entradas[0]);
  andaux[407].run(aux[407],29,entradas[1]);
  andaux[408].run(aux[408],28,entradas[2]);
  andaux[409].run(aux[409],27,entradas[3]);
  andaux[410].run(aux[410],26,entradas[4]);
  andaux[411].run(aux[411],25,entradas[5]);
  andaux[412].run(aux[412],24,entradas[6]);
  andaux[413].run(aux[413],23,entradas[7]);
  andaux[414].run(aux[414],22,entradas[8]);
  andaux[415].run(aux[415],21,entradas[9]);
  andaux[416].run(aux[416],20,entradas[10]);
  andaux[417].run(aux[417],19,entradas[11]);
  andaux[418].run(aux[418],18,entradas[12]);
  andaux[419].run(aux[419],17,entradas[13]);
  andaux[420].run(aux[420],16,entradas[14]);
  andaux[421].run(aux[421],15,entradas[15]);
  andaux[422].run(aux[422],14,entradas[16]);
  andaux[423].run(aux[423],13,entradas[17]);
  andaux[424].run(aux[424],12,entradas[18]);
  andaux[425].run(aux[425],11,entradas[19]);
  andaux[426].run(aux[426],10,entradas[20]);
  andaux[427].run(aux[427],9,entradas[21]);
  andaux[428].run(aux[428],8,entradas[22]);
  andaux[429].run(aux[429],7,entradas[23]);
  andaux[430].run(aux[430],6,entradas[24]);
  andaux[431].run(aux[431],5,entradas[25]);
  andaux[432].run(aux[432],4,entradas[26]);
  andaux[433].run(aux[433],3,entradas[27]);
  andaux[434].run(aux[434],2,entradas[28]);
  oraux[28].run(c[28],30,aux[406],aux[407],aux[408],aux[409],aux[410],aux[411],aux[412],aux[413],
			 aux[414],aux[415],aux[416],aux[417],aux[418],aux[419],aux[420],aux[421],
			 aux[422],aux[423],aux[424],aux[425],aux[426],aux[427],aux[428],aux[429],
			 aux[430],aux[431],aux[432],aux[433],aux[434],g[28]);

  // Carry para la ALU de 1 bit numero 30
  andaux[435].run(aux[435],31,entradas[0]);
  andaux[436].run(aux[436],30,entradas[1]);
  andaux[437].run(aux[437],29,entradas[2]);
  andaux[438].run(aux[438],28,entradas[3]);
  andaux[439].run(aux[439],27,entradas[4]);
  andaux[440].run(aux[440],26,entradas[5]);
  andaux[441].run(aux[441],25,entradas[6]);
  andaux[442].run(aux[442],24,entradas[7]);
  andaux[443].run(aux[443],23,entradas[8]);
  andaux[444].run(aux[444],22,entradas[9]);
  andaux[445].run(aux[445],21,entradas[10]);
  andaux[446].run(aux[446],20,entradas[11]);
  andaux[447].run(aux[447],19,entradas[12]);
  andaux[448].run(aux[448],18,entradas[13]);
  andaux[449].run(aux[449],17,entradas[14]);
  andaux[450].run(aux[450],16,entradas[15]);
  andaux[451].run(aux[451],15,entradas[16]);
  andaux[452].run(aux[452],14,entradas[17]);
  andaux[453].run(aux[453],13,entradas[18]);
  andaux[454].run(aux[454],12,entradas[19]);
  andaux[455].run(aux[455],11,entradas[20]);
  andaux[456].run(aux[456],10,entradas[21]);
  andaux[457].run(aux[457],9,entradas[22]);
  andaux[458].run(aux[458],8,entradas[23]);
  andaux[459].run(aux[459],7,entradas[24]);
  andaux[460].run(aux[460],6,entradas[25]);
  andaux[461].run(aux[461],5,entradas[26]);
  andaux[462].run(aux[462],4,entradas[27]);
  andaux[463].run(aux[463],3,entradas[28]);
  andaux[464].run(aux[464],2,entradas[29]);
  oraux[29].run(c[29],31,aux[435],aux[436],aux[437],aux[438],aux[439],aux[440],aux[441],aux[442],
			 aux[443],aux[444],aux[445],aux[446],aux[447],aux[448],aux[449],aux[450],
			 aux[451],aux[452],aux[453],aux[454],aux[455],aux[456],aux[457],aux[458],
			 aux[459],aux[460],aux[461],aux[462],aux[463],aux[464],g[29]);

  // Carry para la ALU del bit mas significativo
  andaux[465].run(aux[465],32,entradas[0]);
  andaux[466].run(aux[466],31,entradas[1]);
  andaux[467].run(aux[467],30,entradas[2]);
  andaux[468].run(aux[468],29,entradas[3]);
  andaux[469].run(aux[469],28,entradas[4]);
  andaux[470].run(aux[470],27,entradas[5]);
  andaux[471].run(aux[471],26,entradas[6]);
  andaux[472].run(aux[472],25,entradas[7]);
  andaux[473].run(aux[473],24,entradas[8]);
  andaux[474].run(aux[474],23,entradas[9]);
  andaux[475].run(aux[475],22,entradas[10]);
  andaux[476].run(aux[476],21,entradas[11]);
  andaux[477].run(aux[477],20,entradas[12]);
  andaux[478].run(aux[478],19,entradas[13]);
  andaux[479].run(aux[479],18,entradas[14]);
  andaux[480].run(aux[480],17,entradas[15]);
  andaux[481].run(aux[481],16,entradas[16]);
  andaux[482].run(aux[482],15,entradas[17]);
  andaux[483].run(aux[483],14,entradas[18]);
  andaux[484].run(aux[484],13,entradas[19]);
  andaux[485].run(aux[485],12,entradas[20]);
  andaux[486].run(aux[486],11,entradas[21]);
  andaux[487].run(aux[487],10,entradas[22]);
  andaux[488].run(aux[488],9,entradas[23]);
  andaux[489].run(aux[489],8,entradas[24]);
  andaux[490].run(aux[490],7,entradas[25]);
  andaux[491].run(aux[491],6,entradas[26]);
  andaux[492].run(aux[492],5,entradas[27]);
  andaux[493].run(aux[493],4,entradas[28]);
  andaux[494].run(aux[494],3,entradas[29]);
  andaux[495].run(aux[495],2,entradas[30]);
  oraux[30].run(c[30],32,aux[465],aux[466],aux[467],aux[468],aux[469],aux[470],aux[471],aux[472],
			 aux[473],aux[474],aux[475],aux[476],aux[477],aux[478],aux[479],aux[480],
			 aux[481],aux[482],aux[483],aux[484],aux[485],aux[486],aux[487],aux[488],
			 aux[489],aux[490],aux[491],aux[492],aux[493],aux[494],aux[495],g[30]);


  // El resto de ejecución es como el de la otra ALU 
  z=0;
  alu[0].run(a[0],b[0],bnegate,bnegate,op,less,res[0]);
  for(int j = 1; j < 31; j++) {
    alu[j].run(a[j],b[j],bnegate,c[j-1],op,z,res[j]);
  }
  alu31.run(a[31],b[31],bnegate,c[30],op,z,res[31],carry_out,less,overflow);
  no.run(zero,32,res);
}
