/* raster.h
*
* Funcoes 2D, simulado graficos rasterizados em uma tela opengl 3D
*/

#ifndef RasterH
#define RasterH

#include "easybmp/EasyBMP.h"
#include <stack> 


typedef struct _t_box {
  int X;
  int Y;
  int Width;
  int Height;
} t_box;

class Raster {
    public:
       static void ImprimeTexto(float x, float y, void *font,char *string, int w, int h);
       static void SetColor(unsigned char R, unsigned char G, unsigned char B);
       static void SetColor(unsigned char R, unsigned char G, unsigned char B, unsigned char A);
       static void PutPixel(float x, float y, int w, int h);
       static void DrawImage(float x, float y, int imageWidth, int imageHeight, unsigned char *imageData, int w, int h);
       static void Box(float x1, float y1, float x2, float y2, int w, int h);
       static void RoundedRectangle(float x1, float y1, float x2, float y2, int radius, int w, int h);
       static void Ellipse(float xc, float yc, int rad_x, int rad_y, int w, int h);
       static void Line(float x1, float y1, float x2, float y2, int w, int h);
       static void Triangle(float x1, float y1, float x2, float y2, float x3, float y3, int w, int h);
       static void setOrthographicProjection(int w, int h);
       static void resetPerpectiveProjection();
       static void PushWorkingArea(t_box box);
       static void PopWorkingArea();
       static void ResetWorkingArea();
       static t_box GetCurrentWorkingArea();
    protected:   
       static t_box WorkingArea;
       static std::stack<t_box> WorkingAreaStack;
       static void SetWorkingArea(t_box box);
       static void SetWorkingArea(int X, int Y, int Width, int Height);
       static bool ValidateCoordinates(int *x, int *y);
       static bool ValidateCoordinates(float *x, float *y);
       
};

class TFont {
      public:
             TFont(const char* nome_arq, int larg, int alt);
             void Draw(int X, int Y, const char* text, unsigned char R, unsigned char G, unsigned char B, int screen_w, int screen_h);
             int Width, Height;
      private:
              BMP *Figura;
                     
};

#endif
