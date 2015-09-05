#include "tipos.h"
#include <math.h>
#include "arquivo.h"
#include "string.h"
#include <stdlib.h>

void normalizar(float *x, float *y, float *z)
{
    float tamanho = (float)(sqrt( (*x)*(*x) + (*y)*(*y) + (*z)*(*z) ));
    if (tamanho == 0.0f) tamanho = 1.0f;

    *x /= tamanho;
    *y /= tamanho;
    *z /= tamanho;
}

void normal (float ax,float ay,float az,float bx,float by,float bz,float cx,float cy,float cz,float *x, float *y, float *z)
{
    float dx, dy, dz, ex, ey, ez;

    dx = ax - bx; //calcula os vetores iniciais
    dy = ay - by;
    dz = az - bz;
    
    ex = bx - cx;
    ey = by - cy;
    ez = bz - cz;

    *x = (dy * ez) - (dz * ey); //vetor normal
    *y = (dz * ex) - (dx * ez);
    *z = (dx * ey) - (dy * ex);


    normalizar(x,y,z);
}


void GetCor(int n_cor, tcor* cor) {
n_cor = n_cor % 8;

switch (n_cor) {
       case COR_VERMELHO:
            cor->r = 127;
            cor->g = 0;
            cor->b = 0;
            break;
       case COR_AMARELO:
            cor->r = 127;
            cor->g = 127;
            cor->b = 0;            
            break;
       case COR_AZUL:
            cor->r = 0;
            cor->g = 0;
            cor->b = 127;            
            break;
       case COR_VERDE:
            cor->r = 0;
            cor->g = 90;
            cor->b = 0;            
            break;
       case COR_PRETO:
            cor->r = 15;
            cor->g = 15;
            cor->b = 15;            
            break;
       case COR_BRANCO:
            cor->r = 120;
            cor->g = 120;
            cor->b = 120;            
            break;
       case COR_CINZA:
            cor->r = 60;
            cor->g = 60;
            cor->b = 60;            
            break;
       case COR_MARROM:
            cor->r = 75;
            cor->g = 25;
            cor->b = 0;            
            break;
       }
}



ptobj LoadObject(const char *fileName) {
    ptobj brickModel = (ptobj) malloc(sizeof(tobj));
    TArquivoLeitura *file;
    
    char fileNameRW[255];
    strcpy(fileNameRW, fileName);
    for (int i = 0; i < strlen(fileNameRW); i++) if (fileNameRW[i] == '\\' || fileNameRW[i] == '/') fileNameRW[i] = DIR_SEP;  
       file = new TArquivoLeitura(fileNameRW);
       strcpy(brickModel->nome, fileNameRW);
       
       
       char linha_temp[255];
       
       
       file->GetProxLinha(linha_temp);
       int vert = 0;
       int polig = 0;
       do {  
             float x[4], y[4], z[4];
             for (int i = 0; i < 4; i++) {               // le uma face -> 4 vertices
                 file->GetXYZ(linha_temp, &x[i], &y[i], &z[i]);                 
                 brickModel->vertice[vert + i].x =  x[i] / DIVISAO;
                 brickModel->vertice[vert + i].y =  y[i] / DIVISAO;
                 brickModel->vertice[vert + i].z =  z[i] / DIVISAO;
                 
                 if (i < 3) file->GetProxLinha(linha_temp);

             }
       
           brickModel->poligono[polig].a = vert;  
           brickModel->poligono[polig].b = vert + 1;
           brickModel->poligono[polig].c = vert + 2;
           brickModel->poligono[polig].d = vert + 3;
           polig++;
           
           vert = vert + 4;
       } while (file->GetProxLinha(linha_temp));
       brickModel->num_poligonos = polig;
       
       delete(file);
       
       return brickModel;
}