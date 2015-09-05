#include "textbox.h"
#include "horizontalscrollbar.h"

TTextBox::TTextBox(TWidget *Parent) : TButton(Parent) {
  Construct();
}


void TTextBox::Draw(int w, int h) {
     int x = Parent->X;
     int y = Parent->Y;     
    
     int Height = this->Height - 4;
     int Width = this->Width - 4;

     int fposx = (GetScreenX()) + 2;
     int fposy = (GetScreenY()) + this->Height/2 - Font->Height/2 + 2;
     
     char TextShown[WIDG_MAX_TEXT];
     int NumShown = Width/Font->Width;
     int StartIndex = 0;
     if (HasFocus && NumShown - 1 < strlen(Text)) StartIndex = strlen(Text) - NumShown + 1;
     else StartIndex = horizontalScrollPos;
     
     strcpy(TextShown, &Text[StartIndex]);
     if (HasFocus) strcat(TextShown, "_");
     TextShown[NumShown] = 0;
     
     Font->Draw(fposx, fposy, TextShown, 0, 0, 0, w, h);    

     Pressed = HasFocus;
     strcpy(TextShown, Text);
     SetText("");
     this->TButton::Draw(w, h);
     SetText(TextShown);
     
}   
     
void TTextBox::ClickUp(int X, int Y) {
     if (this->IsMouseOver(X + Parent->X, Y + Parent->Y)) {                  
         if  (OnClick) OnClick(this);
     }
     
}

void TTextBox::KeyPress(char key) {
     if (key == 13) return;
     if (key == 8) {
         if (strlen(Text) > 0) Text[strlen(Text) - 1] = 0;    
     } else {
         char entrada[2];
         sprintf(entrada, "%c", key);
         strcat(Text, entrada);
     }
}     

void TTextBox::MouseMove(int X, int Y) {
     glutSetCursor(GLUT_CURSOR_TEXT);
} 





void TTextBox::LoadFromXML(XMLElement *elementPointer) {
  this->TButton::LoadFromXML(elementPointer);
  
  //Connect to a scrollbar, if specified
  if (elementPointer->FirstChildElement("scrollbar") != NULL) {
    THorizontalScrollBar *ScrollBar = (THorizontalScrollBar*) Parent->GetChildByName(elementPointer->FirstChildElement("scrollbar")->GetText());
    ScrollBar->ConnectTo(this);    
    ScrollBar->RefreshPosition();
  }
  
}




void TTextBox::ScrollLeft() {
  if (horizontalScrollPos > 0) horizontalScrollPos--;
}

void TTextBox::ScrollRight() {
  if (horizontalScrollPos + GetHorizontalQtyShown() < GetHorizontalMaxScroll()) horizontalScrollPos++;
}

int TTextBox::GetHorizontalScrollPosition() {
  return horizontalScrollPos;
}

void TTextBox::SetHorizontalScrollPosition(int position) {
  horizontalScrollPos = position;
}

int TTextBox::GetHorizontalMaxScroll() {
  return strlen(Text);
}

int TTextBox::GetHorizontalQtyShown() {
  return Width/Font->Width;
}


void TTextBox::Construct() {
  HasFocus = false;
  horizontalScrollPos = 0;
  
  
  TButton::Style.Background.R = 255;
  TButton::Style.Background.G = 255;
  TButton::Style.Background.B = 255;
  TButton::Style.Border.R = 182;
  TButton::Style.Border.G = 184;
  TButton::Style.Border.B = 187;
  TButton::Style.Border.Width = 1;
  TButton::Style.Border.Radius = 0;
  TButton::Style.Shadow.OffsetX = 0;
  TButton::Style.Shadow.OffsetY = 0;
  
  TButton::Selected = TButton::Style;
  TButton::Selected.Border.R = 144;
  TButton::Selected.Border.G = 201;
  TButton::Selected.Border.B = 227;
  TButton::Selected.OffsetX = 0;
  TButton::Selected.OffsetY = 0;
  
  TButton::Hover = TButton::Style;
  
  
}
