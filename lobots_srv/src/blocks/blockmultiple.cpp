#include "blockmultiple.h"
#include "world.h"
#include <stdio.h>

TBlockMultiple::TBlockMultiple(float x, float y, float z) {
  lista_pecas_count = 0;
  this->x = x;
  this->y = y;
  this->z = z;
  this->solido = false;
  tipo = TBLOCKMULTIPLE;
}

TBlockMultiple::TBlockMultiple(TWorld *Mundo) {  //Importa do mundo
  TBlockSingle *Peca;
  for (int i = 0; i < Mundo->blockCount; i++) {
    Peca = Mundo->blockList[i];
    //Converter as coorodenadas da peça para relativas
    int px, py, pz;
    
    //Adicionar ao conjunto
    AddBlock(Peca, px, py, pz);
  }
  
  
}


void TBlockMultiple::AddBlock(TBlock *Peca, int x, int y, int z) {
  lista_pecas[lista_pecas_count].Peca = Peca;
  lista_pecas[lista_pecas_count].x = x;
  lista_pecas[lista_pecas_count].y = y;
  lista_pecas[lista_pecas_count].z = z;
  
  //o xyz do objeto Peça não importa! É utilizada o xyz do objeto PecaMultipla junto com o xyz deste método que diz a coordenada relativa
  
  lista_pecas_count++;
  EvaluateBricksCoordinates();
  
  
  
  
}

void TBlockMultiple::RemoveBlock(int i) {
  printf("\nremover\n");
  for (int j = i; j < lista_pecas_count - 1; j++) {
     lista_pecas[j].Peca = lista_pecas[j+1].Peca;
     lista_pecas[j].x = lista_pecas[j+1].x;
     lista_pecas[j].y = lista_pecas[j+1].y;
     lista_pecas[j].z = lista_pecas[j+1].z;
  }
  if (i < lista_pecas_count) lista_pecas_count--;
}

void TBlockMultiple::Draw(TCamera *Camera, bool wire) {
  for (int i = 0; i < lista_pecas_count; i++) {
      lista_pecas[i].Peca->Draw(Camera, wire);
  }
}

void TBlockMultiple::DrawBlinking(TCamera *Camera, float t) {
  for (int i = 0; i < lista_pecas_count; i++) {
      lista_pecas[i].Peca->DrawBlinking(Camera, t);
  }
}

