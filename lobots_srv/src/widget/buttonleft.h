#ifndef buttonleft_H
#define buttonleft_H

#include "button.h"


class TButtonLeft : public TButton {
      public:
             TButtonLeft(TWidget *Parent);
             void Draw(int w, int h);
};



#endif
