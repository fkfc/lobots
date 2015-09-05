#include "world.h"
#include "arquivo.h"
#include <string.h>
#include <stdio.h>
#define PI 3.14f
#include <math.h>


TWorld::TWorld() {
        for (int x = 0; x < MAX_X; x++) {
            for (int y = 0; y < MAX_Y; y++) {
                for (int z = 0; z < MAX_Z; z++) {
                    volumeMap[x][y][z] = 0;
                }
            }
        }
        
        blockCount = 0;   
        floor_y = MAX_Y/2 - 4*DIVISAO;            
}


void trim(char *str) {
    int i,j=0;
    
    for(i=0;str[i]!='\0';i++) {
        if (str[i] != ' ' && str[i] != '\t')
            str[j++]=str[i];
    }
    str[j]='\0';
} 

void TWorld::AddBlock(TBlockSingle *peca) {
	     if (peca->solido == true) {
		char strtemp[50];
		strcpy(strtemp, peca->modelo->nome);
		strtemp[strlen(strtemp) - 3] = 'v';  // abrir arquivo de volume da peça
		strtemp[strlen(strtemp) - 2] = 'o';
		strtemp[strlen(strtemp) - 1] = 'l';
		TArquivoLeitura *Arquivo = new TArquivoLeitura(strtemp);
		

		if (peca->ry < 0) peca->ry = 360 + peca->ry;
		peca->ry = ((int) peca->ry) % 360;
		
		
		char linha[50];
		int z = 0;
		int y = 0;
		bool colisao = false;


		int px = (int) ((peca->x*DIVISAO + 0.5)/2);
		int pz = (int) ((peca->z*DIVISAO + 0.5)/2);
		int py = (int) (peca->y*DIVISAO + 0.5);

		
		
		    while (Arquivo->GetProxLinha(linha)) {
				for (int x = 0; x < strlen(linha); x++) {
				    switch (  (((int) peca->ry) / 90) % 4  ) { 
					    case 0: // 0°
						if (linha[x] == 'X') volumeMap[px + x ][py + y][pz + z ] = peca;
						break;
					    case 1: // 90º
						if (linha[x] == 'X') volumeMap[px - z - 1 ][py + y][pz + x ] = peca;
						break;
					    case 2: // 180°
						if (linha[x] == 'X') volumeMap[px - x - 1 ][py + y][pz - z - 1 ] = peca;
						break;
					    case 3: // 270°
						if (linha[x] == 'X') volumeMap[px + z ][py + y][pz - x - 1 ] = peca;
						break;
				    }
				}   
				z++;
				if (linha[0] == '-') {
					    y++;
					    z = 0;
				}
		    }


		
		delete(Arquivo);
	     }
             
             blockList[blockCount] = peca;
             blockCount++;      
}

void TWorld::RemoveBlock(TBlockSingle *peca) {
             char strtemp[50];
             strcpy(strtemp, peca->modelo->nome);
             strtemp[strlen(strtemp) - 3] = 'v';  // abrir arquivo de volume da peça
             strtemp[strlen(strtemp) - 2] = 'o';
             strtemp[strlen(strtemp) - 1] = 'l';
             TArquivoLeitura *Arquivo = new TArquivoLeitura(strtemp);
             

             if (peca->ry < 0) peca->ry = 360 + peca->ry;
             peca->ry = ((int) peca->ry) % 360;
             
             
             char linha[50];
             int z = 0;
             int y = 0;
             bool colisao = false;


             int px = (int) ((peca->x*DIVISAO + 0.5)/2);
             int pz = (int) ((peca->z*DIVISAO + 0.5)/2);
             int py = (int) (peca->y*DIVISAO + 0.5);

             
            
                 while (Arquivo->GetProxLinha(linha)) {
                            for (int x = 0; x < strlen(linha); x++) {
                                 switch (  (((int) peca->ry) / 90) % 4  ) { 
                                        case 0: // 0°
                                             if (linha[x] == 'X') volumeMap[px + x ][py + y][pz + z ] = 0;
                                             break;
                                        case 1: // 90º
                                             if (linha[x] == 'X') volumeMap[px - z - 1 ][py + y][pz + x ] = 0;
                                             break;
                                        case 2: // 180°
                                             if (linha[x] == 'X') volumeMap[px - x - 1 ][py + y][pz - z - 1 ] = 0;
                                             break;
                                        case 3: // 270°
                                             if (linha[x] == 'X') volumeMap[px + z ][py + y][pz - x - 1 ] = 0;
                                             break;
                                 }
                            }   
                            z++;
                            if (linha[0] == '-') {
                                         y++;
                                         z = 0;
                            }
                 }


             
             delete(Arquivo);
             
           
 
              /*   int i = 0;
                 while (blockList[i] != peca) i++;
                 for (int j = i; j < blockCount - 1; j++) blockList[j] = blockList[j+1];
                 blockCount--;  
              */
              
              int i = 0;
              while ( i < blockCount && 
                     (peca->x != blockList[i]->x ||
                      peca->y != blockList[i]->y ||
                      peca->z != blockList[i]->z) )
                         i++;
              
              if (i < blockCount) {
                    for (int j = i; j < blockCount - 1; j++) blockList[j] = blockList[j+1];
                    blockCount--;
              }
}



