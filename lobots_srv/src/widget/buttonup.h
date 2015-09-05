#ifndef buttonup_H
#define buttonup_H

#include "button.h"


class TButtonUp : public TButton {
      public:
             TButtonUp(TWidget *Parent);
             void Draw(int w, int h);
};



#endif
