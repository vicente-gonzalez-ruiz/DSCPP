//-*- C++ -*-
// Jacobo Jimenez Martinez
// Laboratorio de Arquitectura de Computadores
// Marzo 1998
//
#include <stdlib.h>
#include <fstream>
#include <strings.h>
#include <iostream>

#define MAXUNSINT 65535
#define MINLONG -2147483647
#define MAXLONG 2147483647

using namespace std;

class BASEn 
{
  public: 
    void d2b(int dec,int bin[5])  // para numeros de registros (dec<32).
    {
      if(dec<0 || dec>=32) 
      {
        cerr << "\nError(17): Imposible conversion de "<< dec <<"\n";
        exit(1);
      }
      else 
      {
         for (int i=0;i<4;i++)
         {
            bin[i]=dec%2;
            dec=dec/2;
         }
         bin[4]=dec;
      }
    }
    void ui2b(unsigned int dec,int bin[16]) //para direcc. de datos (dec<65535).
    {
       int binaux[16];
       if(dec<0 || dec>MAXUNSINT) 
       {
          cerr << "\nError(34): Imposible conversion de "<< dec <<"\n";
          exit(1);
       }
       else 
       {
          int i,j;
          for(i=0;i<16;i++)
             binaux[i]=0;      // inicializa a 0.
          i=0;
          while(dec>=2&&i<16)
          {
             binaux[i++]=dec%2;
             dec=dec/2;
          }
          binaux[i]=dec;
          for(j=0;j<=15;j++) // desplazamiento a la derecha;
             bin[15-j]=binaux[j];
      }
    }
    void l2b(long dec,int bin[32])  // para palabras de datos (32 bits).
    {
      if(dec<MINLONG || dec>MAXLONG) 
      {
         cerr << "\nError(59): Imposible conversion de "<< dec <<"\n";
         exit(1);
      }
      else 
      {
         int hacercompl2=0;
         if (dec<0)
         {
            dec=-dec;
            hacercompl2=1;
         }
         for (int i=0;i<31;i++)
         {
            bin[i]=dec%2;
            dec=dec/2;
         }
         bin[31]=dec;
         if (hacercompl2)
            compl2(bin);
      }
    }
    void compl2(int bin[32])
    {
       int i,carry=1;
       for (i=0;i<32;i++)
          if (bin[i]==0)
             bin[i]=1;
          else
             bin[i]=0;
       i=0;
       while (i<32)
       {
          if (carry==1 && bin[i]==1)
          {
             bin[i]=0;
             carry=1;
          }
          if (carry==1 && bin[i]==0)
          {
             bin[i]=1;
             carry=0;
          }
          i++;
       }
    }
};

//***************************** operacion ***********************************

