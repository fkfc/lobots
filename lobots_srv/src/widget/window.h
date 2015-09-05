#ifndef window_H
#define window_H
#include "support/raster.h"
#include "widget.h"

class TWindow : public TWidget {
      public:
             TWindow();
             TWindow( TWidget *Parent );
	     virtual const char* GetType();
             void Draw(int w, int h);
             void ClickDown(int X, int Y);
             void ClickUp(int X, int Y);
             void MouseMove(int X, int Y);
             bool IsMouseOver(int mx, int my);
             void KeyPress(char key);
	     void LoadFromXML(XMLElement *elementPointer);
	             
	     bool draggable;
	     
	     class TStyle : public TWidget::TStyle {
	      public:
		TStyle();
		void LoadFromXML(XMLElement *elementPointer, TWidget *Widget);
		
		struct {
		  unsigned char R;
		  unsigned char G;
		  unsigned char B;
		} Background;
		
		struct {
		  int Width;
		  int Radius;
		  unsigned char R;
		  unsigned char G;
		  unsigned char B;
		} Border;
	    } Style, Selected;
	     
       private:
	     void Construct();
             int mousedown_x;
             int mousedown_y;
	     int initialX;
	     int initialY;
	protected:
	     bool window_moving;
	     
	     
        
};  


#endif
