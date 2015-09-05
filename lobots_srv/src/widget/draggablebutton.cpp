#include "draggablebutton.h"

TDraggableButton::TDraggableButton(TWidget *Parent) : TButton(Parent) { }

void TDraggableButton::ClickDown(int X, int Y) {
  this->TWindow::ClickDown(X, Y);
  this->TButton::ClickDown(X, Y);
}

void TDraggableButton::ClickUp(int X, int Y) {
  this->TWindow::ClickUp(X, Y);
  this->TButton::ClickUp(X, Y);
}

void TDraggableButton::MouseMove(int X, int Y) {
  this->TWindow::MouseMove(X,Y);
}

