#ifndef button_H
#define button_H

#include "window.h"

class ButtonConnectable;

class TButton : public TWindow {
      public:
             TButton(TWidget *Parent);
	     TButton(TWidget *Parent, ButtonConnectable *ConnectTo);
	     virtual const char* GetType();
             void Draw(int w, int h);
             void ClickUp(int X, int Y);
             void ClickDown(int X, int Y);
             void SetText(const char* Text);
             void MouseMove(int X, int Y);
	     void LoadFromXML(XMLElement *elementPointer);
             
	     class TStyle : public TWindow::TStyle {
	      public:
		TStyle();
		void LoadFromXML(XMLElement *elementPointer, TWidget *Widget);
		
		struct {
		  int OffsetX;
		  int OffsetY;
		  unsigned char R;
		  unsigned char G;
		  unsigned char B;
		} Shadow;
		
		struct {
		  unsigned char R;
		  unsigned char G;
		  unsigned char B;
		} Font;
		
		int OffsetX;
		int OffsetY;
		
	    } Style, Selected, Hover;
	    
             void (*OnClick)(TButton *Button);    
	     ButtonConnectable *ConnectedObject;
             char Text[WIDG_MAX_TEXT];            
             bool Pressed;
             TFont *Font;
      private:
	     void Construct();
                      
};




#endif
