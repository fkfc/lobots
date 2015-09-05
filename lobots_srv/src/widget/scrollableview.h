#ifndef scrollableview_H
#define scrollableview_H

#include "window.h"
#include "scrollable.h"

class TScrollableView : public TWindow, public Scrollable {
  public:
    TScrollableView();
    TScrollableView( TWidget *Parent );
    virtual const char* GetType();
    void ClickDown(int X, int Y);
    void ClickUp(int X, int Y);
    void MouseMove(int X, int Y);
    void Draw(int w, int h);
    void LoadFromXML(XMLElement *elementPointer);
    
    //Scrollable
    void ScrollUp();
    void ScrollDown();
    void ScrollLeft();
    void ScrollRight();
    
    int GetVerticalScrollPosition();
    void SetVerticalScrollPosition(int position);
    int GetVerticalMaxScroll();
    int GetVerticalQtyShown();
    
    int GetHorizontalScrollPosition();
    void SetHorizontalScrollPosition(int position);
    int GetHorizontalMaxScroll();
    int GetHorizontalQtyShown();
  
  private:
    void Construct();
    int horizontalScrollPosition, verticalScrollPosition;
  
};

#endif
