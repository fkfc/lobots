#ifndef widget_loadwindow_H
#define widget_loadwindow_H


#include "widgets.h"
#include "support/tinyxml2.h"
using namespace tinyxml2;


TWindow* LoadWindowFromFile(const char *filename);
TWindow* LoadWindowFromString(const char *xmlstring);
TWidget *LoadWidgetFromElement(XMLElement *elementPointer, TWidget *Parent);

#endif
