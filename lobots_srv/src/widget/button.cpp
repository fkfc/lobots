#include "button.h"
#include "buttonconnectable.h"
#include "support/splitstr.h"

TButton::TButton(TWidget *Parent) : TWindow (Parent) {
   Construct();
}     

TButton::TButton(TWidget *Parent, ButtonConnectable *ConnectTo) {
  Construct();
  this->ConnectedObject = ConnectTo;
}

const char* TButton::GetType() {
  return "button";
}

void TButton::Draw(int w, int h) {
if (!Visible) return;
  
     //if (Height < Font->Height + 5) Height = Font->Height + 5;
     //if (Width < Font->Width*strlen(Text) + 5) Width = Font->Width*strlen(Text) + 5;

     TButton::TStyle *S = &Style;
     if (Pressed) S = &Selected;
     else if (Parent->MouseOver == this) S = &Hover; 
     
     
     int fposx = (GetScreenX()) + Width / 2 - strlen(Text)*Font->Width/2;
     int fposy = (GetScreenY()) + Height/2 - Font->Height/2 + 1;
     

     Raster::PushWorkingArea(GetWorkingArea());
     Font->Draw(fposx + S->OffsetX, fposy + S->OffsetY, Text, S->Font.R, S->Font.G, S->Font.B, w, h);          
     Raster::PopWorkingArea();
     
     
     //box
     Raster::SetColor(S->Background.R, S->Background.G, S->Background.B);
     
     Raster::RoundedRectangle(GetScreenX() + S->OffsetX, GetScreenY() + S->OffsetY, GetScreenX() + Width + S->OffsetX, GetScreenY() + Height + S->OffsetY, S->Border.Radius, w, h);
    
     //Border
     Raster::SetColor(S->Border.R, S->Border.G, S->Border.B);
     Raster::RoundedRectangle(GetScreenX() - S->Border.Width + S->OffsetX,
			      GetScreenY() - S->Border.Width + S->OffsetY,
			      GetScreenX() + Width + S->Border.Width + S->OffsetX, 
			      GetScreenY() + Height + S->Border.Width + S->OffsetY, 
			      S->Border.Radius, 
			      w, 
			      h);
     
     //Shadow
     Raster::SetColor(S->Shadow.R, S->Shadow.G, S->Shadow.B); 
     Raster::RoundedRectangle(GetScreenX() - S->Border.Width + S->Shadow.OffsetX,
			      GetScreenY() - S->Border.Width + S->Shadow.OffsetY,
			      GetScreenX() + Width + S->Border.Width + S->Shadow.OffsetX,
			      GetScreenY() + Height + S->Border.Width + S->Shadow.OffsetY,
			      S->Border.Radius, 
			      w, h);
     
}

void TButton::ClickDown(int X, int Y) {
     this->Pressed = true;
}

void TButton::ClickUp(int X, int Y) {
   this->Pressed = false;
     
   if (this->IsMouseOver(X + this->X, Y + this->Y)) {
     if (ConnectedObject != NULL) ConnectedObject->ButtonAction(this);
     if (OnClick != NULL) OnClick(this);
   }
}     
     
void TButton::SetText(const char* Text) {
     if (Text != NULL) strcpy(this->Text, Text);
     else strcpy(this->Text, "");
}

void TButton::MouseMove(int X, int Y) {
    
}  

void TButton::LoadFromXML(XMLElement *elementPointer) {
  if (elementPointer->FirstChildElement("text") != NULL) {
    SetText(elementPointer->FirstChildElement("text")->GetText());
  }
  
  if (elementPointer->FirstChildElement("style") != NULL) {
    Style.LoadFromXML(elementPointer->FirstChildElement("style"), this);
  }
  
  this->TWindow::LoadFromXML(elementPointer);
}


void TButton::Construct() {

  
  Visible = true;
  Pressed = false;
  strcpy(this->Text, "");
  OnClick = NULL;
  Font = new TFont("fonte_8_13.bmp", 8, 13);
  this->Width = 0;
  this->Height = 0;
  ConnectedObject = NULL;
  
  
  //Style
  Selected.Background.R = 144;
  Selected.Background.G = 201;
  Selected.Background.B = 227;
  Selected.Border.R = 144;
  Selected.Border.G = 201;
  Selected.Border.B = 227;
  Selected.OffsetX = 1;
  Selected.OffsetY = 1;
  Selected.Font.R = 237;
  Selected.Font.G = 238;
  Selected.Font.B = 239;
  
  Hover.Border.R = 144;
  Hover.Border.G = 201;
  Hover.Border.B = 227;
  Hover.Shadow.R = 158;
  Hover.Shadow.G = 211;
  Hover.Shadow.B = 236;
  

}





TButton::TStyle::TStyle() : TWindow::TStyle() {
 
  OffsetX = 0;
  OffsetY = 0;
  
  Background.R = 237;
  Background.G = 238;
  Background.B = 239;

  Border.Width = 1;
  Border.Radius = 2;
  Border.R = 182;
  Border.G = 184;
  Border.B = 187;
  
  Shadow.OffsetX = 1;
  Shadow.OffsetY = 1;
  Shadow.R = 213;
  Shadow.G = 215; 
  Shadow.B = 216; 
  
  Font.R = 69;
  Font.G = 73;
  Font.B = 78;
 

}




void TButton::TStyle::LoadFromXML(XMLElement *elementPointer, TWidget *Widget) {
  this->TWindow::TStyle::LoadFromXML(elementPointer, Widget);
  
  char xstr[5], ystr[5], zstr[5];
  
  if (elementPointer->FirstChildElement("offset") != NULL) {
      splitstr(elementPointer->FirstChildElement("offset")->GetText(), ',', xstr, ystr);
      OffsetX = atoi(xstr);
      OffsetY = atoi(ystr);
  }
  
  if (elementPointer->FirstChildElement("shadow") != NULL) {
    XMLElement *p = elementPointer->FirstChildElement("shadow");
    
    if (p->FirstChildElement("offset") != NULL) {
      splitstr(p->FirstChildElement("offset")->GetText(), ',', xstr, ystr);
      Shadow.OffsetX = atoi(xstr);
      Shadow.OffsetY = atoi(ystr);
    }

    if (p->FirstChildElement("color") != NULL) {
      splitstr(p->FirstChildElement("color")->GetText(), ',', xstr, ystr);
      Shadow.R = atoi(xstr);
      splitstr(ystr, ',', xstr, zstr);
      Shadow.G = atoi(xstr);
      Shadow.B = atoi(zstr);
    }
    
  }
  
  
  if (elementPointer->FirstChildElement("font") != NULL) {
    XMLElement *p = elementPointer->FirstChildElement("font");
    
    if (p->FirstChildElement("color") != NULL) {
      splitstr(p->FirstChildElement("color")->GetText(), ',', xstr, ystr);
      Font.R = atoi(xstr);
      splitstr(ystr, ',', xstr, zstr);
      Font.G = atoi(xstr);
      Font.B = atoi(zstr);
    }
    
  }
  
  
  if (elementPointer->FirstChildElement("selected") != NULL) {
    ((TButton*) Widget)->Selected = ((TButton*) Widget)->Style;
    ((TButton*) Widget)->Selected.LoadFromXML(elementPointer->FirstChildElement("selected"), Widget);
  }
  
  if (elementPointer->FirstChildElement("hover") != NULL) {
    ((TButton*) Widget)->Hover = ((TButton*) Widget)->Style;
    ((TButton*) Widget)->Hover.LoadFromXML(elementPointer->FirstChildElement("hover"), Widget);
  }
}