#include "buttonright.h"


TButtonRight::TButtonRight(TWidget *Parent) : TButton(Parent) { 
}

void TButtonRight::Draw(int w, int h) {
     if (!Visible) return;
     
     int fposx = (GetScreenX()) + Width / 2 - strlen(Text)*Font->Width/2;
     int fposy = (GetScreenY()) + Height/2 - Font->Height/2;
     
     TButton::TStyle *S = &Style;
     if (Pressed) S = &Selected;
     else if (Parent->MouseOver == this) S = &Hover;
     
     Font->Draw(fposx + S->OffsetX, fposy + S->OffsetY, Text, S->Font.R, S->Font.G, S->Font.B, w, h); 
     
     int x1, y1, x2, y2, x3, y3;
     //Right middle
     x1 = GetScreenX() + S->OffsetX + Width;
     y1 = GetScreenY() + S->OffsetY + Height/2;
     
     //Upper left corner
     x2 = GetScreenX() + S->OffsetX;
     y2 = GetScreenY() + S->OffsetY;
     
     //Lower left corner
     x3 = GetScreenX() + S->OffsetX;
     y3 = GetScreenY() + S->OffsetY + Height;
     
     //Border
     Raster::SetColor(S->Border.R, S->Border.G, S->Border.B);
     for (int i = 1; i <= S->Border.Width; i++) {
	Raster::Line(x1 + i, y1,
		     x2 - i, y2 - i,
		     w, h);
	Raster::Line(x2 - i, y2 - i,
		     x3 - i, y3 + i,
		     w, h);
	Raster::Line(x3 - i, y3 + i,
		     x1 + i, y1,
		     w, h);
     }
     //box
     Raster::SetColor(S->Background.R, S->Background.G, S->Background.B);
     Raster::Triangle(x1, y1,
		      x2, y2,
		      x3, y3,
		      w, h);
     
     
     //Shadow
     Raster::SetColor(S->Shadow.R, S->Shadow.G, S->Shadow.B); 
     Raster::Triangle(x1 + S->Shadow.OffsetX + S->Border.Width - S->OffsetX, y1 + S->Shadow.OffsetY - S->OffsetY,
		      x2 + S->Shadow.OffsetX - S->Border.Width - S->OffsetX, y2 + S->Shadow.OffsetY - S->Border.Width - S->OffsetY,
		      x3 + S->Shadow.OffsetX - S->Border.Width - S->OffsetX, y3 + S->Shadow.OffsetY + S->Border.Width - S->OffsetY,
		      w, h);
     
     
     
}
 
