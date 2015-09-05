/* camera.h
*
* Armazena as propriedades e fornece m�todos para movimentar a c�mera que
* representa o ponto de vista do usu�rio
*/


#ifndef CameraH
#define CameraH



class TCamera {
      public:
             TCamera(float tx, float ty, float tz, float rx, float ry, float rz, int w, int h);
             float transx;
             float transy;
             float transz;
             
             float rotx;
             float roty;
             float rotz;
             
             int XMouse;
             int YMouse;
             
             int janela_largura;
             int janela_altura;
             
             
             // Movimenta a c�mera. Os par�metros fornecidos s�o as coordenadas 
             // iniciais e finais do mouse
             void CameraXZ(int x1, int y1, int x2, int y2); 
             void CameraRotXY(int x1, int y1, int x2, int y2);
             void CameraY(int x1, int y1, int x2, int y2);
             void CameraX(int x1, int y1, int x2, int y2);
             void CameraXY(int x1, int y1, int x2, int y2);
             void IniciaMovimento(int x, int y);
             bool iniciado;
      private:
              
             
};



#endif
