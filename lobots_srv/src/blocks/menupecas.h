/* menupeças.h
*
* Contém a classe que manipula o menu de peças
*/

#ifndef MenupecasH
#define MenupecasH

#include "balde.h"
#include "camera.h"
#include "tipos.h"
#include "world.h"

class TMenuPecas {
      public:
             TMenuPecas(TBlockContainer *balde, TBlockContainer *balde_esp, TCamera *Camera);
             int pecaAtiva;
             int ferramentaAtiva;
             void Draw(float t, bool mao, bool selecao);    
             void MousePassivo(int x, int y); 
             bool MouseAtivo(int x, int y, TBlock **peca);
             TCamera *camera;
             int inicioLista;        
      private:
              TBlockContainer *Balde, *Balde_esp;
              TBlockSingle *ListaPecas[MAX_BALDE];
              int ValidaMousePos(int x, int y);
              int ValidaMousePosV(int x, int y);
              int ultimacor;
              TBlockSingle *IconeSelect, *IconeAbrir, *Disk1, *Disk2, *Disk3, *Seta, *SetaInv, *Delete, *Accept, *New, *Connect;

              
      
};

#endif
