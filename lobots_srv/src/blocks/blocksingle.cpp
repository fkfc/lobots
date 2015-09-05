#include "blocksingle.h"
#include "hsv.h"
#include <math.h>
#include "world.h"

TBlockSingle::TBlockSingle(ptobj modelo, float x, float y, float z) {
                   this->modelo = modelo;
                   this->x = x;
                   this->y = y;
                   this->z = z;
                   this->ry = 0;
                   this->rx = 0;
                   
                   cor.r = 127;
                   cor.g = 0;
                   cor.b = 0;
		   
		   this->solido = true;
		   
		   tipo = TBLOCKSINGLE;
}

  
void TBlockSingle::Draw(TCamera *Camera, bool wire) {
     if (wire) glDisable(GL_LIGHTING);
     glPushMatrix();
           glRotated(Camera->rotx,1,0,0);   // rotacao de camera  x   
           glRotated(Camera->roty,0,1,0);   // rotacao de camera  y  
           glRotated(Camera->rotz,0,0,1);   // rotacao de camera  z  
           glTranslated(Camera->transx, Camera->transy, Camera->transz); // translacao da camera
           glTranslated(x,y,z); // tranlacao da peça
           glRotated(-ry,0,1,0); // rotacao da peça
           glRotated(rx,1,0,0); // rotacao da peça
           
           if (wire) glColor3b(0, 0, 0); // cor 
           else glColor3b(cor.r, cor.g, cor.b); // cor
    
            glBegin(GL_QUADS); // glBegin e glEnd delimitam os vértices que definem uma primitiva
            int num =  modelo->num_poligonos;
            for (int l_index=0; l_index < num; l_index++)
            {  
                     float nx;
                     float ny;
                     float nz;
                     float ax = modelo->vertice[ modelo->poligono[l_index].a ].x;
                     float ay = modelo->vertice[ modelo->poligono[l_index].a ].y;
                     float az = modelo->vertice[ modelo->poligono[l_index].a ].z;
                     float bx = modelo->vertice[ modelo->poligono[l_index].b ].x;
                     float by = modelo->vertice[ modelo->poligono[l_index].b ].y;
                     float bz = modelo->vertice[ modelo->poligono[l_index].b ].z;
                     float cx = modelo->vertice[ modelo->poligono[l_index].c ].x;
                     float cy = modelo->vertice[ modelo->poligono[l_index].c ].y;
                     float cz = modelo->vertice[ modelo->poligono[l_index].c ].z;
                     float dx = modelo->vertice[ modelo->poligono[l_index].d ].x;
                     float dy = modelo->vertice[ modelo->poligono[l_index].d ].y;
                     float dz = modelo->vertice[ modelo->poligono[l_index].d ].z;
                     
    
                     normal(ax,ay,az,bx,by,bz,cx,cy,cz,&nx, &ny, &nz);
                     glNormal3f(nx, ny,nz);
                     
                     glVertex3f( ax, ay, az);//define 1 vértice
                     glVertex3f( bx, by, bz);
                     glVertex3f( cx, cy, cz);
                     glVertex3f( dx, dy, dz);
                     
            }
            glEnd();
    glPopMatrix();
    if (wire) glEnable(GL_LIGHTING);
}


