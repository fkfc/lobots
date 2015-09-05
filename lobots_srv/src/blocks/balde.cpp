#include "balde.h"

#include "arquivo.h"
#include <string.h>
#include <stdlib.h>

TBlockContainer::TBlockContainer(char* arquivoLista) {                  
   TArquivoLeitura *Arquivo;
   blockCount = 0;

   Arquivo = new TArquivoLeitura(arquivoLista);
   char * brickFileName = (char*) malloc(sizeof(char)*255);
   while (Arquivo->GetProxLinha(brickFileName)) { // build the bricks' file list
       fileList.push_back(brickFileName);
       blockList.push_back(LoadObject(brickFileName)); //load each brick's vertex list
       blockCount++; 
       brickFileName = (char*) malloc(sizeof(char)*255);
   }
   free(brickFileName);
   delete(Arquivo);
   

}

TBlockContainer::~TBlockContainer() {
  
}

ptobj TBlockContainer::GetBlock(int i) {
      return blockList[i];
}


ptobj TBlockContainer::GetBlock(char *nome) {
      ptobj aux = 0;
      int i = 0;
      aux = blockList[0];
	
      while (i < blockCount && strcmp(blockList[i]->nome, nome) != 0) {
            i++;
            aux = blockList[i];
            }
      return aux;
      
}