class Operacion
{
   private:
      BASEn convert;
   public:
      void ADD(ifstream &f_in,ofstream &f_out);
      void AND(ifstream &f_in,ofstream &f_out);
      void BEQ(ifstream &f_in,ofstream &f_out);
      void LW(ifstream &f_in,ofstream &f_out);
      void OR(ifstream &f_in,ofstream &f_out);
      void SLT(ifstream &f_in,ofstream &f_out);
      void SUB(ifstream &f_in,ofstream &f_out);
      void SW(ifstream &f_in,ofstream &f_out);
      void LI(ifstream &f_in,ofstream &f_out);
      void ORI(ifstream &f_in,ofstream &f_out);
      void J(ifstream &f_in,ofstream &f_out);
      void W(ifstream &f_in,ofstream &f_out);
};

   // ------------------------------------------------

   void Operacion::ADD(ifstream &f_in,ofstream &f_out)
   {
      int bin[5],reg,destino[5];

      // escribe los bits (31-26)
      for (int i=0;i<6;i++)
         f_out << 0;

      // registro rs(25-21), rt(20-16), rd(15-11)
      f_in >> reg;
      f_in.ignore(2);
      convert.d2b(reg,destino);  // rs: pasarlo a binario
      
      f_in >> reg;
      f_in.ignore(2);
      convert.d2b(reg,bin);  // rt: pasarlo a binario
      for (int j=0;j<5;j++)  // escribirlo en el archivo
         f_out << bin[4-j];
      f_in >> reg;
      convert.d2b(reg,bin);  // rd: pasarlo a binario
      for (int j=0;j<5;j++)  // escribirlo en el archivo
         f_out << bin[4-j];
      
      for (int j=0;j<5;j++)  // escribirlo en el archivo
         f_out << destino[4-j];
      

      // nada
      for (int i=0;i<5;i++)
         f_out << 0;    // este trozo de la instruccion no se usa
      // funcion de la ALU (suma)
      f_out << 1;
      f_out << 0;
      f_out << 0;
      f_out << 0;
      f_out << 0;
      f_out << 0;
   }

   // -----------------------------------------------------------
   
   void Operacion::SUB(ifstream &f_in,ofstream &f_out)
   {
      int bin[5],reg,destino[5];
      
      // escribe los bits (31-26)
      for (int i=0;i<6;i++)
         f_out << 0;
      
      // registro rs(25-21), rt(20-16), rd(15-11)
      f_in >> reg;
      f_in.ignore(2);
      convert.d2b(reg,destino);  // rs: pasarlo a binario
      
      f_in >> reg;
      f_in.ignore(2);
      convert.d2b(reg,bin);  // rt: pasarlo a binario
      for (int j=0;j<5;j++)  // escribirlo en el archivo
         f_out << bin[4-j];
      f_in >> reg;
      convert.d2b(reg,bin);  // rd: pasarlo a binario
      for (int j=0;j<5;j++)  // escribirlo en el archivo
         f_out << bin[4-j];


      for (int j=0;j<5;j++)  // escribirlo en el archivo registro destino
         f_out << destino[4-j];
      
      // nada
      for (int i=0;i<5;i++)
         f_out << 0;    // este trozo de la instruccion no se usa
      
      // funcion de la ALU (resta)
      f_out << 1;
      f_out << 0;
      f_out << 0;
      f_out << 0;
      f_out << 1;
      f_out << 0;
   }
   
   // -----------------------------------------------------------
   
   void Operacion::AND(ifstream &f_in,ofstream &f_out)
   {
      int bin[5],num,reg,destino[5];
      
      // escribe los bits (31-26)
      for (int i=0;i<6;i++)
         f_out << 0;
      
      // registro rs(25-21), rt(20-16), rd(15-11)
      f_in >> reg;
      f_in.ignore(2);
      convert.d2b(reg,destino);  // rs: pasarlo a binario

      f_in >> reg;
      f_in.ignore(2);
      convert.d2b(reg,bin);  // rt: pasarlo a binario
      for (int j=0;j<5;j++)  // escribirlo en el archivo
         f_out << bin[4-j];
      f_in >> reg;
      convert.d2b(reg,bin);  // rd: pasarlo a binario
      for (int j=0;j<5;j++)  // escribirlo en el archivo
         f_out << bin[4-j];

       for (int j=0;j<5;j++)  // escribirlo en el archivo registro destino
         f_out << destino[4-j];
      
      // nada
      for (int i=0;i<5;i++)
         f_out << 0;    // este trozo de la instruccion no se usa
      // funcion de la ALU (and)
      f_out << 1;
      f_out << 0;
      f_out << 0;
      f_out << 1;
      f_out << 0;
      f_out << 0;
   }
   
   // -----------------------------------------------------------
   
   void Operacion::OR(ifstream &f_in,ofstream &f_out)
   {
      int bin[5],reg,destino[5];         
      
      // escribe los bits (31-26)
      for (int i=0;i<6;i++)
         f_out << 0;
      
      // registro rs(25-21), rt(20-16), rd(15-11)
      f_in >> reg;
      f_in.ignore(2);   // ",$"
      convert.d2b(reg,destino);  // rs: pasarlo a binario
      
      f_in >> reg;
      f_in.ignore(2);   // ",$"
      convert.d2b(reg,bin);  // rt: pasarlo a binario
      for (int j=0;j<5;j++)  // escribirlo en el archivo
         f_out << bin[4-j];
      f_in >> reg;
      convert.d2b(reg,bin);  // rd: pasarlo a binario
      for (int j=0;j<5;j++)  // escribirlo en el archivo
         f_out << bin[4-j];
    
       for (int j=0;j<5;j++)  // escribirlo en el archivo
         f_out << destino[4-j];


      // nada
      for (int i=0;i<5;i++)
         f_out << 0;    // este trozo de la instruccion no se usa
    
      // funcion de la ALU (or)
      f_out << 1;
      f_out << 0;
      f_out << 0;
      f_out << 1;
      f_out << 0;
      f_out << 1;
   }
   
   // -----------------------------------------------------------
   
   void Operacion::SLT(ifstream &f_in,ofstream &f_out)
   {
      int bin[5],num,reg,destino[5];
      
      // escribe los bits (31-26)
      for (int i=0;i<6;i++)
         f_out << 0;
      
      // registro rs(25-21), rt(20-16), rd(15-11)
      f_in >> reg;
      f_in.ignore(2);
      convert.d2b(reg,destino);  // rs: pasarlo a binario
      
      f_in >> reg;
      f_in.ignore(2);
      convert.d2b(reg,bin);  // rt: pasarlo a binario
      for (int j=0;j<5;j++)  // escribirlo en el archivo
         f_out << bin[4-j];
      f_in >> reg;
      convert.d2b(reg,bin);  // rd: pasarlo a binario
      for (int j=0;j<5;j++)  // escribirlo en el archivo
         f_out << bin[4-j];
      
      for (int j=0;j<5;j++)  // escribirlo en el archivo
         f_out << destino[4-j];

      // nada
      for (int i=0;i<5;i++)
         f_out << 0;    // este trozo de la instruccion no se usa
      
      // funcion de la ALU (slt)
      f_out << 1;
      f_out << 0;
      f_out << 1;
      f_out << 0;
      f_out << 1;
      f_out << 0;
   }
   
   // -----------------------------------------------------------
   
   void Operacion::LW(ifstream &f_in,ofstream &f_out)
   {
      int binrs[5],binrt[5],bin[16],reg;
      unsigned int num;
      
      // operacion lw(31-26) 
      f_out << 1;
      f_out << 0;
      f_out << 0;
      f_out << 0;
      f_out << 1;
      f_out << 1;
      
      // registro rt(25-21) base, desplaz(15-0) y rs(20-16) destino
      f_in >> reg;
      f_in.ignore(1); // saltar la coma
      convert.d2b(reg,binrs);  // rs: pasarlo a binario
      f_in >> num;
      f_in.ignore(2); // saltar: "($"
      convert.ui2b(num,bin);
      f_in >> reg;
      f_in.ignore(1); // saltar: ")"
      convert.d2b(reg,binrt);  // rt: pasarlo a binario
      for (int j=0;j<5;j++)  // escribir rt el archivo
         f_out << binrt[4-j];
      for (int j=0;j<5;j++)  // escribir rs en el archivo
         f_out << binrs[4-j];
      for (int i=15;i>=0;i--)
         f_out << bin[15-i];
   }
   
   // -----------------------------------------------------------
   
   void Operacion::SW(ifstream &f_in,ofstream &f_out)
   {
      int binrs[5],binrt[5],bin[16],reg;
      unsigned int num;
      
      // operacion sw(31-26) 
      f_out << 1;
      f_out << 0;
      f_out << 1;
      f_out << 0;
      f_out << 1;
      f_out << 1;
      
      // registro rs(25-21) base, desplaz(15-0) y rt(20-16) destino
      f_in >> reg;
      f_in.ignore(1); // saltar la coma
      convert.d2b(reg,binrs);  // pasarlo a binario
      f_in >> num;
      convert.ui2b(num,bin);
      f_in.ignore(2); // saltar: "($"
      f_in >> reg;
      f_in.ignore(1); // saltar: ")"
      convert.d2b(reg,binrt);  // pasarlo a binario
      for (int j=0;j<5;j++)  // escribir rt en el archivo
         f_out << binrt[4-j];
      for (int j=0;j<5;j++)  // escribir rs en el archivo
         f_out << binrs[4-j];
      for (int i=15;i>=0;i--)
         f_out << bin[15-i];
   }
   
   // -----------------------------------------------------------
   
   void Operacion::BEQ(ifstream &f_in,ofstream &file_out)
   {
      int bin5[5],bin[32],reg;
      long num;
      
      // operacion lw(31-26) 
      file_out << 0;
      file_out << 0;
      file_out << 0;
      file_out << 1;
      file_out << 0;
      file_out << 0;
      
      // registro rs(25-21) base, rt(20-16) destino y desplaz(15-0)       
      f_in >> reg;
      f_in.ignore(2); // saltar: ",$"
      convert.d2b(reg,bin5);  // pasarlo a binario
      for (int j=4;j>=0;j--)  // escribirlo en el archivo
         file_out << bin5[j];
      
      f_in >> reg;
      f_in.ignore(1); // ","
      convert.d2b(reg,bin5);  // pasarlo a binario
      for (int j=4;j>=0;j--)  // escribirlo en el archivo
         file_out << bin5[j];
      
      f_in >> num;
      convert.l2b(num,bin);
      for (int j=15;j>=0;j--)
         file_out << bin[j];
   }
   
   // ------------------------------------------------------
   
   void Operacion::ORI(ifstream &f_in,ofstream &f_out)
   {
      int bin5[5],bin[32];
      int reg;
      long num;
      
      f_out << 0;
      f_out << 0;
      f_out << 1;
      f_out << 1;
      f_out << 0;
      f_out << 1;
      f_in >> reg; // registro destino
      convert.d2b(reg,bin5);
      f_in.ignore(2);  // ",$"
      for(int i=4;i>=0;i--)
         f_out << bin5[i];
      f_in >> reg; // registro de operacion OR
      convert.d2b(reg,bin5);
      f_in.ignore(1);   // ","
      for(int i=4;i>=0;i--)
         f_out << bin5[i];
      f_in >> num; // constante
      convert.l2b(num,bin);
      for(int i=15;i>=0;i--)
         f_out << bin[i];
   }
   
   // ------------------------------------------------------
   
   void Operacion::LI(ifstream &f_in,ofstream &f_out)
   {
      int bin5[5],bin[32];
      int reg;
      long num;
      
      f_out << "$";
      f_in >> reg; // registro destino
      convert.d2b(reg,bin5);
      f_in.ignore(1);
      f_in >> num; // constante
      convert.l2b(num,bin);
      for(int i=4;i>=0;i--)
         f_out << bin5[i];
      for(int i=26;i>=0;i--)
         f_out << bin[i];
   }
   
   // ------------------------------------------------------
   
   void Operacion::J(ifstream &f_in,ofstream &f_out)
   {
      int bin[32];
      long num;
      
      f_out << 0;
      f_out << 0;
      f_out << 0;
      f_out << 0;
      f_out << 1;
      f_out << 0;
      
      f_in >> num; // constante
      convert.l2b(num,bin);
      for(int i=25;i>=0;i--)
         f_out << bin[i];
   }
   
   // ------------------------------------------------------
   
   void Operacion::W(ifstream &f_in,ofstream &f_out)
   {
      int bin[32];
      long num;
      
      f_out << '.';
      f_in >> num;
      convert.l2b(num,bin);
      for(int i=0;i<32;i++)
         f_out << bin[31-i];
   }

