#ifndef scrollbar_H
#define scrollbar_H

#include "window.h"
#include "button.h"
#include "widget.h"
#include "buttonconnectable.h"
#include "draggablebutton.h"

class Scrollable;

class TScrollBar : public TWindow, public ButtonConnectable {
  public:
      TScrollBar(TWidget *Parent);
      TScrollBar(TWidget *Parent, Scrollable *ObjectToConnect);
      virtual const char* GetType();
      virtual void Resize(int W, int H);
      virtual void RefreshPosition();
      virtual void MouseMove(int X, int Y);
      
      void ConnectTo(Scrollable *Object);
      void Draw(int w, int h);
      void ClickUp(int X, int Y);
      void ClickDown(int X, int Y);
      void LoadFromXML(XMLElement *elementPointer);
      void (*OnClickScrollUp)(TScrollBar *ScrollBar);
      void (*OnClickScrollDown)(TScrollBar *ScrollBar);
      void ButtonAction(TButton *Button);
      
      
      
      class TStyle : public TWindow::TStyle {
	  public:
	    TStyle();
      } Style;
      
      
  protected:
      Scrollable *ConnectedObject;
      TDraggableButton *ProgressIndicator;
      TWindow *ProgressIndicatorRail;
      TButton *ButtonDown, *ButtonUp;
      float ProgressIndicatorSize, ProgressIndicatorPosition;
      void SetDefaultStyle();

  private:
      void Construct();
  
      
};


#endif

