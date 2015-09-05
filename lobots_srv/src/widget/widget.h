#ifndef widget_H
#define widget_H
#define MAX_WIDGET 512
#define WIDG_MAX_TEXT 255
#define WIDG_MAX_NAME_LEN 100

#include "support/raster.h"
#include "support/tinyxml2.h"

#include "display.h"
#include <vector>

using namespace tinyxml2;

class TWidget {
      public:
	    TWidget();
	    TWidget(TWidget *Parent);
	    ~TWidget();
	    virtual const char* GetType();
	    void DeleteChild(TWidget *Child);
	    void DeleteLater(); //safe delete
	    void AddChildren(TWidget *Children);
	    void SetDisplay (TDisplay *Display);
	    void SetFocus(bool focus);  
	    void SetName(const char *name);
	    const char* GetName();
	    virtual void Draw(int w, int h);
	    virtual void Resize(int W, int H);
	    virtual void ClickDown(int X, int Y) = 0;
	    virtual void ClickUp(int X, int Y) = 0;
	    virtual bool IsMouseOver(int mx, int my) = 0;
	    virtual void MouseMove(int X, int Y) = 0;
	    virtual void KeyPress(char key) = 0;
	    TWidget* GetChildByName(const char *name);
            void PrintChildrenNames();
	    TWidget* GetParentByType(const char *type);
	    void SetParent(TWidget *Parent);
            void Hide();
            void Show();
            static void Hide(TWidget *Widget);
            static void Show(TWidget *Widget);
	    TWidget *Parent;
	    int X;
	    int Y;
	    int Width;
            int Height;
	    bool Visible;
	    bool HasFocus;
	    TWidget *MouseOver;
	    TWidget *WidgetChildren[MAX_WIDGET];       
	    int nChildren;
	    TDisplay *Display;
	    void LoadFromXML(XMLElement *elementPointer);
	    
	    class TStyle {
	      public:
		TStyle();
		void LoadFromXML(XMLElement *elementPointer, TWidget *Widget);
	    } Style;
	    
      protected:
	    int GetScreenX();
	    int GetScreenY();
            t_box GetWorkingArea();
      private:
	    char name[WIDG_MAX_NAME_LEN];
	    void Construct();
	    void DeleteAllChildren();
};

typedef enum {
  TWINDOW, 
  TBUTTON, 
  TLABEL, 
  TTEXTBOX, 
  TLISTBOX, 
  TBUTTONUP, 
  TBUTTONDOWN, 
  TBUTTONLEFT, 
  TBUTTONRIGHT, 
  TBRICKBUTTON, 
  TSCROLLBAR, 
  TDRAGGABLEBUTTON,
  TSCROLLABLEVIEW, 
  TCANVAS,
  T_WIDGET_TYPE_SIZE} t_widget_type;


#endif