void TBlockMultiple::Move(int tecla, TCamera* Camera) {
  
  switch(tecla) {
      case GLUT_KEY_UP : 
		      this->z += 2 / DIVISAO * ((((int) (-this->ry / 90) % 4) - 1) % 2) * -((((int) ((Camera->roty + 30) / 90) % 4) - 1) % 2);
		      this->x += 2 / DIVISAO * ((((int) (-this->ry / 90) % 4) - 1) % 2) * -((((int) ((Camera->roty + 120) / 90) % 4) - 1) % 2);

		      this->z += 2 / DIVISAO * ((((int) ((-this->ry + 90)/ 90) % 4) - 1) % 2) * -((((int) ((Camera->roty + 30) / 90) % 4) - 1) % 2);
		      this->x += 2 / DIVISAO * ((((int) ((-this->ry + 90)/ 90) % 4) - 1) % 2) * -((((int) ((Camera->roty + 120) / 90) % 4) - 1) % 2);
      break;
      case GLUT_KEY_DOWN: 
      this->z -= 2 / DIVISAO * ((((int) (-this->ry / 90) % 4) - 1) % 2) * -((((int) ((Camera->roty + 30) / 90) % 4) - 1) % 2);
		      this->x -= 2 / DIVISAO * ((((int) (-this->ry / 90) % 4) - 1) % 2) * -((((int) ((Camera->roty + 120) / 90) % 4) - 1) % 2);

		      this->z -= 2 / DIVISAO * ((((int) ((-this->ry + 90)/ 90) % 4) - 1) % 2) * -((((int) ((Camera->roty + 30) / 90) % 4) - 1) % 2);
		      this->x -= 2 / DIVISAO * ((((int) ((-this->ry + 90)/ 90) % 4) - 1) % 2) * -((((int) ((Camera->roty + 120) / 90) % 4) - 1) % 2);
      break;
      case GLUT_KEY_LEFT: 
      this->x += 2 / DIVISAO * ((((int) (-this->ry / 90) % 4) - 1) % 2) * -((((int) ((Camera->roty + 30) / 90) % 4) - 1) % 2);
		      this->z -= 2 / DIVISAO * ((((int) (-this->ry / 90) % 4) - 1) % 2) * -((((int) ((Camera->roty + 120) / 90) % 4) - 1) % 2);

		      this->x += 2 / DIVISAO * ((((int) ((-this->ry + 90) / 90) % 4) - 1) % 2) * -((((int) ((Camera->roty + 30) / 90) % 4) - 1) % 2);
		      this->z -= 2 / DIVISAO * ((((int) ((-this->ry + 90) / 90) % 4) - 1) % 2) * -((((int) ((Camera->roty + 120) / 90) % 4) - 1) % 2);
      break;
      case GLUT_KEY_RIGHT: 
      this->x -= 2 / DIVISAO * ((((int) (-this->ry / 90) % 4) - 1) % 2) * -((((int) ((Camera->roty + 30) / 90) % 4) - 1) % 2);
		      this->z += 2 / DIVISAO * ((((int) (-this->ry / 90) % 4) - 1) % 2) * -((((int) ((Camera->roty + 120) / 90) % 4) - 1) % 2);

		      this->x -= 2 / DIVISAO * ((((int) ((-this->ry + 90) / 90) % 4) - 1) % 2) * -((((int) ((Camera->roty + 30) / 90) % 4) - 1) % 2);
		      this->z += 2 / DIVISAO * ((((int) ((-this->ry + 90) / 90) % 4) - 1) % 2) * -((((int) ((Camera->roty + 120) / 90) % 4) - 1) % 2);
      break;
  }
  
  EvaluateBricksCoordinates();

}


void TBlockMultiple::AddToWorld(TWorld *Mundo) {
  for (int i = 0; i < lista_pecas_count; i++) {
    lista_pecas[i].Peca->AddToWorld(Mundo);
  }
}


void TBlockMultiple::MoveUp() {
  y += 1/DIVISAO;
  EvaluateBricksCoordinates();
}

void TBlockMultiple::MoveDown() {
  y -= 1/DIVISAO;
  EvaluateBricksCoordinates();
}


//Recalcula o xyz da subpeça, utilizando coordenada relativa da peça somada à coordenada deste objeto
void TBlockMultiple::EvaluateBricksCoordinates() {
  TBlock *Peca;
  for (int i = 0; i < lista_pecas_count; i++) {
      Peca = lista_pecas[i].Peca;
      //printf("conjunto %f  relativo %f\n", this->x, (float) lista_pecas[i].x);
      Peca->x = this->x + ((float) lista_pecas[i].x)*2.0/DIVISAO;
      Peca->y = this->y + ((float) lista_pecas[i].y)*1.0/DIVISAO;
      Peca->z = this->z + ((float) lista_pecas[i].z)*2.0/DIVISAO;
      //printf("peca %f\n", Peca->x);
  }
}




void TBlockMultiple::RotateCW() {
  TBlock *Peca;
  for (int i = 0; i < lista_pecas_count; i++) {
      Peca = lista_pecas[i].Peca;
      float tx = lista_pecas[i].x;
      float tz = lista_pecas[i].z;
      lista_pecas[i].z = tx;
      lista_pecas[i].x = -tz;
      Peca->RotateCW();
  }
  EvaluateBricksCoordinates();
}

void TBlockMultiple::RotateCCW() {
  TBlock *Peca;
  for (int i = 0; i < lista_pecas_count; i++) {
      Peca = lista_pecas[i].Peca;
      float tx = lista_pecas[i].x;
      float tz = lista_pecas[i].z;
      lista_pecas[i].z = -tx;
      lista_pecas[i].x = tz;
      Peca->RotateCCW();
  }
  EvaluateBricksCoordinates();
}