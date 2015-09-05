#ifndef draggablebutton_H
#define draggablebutton_H

#include "button.h"


class TDraggableButton : public TButton {
      public:
             TDraggableButton(TWidget *Parent); 
	     void ClickDown(int X, int Y);
             void ClickUp(int X, int Y);
             void MouseMove(int X, int Y);
}; 



#endif
