#ifndef horizontalscrollbar_H
#define horizontalscrollbar_H

#include "scrollbar.h"
#include "buttonleft.h"
#include "buttonright.h"

class THorizontalScrollBar : public TScrollBar {
  public:
      THorizontalScrollBar(TWidget *Parent);
      THorizontalScrollBar(TWidget *Parent, Scrollable *ObjectToConnect);
      virtual const char* GetType();
      void Resize(int W, int H);
      void MouseMove(int X, int Y);
      void SetProgressIndicatorSize(float proportion);
      void SetProgressIndicatorPosition(float position);
      void RefreshPosition();
     
      
  private:
      void Construct();       


        
};





#endif