void TBlockSingle::DrawBlinking(TCamera *Camera, float t) {
     glPushMatrix();
           glRotated(Camera->rotx,1,0,0);   // rotacao de camera  x   
           glRotated(Camera->roty,0,1,0);   // rotacao de camera  y  
           glRotated(Camera->rotz,0,0,1);   // rotacao de camera  z  
           glTranslated(Camera->transx, Camera->transy, Camera->transz); // translacao da camera
           glTranslated(x,y,z); // tranlacao da peça
           glRotated(-ry,0,1,0); // rotacao da peça
                 
           
           float vt = (float) ((int) t%200);
           vt =  (vt - 50)/100; // algo variando entre -0.5 e +1.5;
           if (vt > 0.5f) vt = 1.0f - vt;
           
           tcor cortemp;
           HSV (&cortemp, GetHValue(&cor), GetSValue(&cor) + vt*1.2, GetVValue(&cor));
           
           glColor3b( cortemp.r, cortemp.g, cortemp.b); // cor
    
            glBegin(GL_QUADS); // glBegin e glEnd delimitam os vértices que definem uma primitiva
            int num =  modelo->num_poligonos;
            for (int l_index=0; l_index < num; l_index++)
            {  
                     float nx;
                     float ny;
                     float nz;
                     float ax = modelo->vertice[ modelo->poligono[l_index].a ].x;
                     float ay = modelo->vertice[ modelo->poligono[l_index].a ].y;
                     float az = modelo->vertice[ modelo->poligono[l_index].a ].z;
                     float bx = modelo->vertice[ modelo->poligono[l_index].b ].x;
                     float by = modelo->vertice[ modelo->poligono[l_index].b ].y;
                     float bz = modelo->vertice[ modelo->poligono[l_index].b ].z;
                     float cx = modelo->vertice[ modelo->poligono[l_index].c ].x;
                     float cy = modelo->vertice[ modelo->poligono[l_index].c ].y;
                     float cz = modelo->vertice[ modelo->poligono[l_index].c ].z;
                     float dx = modelo->vertice[ modelo->poligono[l_index].d ].x;
                     float dy = modelo->vertice[ modelo->poligono[l_index].d ].y;
                     float dz = modelo->vertice[ modelo->poligono[l_index].d ].z;
                     
    
                     normal(ax,ay,az,bx,by,bz,cx,cy,cz,&nx, &ny, &nz);
                     glNormal3f(nx, ny,nz);
                     
                     glVertex3f( ax, ay, az);//define 1 vértice
                     glVertex3f( bx, by, bz);
                     glVertex3f( cx, cy, cz);
                     glVertex3f( dx, dy, dz);
                     
            }
            glEnd();
    glPopMatrix();
}

// a = angulo de rotação
// d = divisor do tamanho
void TBlockSingle::DrawMini(float a, float d) {
     glPushMatrix();
            glTranslated(x,y,z); // tranlacao da peça
             glRotated(45,1,0,0);
             glRotated(rx,1,0,0); // rotacao da peça
           glRotated(a,0,1,0); // rotacao da peça
           
          
            
           glColor3b(cor.r, cor.g, cor.b); // cor
          
    
            glBegin(GL_QUADS); // glBegin e glEnd delimitam os vértices que definem uma primitiva
            int num =  modelo->num_poligonos;
            for (int l_index=0; l_index < num; l_index++)
            {  
                     float nx;
                     float ny;
                     float nz;
                     float ax = modelo->vertice[ modelo->poligono[l_index].a ].x/d;
                     float ay = modelo->vertice[ modelo->poligono[l_index].a ].y/d;
                     float az = modelo->vertice[ modelo->poligono[l_index].a ].z/d;
                     float bx = modelo->vertice[ modelo->poligono[l_index].b ].x/d;
                     float by = modelo->vertice[ modelo->poligono[l_index].b ].y/d;
                     float bz = modelo->vertice[ modelo->poligono[l_index].b ].z/d;
                     float cx = modelo->vertice[ modelo->poligono[l_index].c ].x/d;
                     float cy = modelo->vertice[ modelo->poligono[l_index].c ].y/d;
                     float cz = modelo->vertice[ modelo->poligono[l_index].c ].z/d;
                     float dx = modelo->vertice[ modelo->poligono[l_index].d ].x/d;
                     float dy = modelo->vertice[ modelo->poligono[l_index].d ].y/d;
                     float dz = modelo->vertice[ modelo->poligono[l_index].d ].z/d;
                     
    
                     normal(ax,ay,az,bx,by,bz,cx,cy,cz,&nx, &ny, &nz);
                     glNormal3f(nx, ny,nz);
                     
                     glVertex3f( ax, ay, az);//define 1 vértice
                     glVertex3f( bx, by, bz);
                     glVertex3f( cx, cy, cz);
                     glVertex3f( dx, dy, dz);
                     
            }
            glEnd();
    glPopMatrix();
     
}

