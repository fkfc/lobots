#include "window.h"
#include "support/splitstr.h"

TWindow::TWindow(TWidget *Parent) : TWidget(Parent) { 
  Construct(); 
}

TWindow::TWindow() : TWidget() {
  Construct();
}


const char* TWindow::GetType() {
  return "window";
}

void TWindow::Draw(int w, int h) {
     if (!Visible) return;
     
     this->TWidget::Draw(w, h);
     
     TWindow::TStyle *S;
     if (window_moving == false) S = &Style;
     else S = &Selected;
     
     
     //Window  
     Raster::SetColor(S->Background.R, S->Background.G, S->Background.B);
     Raster::RoundedRectangle(GetScreenX(), GetScreenY(), GetScreenX() + Width, GetScreenY() + Height, S->Border.Radius, w, h);
        
     //Border 
     if (S->Border.Width > 0) {
      Raster::SetColor(S->Border.R, S->Border.G, S->Border.B);
      Raster::RoundedRectangle(
	GetScreenX() - S->Border.Width, 
	GetScreenY() - S->Border.Width,
	GetScreenX() + Width + S->Border.Width, 
	GetScreenY() + Height + S->Border.Width,
	S->Border.Radius,
	w,
	h);
     }
    
}

void TWindow::MouseMove(int X, int Y) {
     glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
      if (window_moving) {                       
	this->X = initialX + (X + this->X) - mousedown_x;
	this->Y = initialY + (Y + this->Y) - mousedown_y;
      } else {
         MouseOver = this;
         for (int i = 0; i < nChildren; i++) {
            TWidget *Child = WidgetChildren[i];
            if (Child->IsMouseOver(X, Y)) {
		MouseOver = Child;
		Child->MouseMove(X - Child->X, Y - Child->Y);
            }
         }
     }     
                  
}


void TWindow::ClickDown(int X, int Y) { 
       bool click = false;
       for (int i = 0; i < nChildren; i++) {
         click = click || WidgetChildren[i]->IsMouseOver(X, Y);  
         if (WidgetChildren[i]->IsMouseOver(X, Y)) {
             WidgetChildren[i]->HasFocus = true;                                  
             WidgetChildren[i]->ClickDown(X - WidgetChildren[i]->X, Y - WidgetChildren[i]->Y);
         } else {
             WidgetChildren[i]->HasFocus = false;
         }       
       }

       if (!click && draggable) { // not clicking on any children widget
          window_moving = true;
          mousedown_x = X + this->X;
          mousedown_y = Y + this->Y;
	  initialX = this->X;
	  initialY = this->Y; 
       } else {
          window_moving = false;    
       }
}
void TWindow::ClickUp(int X, int Y) {
       if (window_moving) {  
         window_moving = false;
     } else {
       for (int i = 0; i < nChildren; i++) {
         WidgetChildren[i]->ClickUp(X - WidgetChildren[i]->X, Y - WidgetChildren[i]->Y);
       }
     
     }
       
}     
 
void TWindow::KeyPress(char key) {
       for (int i = 0; i < nChildren; i++) {
         if (WidgetChildren[i]->HasFocus) WidgetChildren[i]->KeyPress(key);
       }     
}    
   
bool TWindow::IsMouseOver(int mx, int my) {
     if (!Visible) return false;
     bool ret = (mx >= X && mx <= (X + Width) && my >= Y && my <= (Y + Height));

     return ret;
} 


void TWindow::LoadFromXML(XMLElement *elementPointer) {
  
  if (elementPointer->FirstChildElement("draggable") != NULL) {
    if (strcmp(elementPointer->FirstChildElement("draggable")->GetText(), "false") == 0) this->draggable = false;
    else this->draggable = true;
  }
  
  if (elementPointer->FirstChildElement("style") != NULL) {
    Style.LoadFromXML(elementPointer->FirstChildElement("style"), this);
  }
  
  this->TWidget::LoadFromXML(elementPointer);
}


void TWindow::Construct() {

  this->Visible = true;
  window_moving = false;
  draggable = true;
  
  
  //Style
  Selected.Background.R = 217;
  Selected.Background.G = 218;
  Selected.Background.B = 219;
  
  TWidget *ParentStyle = GetParentByType(GetType());
  if (ParentStyle != NULL) {
    Style = ((TWindow*) ParentStyle)->Style;
    Selected = ((TWindow*) ParentStyle)->Style;
  }
  
}





TWindow::TStyle::TStyle() : TWidget::TStyle() {
  
  
  Background.R = 239;
  Background.G = 240;
  Background.B = 241;
  
  Border.Width = 1;
  Border.Radius = 5;
  Border.R = 217;
  Border.G = 218;
  Border.B = 219;

}

void TWindow::TStyle::LoadFromXML(XMLElement *elementPointer, TWidget *Widget) {
  this->TWidget::TStyle::LoadFromXML(elementPointer, Widget);
  
  char xstr[30], ystr[30], zstr[30];
  if (elementPointer->FirstChildElement("backgroundcolor") != NULL) {
    splitstr(elementPointer->FirstChildElement("backgroundcolor")->GetText(), ',', xstr, ystr);
    Background.R = atoi(xstr);
    splitstr(ystr, ',', xstr, zstr);
    Background.G = atoi(xstr);
    Background.B = atoi(zstr);
  }
  
  
  if (elementPointer->FirstChildElement("border") != NULL) {
    XMLElement *p = elementPointer->FirstChildElement("border");
    
    if (p->FirstChildElement("radius") != NULL) 
      Border.Radius = atoi(p->FirstChildElement("radius")->GetText());
    
    if (p->FirstChildElement("width") != NULL) 
      Border.Width = atoi(p->FirstChildElement("width")->GetText());
    
    if (p->FirstChildElement("color") != NULL) {
      splitstr(p->FirstChildElement("color")->GetText(), ',', xstr, ystr);
      Border.R = atoi(xstr);
      splitstr(ystr, ',', xstr, zstr);
      Border.G = atoi(xstr);
      Border.B = atoi(zstr);
    }
    
  }
  
  
  if (elementPointer->FirstChildElement("selected") != NULL) {
    ((TWindow*) Widget)->Selected = ((TWindow*) Widget)->Style;
    ((TWindow*) Widget)->Selected.LoadFromXML(elementPointer->FirstChildElement("selected"), Widget);
  }
}