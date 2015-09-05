#include "camera.h"
#include <math.h>
#define PI 3.1416f
#define PASSO_CAMERA 0.05f

TCamera::TCamera(float tx, float ty, float tz, float rx, float ry, float rz, int w, int h) {
                       transx = tx;
                       transy = ty;
                       transz = tz;
                       
                       rotx = rx;
                       roty = ry;
                       rotz = rz;
                       
                       janela_largura = w;
                       janela_altura = h;
                       iniciado = false;
}
void TCamera::IniciaMovimento(int x, int y) {
      iniciado = true;
      XMouse = x;
      YMouse = y;
}

void TCamera::CameraXZ(int x1, int y1, int x2, int y2) {   
     if (!iniciado) {
                    XMouse = x2;
                    YMouse = y2;
                    iniciado = true;
                    return;
     }
     transz += cos(roty*PI/180)*cos(rotx*PI/180)*(y2-y1)*PASSO_CAMERA;
     transx -= sin(roty*PI/180)*cos(rotx*PI/180)*(y2-y1)*PASSO_CAMERA;
     
     transx += cos(roty*PI/180)*(x2-x1)*PASSO_CAMERA;;
     transz += sin(roty*PI/180)*(x2-x1)*PASSO_CAMERA;;
     
     
     XMouse = x2;
     YMouse = y2;
}

void TCamera::CameraY(int x1, int y1, int x2, int y2) {
     
     transy += (y2-y1)*PASSO_CAMERA;
     
     if (!iniciado) {
                    iniciado = true;
                    return;
     }
     XMouse = x2;
     YMouse = y2;     
}

void TCamera::CameraXY(int x1, int y1, int x2, int y2) {
     
     transy += (y2-y1)*PASSO_CAMERA;
     
     
     if (!iniciado) {
                    iniciado = true;
                    return;
     }
     XMouse = x2;
     YMouse = y2;     
}

void TCamera::CameraX(int x1, int y1, int x2, int y2) {
     
     transx += (x2-x1)*PASSO_CAMERA;
     
     if (!iniciado) {
                    iniciado = true;
                    return;
     }
     XMouse = x2;
     YMouse = y2;     
}


void TCamera::CameraRotXY(int x1, int y1, int x2, int y2) {
     if (!iniciado) {
                    XMouse = x2;
                    YMouse = y2;
                    iniciado = true;
                    return;
     }
     rotx += (y2-y1)/3;
     roty += cos(rotx*PI/180)*cos(rotz*PI/180)*(x2-x1)/3;
     
     if (roty < 0) roty += 360.0f; 
     
     XMouse = x2;
     YMouse = y2;
          
}