// a = angulo de rotação
// d = divisor do tamanho
void TBlockSingle::DrawMiniC(float a, float d) {
     glPushMatrix();
     
            int num =  modelo->num_poligonos;
            float Mx, mx, Mz, mz;
            Mx = -999;
            mx = 999;
            Mz = -999;
            mz = 999;
            for (int l_index=0; l_index < num; l_index++)
            {  

                     float ax = modelo->vertice[ modelo->poligono[l_index].a ].x/d;
                     float ay = modelo->vertice[ modelo->poligono[l_index].a ].y/d;
                     float az = modelo->vertice[ modelo->poligono[l_index].a ].z/d;
                     float bx = modelo->vertice[ modelo->poligono[l_index].b ].x/d;
                     float by = modelo->vertice[ modelo->poligono[l_index].b ].y/d;
                     float bz = modelo->vertice[ modelo->poligono[l_index].b ].z/d;
                     float cx = modelo->vertice[ modelo->poligono[l_index].c ].x/d;
                     float cy = modelo->vertice[ modelo->poligono[l_index].c ].y/d;
                     float cz = modelo->vertice[ modelo->poligono[l_index].c ].z/d;
                     float dx = modelo->vertice[ modelo->poligono[l_index].d ].x/d;
                     float dy = modelo->vertice[ modelo->poligono[l_index].d ].y/d;
                     float dz = modelo->vertice[ modelo->poligono[l_index].d ].z/d;                     
                     
                     
                     if (ax > Mx) Mx = ax;
                     if (ax < mx) mx = ax;
                     if (az > Mz) Mz = az;
                     if (az < mz) mz = az;
                     if (bx > Mx) Mx = bx;
                     if (bx < mx) mx = bx;
                     if (bz > Mz) Mz = bz;
                     if (bz < mz) mz = bz;
                     if (cx > Mx) Mx = cx;
                     if (cx < mx) mx = cx;
                     if (cz > Mz) Mz = cz;
                     if (cz < mz) mz = cz;       
                     if (dx > Mx) Mx = dx;
                     if (dx < mx) mx = dx;
                     if (dz > Mz) Mz = dz;
                     if (dz < mz) mz = dz;       
                     
            }
            
           float mediax = -(Mx - mx)/3 ;
           float mediaz = -(Mz - mz)/3;
        
           
           glTranslated(x,y,z ); // tranlacao da peça
           glRotated(45,1,0,0);
           glRotated(rx,1,0,0); // rotacao da peça
           glRotated(a,0,1,0); // rotacao da peça
           
          
            
           glColor3b(cor.r, cor.g, cor.b); // cor
          
    
            glBegin(GL_QUADS); // glBegin e glEnd delimitam os vértices que definem uma primitiva
            num =  modelo->num_poligonos;
            for (int l_index=0; l_index < num; l_index++)
            {  
                     float nx;
                     float ny;
                     float nz;
                     float ax = modelo->vertice[ modelo->poligono[l_index].a ].x/d;
                     float ay = modelo->vertice[ modelo->poligono[l_index].a ].y/d;
                     float az = modelo->vertice[ modelo->poligono[l_index].a ].z/d;
                     float bx = modelo->vertice[ modelo->poligono[l_index].b ].x/d;
                     float by = modelo->vertice[ modelo->poligono[l_index].b ].y/d;
                     float bz = modelo->vertice[ modelo->poligono[l_index].b ].z/d;
                     float cx = modelo->vertice[ modelo->poligono[l_index].c ].x/d;
                     float cy = modelo->vertice[ modelo->poligono[l_index].c ].y/d;
                     float cz = modelo->vertice[ modelo->poligono[l_index].c ].z/d;
                     float dx = modelo->vertice[ modelo->poligono[l_index].d ].x/d;
                     float dy = modelo->vertice[ modelo->poligono[l_index].d ].y/d;
                     float dz = modelo->vertice[ modelo->poligono[l_index].d ].z/d;
                     
    
                     normal(ax,ay,az,bx,by,bz,cx,cy,cz,&nx, &ny, &nz);
                     glNormal3f(nx, ny,nz);
                     
                     glVertex3f( ax + mediax, ay, az + mediaz);//define 1 vértice
                     glVertex3f( bx + mediax, by, bz + mediaz);
                     glVertex3f( cx + mediax, cy, cz + mediaz);
                     glVertex3f( dx + mediax, dy, dz + mediaz);
                     
            }
            glEnd();
    glPopMatrix();
     
}


