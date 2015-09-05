#include "scrollableview.h"
#include "scrollbar.h"
#include "horizontalscrollbar.h"
#include "verticalscrollbar.h"
#include "widget.h"
#include "support/splitstr.h"

TScrollableView::TScrollableView()  : TWindow(), Scrollable() {
  Construct();
}

TScrollableView::TScrollableView( TWidget *Parent ) : TWindow(Parent), Scrollable() {
  Construct();
}

const char* TScrollableView::GetType() {
  return "scrollableview";
}


void TScrollableView::ClickDown(int X, int Y) {
  TWindow::ClickDown(X + horizontalScrollPosition, Y + verticalScrollPosition);
}

void TScrollableView::ClickUp(int X, int Y) {
  TWindow::ClickUp(X + horizontalScrollPosition, Y + verticalScrollPosition);
}

void TScrollableView::MouseMove(int X, int Y) {
  TWindow::MouseMove(X + horizontalScrollPosition, Y + verticalScrollPosition);
}


void TScrollableView::Draw(int w, int h) {
  if (!Visible) return;
  
  Raster::PushWorkingArea(GetWorkingArea());
  
  X -= horizontalScrollPosition;
  Y -= verticalScrollPosition;
  
  for (int i = 0; i < nChildren; i++) if (WidgetChildren[i]->Visible) WidgetChildren[i]->Draw(w, h);
  
  X += horizontalScrollPosition;
  Y += verticalScrollPosition;
  
  Raster::PopWorkingArea();
}


void TScrollableView::LoadFromXML(XMLElement *elementPointer) {
  
  
  this->TWindow::LoadFromXML(elementPointer);
  
  //Connect to a scrollbar, if specified
  if (elementPointer->FirstChildElement("scrollbar") != NULL) {
    char names[2][255];
    const char* scrollbarList = elementPointer->FirstChildElement("scrollbar")->GetText();
    splitstr(scrollbarList, ',', names[0], names[1]);
    
    for (int i = 0; i < 2; i++) {
      const char *scrollbarName = names[i];
      if (strlen(scrollbarName) > 0) {
	if (strcmp(Parent->GetChildByName(scrollbarName)->GetType(), "horizontalscrollbar") == 0 ) {
	  THorizontalScrollBar* ScrollBar = (THorizontalScrollBar*) Parent->GetChildByName(scrollbarName);
	  ScrollBar->ConnectTo(this);    
	  ScrollBar->RefreshPosition();
	} else {
	  TVerticalScrollBar* ScrollBar = (TVerticalScrollBar*) Parent->GetChildByName(scrollbarName);
	  ScrollBar->ConnectTo(this);    
	  ScrollBar->RefreshPosition();
	}
      }
    }
  }
  
}



void TScrollableView::ScrollUp() {
  if (verticalScrollPosition > 0) verticalScrollPosition--;
}

void TScrollableView::ScrollDown() {
  if (verticalScrollPosition + GetVerticalQtyShown() < GetVerticalMaxScroll()) verticalScrollPosition++;
}

void TScrollableView::ScrollLeft() {
  if (horizontalScrollPosition > 0) horizontalScrollPosition--;
}

void TScrollableView::ScrollRight() {
  if (horizontalScrollPosition + GetHorizontalQtyShown() < GetHorizontalMaxScroll()) horizontalScrollPosition++;
}


int TScrollableView::GetVerticalScrollPosition() {
  return verticalScrollPosition;
}

void TScrollableView::SetVerticalScrollPosition(int position) {
  verticalScrollPosition = position;
}

int TScrollableView::GetVerticalMaxScroll() {
  int max = 0;
  for (int i = 0; i < nChildren; i++) {
    int point = WidgetChildren[i]->Height + WidgetChildren[i]->Y;
    if (point > max) max = point;
  }
  return max;
}

int TScrollableView::GetVerticalQtyShown() {
  return this->Height;
}


int TScrollableView::GetHorizontalScrollPosition() {
  return horizontalScrollPosition;
}

void TScrollableView::SetHorizontalScrollPosition(int position) {
  horizontalScrollPosition = position;
}

int TScrollableView::GetHorizontalMaxScroll() {
  int max = 0;
  for (int i = 0; i < nChildren; i++) {
    int point = WidgetChildren[i]->Width + WidgetChildren[i]->X;
    if (point > max) max = point;
  }
  return max;
}

int TScrollableView::GetHorizontalQtyShown() {
  return this->Width;
}

void TScrollableView::Construct() {
  horizontalScrollPosition = 0;
  verticalScrollPosition = 0;
}



    