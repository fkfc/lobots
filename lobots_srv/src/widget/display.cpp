#include "display.h"

#include "support/raster.h"
#include "button.h"
#include "label.h"
#include <string.h>
#include <GL/freeglut.h>
#include <algorithm>


TDisplay::TDisplay(int width, int height) {
  this->width = width;
  this->height = height;                       
  nWidgets = 0;
  Raster::ResetWorkingArea();
}

void TDisplay::SetSize(int width, int height) {
  this->width = width;
  this->height = height;                       
}

int TDisplay::AddWidget( TWidget* Widget) {
    Widget->SetDisplay(this);
    List.push_back(Widget); 
    return 0;
}

void TDisplay::DeleteWidget(TWidget *Widget) {
  if(std::find(List.begin(), List.end(), Widget) != List.end()) {
      List.erase(std::remove(List.begin(), List.end(), Widget), List.end());
  } 
  delete(Widget);
}


TWidget* TDisplay::GetWidgetByName(const char *name) {
  for (std::vector<TWidget*>::const_iterator it = List.begin(); it != List.end(); ++it) {
        if ( strcmp((*it)->GetName(), name) == 0) return (*it);
  }
  
  for (std::vector<TWidget*>::const_iterator it = List.begin(); it != List.end(); ++it) {
    TWidget *Widget = (*it)->GetChildByName(name);
    if (Widget != NULL) return Widget;
  }
    
  return NULL;
}


void TDisplay::Draw() {
    ProcessPendingEvents();
    for (std::vector<TWidget*>::const_iterator it = List.begin(); it != List.end(); ++it) {
        (*it)->Draw(width, height);
    }

    
}

void TDisplay::BringOver(TWidget *Widget) {
     for (int i = 0; i < List.size(); i++) {
         if ((TWidget *) List[i] == Widget) {
            TWidget *aux = (TWidget *) List[i];          
            for (int j = i - 1; j >= 0; j--) {
                List[j + 1] = List[j];
            }
            List[0] = aux;
            return;
         }
     }
}

void TDisplay::ClickDown(int X, int Y) {
     for (std::vector<TWidget*>::const_iterator it = List.begin(); it != List.end(); ++it) {
         if ((*it)->Visible && (*it)->IsMouseOver(X, Y)) {
             (*it)->HasFocus = true;
             (*it)->ClickDown(X - (*it)->X, Y - (*it)->Y);     
             BringOver( (*it) );
             return;
         } else {
             (*it)->SetFocus(false);
         }       
     }
}
void TDisplay::ClickUp(int X, int Y) {
     for (std::vector<TWidget*>::const_iterator it = List.begin(); it != List.end(); ++it) {
          if ((*it)->Visible) {
                              (*it)->ClickUp(X - (*it)->X, Y - (*it)->Y);
                              return;
                              }   
     }
}     

bool TDisplay::IsVisible() {
     for (std::vector<TWidget*>::const_iterator it = List.begin(); it != List.end(); ++it) {
         if ((*it)->Visible) return true;     
     }
     return false;
}     

void TDisplay::MouseMove(int X, int Y) {
     for (std::vector<TWidget*>::const_iterator it = List.begin(); it != List.end(); ++it) {
         if ((*it)->IsMouseOver(X, Y)) {
            (*it)->MouseMove(X - (*it)->X, Y - (*it)->Y);    
            return;
         }
     }     
}

bool TDisplay::MouseOver(int X, int Y) {
     for (std::vector<TWidget*>::const_iterator it = List.begin(); it != List.end(); ++it) {
         if ((*it)->IsMouseOver(X, Y)) return true;    
     }               
     return false;
}

void TDisplay::KeyPress(char key) {
     for (std::vector<TWidget*>::const_iterator it = List.begin(); it != List.end(); ++it) {
         if ((*it)->HasFocus == true) (*it)->KeyPress(key);
     }    
}

bool TDisplay::HasFocus() { // is anyone listening the keyboard?
     for (std::vector<TWidget*>::const_iterator it = List.begin(); it != List.end(); ++it) {
         if ((*it)->HasFocus == true) return true;
     }    
   return false;
}

int TDisplay::GetWidth() { return width; }
int TDisplay::GetHeight() { return height; }

void ButtonClose(TButton *Button) {
  Button->Parent->Visible = false;
}

void TDisplay::Messagebox(char *msg) {
   char text[1024];
   strcpy(text, msg);
   
   int n_lines = 0;
   char line[5][1024];
   int i = 0;
   int j = 0;
   while (i < strlen(text)) {
         if (j == 0) {
               n_lines++;
         }
         if (text[i] != '\n') line[n_lines - 1][j++] = text[i];
         else {
              line[n_lines - 1][j] = 0;
              j = 0;
         }
         i++;
   }
   line[n_lines - 1][j] = 0;
   
     
   TWindow *Janela = new TWindow();
   Janela->Visible = false;
   Janela->Height = 90;
   
   
   int longest = 0;
   for (i = 0; i < n_lines; i++) {
       if (strlen(line[i]) > longest) longest = strlen(line[i]);
       TLabel *Label1 = new TLabel(Janela);
       Label1->SetText(line[i]);
       Label1->X = 10;
       Label1->Y = 25 + i*(Label1->Font->Height + 1);      
       Janela->Height += (Label1->Font->Height + 1);
   }
   
   Janela->Width = longest*8 + 20;
   Janela->X = GetWidth() / 2 - Janela->Width/2;
   Janela->Y = GetHeight() / 2 - Janela->Height/2;
   
   

   TButton *Button1 = new TButton(Janela);
   Button1->SetText("OK");
   Button1->X = Janela->Width/2 - 15;
   Button1->Y = Janela->Height - 50;
   Button1->Width = 30;
   Button1->Height = 30; 
   Button1->OnClick = ButtonClose;
   
   Janela->Visible = true;
   this->AddWidget(Janela);
   this->BringOver(Janela);
     
}     



void TDisplay::DeleteLater(TWidget *Widget) {
  DeleteList.push_back(Widget);
}

void TDisplay::ProcessPendingEvents() {
  for (std::vector<TWidget*>::const_iterator it = DeleteList.begin(); 
       it != DeleteList.end();   
       ++it) {
		TWidget *Widget = *it;
		DeleteWidget(Widget);
  }
  DeleteList.clear();
}
