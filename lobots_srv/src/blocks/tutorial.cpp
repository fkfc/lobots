#include "tutorial.h"

#define PI 3.1416f

#include <math.h>


TTutorial::TTutorial(TWorld *mundo) {
                            mundo_ref = mundo;
                            atual = -1;
                            prox = true;
                            fim = false;
}

TTutorial::~TTutorial() {
                        delete (mundo_ref);
                        }

void TTutorial::Avancar() {
     Peca_wire->x = ox;
     Peca_wire->y = oy;
     Peca_wire->z = oz;
     Peca_wire->ry = ory;
     
     prox = true;
     }
     
void TTutorial::Voltar() {
     if (atual >= 0) atual -= 2;
     if (atual == -2) atual = -1;
     Peca_wire->x = ox;
     Peca_wire->y = oy;
     Peca_wire->z = oz;
     Peca_wire->ry = ory;     
     prox = true;
     }


void TTutorial::ColocarProx(double t, TCamera *camera) {
                         atual++;
                         prox = false;
                         if (mundo_ref->blockCount > atual + 1) {
                                                 Peca_wire = mundo_ref->blockList[atual];
                                                 ox = Peca_wire->x;
                                                 oy = Peca_wire->y;
                                                 oz = Peca_wire->z;
                                                 ory = Peca_wire->ry;
                                                 
                                                 float xp = -camera->transx + 6*cos(camera->rotx*PI/180)*sin(camera->roty*PI/180);
                                                 float yp = -camera->transy - 6*sin(camera->rotx*PI/180);
                                                 float zp = -camera->transz - 6*cos(camera->rotx*PI/180)*cos(camera->roty*PI/180);
                      
                                                 xp = ((int) (xp*DIVISAO/2))*2/DIVISAO;
                                                 zp = ((int) (zp*DIVISAO/2))*2/DIVISAO;
                                                 yp = ((int) (yp*DIVISAO))/DIVISAO;
                                                 
                                                 Peca_wire->x = xp;
                                                 Peca_wire->y = yp;
                                                 Peca_wire->z = zp;
                                                 Peca_wire->ry = 0;
                      
                                                 distancia = 100;
                                                 passox = (xp - ox)/distancia;
                                                 passoy = (yp - oy)/distancia;
                                                 passoz = (zp - oz)/distancia;
                                                 passory = (0 - ory)/distancia;
                                                 
                                                 
                                                 tempo = (float) ((int) (t*100)%200);
                         } else {
                                fim = true;
                         }
}

void TTutorial::Animar(double t) {
                        t = (float) ((int) (t*100)%200);  
                        if (tempo > t) tempo = t;
                        if (Peca_wire->x != ox || Peca_wire->y != oy || Peca_wire->z != oz || Peca_wire->ry != ory) {
                                if (tempo == 0) tempo = t;
                                
                                if (distancia - (t - tempo) < 5) {
                                              Peca_wire->x = ox;
                                              Peca_wire->y = oy;
                                              Peca_wire->z = oz;
                                              Peca_wire->ry = ory;
                                              
                                } else {
                                                    distancia = distancia - (t - tempo);
                                                    Peca_wire->x = ox + passox*distancia;   
                                                    Peca_wire->y = oy + passoy*distancia;
                                                    Peca_wire->z = oz + passoz*distancia;
                                                    Peca_wire->ry = ory + passory*distancia;
                                }
                                tempo = t;
                        }
                        else {
                               // alguma animação qdo chega no lugar
                               }
                        }

