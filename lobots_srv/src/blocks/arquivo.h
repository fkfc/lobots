/* arquivo.h
*
* TArquivoLeitura: Esta classe fornece m�todos para a leitura de arquivos de 
* texto
* TArquivoEscrita: Cria e escreve dados em arquivos bin�rios
*/

#ifndef ArquivoH
#define ArquivoH

#include <stdio.h>

#if defined(unix) || defined(__APPLE__)
  #define DIR_SEP '/'
#else 
  #define DIR_SEP '\\'
#endif


class TArquivoLeitura {
      public:
             TArquivoLeitura(char* nome_arq);
             bool GetProxLinha(char* linha);
             void GetXYZ(char *linha, float *x, float *y, float *z);
             ~TArquivoLeitura();
             void Reset();
	     bool isOpen;
      private:
             char *buffer_arq;
             long ponteiro;
             long tamanho;
};

class TArquivoEscrita {
      public:
             TArquivoEscrita(char* nome_arq);
             ~TArquivoEscrita();
             void Escrever(char *buffer, int count);
      private:
              FILE* arquivo;
};      
      
#endif      
