#include "loadsave.h"
#include "arquivo.h"
#include "../endian/reverse_endianness.h"
#include <string.h>
#include <stdio.h>
#if defined(unix) || defined(__APPLE__)
	#include <unistd.h>
#endif

#define LOAD_DELAY 50



void Salvar(char *nome_ar, TWorld *Mundo, TCamera *Camera) {
          char nome_arq[255];
          sprintf(nome_arq, "save\\%s", nome_ar);
          nome_arq[4] = DIR_SEP;
          
          TArquivoEscrita *Arquivo = new TArquivoEscrita(nome_arq);
          
          char c = 2; //indicativo da versao do arquivo
          Arquivo->Escrever(&c, 1);

          float rotx = ReverseFloat(Camera->rotx);
          float roty = ReverseFloat(Camera->roty);
          float rotz = ReverseFloat(Camera->rotz);
          float transx = ReverseFloat(Camera->transx);
          float transy = ReverseFloat(Camera->transy);
          float transz = ReverseFloat(Camera->transz);
          int floor_y = ReverseInt(Mundo->floor_y);
          Arquivo->Escrever((char*) &rotx, sizeof(float));
          Arquivo->Escrever((char*) &roty, sizeof(float));
          Arquivo->Escrever((char*) &rotz, sizeof(float));
          Arquivo->Escrever((char*) &transx, sizeof(float));
          Arquivo->Escrever((char*) &transy, sizeof(float));
          Arquivo->Escrever((char*) &transz, sizeof(float));
          Arquivo->Escrever((char*) &floor_y, sizeof(int));
          
          for (int i = 0; i < Mundo->blockCount; i++) {
                    char tam = strlen(Mundo->blockList[i]->modelo->nome);
                    Arquivo->Escrever(&tam, 1);
                    Arquivo->Escrever(Mundo->blockList[i]->modelo->nome, tam+1);
                    float ry = ReverseFloat(Mundo->blockList[i]->ry);
                    float x = ReverseFloat(Mundo->blockList[i]->x);
                    float y = ReverseFloat(Mundo->blockList[i]->y);
                    float z = ReverseFloat(Mundo->blockList[i]->z);
                    Arquivo->Escrever((char*) &ry, sizeof(float));
                    Arquivo->Escrever((char*) &x, sizeof(float));
                    Arquivo->Escrever((char*) &y, sizeof(float));
                    Arquivo->Escrever((char*) &z, sizeof(float));
                    Arquivo->Escrever((char*) &Mundo->blockList[i]->cor.r, 1);
                    Arquivo->Escrever((char*) &Mundo->blockList[i]->cor.g, 1);
                    Arquivo->Escrever((char*) &Mundo->blockList[i]->cor.b, 1);
          }
          delete(Arquivo);
}

void Abrir(char *nome_ar, TWorld **Mundo, TBlockContainer* Balde, TCamera *Camera) {
   char nome_arq[255];
   sprintf(nome_arq, "save\\%s", nome_ar);
   nome_arq[4] = DIR_SEP;
   
   *Mundo = new TWorld();  
   FILE *arquivo = fopen(nome_arq, "rb");
   
   if (!feof(arquivo)) { //verifica se é a versão nova do save, que salva a posicao da camera
      char c;
      fread(&c, 1, 1, arquivo);
      if (c == 1 || c == 2) { // versão do arquivo
      
             float tx, ty, tz, rx, ry, rz; // coordenadas de rotação e translação da camera
             fread(&rx, 1, sizeof(float), arquivo); 
             fread(&ry, 1, sizeof(float), arquivo); 
             fread(&rz, 1, sizeof(float), arquivo); 
             fread(&tx, 1, sizeof(float), arquivo); 
             fread(&ty, 1, sizeof(float), arquivo); 
             fread(&tz, 1, sizeof(float), arquivo);          
             
             //Corrige o endianness, se necessário
             tx = ReverseFloat(tx);
             ty = ReverseFloat(ty);
             tz = ReverseFloat(tz);
             rx = ReverseFloat(rx);
             ry = ReverseFloat(ry);
             rz = ReverseFloat(rz);
             
             Camera->transx = tx;
             Camera->transy = ty;
             Camera->transz = tz;
             Camera->rotx = rx;
             Camera->roty = ry;
             Camera->rotz = rz;
             
             
             if  (c == 2) { // versao 2 inclui informacao da altura da base
                 int floor_y;
                 fread(&floor_y, 1, sizeof(int), arquivo);
                 floor_y = ReverseInt(floor_y);
                 (*Mundo)->floor_y = floor_y;
             }
      } else {
             rewind(arquivo);
      }
   }
                       
                       
   while (!feof(arquivo)) {
       char nome[50];
       float x, y, z, ry;
       char r, g, b, tam;
       
       fread(&tam, 1, 1, arquivo); 
       fread(&nome, 1, tam+1, arquivo); 
       fread(&ry, 1, sizeof(float), arquivo); 
       fread(&x, 1, sizeof(float), arquivo); 
       fread(&y, 1, sizeof(float), arquivo); 
       fread(&z, 1, sizeof(float), arquivo); 
       fread(&r, 1, 1, arquivo); 
       fread(&g, 1, 1, arquivo); 
       fread(&b, 1, 1, arquivo); 
       
       ry = ReverseFloat(ry);
       x = ReverseFloat(x);
       y = ReverseFloat(y);
       z = ReverseFloat(z);
      
       for (int i = 0; i < strlen(nome); i++) if (nome[i] == '\\' || nome[i] == '/') nome[i] = DIR_SEP;
       TBlockSingle *novaPeca = new TBlockSingle(Balde->GetBlock(nome), x, y, z);
       novaPeca->ry = ry;
       novaPeca->cor.r = r;
       novaPeca->cor.g = g;
       novaPeca->cor.b = b;
       if ( (*Mundo)->EvalCollision(novaPeca) == false ) {
           (*Mundo)->AddBlock(novaPeca);
	   /*
           if (Conexao && Conexao->connected) {
                       Conexao->Send(novaPeca);
    		   #if defined(unix) || defined(__APPLE__)
                          sleep(LOAD_DELAY);
    		   #else	
                          Sleep(LOAD_DELAY);
    		   #endif
                       }*/
        }
   }

     
   
   fclose (arquivo);

}
