#ifndef display_H
#define display_H

#include <GL/freeglut.h>
#include <vector>

class TWidget;

class TDisplay {
      public:
             TDisplay(int width, int height);
             void SetSize(int width, int height);
             int AddWidget(TWidget *Widget);
	     void DeleteLater(TWidget *Widget);
	     void DeleteWidget(TWidget *Widget);
	     TWidget* GetWidgetByName(const char *name);
	     void ProcessPendingEvents();
             void Draw();
             void ClickDown(int X, int Y);
             void ClickUp(int X, int Y);
             void MouseMove(int X, int Y);
             bool MouseOver(int X, int Y);
             bool IsVisible();
             bool HasFocus();
             void KeyPress(char key);
             int GetWidth();
             int GetHeight();
             void Messagebox(char *msg);
             void BringOver(TWidget *Widget);
      private:
              int width;
              int height;
              int nWidgets;
              std::vector<TWidget*> List;
	      std::vector<TWidget*> DeleteList;
              

};


#endif
