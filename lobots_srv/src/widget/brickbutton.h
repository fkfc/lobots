#ifndef brickbutton_H
#define brickbutton_H

#include "button.h"
#include "../blocks/blocksingle.h"

class TBrickButton : public TButton {
  public:
      TBrickButton(TWidget *Parent, TBlockSingle *Brick);
      TBrickButton(TWidget *Parent, TBlockSingle *Brick, ButtonConnectable *ConnectTo);
      virtual const char* GetType();
      void Draw(int w, int h);
	     
      
      TBlockSingle *Brick;
      
  private:
      void Construct(TBlockSingle *Brick);
      float brickWidth;
  
};




#endif
