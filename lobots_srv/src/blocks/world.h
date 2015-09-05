/* world.h
*
* Classe e estrutura de dados que armazena a construção do usuário.
* Trata também as colisões entre as peças.
*/

#ifndef world_H
#define world_H
#include "blocksingle.h"
#include "balde.h"
#include "camera.h"
#include <GL/freeglut.h>

#define MAX_X 250
#define MAX_Y 250
#define MAX_Z 250
#define MAX_PECAS_MUNDO 2000

class TBlockSingle;

class TWorld {
      public:
             TWorld();
             void AddBlock(TBlockSingle *peca);
             void RemoveBlock(TBlockSingle *peca);
             bool EvalCollision(TBlockSingle *peca);
             void DropBlock(TBlockSingle *Peca); //'derruba' a peça até encostar em outra, ou no grid da base 
             TBlockSingle* GetCollision(TBlockSingle *peca);
             int blockCount;
             TBlockSingle *blockList[MAX_PECAS_MUNDO];
             TBlockSingle* GetCenterBlock(TCamera* camera);
             //void SelecionaTecla(int key, TBlock **peca);
             int floor_y;
             TBlockSingle *volumeMap[MAX_X][MAX_Y][MAX_Z];
              
};

#endif
