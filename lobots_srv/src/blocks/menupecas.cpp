#include "menupecas.h"
#include "tipos.h"
#include <GL/freeglut.h>
#include "../widget/support/raster.h"
#include <math.h>

#define PI 3.14f





TMenuPecas::TMenuPecas(TBlockContainer *balde, TBlockContainer *balde_esp, TCamera* Camera) {
    pecaAtiva = -1;
    inicioLista = 0;
    Balde = balde;
    Balde_esp = balde_esp;
    camera = Camera;
    for (int i = 0; i < Balde->blockCount; i++) {
        ListaPecas[i] = new TBlockSingle(Balde->GetBlock(i), -1.6 + 0.3*i , 1.11, -2.5);
        GetCor(i, &ListaPecas[i]->cor);
    }
   
    IconeSelect = new TBlockSingle(Balde_esp->GetBlock(0), -1.3 , 0.9, -2.5);
    IconeSelect->rx = 40;
    
    Seta = new TBlockSingle(Balde_esp->GetBlock(5), -1.3 , 0.7, -2.5);
    Seta->rx = 230;
    GetCor(COR_PRETO, &Seta->cor) ;
    
    SetaInv = new TBlockSingle(Balde_esp->GetBlock(5), -1.3 , 0.5, -2.5);
    SetaInv->rx = 50;
    GetCor(COR_PRETO, &SetaInv->cor) ;
    
    Accept = new TBlockSingle(Balde_esp->GetBlock(7), -1.3 , 0.3, -2.5);
    GetCor(COR_VERDE, &Accept->cor) ;
    Accept->rx = -30;
    
    Delete = new TBlockSingle(Balde_esp->GetBlock(6), -1.3 , 0.1, -2.5);
    GetCor(COR_VERMELHO, &Delete->cor) ;
    Delete->rx = -20;
    
    
    //Connect = new TBlockSingle(Balde_esp->GetBlock(9), -1.3 , -0.4, -2.5);
    //GetCor(COR_VERDE, &Connect->cor) ;
    //Connect->rx = -30;
    
    New = new TBlockSingle(Balde_esp->GetBlock(8), -1.3 , -0.7, -2.5);
    GetCor(COR_BRANCO, &New->cor) ;
    New->rx = -30;
    
    IconeAbrir = new TBlockSingle(Balde_esp->GetBlock(1), -1.3 , -1.1, -2.5);
    IconeAbrir->cor.r = 75;
    IconeAbrir->cor.g = 60;
    IconeAbrir->cor.b = 30;
    
    Disk1 = new TBlockSingle(Balde_esp->GetBlock(2), -1.3 , -0.9, -2.5);
    GetCor(COR_PRETO, &Disk1->cor) ;
    Disk1->rx = -30;
    Disk2 = new TBlockSingle(Balde_esp->GetBlock(3), -1.3 , -0.9, -2.5);
    GetCor(COR_BRANCO, &Disk2->cor) ;
    Disk2->rx = -30;
    Disk3 = new TBlockSingle(Balde_esp->GetBlock(4), -1.3 , -0.9, -2.5);
    Disk3->cor.r = 100;
    Disk3->cor.g = 100;
    Disk3->cor.b = 127;
    Disk3->rx = -30;
    
    
 
    
    ultimacor = COR_VERMELHO;
    
    
}

void TMenuPecas::Draw(float t, bool mao, bool selecao){
    float largura = 640 * 0.005 * camera->janela_largura * 3 / ( camera->janela_altura * 4 );
    
    int j = inicioLista;
    for (int i = 0; i < 10; i++) {
        if (j >= Balde->blockCount) j = 0;
        ListaPecas[j]->x = -(largura/2) + (largura)*(i + 1)/12;
        if (j == pecaAtiva) ListaPecas[j]->DrawMiniC(t, 3);
        else ListaPecas[j]->DrawMiniC(0, 5);
        j++;
   }  
   
   IconeSelect->x =  -(largura/2) + largura/50;
   if (ferramentaAtiva == 0) IconeSelect->DrawMiniC(t,2);
   else IconeSelect->DrawMiniC(0,3);
   
   if (mao) {
       if (!selecao) {     
           Seta->x =  -(largura/2) + largura/50;
           if (ferramentaAtiva == 1) Seta->DrawMiniC(t,1.5);
           else Seta->DrawMiniC(0,2);
        
           SetaInv->x =  -(largura/2) + largura/50;
           if (ferramentaAtiva == 2) SetaInv->DrawMiniC(t,1.5);
           else SetaInv->DrawMiniC(180,2);   
           
           Delete->x =  -(largura/2) + largura/50;
           if (ferramentaAtiva == 4) Delete->DrawMini(0,1.5);
           else Delete->DrawMini(0,2);
       }
       
       Accept->x =  -(largura/2) + largura/50;
       if (ferramentaAtiva == 3) Accept->DrawMini(0,1.5);
       else Accept->DrawMini(0,2);
       
       
   }

   //Connect->x =  -(largura/2) + largura/50;
   //if (ferramentaAtiva == 7) Connect->DrawMiniC(t,1);
   //else Connect->DrawMiniC(0,1.3);   
   
   New->x =  -(largura/2) + largura/50;
   if (ferramentaAtiva == 9) New->DrawMini(t,1.5);
   else New->DrawMini(0,2);
   
   IconeAbrir->x =  -(largura/2) + largura/50;
   if (ferramentaAtiva == 12) IconeAbrir->DrawMini(t,2);
   else IconeAbrir->DrawMini(0,3);
   
   Disk1->x =  -(largura/2) + largura/50;
   if (ferramentaAtiva == 11) Disk1->DrawMini(t,1.5);
   else Disk1->DrawMini(0,2);
   Disk2->x =  -(largura/2) + largura/50;
   if (ferramentaAtiva == 11) Disk2->DrawMini(t,1.5);
   else Disk2->DrawMini(0,2);
   Disk3->x =  -(largura/2) + largura/50;
   if (ferramentaAtiva == 11) Disk3->DrawMini(t,1.5);
   else Disk3->DrawMini(0,2);
   
   
   
   glColor3b(0,0,0);
   Raster::ImprimeTexto(camera->janela_largura/12 - camera->janela_largura/20,camera->janela_largura/25,GLUT_BITMAP_HELVETICA_18,"<", camera->janela_largura, camera->janela_altura);
   Raster::ImprimeTexto(camera->janela_largura - camera->janela_largura/10,camera->janela_largura/25,GLUT_BITMAP_HELVETICA_18,">", camera->janela_largura, camera->janela_altura);
   


   

   
}

