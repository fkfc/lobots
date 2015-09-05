#include "widget.h"
#include "support/splitstr.h"
#include <string.h>
#include "loadwindow.h"

const char  *widget_type_str[] = {
  "window", 
  "button", 
  "label", 
  "textbox", 
  "listbox", 
  "buttonup", 
  "buttondown", 
  "buttonleft", 
  "buttonright", 
  "brickbutton", 
  "scrollbar", 
  "draggablebutton", 
  "scrollableview",
  "canvas"  
};


TWidget::TWidget() {
   Construct();
}

TWidget::TWidget(TWidget *Parent ) { 
   Construct();
   SetParent(Parent);
}

TWidget::~TWidget() {
  DeleteAllChildren();
}

const char* TWidget::GetType() {
  return "widget";
}


void TWidget::Draw(int w, int h) {
    //Draw children widgets
    Raster::PushWorkingArea(GetWorkingArea());

    for (int i = 0; i < nChildren; i++) if (WidgetChildren[i]->Visible) WidgetChildren[i]->Draw(w, h);

    Raster::PopWorkingArea();
}

void TWidget::SetParent(TWidget *Parent) {
  this->Parent = Parent;
  Parent->AddChildren(this); 
}

void TWidget::DeleteAllChildren() {
  int n = nChildren;
  for (int i = 0; i < n; i++) {
    DeleteChild(WidgetChildren[0]);
  }
}

void TWidget::DeleteChild(TWidget *Child) {
  int i = 0;
  while (i < nChildren && WidgetChildren[i] != Child) i++;
  if (i < nChildren) {
    for (int j = i; j < nChildren - 1; j++) {
      WidgetChildren[j] = WidgetChildren[j + 1];
    }
    nChildren--;
  }
  delete(Child);
  
}




void TWidget::AddChildren( TWidget *Children ) {
   Children->SetDisplay(this->Display);
   WidgetChildren[nChildren] =  Children;
   nChildren++;                    
}

void TWidget::SetDisplay(TDisplay *Display) {
     this->Display = Display;
     for (int i = 0; i < nChildren; i++) WidgetChildren[i]->SetDisplay(Display);
     
}

void TWidget::SetFocus(bool focus) {
     HasFocus = focus;
     for (int i = 0; i < nChildren; i++) WidgetChildren[i]->SetFocus(false);
}

void TWidget::SetName(const char *name) {
  strcpy(this->name, name);
}

const char* TWidget::GetName() {
  return this->name;
}

TWidget* TWidget::GetChildByName(const char *name) {
  for (int i = 0; i < nChildren; i++) {
    if (strcmp(name, WidgetChildren[i]->GetName()) == 0) return WidgetChildren[i];
  }
  
  
  for (int i = 0; i < nChildren; i++) {
    TWidget *Child = WidgetChildren[i]->GetChildByName(name);
    if (Child != NULL) return Child;
  }
  
  return NULL;
}

void TWidget::PrintChildrenNames() {
    
  for (int i = 0; i < nChildren; i++) {
    printf("%s\n", WidgetChildren[i]->GetName());
  }
  
  
  for (int i = 0; i < nChildren; i++) {
    WidgetChildren[i]->PrintChildrenNames();
  }

}

TWidget* TWidget::GetParentByType(const char *type) {
  //printf("DEBUG GetParentByType %s\n", type);
  if (this->Parent == NULL) return NULL;
  if ( strcmp(this->Parent->GetType(), type) == 0 ) return this->Parent;
  return Parent->GetParentByType(type);
}

int TWidget::GetScreenX() {
  if (this->Parent == NULL) {
    return this->X;
  } else {
    return this->X + Parent->GetScreenX();
  } 
}

int TWidget::GetScreenY() {
  if (this->Parent == NULL) {
    return this->Y;
  } else {
    return this->Y + Parent->GetScreenY();
  } 
}

void TWidget::Resize(int W, int H) {
  this->Width = W;
  this->Height = H;
}

void TWidget::Hide() {
    Hide(this);
}

void TWidget::Show() {
    Show(this);
}
            
void TWidget::Hide(TWidget *Widget) {
    Widget->Visible = false;
}

void TWidget::Show(TWidget *Widget) {
    Widget->Visible = true;
}

void TWidget::Construct() {
  X = 0;
  Y = 0;          
  Width = 0;
  Height = 0;
  nChildren = 0;
  Visible = true;
  HasFocus = false;
  this->Parent = NULL;
}




void TWidget::LoadFromXML(XMLElement *elementPointer) {
  char xstr[5], ystr[5];
  splitstr(elementPointer->FirstChildElement("position")->GetText(), ',', xstr, ystr);
  X = atoi(xstr);
  Y = atoi(ystr);
  
  if (elementPointer->FirstChildElement("size") != NULL) {
    splitstr(elementPointer->FirstChildElement("size")->GetText(), ',', xstr, ystr);
    Resize(atoi(xstr), atoi(ystr));
  }
  
  if (elementPointer->FirstChildElement("name") != NULL) {
    SetName(elementPointer->FirstChildElement("name")->GetText());
  }
  
  //are there nested widgets? if so, load them as well
  elementPointer = elementPointer->FirstChildElement("widget");
  while (elementPointer != NULL) {
    TWidget *NewWidget = LoadWidgetFromElement(elementPointer, this);  
    elementPointer = elementPointer->NextSiblingElement("widget");
  }
    
}


TWidget::TStyle::TStyle() {
}

void TWidget::DeleteLater() {
  Display->DeleteLater(this);
}

void TWidget::TStyle::LoadFromXML(XMLElement *elementPointer, TWidget *Widget) {
}

t_box TWidget::GetWorkingArea() {
  t_box box;
  box.X = this->GetScreenX();
  box.Y = this->GetScreenY();
  box.Width = this->Width;
  box.Height = this->Height;
  
  return box;
}
