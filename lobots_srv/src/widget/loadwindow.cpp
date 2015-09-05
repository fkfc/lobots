#include "loadwindow.h"

#include "widgets.h"
#include "support/splitstr.h"
#include <string.h>


extern const char  *widget_type_str[]; //declared in "widget.cpp", indexes in "widget.h"


TWidget *LoadWidgetFromElement(XMLElement *elementPointer, TWidget *Parent) {
  
  int type = 0;
  const char* type_str = elementPointer->FirstChildElement("type")->GetText();
  for (int i = 0; i < T_WIDGET_TYPE_SIZE; i++) {
    if (strcmp(widget_type_str[i], type_str) == 0) type = i;
  }
  
  //printf("DEBUG: new widget type_id = %d , str_id = %s\n", type, type_str);
  
  TWidget *NewWidget;

  
  switch (type) {
    case TWINDOW:
      NewWidget = new TWindow(Parent);
      ((TWindow*) NewWidget)->LoadFromXML(elementPointer);
      break;
    case TBUTTON:
      NewWidget = new TButton(Parent);
      ((TButton*) NewWidget)->LoadFromXML(elementPointer);
      break;
    case TBUTTONUP:
      NewWidget = new TButtonUp(Parent);
      ((TButtonUp*) NewWidget)->LoadFromXML(elementPointer);
      break;  
    case TBUTTONDOWN:
      NewWidget = new TButtonDown(Parent);
      ((TButtonDown*) NewWidget)->LoadFromXML(elementPointer);
      break;  
    case TLABEL:
      NewWidget = new TLabel(Parent);
      ((TLabel*) NewWidget)->LoadFromXML(elementPointer);
      break;  
   case TTEXTBOX:
      NewWidget = new TTextBox(Parent);
      ((TTextBox*) NewWidget)->LoadFromXML(elementPointer);
      break;   
   case TLISTBOX:   
      NewWidget = new TListBox(Parent);
      ((TListBox*) NewWidget)->LoadFromXML(elementPointer);
      break;
   case TBRICKBUTTON:   
      NewWidget = new TBrickButton(Parent, NULL);
      ((TBrickButton*) NewWidget)->LoadFromXML(elementPointer);
      break;   
   case TSCROLLBAR:
      if (strcmp(elementPointer->FirstChildElement("orientation")->GetText(), "horizontal") == 0) {
	NewWidget = new THorizontalScrollBar(Parent);
	((THorizontalScrollBar*) NewWidget)->LoadFromXML(elementPointer);
      } else {
	NewWidget = new TVerticalScrollBar(Parent);
	((TVerticalScrollBar*) NewWidget)->LoadFromXML(elementPointer);
      }
      break;   
   case TDRAGGABLEBUTTON:
      NewWidget = new TDraggableButton(Parent);
      ((TDraggableButton*) NewWidget)->LoadFromXML(elementPointer);
      break;  
   case TSCROLLABLEVIEW:
      NewWidget = new TScrollableView(Parent);
      ((TScrollableView*) NewWidget)->LoadFromXML(elementPointer);
      break;
   case TCANVAS:
       NewWidget = new TCanvas(Parent);
       ((TCanvas*) NewWidget)->LoadFromXML(elementPointer);
       break;
   default:
      NewWidget = NULL;
      break;
  }
  
  
  return NewWidget;
  
}

TWindow* LoadWindowFromXMLDocument(XMLDocument *doc) {
  TWindow* Window = new TWindow();
  
  XMLElement* root = doc->FirstChildElement("window");
  
  Window->LoadFromXML(root);

  return Window;
}

TWindow* LoadWindowFromString(const char *xmlstring) {
  XMLDocument doc;
  doc.Parse( xmlstring );
  return LoadWindowFromXMLDocument(&doc);
}

TWindow* LoadWindowFromFile(const char *filename) {
  XMLDocument doc;
  doc.LoadFile(filename);
  return LoadWindowFromXMLDocument(&doc);
}
