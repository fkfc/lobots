#include "caixaentrada.h"
#include <GL/freeglut.h>


TCaixaEntrada::TCaixaEntrada() {
                                 i = 0;    
                                 buffer[0] = '_';                     
                                 buffer[1] = '\0'; 
                                 cancelado = false;                    
}                                 

bool TCaixaEntrada::Entrada(char letra) {
     if (letra == 13) {
               buffer[i] = '\0';
               return true;
     }
     if (letra == 27) {
               cancelado = true;
               return true;
     }
     if (letra == 8) {
              if (i > 0) i--;
     } else {
          buffer[i] = letra;
          i++;
     }
     buffer[i] = '_';
     buffer[i+1] = '\0';
     return false;
}


