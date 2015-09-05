#ifndef blockmultiple_H
#define blockmultiple_H

#include "block.h"

#define MAX_PECAS_MAO 250

class TWorld;

class TBlockMultiple : public TBlock {
        public:
             TBlockMultiple(float x, float y, float z);
	     TBlockMultiple(TWorld *Mundo); //Importa do mundo

	     
	     struct {
		TBlock *Peca;
		int x; //Coordenadas relativas
		int y;
		int z;
	     } lista_pecas[MAX_PECAS_MAO];
	     int lista_pecas_count;
	     
	     void AddBlock(TBlock *Peca, int x, int y, int z);
	     void RemoveBlock(int i);
	     
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
             float Distance (TCamera *Camera);
      private:
	     void EvaluateBricksCoordinates();
};

#endif
