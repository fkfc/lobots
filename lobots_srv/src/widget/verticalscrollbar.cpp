#include "verticalscrollbar.h"
#include "scrollbar.h"
#include <string.h>
#include <stdio.h>
#include "scrollable.h"

TVerticalScrollBar::TVerticalScrollBar(TWidget *Parent) : TScrollBar(Parent) {
  Construct();
}

TVerticalScrollBar::TVerticalScrollBar(TWidget *Parent, Scrollable *ObjectToConnect) : TScrollBar(Parent, ObjectToConnect) {
  Construct();
}

const char* TVerticalScrollBar::GetType() {
  return "verticalscrollbar";
}

void TVerticalScrollBar::MouseMove(int X, int Y) {
  int ProgressIndicator_Y = ProgressIndicator->Y;
  int ProgressIndicator_X = ProgressIndicator->X;
  
  this->TWindow::MouseMove(X, Y);
  ProgressIndicator->X = ProgressIndicator_X;
  
  if (ProgressIndicator_Y != ProgressIndicator->Y) {
    float min = 2;
    float max = ProgressIndicatorRail->Height - 2 - ProgressIndicator->Height;
    if (ProgressIndicator->Y < min) ProgressIndicator->Y = min;
    if (ProgressIndicator->Y > max) ProgressIndicator->Y = max;
    
    float proportion_den = max - min;
    if (proportion_den == 0) return;
    float pos = ProgressIndicator->Y - (min + 2);
    float proportion = pos/proportion_den;
    float m = ConnectedObject->GetVerticalMaxScroll();
    float s = ConnectedObject->GetVerticalQtyShown();
    float p = proportion * (m - s) + 1;
    int scrollpos = p;
    ConnectedObject->SetVerticalScrollPosition(scrollpos);
    
    
  }
  
}



void TVerticalScrollBar::Construct() {
  
  ProgressIndicatorRail = new TWindow(this);
  ProgressIndicatorRail->draggable = false;
  
  ButtonUp = new TButtonUp(this);
  ButtonUp->SetName("ButtonUp");
  ButtonUp->ConnectedObject = this;
  
  ButtonDown = new TButtonDown(this);
  ButtonDown->SetName("ButtonDown");
  ButtonDown->ConnectedObject = this;
  
  ProgressIndicator = new TDraggableButton(ProgressIndicatorRail);
  ProgressIndicator->SetName("ProgressIndicator");
  ProgressIndicator->ConnectedObject = this;
  
  ProgressIndicatorSize = 1;
  ProgressIndicatorPosition = 0.5;
  
  SetDefaultStyle();
}

void TVerticalScrollBar::Resize(int W, int H) {
  this->TWindow::Resize(W, H);
  
  ButtonUp->X = 2;
  ButtonUp->Height = 14;
  ButtonUp->Y = 2;
  ButtonUp->Width = this->Width - 5;
  
  ButtonDown->X = 2;
  ButtonDown->Height = 14;
  ButtonDown->Y = this->Height - ButtonDown->Height - 2;
  ButtonDown->Width = this->Width - 5;
  
  
  ProgressIndicatorRail->X = 2;
  ProgressIndicatorRail->Y = 4 + ButtonUp->Height + ButtonUp->Y;
  ProgressIndicatorRail->Width = this->Width - 5;
  ProgressIndicatorRail->Height = this->Height - 3 - ProgressIndicatorRail->Y - (this->Height - ButtonDown->Y);
  
  ProgressIndicator->X = 1;
  ProgressIndicator->Width = this->Width - 7;
  
  RefreshPosition();
  
}




void TVerticalScrollBar::SetProgressIndicatorSize(float proportion) {
  if (proportion > 1) proportion = 1;
  float size = (ProgressIndicatorRail->Height - 4)*proportion;
  if (size <= 0) size = 1;
  ProgressIndicator->Height = size;
  ProgressIndicator->Y -= size/2;
}

void TVerticalScrollBar::SetProgressIndicatorPosition(float proportion) {
  float min = 0;
  float max = ProgressIndicatorRail->Height - 4;
  //float max = (this->Height - (ButtonUp->Height + ButtonUp->Y + 2) - (ButtonDown->Height + 2) - 6);
  float pos = proportion*((max - ProgressIndicator->Height) - min);
  
  if (pos <= 0) pos = 0;
  ProgressIndicator->Y = pos + 2;
  
  
}


void TVerticalScrollBar::RefreshPosition() {
  if (ConnectedObject != NULL) {
    float p = ConnectedObject->GetVerticalScrollPosition();
    float m = ConnectedObject->GetVerticalMaxScroll();
    float s = ConnectedObject->GetVerticalQtyShown();
    if (s > 0 && m > 0) ProgressIndicatorSize = s/m; else ProgressIndicatorSize = 1;
    if (m > 0) {
      if (p == 0) {
	ProgressIndicatorPosition = 0;
      } else {
	ProgressIndicatorPosition = (p - 1)/(m - s);
	if (ProgressIndicatorPosition > 1) ProgressIndicatorPosition = 1;
	if (p + s >= m) ProgressIndicatorPosition = 1;
      }
    }
    else ProgressIndicatorPosition = 0.5;
    
    //printf("DEBUG: p = %f m = %f s = %f size = %f pos = %f\n", p, m, s, ProgressIndicatorSize, ProgressIndicatorPosition); 
  }
  
  SetProgressIndicatorSize(ProgressIndicatorSize);
  SetProgressIndicatorPosition(ProgressIndicatorPosition);
  
}