void TMenuPecas::MousePassivo(int x, int y) {
     pecaAtiva = ValidaMousePos(x, y);
     ferramentaAtiva = ValidaMousePosV(x, y);
     
}

bool TMenuPecas::MouseAtivo(int x, int y, TBlock **peca) {
     pecaAtiva = ValidaMousePos(x, y);
     ferramentaAtiva = ValidaMousePosV(x, y);
     
     if (pecaAtiva == -2) inicioLista++;
     if (pecaAtiva == -3) inicioLista--;
     if (inicioLista == -1) inicioLista = Balde->blockCount - 1;
     inicioLista = inicioLista % Balde->blockCount;
     
     if (pecaAtiva > -1) {
                   if (*peca != 0 && (*peca)->tipo == TBLOCKSINGLE) {
			     TBlockSingle* pecasimples = (TBlockSingle*) (*peca);
                             if (pecasimples->modelo == Balde->GetBlock(pecaAtiva)) ultimacor++;
                             else {
                                  float xp = (*peca)->x;
                                  float yp = (*peca)->y;
                                  float zp = (*peca)->z;
                                  delete(*peca);
                                  *peca = new TBlockSingle(Balde->GetBlock(pecaAtiva), xp, yp, zp);
                             }
                   } else {
                      float xp = -camera->transx + 6*cos(camera->rotx*PI/180)*sin(camera->roty*PI/180);
                      float yp = -camera->transy - 6*sin(camera->rotx*PI/180);
                      float zp = -camera->transz - 6*cos(camera->rotx*PI/180)*cos(camera->roty*PI/180);
                      
                      xp = ((int) (xp*DIVISAO/2))*2/DIVISAO;
                      zp = ((int) (zp*DIVISAO/2))*2/DIVISAO;
                      yp = ((int) (yp*DIVISAO))/DIVISAO;
                      
                      *peca = new TBlockSingle(Balde->GetBlock(pecaAtiva), xp, yp, zp);
                   }   
                   GetCor(ultimacor, &((TBlockSingle*) *peca)->cor);
                   return true;
     } else {
            if (ferramentaAtiva > -1) return true;
            }
     return false;
}

int TMenuPecas::ValidaMousePos(int x, int y) {
     if (y > camera->janela_altura/16) return -1;
     int quad = (x + (camera->janela_largura/36) ) /(camera->janela_largura/12);
     if (quad >= 11) return -2;
     if (quad < 1) return -3;

     return (quad - 1 + inicioLista) % Balde->blockCount;
}


int TMenuPecas::ValidaMousePosV(int x, int y) {
     if (x > camera->janela_largura/16) return -1;
     if (y <= camera->janela_altura/16) return -1;
     
     float offset = (camera->janela_altura / 16.0);
     float quad = (y - offset) / (camera->janela_altura / 15.0);
     
     int ret = ((int) quad - 1);
     
     if (ret > 2) { //corrige para icones no meio
             offset = (camera->janela_altura / 10.0);
             quad = (y - offset) / (camera->janela_altura / 15.0);
             ret = ((int) quad - 1);
     }

     
     
     if (ret > 9) { //corrige para icones embaixo
             offset = (camera->janela_altura / 16.0);
             quad = (y - offset) / (camera->janela_altura / 16.0);
             ret = ((int) quad - 1);
     }
     
     if (ret == 7) return -1; //connect desabilitado
     return ret;

    /* int quad = (x + (camera->janela_largura/36) ) /(camera->janela_largura/12);
     if (quad >= 11) return -2;
     if (quad < 1) return -3;

     return (quad - 1 + inicioLista) % Balde->blockCount;*/
}

