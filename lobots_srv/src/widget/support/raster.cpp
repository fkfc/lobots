#include "raster.h"
#include <GL/freeglut.h>
#include <stdlib.h>
#include <stdio.h>


#ifdef __CYGWIN__
  #define RASTER_COORD_CORR_X 0
  #define RASTER_COORD_CORR_Y 1
#else
  #define RASTER_COORD_CORR_X 0
  #define RASTER_COORD_CORR_Y 0
#endif



t_box Raster::WorkingArea;
std::stack<t_box> Raster::WorkingAreaStack;

void setOrthographicProjection(int w, int h) {

	// vai para modo PROJECTION
	glMatrixMode(GL_PROJECTION);
	// salva a matriz anterior, que contém as
	// configurações para projeção de perspectiva
	glPushMatrix();
	// reset matrix
	glLoadIdentity();
	// 2D orthographic projection
	gluOrtho2D(0, w, 0, h);
	// inverter o eixo Y: para baixo é positivo
	glScalef(1, -1, 1);
	// mover a origem para o canto superior esquerdo
	glTranslatef(0, -h, 0);
	glMatrixMode(GL_MODELVIEW);
}

void Raster:: resetPerpectiveProjection() {
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_LIGHTING);
}

void Raster::ImprimeTexto(float x, float y, void *font,char *string, int w, int h)
{

    setOrthographicProjection(w, h);
    glPushMatrix();
    glLoadIdentity();         
         
    char *c;
    glRasterPos2f(x, y);
    for (c=string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
    
    glPopMatrix();
    resetPerpectiveProjection();

}

void Raster::SetColor(unsigned char R, unsigned char G, unsigned char B) {
  Raster::SetColor(R, G, B, 0);
}

void Raster::SetColor(unsigned char R, unsigned char G, unsigned char B, unsigned char A) {
  glColor4b((char) (R/2), (char) (G/2), (char) (B/2), (char) (A/2));
}


void Raster::PutPixel(float x, float y, int w, int h) {
    //printf("DEBUG: (%d, %d)  (%d, %d   %d, %d)\n", x, y, WorkingArea.X, WorkingArea.Y, WorkingArea.X + WorkingArea.Width,  WorkingArea.Y + WorkingArea.Height);
    if (x < WorkingArea.X || x > WorkingArea.X + WorkingArea.Width || y < WorkingArea.Y || y > WorkingArea.Y + WorkingArea.Height) return;
    
    
    setOrthographicProjection(w, h);
    glPushMatrix();
    glLoadIdentity();     
         

    glBegin(GL_POINTS);
       glVertex2f(x, y);
    glEnd();
    glPopMatrix();
    resetPerpectiveProjection();
       
}


void Raster::DrawImage(float x, float y, int imageWidth, int imageHeight, unsigned char *imageData, int w, int h) {
    float x1 = x;
    float y1 = y;
    float x2 = x + imageWidth;
    float y2 = y + imageHeight;
    
    ValidateCoordinates(&x1, &y1);
    ValidateCoordinates(&x2, &y2);
    
    int finalImageWidth = x2 - x1;
    int finalImageHeight = y2 - y1;
    
    if (imageWidth == finalImageWidth && imageHeight == finalImageHeight) {
    
        setOrthographicProjection(w, h);
            glPushMatrix();
                glLoadIdentity();            
                
                glPixelZoom(1, -1); 
                glRasterPos2f(x1 , y1);
                glDrawPixels(finalImageWidth,  finalImageHeight, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
            
            glPopMatrix();
        resetPerpectiveProjection();
    } else { //image must be cropped to fit into the visible area
        setOrthographicProjection(w, h);
        glPushMatrix();
        glLoadIdentity();     
        glBegin(GL_POINTS);
        
        
        int nextPointerLine, lastPointerLine;
        int xp, yp;
        yp = y1-y; //starting y position
        
        int lastLine = (y2-y)*imageWidth*4; //pointer to the beginning of the first line which won't be drawn (outside visible area)
        for (int pointer = (y1 - y)*imageWidth*4; pointer < lastLine;) {
            nextPointerLine = pointer + imageWidth*4; //start of the next line
            lastPointerLine = pointer + (x2-x)*4; //first pixel in this line that won't be shown
            pointer += (x1-x)*4;
            for (xp = x1-x; pointer < lastPointerLine;) { //draw current line
                unsigned char R = imageData[pointer++];
                unsigned char G = imageData[pointer++];
                unsigned char B = imageData[pointer++];
                unsigned char A = imageData[pointer++];
                SetColor(R, G, B, A);
                glVertex2f(x+xp, y+yp);
                xp++;
            }
            pointer = nextPointerLine; //advances to the next line
            yp++;
        }
        
        glEnd();
        glPopMatrix();
        resetPerpectiveProjection();
    }
    
    
}
     
void Raster::Box(float x1, float y1, float x2, float y2, int w, int h) {
    x1 += RASTER_COORD_CORR_X;
    y1 += RASTER_COORD_CORR_Y;
    x2 += RASTER_COORD_CORR_X;
    y2 += RASTER_COORD_CORR_Y;
    
     ValidateCoordinates(&x1, &y1);
     ValidateCoordinates(&x2, &y2);
     //if ((ValidateCoordinates(&x1, &y1) | ValidateCoordinates(&x2, &y2)) == false) return;
  
   
     
    setOrthographicProjection(w, h);
    glPushMatrix();
    glLoadIdentity();  
    
    glBegin(GL_QUADS);
       glVertex2f(x1, y1);
       glVertex2f(x1, y2);
       glVertex2f(x2, y2);
       glVertex2f(x2, y1);
    glEnd();
   
    
    glPopMatrix();
    resetPerpectiveProjection();
     

}

void Raster::RoundedRectangle(float x1, float y1, float x2, float y2, int radius, int w, int h) {
  Ellipse(x1 + radius, y1 + radius, radius, radius, w, h);
  Ellipse(x2 - radius, y1 + radius, radius, radius, w, h);
  
  Ellipse(x1 + radius, y2 - radius, radius, radius, w, h);
  Ellipse(x2 - radius, y2 - radius, radius, radius, w, h);
  
  Box( x1 + radius, y1, x2 - radius, y2-1, w, h);
  Box( x1, y1 + radius, x2, y2 - radius, w, h);   
}

void Raster::Ellipse(float xc, float yc, int rad_x, int rad_y, int w, int h) {
  int width = rad_x;
  int height = rad_y;
  int hh = height * height;
  int ww = width * width;
  int hhww = hh * ww;
  int x0 = width;
  int dx = 0;
  
    

  // do the horizontal diameter
  Line(xc - width, yc, xc + width, yc, w, h);
  /*
  for (int x = -width; x <= width; x++)
      PutPixel(xc + x, yc, w, h);
  */
  
  // now do both halves at the same time, away from the diameter
  for (int y = 1; y <= height; y++)
  {
      int x1 = x0 - (dx - 1);  // try slopes of dx - 1 or more
      for ( ; x1 > 0; x1--)
	  if (x1*x1*hh + y*y*ww <= hhww)
	      break;
      dx = x0 - x1;  // current approximation of the slope
      x0 = x1;

      //for (int x = -x0; x <= x0; x++)
      //{
	  Line(xc - x0, yc - y, xc + x0, yc - y, w, h);
	  Line(xc - x0, yc + y, xc + x0, yc + y, w, h);
	  //PutPixel(xc + x, yc - y, w, h);
	  //PutPixel(xc + x, yc + y, w, h);
      //}
  }
            
}

void Raster::Triangle(float x1, float y1, float x2, float y2, float x3, float y3, int w, int h) {
    if ((ValidateCoordinates(&x1, &y1) | ValidateCoordinates(&x2, &y2)  | ValidateCoordinates(&x3, &y3)) == false) return;
    
    
  
    setOrthographicProjection(w, h);
    glPushMatrix();
    glLoadIdentity();  
    
    glBegin(GL_TRIANGLES);
       glVertex2f(x1, y1);
       glVertex2f(x2, y2);
       glVertex2f(x3, y3);
    glEnd();
   
    
    glPopMatrix();
    resetPerpectiveProjection();
     

}



void Raster::Line(float x1, float y1, float x2, float y2, int w, int h) {
    if ((ValidateCoordinates(&x1, &y1) | ValidateCoordinates(&x2, &y2)) == false) return;
    
    setOrthographicProjection(w, h);
    glPushMatrix();
    glLoadIdentity();  
    
    glBegin(GL_LINES);
       glVertex2f(x1, y1);
       glVertex2f(x2, y2);
    glEnd();
   
    
    glPopMatrix();
    resetPerpectiveProjection();
     

}

     
void Raster::setOrthographicProjection(int w, int h) {
     glDisable(GL_LIGHTING);
	// vai para modo PROJECTION
	glMatrixMode(GL_PROJECTION);
	// salva a matriz anterior, que contém as
	// configurações para projeção de perspectiva
	glPushMatrix();
	// reset matrix
	glLoadIdentity();
	// 2D orthographic projection
	gluOrtho2D(0, w, 0, h);
	// inverter o eixo Y: para baixo é positivo
	glScalef(1, -1, 1);
	// mover a origem para o canto superior esquerdo
	glTranslatef(0, -h, 0);
	glMatrixMode(GL_MODELVIEW);
}     

TFont::TFont(const char* nome_arq, int larg, int alt) {
                   
    Figura = new BMP();
    Figura->ReadFromFile(nome_arq); 
  /*  
    char ext[1024];
    strcpy(ext, nome_arq);
    strcpy(&ext[strlen(ext) - 4], "ext.bmp");
    Figura2 = new BMP;
    Figura2->ReadFromFile(ext); 
    */
    this->Width = larg;
    this->Height = alt;
    
    
    
   // FILE *arq = fopen("ascii.txt", "wb");
   // for (int i = 32; i < 256; i++) fwrite((unsigned char*) &i, 1, 1, arq);
   // fclose(arq);
    
                  
}

void TFont::Draw(int X, int Y, const char* text, unsigned char R, unsigned char G, unsigned char B, int screen_w, int screen_h) {
     RGBApixel P;
     BMP *Fig;
     Raster::SetColor(R, G, B);
     for (int i = 0; i < strlen(text); i++) {
         char c = text[i];
         switch (c) {
                case 'ã': c = 117 + 32; break;
                case 'ó': c = 118 + 32; break;
                case 'á': c = 119 + 32; break;
                
         }
        
       Fig = Figura;
       c = c - 32;               
         

         
         
         int x0 = (c)*Width;
         for (int x = x0; x < x0 + Width; x++) {
             for (int y = 0; y < Height; y++) {
                 int screen_x = X + (x - x0) + i*Width;
                 int screen_y = Y + y;
                 P = Fig->GetPixel(x, y);
                 if (P.Red != 255 || P.Green != 255 || P.Blue != 255) {
                           Raster::PutPixel(screen_x, screen_y, screen_w, screen_h);
                 }
             }    
         }    
     }    
}


void Raster::SetWorkingArea(t_box box) {
  SetWorkingArea(box.X, box.Y, box.Width, box.Height);
}

void Raster::SetWorkingArea(int X, int Y, int Width, int Height) {
  int X1New = (X > WorkingArea.X)? X : WorkingArea.X;
  int Y1New = (Y > WorkingArea.Y)? Y : WorkingArea.Y;
  int X2New = (X + Width) < (WorkingArea.X + WorkingArea.Width)? X + Width : WorkingArea.X + WorkingArea.Width;
  int Y2New = (Y + Height) < (WorkingArea.Y + WorkingArea.Height)? Y + Height : WorkingArea.Y + WorkingArea.Height;
  
   
  WorkingArea.X = X1New;
  WorkingArea.Y = Y1New;
  WorkingArea.Width = X2New - X1New;
  WorkingArea.Height = Y2New - Y1New;
}

void Raster::ResetWorkingArea() {
  WorkingArea.X = 0;
  WorkingArea.Y = 0;
  WorkingArea.Width = 20000;
  WorkingArea.Height = 20000;
}

void Raster::PushWorkingArea(t_box box) {
  WorkingAreaStack.push(WorkingArea);
  SetWorkingArea(box);
}

void Raster::PopWorkingArea() {
  ResetWorkingArea();
  SetWorkingArea(WorkingAreaStack.top());
  WorkingAreaStack.pop();
}

bool Raster::ValidateCoordinates(int *x, int *y) {
  bool ok = true;
  
  if (*x < WorkingArea.X) { *x = WorkingArea.X; ok = false; }
  if (*y < WorkingArea.Y) { *y = WorkingArea.Y; ok = false; }
  if (*x > WorkingArea.X + WorkingArea.Width) { *x = WorkingArea.X + WorkingArea.Width; ok = false; }
  if (*y > WorkingArea.Y + WorkingArea.Height) { *y = WorkingArea.Y + WorkingArea.Height; ok = false; }
  
  return ok;
}

bool Raster::ValidateCoordinates(float *x, float *y) {
  bool ok = true;
  
  if (*x < WorkingArea.X) { *x = WorkingArea.X; ok = false; }
  if (*y < WorkingArea.Y) { *y = WorkingArea.Y; ok = false; }
  if (*x > WorkingArea.X + WorkingArea.Width) { *x = WorkingArea.X + WorkingArea.Width; ok = false; }
  if (*y > WorkingArea.Y + WorkingArea.Height) { *y = WorkingArea.Y + WorkingArea.Height; ok = false; }
  
  return ok;
}

t_box Raster::GetCurrentWorkingArea() {
    return WorkingArea;
}