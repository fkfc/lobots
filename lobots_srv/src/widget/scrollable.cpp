#include "scrollable.h"



Scrollable::Scrollable() {
}

void Scrollable::ScrollUp() {}
void Scrollable::ScrollDown() {}
void Scrollable::ScrollLeft() {}
void Scrollable::ScrollRight() {}

int Scrollable::GetVerticalScrollPosition() {
  return 0;
}

void Scrollable::SetVerticalScrollPosition(int position) {}

int Scrollable::GetVerticalMaxScroll() {
  return 0;
}

int Scrollable::GetVerticalQtyShown() {
  return 1;
}



int Scrollable::GetHorizontalScrollPosition() {
  return 0;
}

void Scrollable::SetHorizontalScrollPosition(int position) {}

int Scrollable::GetHorizontalMaxScroll() {
  return 0;
}

int Scrollable::GetHorizontalQtyShown() {
  return 1;
}