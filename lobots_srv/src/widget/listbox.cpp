#include "listbox.h"
#include "verticalscrollbar.h"
#include "horizontalscrollbar.h"
#include "support/splitstr.h"

TListBox::TListBox(TWidget *Parent) : TButton(Parent), Scrollable() {
    Items = 0;
    SelectedItem = -1;
    Scroll = 0;
    OnClick = 0;
    
    
    Selected.Font.R = 255;
    Selected.Font.G = 255;
    Selected.Font.B = 255;
    Selected.Font.BackgroundR = 144;
    Selected.Font.BackgroundG = 201;
    Selected.Font.BackgroundB = 227;
    
}

void TListBox::AddItem(const char* Text) {
     strcpy(List[Items], Text);
     Items++;
}

void TListBox::RemoveItem(int i) {
    for (int j = i; j < Items - 1; j++) {
      strcpy(List[j], List[j+1]);
    }
    Items--;
    if (Items < 0) Items = 0;
}


void TListBox::Draw(int w, int h) {
     int x = Parent->X;
     int y = Parent->Y;     
    
     int Height = this->Height - 4;
     int Width = this->Width - 4;

     int fposx = (x + X) + 2;
     int max_items = this->Height / (Font->Height + 1);
     for (int i = Scroll; i < Items && i - Scroll < max_items ; i++) {
         int fposy = (y + Y) + 3 + (i - Scroll)*(Font->Height + 1);
         
         char TextShown[WIDG_MAX_TEXT];
         int NumShown = Width/Font->Width;
         int StartIndex = 0;
         strcpy(TextShown, &List[i][StartIndex]);
         TextShown[StartIndex + NumShown] = 0;
         
         
         if (i == SelectedItem) {
	       Font->Draw(fposx, fposy, TextShown, Selected.Font.R, Selected.Font.G, Selected.Font.B, w, h); 
	       Raster::SetColor(Selected.Font.BackgroundR, Selected.Font.BackgroundG, Selected.Font.BackgroundB);
               Raster::Box( x + X + 2, (y + Y) + (i - Scroll)*(Font->Height + 1) + 2 , x + X + Width, (y + Y) + (i + 1 - Scroll)*(Font->Height + 1), w, h);
         } else {
	      Font->Draw(fposx, fposy, TextShown, Style.Font.R, Style.Font.G, Style.Font.B, w, h); 
	 }
     }
     
     this->TWindow::Style = (TWindow::TStyle) Style;
     this->TWindow::Draw(w, h);
    
    
     
     
}

void TListBox::ClickDown(int X, int Y) {
  
    
     int Height = this->Height - 4;
     int Width = this->Width - 4;
     
     int max_items = this->Height / (Font->Height + 1);
     for (int i = Scroll; i < Items && i - Scroll < max_items ; i++) {
         int x1 = 2;
         int y1 = (i - Scroll)*(Font->Height + 1) + 2;
         int x2 =  Width;
         int y2 = (i + 1 - Scroll)*(Font->Height + 1);
         if (X  >= x1 && X <= x2 && Y >= y1 && Y <= y2) SelectedItem = i;
     }

}

void TListBox::ClickUp(int X, int Y) {
   if ( (OnClick) ) OnClick(this);     
}  


void TListBox::LoadFromXML(XMLElement *elementPointer) {
  
  
  XMLElement *contentPointer, *itemPointer;
  //Load list contents, if any
  contentPointer = elementPointer->FirstChildElement("content");
  if (contentPointer != NULL) {
    itemPointer = contentPointer->FirstChildElement("item");
    while (itemPointer != NULL) {
      AddItem(itemPointer->GetText());
      itemPointer = itemPointer->NextSiblingElement("item");
    }
  }
  
  this->TButton::LoadFromXML(elementPointer);
  
  
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


void TListBox::ScrollUp() {
  if (Scroll > 0) Scroll--;
}

void TListBox::ScrollDown() {
  if (Scroll +  GetVerticalQtyShown() < Items) {
    Scroll++;
  }
}


int TListBox::GetVerticalScrollPosition() {
  return Scroll;
}

void TListBox::SetVerticalScrollPosition(int position) {
  Scroll = position;
}

int TListBox::GetVerticalMaxScroll() {
  return Items;
}

int TListBox::GetVerticalQtyShown() {
  return (this->Height / (Font->Height + 1));
}




TListBox::TStyle::TStyle() : TButton::TStyle() {
 
  
  Background.R = 255;
  Background.G = 255;
  Background.B = 255;

  Border.Width = 1;
  Border.Radius = 1;

  
  Font.R = 69;
  Font.G = 73;
  Font.B = 78;
  Font.BackgroundR = 255;
  Font.BackgroundG = 255;
  Font.BackgroundB = 255;
 

}