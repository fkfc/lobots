#ifndef blocksingle_H
#define blocksingle_H


#include "tipos.h"
#include "camera.h"
#include <GL/freeglut.h>
#include "block.h"


class TWorld;

class TBlockSingle : public TBlock {
      public:
             TBlockSingle(ptobj modelo, float x = 0, float y = 0, float z = 0);
             tcor cor;
             ptobj modelo;
             void Draw(TCamera *Camera, bool wire = false);
             void DrawBlinking(TCamera *Camera, float t);
             void DrawMini(float a, float d);
             void DrawMiniC(float a, float d); // rotaciona no centro
             void Move(int tecla, TCamera* Camera);
	     void MoveUp();
	     void MoveDown();
	     void RotateCW();
	     void RotateCCW();
	     void AddToWorld(TWorld *Mundo);
      private:
};
 


#endif