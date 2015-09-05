#ifndef buttondown_H
#define buttondown_H

#include "button.h"


class TButtonDown : public TButton {
      public:
             TButtonDown(TWidget *Parent);
             void Draw(int w, int h);
}; 



#endif