bool TWorld::EvalCollision(TBlockSingle *peca) {     
	     if (peca->solido == false) return false;

             char strtemp[50];
             strcpy(strtemp, peca->modelo->nome);
             strtemp[strlen(strtemp) - 3] = 'v';  // abrir arquivo de volume da peça
             strtemp[strlen(strtemp) - 2] = 'o';
             strtemp[strlen(strtemp) - 1] = 'l';
             TArquivoLeitura *Arquivo = new TArquivoLeitura(strtemp);
             if (Arquivo->isOpen == false) {
	       delete (Arquivo);
	       return false;
	     }

             if (peca->ry < 0) peca->ry = 360 + peca->ry;
             peca->ry = ((int) peca->ry) % 360;
             
             
             char linha[50];
             int z = 0;
             int y = 0;
             bool colisao = false;
             bool ret = false;


             int px = (int) ((peca->x*DIVISAO + 0.5)/2);
             int pz = (int) ((peca->z*DIVISAO + 0.5)/2);
             int py = (int) (peca->y*DIVISAO + 0.5);

             
             
             do  {
                 colisao = false;  
                 y = 0;
                 z = 0;
                 while (Arquivo->GetProxLinha(linha)) {
                            for (int x = 0; x < strlen(linha); x++) {
                                 switch (  (((int) peca->ry) / 90) % 4  ) { 
                                        case 0: // 0°
                                             if (linha[x] == 'X' && volumeMap[px + x ][py + y][pz + z ]) colisao = true;
                                             break;
                                        case 1: // 90º
                                             if (linha[x] == 'X' && volumeMap[px - z - 1 ][py + y][pz + x ]) colisao = true;
                                             break;
                                        case 2: // 180°
                                             if (linha[x] == 'X' && volumeMap[px - x - 1 ][py + y][pz - z - 1 ]) colisao = true;
                                             break;
                                        case 3: // 270°
                                             if (linha[x] == 'X' && volumeMap[px + z ][py + y][pz - x - 1 ]) colisao = true;
                                             break;
                                 }
                            }   
                            z++;
                            if (linha[0] == '-') {
                                         y++;
                                         z = 0;
                            }
                 }
                 
                 if (colisao) {
                                   Arquivo->Reset();
                                   py++;
                                   peca->y += 1/DIVISAO;   
                                   ret = true;                         
                              }  
             } while (colisao);     

             
             delete(Arquivo);
             return ret;


     
}

