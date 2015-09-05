#include "buttonup.h"


TButtonUp::TButtonUp(TWidget *Parent) : TButton(Parent) { 
}

void TButtonUp::Draw(int w, int h) {
     if (!Visible) return;
     
     int fposx = (GetScreenX()) + Width / 2 - strlen(Text)*Font->Width/2;
     int fposy = (GetScreenY()) + Height/2 - Font->Height/2;
     
     TButton::TStyle *S = &Style;
     if (Pressed) S = &Selected;
     else if (Parent->MouseOver == this) S = &Hover;
     
     Font->Draw(fposx + S->OffsetX, fposy + S->OffsetY, Text, S->Font.R, S->Font.G, S->Font.B, w, h); 
     
     //Border
     Raster::SetColor(S->Border.R, S->Border.G, S->Border.B);
     for (int i = 1; i <= S->Border.Width; i++) {
	Raster::Line(GetScreenX() + S->OffsetX + Width/2, 
		      GetScreenY() + S->OffsetY - i, 
		      GetScreenX() + S->OffsetX - i,
		      GetScreenY() + S->OffsetY + Height + i, 
		      w, h);
	Raster::Line(GetScreenX() + S->OffsetX - i,
		      GetScreenY() + S->OffsetY + Height + i, 
		      GetScreenX() + S->OffsetX + Width + i, 
		      GetScreenY() + S->OffsetY + Height + i,
		      w, h);
	Raster::Line(GetScreenX() + S->OffsetX + Width + i, 
		      GetScreenY() + S->OffsetY + Height + i,
		      GetScreenX() + S->OffsetX + Width/2, 
		      GetScreenY() + S->OffsetY - i, 
		      w, h);
     }
     //box
     Raster::SetColor(S->Background.R, S->Background.G, S->Background.B);
     Raster::Triangle(GetScreenX() + S->OffsetX + Width/2, 
		      GetScreenY() + S->OffsetY,
		      GetScreenX() + S->OffsetX,
		      GetScreenY() + S->OffsetY + Height,
		      GetScreenX() + S->OffsetX + Width, 
		      GetScreenY() + S->OffsetY + Height,
		      w, h);
     
     
    
     
     //Shadow
     Raster::SetColor(S->Shadow.R, S->Shadow.G, S->Shadow.B); 
     Raster::Triangle(GetScreenX() + S->OffsetX + Width/2 + S->Shadow.OffsetX, 
		      GetScreenY() + S->OffsetY - S->Border.Width + S->Shadow.OffsetY,
		      GetScreenX() + S->OffsetX - S->Border.Width + S->Shadow.OffsetX,
		      GetScreenY() + S->OffsetY + Height + S->Border.Width + S->Shadow.OffsetY,
		      GetScreenX() + S->OffsetX + Width + S->Border.Width + S->Shadow.OffsetX, 
		      GetScreenY() + S->OffsetY + Height + S->Border.Width + S->Shadow.OffsetY,
		      w, h);
     
     
     
}
 
