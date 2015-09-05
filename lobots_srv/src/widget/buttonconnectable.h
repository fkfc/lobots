#ifndef buttonconnectable_H
#define buttonconnectable_H

class TButton;

class ButtonConnectable {
  public:
    ButtonConnectable();
    virtual void ButtonAction(TButton *Button) = 0;  
};


#endif
