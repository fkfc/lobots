#ifndef label_H
#define label_H

#include "button.h"

class TLabel : public TButton {
      public:
             TLabel(TWidget *Parent);
             void Draw(int w, int h);
}; 


#endif