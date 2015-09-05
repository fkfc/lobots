/* caixaentrada.h
*
* Fornece uma caixa de entrada texto para ser usada em conjunto com o glut
*/


#ifndef CaixaentradaH
#define CaixaentradaH



class TCaixaEntrada {
      public:
             TCaixaEntrada();
             char buffer[250];
             bool Entrada(char letra); //retorna true qdo terminado
             bool cancelado;
      private:
              int x;
              int y;
              int w;
              int h;
              int pos;
              int i;
};

#endif