//******************************* MAIN **************************************

int main(int nargs,char *argv[]) 
{
  if(nargs!=3) 
  {
    cerr << "Uso: " << argv[0] << " <archivo-asm> <archivo-r2k>\n";
    cerr << "Laboratorio de Arquitectura (Marzo/1998)\n";
    cerr << "Jacobo Jimenez Martinez\n";
    exit(1);
  }
  ifstream fin(argv[1], ios::in );
  if(!fin) 
  { 
    cerr << "Error(502): No se puede abrir archivo.\n";
    exit(1);
  }
  ofstream fout(argv[2]);
  if(!fout) 
  {
    cerr << "Error(508): No se puede crear archivo.\n";
    exit(1);
  }
  
  // **************************** traduccion *******************************
  
  char car,car2;
  Operacion operac;
  int salir=0;
  
  while (!fin.eof()&&!salir)
  {
     fin >> car;
     switch(car) 
     {
        case 'a':
           fin >> car2;
           fin.ignore(3);
           if (car2=='d')
              operac.ADD(fin,fout);
           if (car2=='n')
              operac.AND(fin,fout); 
           break;
        case 'b':
           fin.ignore(4);
           operac.BEQ(fin,fout);
           break;
        case 'l':
           fin >> car2;
           fin.ignore(2);
           if (car2=='w')
             operac.LW(fin,fout);
           if (car2=='i')
             operac.LI(fin,fout);
           break;
        case 'o':
           fin >> car2;
           fin >> car2;
           
           if (car2=='i')
           {
             fin.ignore(2);
             operac.ORI(fin,fout);
           }
           else
           {
              operac.OR(fin,fout);
           }
           break;
        case 's':
           fin >> car2;
           switch (car2)
           {
              case 'l':
                 fin.ignore(3);
                 operac.SLT(fin,fout);
                 break;
              case 'u':
                 fin.ignore(3);
                 operac.SUB(fin,fout);
                 break;
              case 'w':
                 fin.ignore(2);
                 operac.SW(fin,fout);
                 break;
           }
           break;
        case 'j':
           fin.ignore(1);
           operac.J(fin,fout);
           break;
        case '.':
           fin.ignore(2);
           operac.W(fin,fout);
           break;
        case 'e':
           salir=1;
           break;
        default :
           cerr << "Error(585): Se ha obtenido un dato inesperado: "
                << "\"" << car << "\"\n";
           exit(1);
     }
     fout << (char)10;
  } // while 
  fin.close();
  fout.close();
} // main
