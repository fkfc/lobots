#ifndef textbox_H
#define textbox_H


#include "button.h"
#include "scrollable.h"

class TTextBox : public TButton, public Scrollable {
  public:
	  TTextBox(TWidget *Parent);
	  void Draw(int w, int h);
	  void ClickUp(int X, int Y);
	  void KeyPress(char key);
	  void MouseMove(int X, int Y);
	  int horizontalScrollPos;
	  void LoadFromXML(XMLElement *elementPointer);
	  
	  //Scrollable:
	  void ScrollLeft();
	  void ScrollRight();
	  int GetHorizontalScrollPosition();
	  void SetHorizontalScrollPosition(int position);
	  int GetHorizontalMaxScroll();
	  int GetHorizontalQtyShown();
  private:
	  void Construct();
      
};

#endif
