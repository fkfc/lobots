#ifndef scrollable_H
#define scrollable_H

class Scrollable {
  public:
    Scrollable();
    virtual void ScrollUp();
    virtual void ScrollDown();
    virtual void ScrollLeft();
    virtual void ScrollRight();
    
    virtual int GetVerticalScrollPosition();
    virtual void SetVerticalScrollPosition(int position);
    virtual int GetVerticalMaxScroll();
    virtual int GetVerticalQtyShown();
    
    virtual int GetHorizontalScrollPosition();
    virtual void SetHorizontalScrollPosition(int position);
    virtual int GetHorizontalMaxScroll();
    virtual int GetHorizontalQtyShown();
    
  
};

#endif
