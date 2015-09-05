#ifndef block_H
#define block_H

#include "camera.h"


class TWorld;

typedef enum {TBLOCK, TBLOCKSINGLE, TBLOCKMULTIPLE} t_tipo_peca;

class TBlock {
      public:
	     TBlock() {};
             
	     t_tipo_peca tipo;
             float x, y, z;
             float ry; 
             float rx;
             bool solido;
	     
	     
             virtual void Draw(TCamera *Camera, bool wire = false) {};
             virtual void DrawBlinking(TCamera *Camera, float t) {};            
             virtual void Move(int tecla, TCamera* Camera) {};
	     virtual void MoveUp() {};
	     virtual void MoveDown() {};
	     virtual void RotateCW() {};
	     virtual void RotateCCW() {};
	     virtual void AddToWorld(TWorld *Mundo) {};
             float Distance (TCamera *Camera);
	     
      private:
};
 


#endif