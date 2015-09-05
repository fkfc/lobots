#ifndef buttonright_H
#define buttonright_H

#include "button.h"


class TButtonRight : public TButton {
      public:
             TButtonRight(TWidget *Parent);
             void Draw(int w, int h);
};



#endif
