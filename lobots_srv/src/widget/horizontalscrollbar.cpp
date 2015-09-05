#include "horizontalscrollbar.h"
#include "scrollbar.h"
#include <string.h>
#include <stdio.h>
#include "scrollable.h"

THorizontalScrollBar::THorizontalScrollBar(TWidget *Parent) : TScrollBar(Parent) {
  Construct();
}

THorizontalScrollBar::THorizontalScrollBar(TWidget *Parent, Scrollable *ObjectToConnect) : TScrollBar(Parent, ObjectToConnect) {
  Construct();
}

const char* THorizontalScrollBar::GetType() {
  return "horizontalscrollbar";
}


void THorizontalScrollBar::MouseMove(int X, int Y) {
  int ProgressIndicator_Y = ProgressIndicator->Y;
  int ProgressIndicator_X = ProgressIndicator->X;
  
  this->TWindow::MouseMove(X, Y);
  ProgressIndicator->Y = ProgressIndicator_Y;
  
  if (ProgressIndicator_X != ProgressIndicator->X) {
    float min = 2;
    float max = ProgressIndicatorRail->Width - 2 - ProgressIndicator->Width;
    if (ProgressIndicator->X < min) ProgressIndicator->X = min;
    if (ProgressIndicator->X > max) ProgressIndicator->X = max;
    
    float proportion_den = max - min;
    if (proportion_den == 0) return;
    float pos = ProgressIndicator->X - (min + 1);
    float proportion = pos/proportion_den;
    float m = ConnectedObject->GetHorizontalMaxScroll();
    float s = ConnectedObject->GetHorizontalQtyShown();
    float p = proportion * (m - s) + 1;
    int scrollpos = p;
    if (ConnectedObject != NULL) ConnectedObject->SetHorizontalScrollPosition(scrollpos);
    
    
  }
  
}



void THorizontalScrollBar::Construct() {
  
  ProgressIndicatorRail = new TWindow(this);
  ProgressIndicatorRail->draggable = false;
  
  ButtonUp = new TButtonLeft(this);
  ButtonUp->SetName("ButtonLeft");
  ButtonUp->ConnectedObject = this;
  
  ButtonDown = new TButtonRight(this);
  ButtonDown->SetName("ButtonRight");
  ButtonDown->ConnectedObject = this;
  
  ProgressIndicator = new TDraggableButton(ProgressIndicatorRail);
  ProgressIndicator->SetName("ProgressIndicator");
  ProgressIndicator->ConnectedObject = this;
  
  ProgressIndicatorSize = 1;
  ProgressIndicatorPosition = 0.5;
  
  SetDefaultStyle();
}

void THorizontalScrollBar::Resize(int W, int H) {
  this->TWindow::Resize(W, H);
  
  ButtonUp->Y = 3;
  ButtonUp->Width = 14;
  ButtonUp->X = 2;
  ButtonUp->Height = this->Height - 6;
  
  ButtonDown->Y = 3;
  ButtonDown->Width = 14;
  ButtonDown->X = this->Width - ButtonDown->Width - 2;
  ButtonDown->Height = this->Height - 6;
  
  
  ProgressIndicatorRail->Y = 2;
  ProgressIndicatorRail->X = 4 + ButtonUp->Width + ButtonUp->X;
  ProgressIndicatorRail->Height = this->Height - 5;
  ProgressIndicatorRail->Width = this->Width - 3 - ProgressIndicatorRail->X - (this->Width - ButtonDown->X);
  
  
  ProgressIndicator->Y = 1;
  ProgressIndicator->Height = this->Height - 7;
  
  RefreshPosition();
  
}




void THorizontalScrollBar::SetProgressIndicatorSize(float proportion) {
  if (proportion > 1) proportion = 1;
  float size = (ProgressIndicatorRail->Width - 4)*proportion;
  if (size <= 0) size = 1;
  ProgressIndicator->Width = size;
  ProgressIndicator->X -= size/2;
}

void THorizontalScrollBar::SetProgressIndicatorPosition(float proportion) {
  float min = 0;
  float max = ProgressIndicatorRail->Width - 4;
  float pos = proportion*((max - ProgressIndicator->Width) - min);
  
  if (pos <= 0) pos = 0;
  ProgressIndicator->X = pos + 2;
  
  
}


void THorizontalScrollBar::RefreshPosition() {
  if (ConnectedObject != NULL) {
    float p = ConnectedObject->GetHorizontalScrollPosition();
    float m = ConnectedObject->GetHorizontalMaxScroll();
    float s = ConnectedObject->GetHorizontalQtyShown();
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

