#include "arquivo.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


TArquivoLeitura::TArquivoLeitura(char* nome_arq) {
     
   char filen[255];
   strcpy(filen, nome_arq);
   for (int i = 0; i < strlen(filen); i++) if (filen[i] == '\\' || filen[i] == '/') filen[i] = DIR_SEP;
   FILE *arquivo = fopen(filen, "rb");
   
   if (arquivo == NULL) {
     isOpen = false;
     return;
   }
   
   isOpen = true;
   
   fseek(arquivo , 0, SEEK_END);
   tamanho = ftell(arquivo);
   rewind(arquivo);

   buffer_arq = (char*) malloc(sizeof(char)*tamanho); // aloca tamanho do arquivo na ram
   fread(buffer_arq, 1, tamanho, arquivo); // copia arquivo p memoria
   fclose (arquivo);
   
   ponteiro = 0;
  
} 

bool TArquivoLeitura::GetProxLinha(char *linha) {
     int i = 0;
     bool ret = false;
     while (ponteiro < tamanho && buffer_arq[ponteiro] != 13 && buffer_arq[ponteiro] != 10) {
           linha[i] = buffer_arq[ponteiro];
           i++;
           ponteiro++;
           ret = true;
           }
      linha[i] = 0;
      
      if (!ret && ponteiro < tamanho) { // pula linhas em branco
               ponteiro++;
               ret = GetProxLinha(linha);
      }
      
      return ret;
              
}  

void TArquivoLeitura::GetXYZ(char *linha, float *x, float *y, float *z) {
  char temp[10];
  float xyz[3];

  int pont = 0;
  int pont_temp;
  
  for (int i = 0; i < 3; i++) {
      pont_temp = 0;
      while ( (unsigned char) linha[pont] == '.' || 
              (unsigned char) linha[pont] == '-' || 
              (unsigned char) linha[pont] < 58 && 
              (unsigned char) linha[pont] > 47) {
                                temp[pont_temp] = linha[pont];
                                pont++;
                                pont_temp++;
      }
      temp[pont_temp] = 0;
      while (linha[pont] == ' ') pont++;
      xyz[i] = atof(temp);
  }
  
  *x = xyz[0];
  *y = xyz[1];
  *z = xyz[2];
  

     
}

TArquivoLeitura::~TArquivoLeitura() {
   free(buffer_arq);
}

void TArquivoLeitura::Reset() {
     ponteiro = 0;
}

TArquivoEscrita::TArquivoEscrita(char* nome_arq) {
          char filen[255];
          strcpy(filen, nome_arq);
          for (int i = 0; i < strlen(filen); i++) if (filen[i] == '\\' || filen[i] == '/') filen[i] = DIR_SEP;
          arquivo = fopen(filen, "wb");
                                           
}           

TArquivoEscrita::~TArquivoEscrita() {
          fclose(arquivo);
                                           
}              

void TArquivoEscrita::Escrever(char *buffer, int count) {
     fwrite (buffer , 1 , count , arquivo );
}
