#ifndef listbox_H
#define listbox_H

#include "button.h"
#include "scrollable.h"

class TListBox : public TButton, public Scrollable {
      public:
             TListBox(TWidget *Parent);
             void Draw(int w, int h);
             void AddItem(const char* Text);
	     void RemoveItem(int i);
             void ClickDown(int X, int Y);
             void ClickUp(int X, int Y);
             void (*OnClick)(TListBox *ListBox);
	     void LoadFromXML(XMLElement *elementPointer);
	     
	     //Scrollable:
	     void ScrollUp();
	     void ScrollDown();
	     int GetVerticalScrollPosition();
	     void SetVerticalScrollPosition(int position);
	     int GetVerticalMaxScroll();
	     int GetVerticalQtyShown();
	     
	     
	     class TStyle : public TButton::TStyle {
	      public:
		TStyle();
		
		struct {
		  unsigned char R;
		  unsigned char G;
		  unsigned char B;
		  
		  unsigned char BackgroundR;
		  unsigned char BackgroundG;
		  unsigned char BackgroundB;
		} Font;
		
		
		
	    } Style, Selected;
	    
             
             char List[WIDG_MAX_TEXT][WIDG_MAX_TEXT];
             int Items;
             int SelectedItem;
             int Scroll;
      
      
}; 



#endif