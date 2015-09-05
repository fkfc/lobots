#include "label.h"


TLabel::TLabel(TWidget *Parent) : TButton(Parent) {
}

void TLabel::Draw(int w, int h) {
     int fposx = (GetScreenX());
     int fposy = (GetScreenY()) + this->Height/2 - Font->Height/2;     
     
     TButton::TStyle *S = &(TButton::Style);
     Font->Draw(fposx, fposy, Text, S->Font.R, S->Font.G, S->Font.B, w, h); 
}