TBlockSingle* TWorld::GetCollision(TBlockSingle *peca) {     
	     if (peca->solido == false) return 0;

             char strtemp[50];
             strcpy(strtemp, peca->modelo->nome);
             strtemp[strlen(strtemp) - 3] = 'v';  // abrir arquivo de volume da peça
             strtemp[strlen(strtemp) - 2] = 'o';
             strtemp[strlen(strtemp) - 1] = 'l';
             TArquivoLeitura *Arquivo = new TArquivoLeitura(strtemp);
             

             if (peca->ry < 0) peca->ry = 360 + peca->ry;
             peca->ry = ((int) peca->ry) % 360;
             
             
             char linha[50];
             int z = 0;
             int y = 0;
             bool colisao = false;


             int px = (int) ((peca->x*DIVISAO + 0.5)/2);
             int pz = (int) ((peca->z*DIVISAO + 0.5)/2);
             int py = (int) (peca->y*DIVISAO + 0.5);

             
             
             do  {
                 colisao = false;  
                 y = 0;
                 z = 0;
                 while (Arquivo->GetProxLinha(linha)) {
                            for (int x = 0; x < strlen(linha); x++) {
                                 switch (  (((int) peca->ry) / 90) % 4  ) { 
                                        case 0: // 0°
                                             if (linha[x] == 'X' && volumeMap[px + x ][py + y][pz + z ]) {
                                                          colisao = true;
                                                          delete(Arquivo);
                                                          return volumeMap[px + x ][py + y][pz + z ];
                                                          }
                                             break;
                                        case 1: // 90º
                                             if (linha[x] == 'X' && volumeMap[px - z - 1 ][py + y][pz + x ]) {
                                                          colisao = true;
                                                          delete(Arquivo);
                                                          return volumeMap[px - z - 1 ][py + y][pz + x ];
                                                          }
                                             break;
                                        case 2: // 180°
                                             if (linha[x] == 'X' && volumeMap[px - x - 1 ][py + y][pz - z - 1 ]) {
                                                          colisao = true;
                                                          delete(Arquivo);
                                                          return volumeMap[px - x - 1 ][py + y][pz - z - 1 ];
                                                          }
                                             break;
                                        case 3: // 270°
                                             if (linha[x] == 'X' && volumeMap[px + z ][py + y][pz - x - 1 ]) {
                                                          colisao = true;
                                                          delete(Arquivo);
                                                          return volumeMap[px + z ][py + y][pz - x - 1 ];
                                                          }
                                             break;
                                 }
                            }   
                            z++;
                            if (linha[0] == '-') {
                                         y++;
                                         z = 0;
                            }
                 }
                 
                 if (colisao) {
                                   Arquivo->Reset();
                                   py++;
                                   peca->y += 1/DIVISAO;                            
                              }  
             } while (colisao);     

             
             delete(Arquivo);
             
return 0;

     
}

//'derruba' a peça até encostar em outra, ou no grid da base 
void TWorld::DropBlock(TBlockSingle *Peca) {
    float oy = Peca->y;
                  
    while (Peca->y > 0 && GetCollision(Peca) == false) {
          Peca->y = Peca->y - 1/DIVISAO;
    }
    
    if (Peca->y <= 0) {
          Peca->y = MAX_Y/DIVISAO;
          while (Peca->y > 0 && GetCollision(Peca) == false) {
                Peca->y = Peca->y - 1/DIVISAO;
          }
    }
    
    if (Peca->y <= 0) Peca->y = floor_y/DIVISAO;
    return;
     
              
}

TBlockSingle* TWorld::GetCenterBlock(TCamera* camera) {
       //versores
       float xp =   0.2*cos(camera->rotx*PI/180)*sin(camera->roty*PI/180);
       float yp = - 0.1*sin(camera->rotx*PI/180);
       float zp = - 0.2*cos(camera->rotx*PI/180)*cos(camera->roty*PI/180);


       
       
       int xn, yn, zn;
             
       TBlockSingle* tmp = 0;
       bool para = false;
      
      
      int i  = 0;
      while(!tmp && i < 20) {
           float x = -camera->transx;
           float y = -camera->transy;
           float z = -camera->transz;
           
           para = false;        
           while (!tmp && !para) { // vetor para frente
                 x += xp;
                 y += yp;
                 z += zp;
                 
                 float xi, yi, zi;
                 for (int j = -i; j <= i; j++) {
                     for (int k = -i; k <= i; k++) {
                         for (int l = -i; l <= i; l++) {
                             xi = x + j*xp;
                             yi = y + k*yp;
                             zi = z + l*zp;
                         
                             xn = (int) ((xi*DIVISAO - 0.5)/2);
                             yn = (int) (yi*DIVISAO - 0.5);
                             zn = (int) ((zi*DIVISAO - 0.5)/2);
                             
                             if (xn < 0 || yn < 0 || zn < 0 || xn >= MAX_X || yn >= MAX_Y || zn >= MAX_Z) para = true;
                             else if (volumeMap[xn][yn][zn] != 0) {
                                  tmp = volumeMap[xn][yn][zn];
                             }
                             
                         }
                     }
                 }
           }
           i++;
           
       }
       
       if (!tmp && blockCount > 0) tmp = blockList[0];
       return tmp;
       

}
/*
void TWorld::SelecionaTecla(int key, TBlock**peca) {
       if (key == GLUT_KEY_PAGE_DOWN && blockCount > 0) {
	  
          //AddBlock(*peca);
          //*peca = blockList[0]; 
          //RemoveBlock(*peca);
       }
                     
}       

*/