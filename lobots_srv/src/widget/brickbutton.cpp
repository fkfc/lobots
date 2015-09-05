#include "brickbutton.h"
#include "../blocks/tipos.h"

TBrickButton::TBrickButton(TWidget *Parent, TBlockSingle *Brick) : TButton (Parent) {
    Construct(Brick);
}     

TBrickButton::TBrickButton(TWidget *Parent, TBlockSingle *Brick, ButtonConnectable *ConnectTo) : TButton (Parent, ConnectTo) {
    Construct(Brick);
}

const char* TBrickButton::GetType() {
  return "brickbutton";
}
      
void TBrickButton::Draw(int w, int h) {
  
  float resolutionFactor = ((float) Display->GetHeight())/480.0;
  float resizeFactor = resolutionFactor*3.0*brickWidth*100.0/((float) Width);
  
  //cima 1.09  baixo -1.32 @ 800x600
  //esq -1.52    dir  1.73
  
  //dir 2.15 @ 1000
  
  TButton::TStyle *S = &Style;
  if (Pressed) S = &Selected;
  else if (Parent->MouseOver == this) S = &Hover;
  
  float brickScreenX = (float) GetScreenX() + (float) S->OffsetX;
  float brickScreenY = (float) GetScreenY() + (float) S->OffsetY;
  
  //-1..1
  float normalx = (brickScreenX/((float) Display->GetWidth()))*2 - 1;
  float normaly = -((brickScreenY/((float) Display->GetHeight()))*2 - 1);
  
  //float minx = 1.53*((float) Display->GetWidth())/800.0;
  float minx = 1.53 + (((float)Display->GetWidth()) - 800.0)*(1.93-1.53)/(1000.0-800.0);
        minx += (((float)Display->GetHeight()) - 600.0)*(1.17-1.53)/(800.0-600.0);
  //float maxx = 1.73*((float) Display->GetWidth())/800.0;
  float maxx = 1.73 + (((float)Display->GetWidth()) - 800.0)*(2.15-1.73)/(1000.0-800.0);
        maxx += (((float)Display->GetHeight()) - 600.0)*(1.30-1.73)/(800.0-600.0);
  
  //float miny = 1.32*((float) Display->GetHeight())/600.0;
  float miny = 1.32 + (((float) Display->GetHeight()) - 600.0)*(1.30-1.32)/(800.0-600.0);
  //float maxy = 1.09*((float) Display->GetHeight())/600.0;
  float maxy = 1.09 + (((float) Display->GetHeight()) - 600.0)*(1.13-1.09)/(800.0-600.0);
  
  float bx;
  float by;
  
  if (normalx > 0) bx = normalx*maxx;
  else bx = normalx*minx;
  
  if (normaly > 0) by = normaly*maxy;
  else by = normaly*miny;
  
  by -= ((brickWidth)/(resizeFactor))*(1 - normaly*normaly);
  bx += ((brickWidth)/(resizeFactor))*(1 - normalx*normalx)*0.5;
  
  Brick->x = bx;
  Brick->y = by;
  
  Brick->DrawMiniC(0, resizeFactor);
  
  //printf("brick (%.2f, %.2f) button (%d, %d) normal (%.2f, %.2f) display (%d, %d)\n", bx, by, GetScreenX(), GetScreenY(), normalx, normaly, Display->GetWidth(), Display->GetHeight());
  
  TButton::Draw(w, h);
}


void TBrickButton::Construct(TBlockSingle *Brick) {
  this->Brick = Brick;
  
  
  this->Brick = new TBlockSingle(LoadObject("pecas/placa1x1.bri"), 0 , 0, -2.5);
  
  GetCor(0, &this->Brick->cor);
  
  
 // printf("brick num_poligonos = %d\n", this->Brick->modelo->num_poligonos);
  
  float max_x = -9999;
  float min_x = 9999;
  for(int i = 0; i < this->Brick->modelo->num_poligonos; i++) {
    int verticea = this->Brick->modelo->poligono[i].a;
    int verticeb = this->Brick->modelo->poligono[i].b;
    int verticec = this->Brick->modelo->poligono[i].c;
    int verticed = this->Brick->modelo->poligono[i].d;
    
    float xi;
    
    xi = this->Brick->modelo->vertice[verticea].x;
    if (xi > max_x) max_x = xi;
    if (xi < min_x) min_x = xi;
    
    xi = this->Brick->modelo->vertice[verticec].x;
    if (xi > max_x) max_x = xi;
    if (xi < min_x) min_x = xi;
    
    xi = this->Brick->modelo->vertice[verticeb].x;
    if (xi > max_x) max_x = xi;
    if (xi < min_x) min_x = xi;
    
    xi = this->Brick->modelo->vertice[verticed].x;
    if (xi > max_x) max_x = xi;
    if (xi < min_x) min_x = xi;
    
  }
  
  
  
  brickWidth = max_x - min_x;
  
  
  
  
}
