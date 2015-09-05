/* balde.h
*
* Define as classes e m�dotos referentes �s pe�as e fornece uma classe
* para obt�-las
* 
*/



#ifndef BaldeH
#define BaldeH

#define MAX_BALDE 50


#if defined(unix) || defined(__APPLE__)
  #define DIR_SEP '/'
#else 
  #define DIR_SEP '\\'
#endif


#include "tipos.h"
#include <vector>

class TBlockContainer {
      public:
             TBlockContainer(char* arquivoLista);
             ~TBlockContainer();
             ptobj GetBlock(int i);
             ptobj GetBlock(char *nome);
             int blockCount;
             std::vector<ptobj> blockList; 
             std::vector<char*> fileList;
      private:

};

#endif