void TBlockSingle::Move(int tecla, TCamera* Camera) {
     switch(tecla) {
        		case GLUT_KEY_UP : 
        				this->z += 2 / DIVISAO * ((((int) (-this->ry / 90) % 4) - 1) % 2) * -((((int) ((Camera->roty + 30) / 90) % 4) - 1) % 2);
        		 		this->x += 2 / DIVISAO * ((((int) (-this->ry / 90) % 4) - 1) % 2) * -((((int) ((Camera->roty + 120) / 90) % 4) - 1) % 2);
        
        				this->z += 2 / DIVISAO * ((((int) ((-this->ry + 90)/ 90) % 4) - 1) % 2) * -((((int) ((Camera->roty + 30) / 90) % 4) - 1) % 2);
        				this->x += 2 / DIVISAO * ((((int) ((-this->ry + 90)/ 90) % 4) - 1) % 2) * -((((int) ((Camera->roty + 120) / 90) % 4) - 1) % 2);
                        break;
        		case GLUT_KEY_DOWN: 
                        this->z -= 2 / DIVISAO * ((((int) (-this->ry / 90) % 4) - 1) % 2) * -((((int) ((Camera->roty + 30) / 90) % 4) - 1) % 2);
        				this->x -= 2 / DIVISAO * ((((int) (-this->ry / 90) % 4) - 1) % 2) * -((((int) ((Camera->roty + 120) / 90) % 4) - 1) % 2);
        
        				this->z -= 2 / DIVISAO * ((((int) ((-this->ry + 90)/ 90) % 4) - 1) % 2) * -((((int) ((Camera->roty + 30) / 90) % 4) - 1) % 2);
        				this->x -= 2 / DIVISAO * ((((int) ((-this->ry + 90)/ 90) % 4) - 1) % 2) * -((((int) ((Camera->roty + 120) / 90) % 4) - 1) % 2);
                        break;
        		case GLUT_KEY_LEFT: 
                        this->x += 2 / DIVISAO * ((((int) (-this->ry / 90) % 4) - 1) % 2) * -((((int) ((Camera->roty + 30) / 90) % 4) - 1) % 2);
        				this->z -= 2 / DIVISAO * ((((int) (-this->ry / 90) % 4) - 1) % 2) * -((((int) ((Camera->roty + 120) / 90) % 4) - 1) % 2);
        
        				this->x += 2 / DIVISAO * ((((int) ((-this->ry + 90) / 90) % 4) - 1) % 2) * -((((int) ((Camera->roty + 30) / 90) % 4) - 1) % 2);
        				this->z -= 2 / DIVISAO * ((((int) ((-this->ry + 90) / 90) % 4) - 1) % 2) * -((((int) ((Camera->roty + 120) / 90) % 4) - 1) % 2);
                        break;
        		case GLUT_KEY_RIGHT: 
                        this->x -= 2 / DIVISAO * ((((int) (-this->ry / 90) % 4) - 1) % 2) * -((((int) ((Camera->roty + 30) / 90) % 4) - 1) % 2);
        				this->z += 2 / DIVISAO * ((((int) (-this->ry / 90) % 4) - 1) % 2) * -((((int) ((Camera->roty + 120) / 90) % 4) - 1) % 2);
        
        				this->x -= 2 / DIVISAO * ((((int) ((-this->ry + 90) / 90) % 4) - 1) % 2) * -((((int) ((Camera->roty + 30) / 90) % 4) - 1) % 2);
        				this->z += 2 / DIVISAO * ((((int) ((-this->ry + 90) / 90) % 4) - 1) % 2) * -((((int) ((Camera->roty + 120) / 90) % 4) - 1) % 2);
                        break;
        	}
}

void TBlockSingle::AddToWorld(TWorld *Mundo) {
  Mundo->AddBlock(this);
}

void TBlockSingle::MoveUp() {
  y += 1/DIVISAO;
}


void TBlockSingle::MoveDown() {
  y -= 1/DIVISAO;
}

void TBlockSingle::RotateCW() {
  ry += 90;
}

void TBlockSingle::RotateCCW() {
  ry -= 90;
}
