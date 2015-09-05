#include "scrollbar.h"
#include "scrollable.h"

TScrollBar::TScrollBar(TWidget *Parent) : TWindow(Parent) {
  this->ConnectedObject = NULL;
  Construct();
}

TScrollBar::TScrollBar(TWidget *Parent, Scrollable *ObjectToConnect) : TWindow(Parent) {
  ConnectTo(ObjectToConnect);
  Construct();
}

const char* TScrollBar::GetType() {
  return "scrollbar";
}

void TScrollBar::ConnectTo(Scrollable *Object) {
  this->ConnectedObject = Object;
}

void TScrollBar::Draw(int w, int h) {
  this->TWindow::Style = (TWindow::TStyle) Style;
  this->TWindow::Draw(w, h);
}

void TScrollBar::ClickUp(int X, int Y) {
  this->TWindow::ClickUp(X, Y);
}

void TScrollBar::ClickDown(int X, int Y) {
  this->TWindow::ClickDown(X, Y);
  window_moving = false;
}

void TScrollBar::MouseMove(int X, int Y) {
  
}

void TScrollBar::LoadFromXML(XMLElement *elementPointer) {
  this->TWindow::LoadFromXML(elementPointer);
  
  if (elementPointer->FirstChildElement("style") != NULL) {
    ((TWindow::TStyle*) &Style)->LoadFromXML(elementPointer->FirstChildElement("style"), this);
    
    if (elementPointer->FirstChildElement("style")->FirstChildElement("button") != NULL) {
      ((TButton::TStyle*) &(ButtonUp->Style))->LoadFromXML(elementPointer->FirstChildElement("style")->FirstChildElement("button"), ButtonUp);
      ((TButton::TStyle*) &(ButtonDown->Style))->LoadFromXML(elementPointer->FirstChildElement("style")->FirstChildElement("button"), ButtonDown);
    }
    
    if (elementPointer->FirstChildElement("style")->FirstChildElement("rail") != NULL) {
      ((TWindow::TStyle*) &(ProgressIndicatorRail->Style))->LoadFromXML(elementPointer->FirstChildElement("style")->FirstChildElement("rail"), ProgressIndicatorRail);
    }
    
    if (elementPointer->FirstChildElement("style")->FirstChildElement("cursor") != NULL) {
      ((TButton::TStyle*) &(ProgressIndicator->Style))->LoadFromXML(elementPointer->FirstChildElement("style")->FirstChildElement("cursor"), ProgressIndicator);
    }
    
    
  }
}


void TScrollBar::Resize(int W, int H) {
  
  
}

void TScrollBar::ButtonAction(TButton *Button) {
  //printf("DEBUG: scrollbar %s , button %s press\n", GetName(), Button->GetName());
  if (ConnectedObject != NULL) {
    if (strcmp(Button->GetName(), "ButtonUp") == 0) ConnectedObject->ScrollUp();
    else if (strcmp(Button->GetName(), "ButtonDown") == 0) ConnectedObject->ScrollDown();
    else if (strcmp(Button->GetName(), "ButtonLeft") == 0) ConnectedObject->ScrollLeft();
    else if (strcmp(Button->GetName(), "ButtonRight") == 0) ConnectedObject->ScrollRight();
  }
  
  RefreshPosition();
}





void TScrollBar::RefreshPosition() {
  
}



TScrollBar::TStyle::TStyle() : TWindow::TStyle() {

  Background.R = 200;
  Background.G = 200;
  Background.B = 220;

  Border.Width = 0;
  Border.Radius = 1;
  Border.R = 100;
  Border.G = 100;
  Border.B = 100;
}


void TScrollBar::Construct() {
}

void TScrollBar::SetDefaultStyle() {
  ProgressIndicatorRail->Style.Background.R = 239;
  ProgressIndicatorRail->Style.Background.G = 240;
  ProgressIndicatorRail->Style.Background.B = 241;
  ProgressIndicatorRail->Style.Border.Width = 1;
  ProgressIndicatorRail->Style.Border.Radius = 5;
  ProgressIndicatorRail->Style.Border.R = 217;
  ProgressIndicatorRail->Style.Border.G = 218;
  ProgressIndicatorRail->Style.Border.B = 219;
}