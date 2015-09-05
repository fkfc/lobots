#ifndef verticalscrollbar_H
#define verticalscrollbar_H

#include "scrollbar.h"
#include "buttonup.h"
#include "buttondown.h"

class TVerticalScrollBar : public TScrollBar {
  public:
      TVerticalScrollBar(TWidget *Parent);
      TVerticalScrollBar(TWidget *Parent, Scrollable *ObjectToConnect);
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
