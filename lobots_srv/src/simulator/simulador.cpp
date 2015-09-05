#include "simulador.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "server_thread.h"
#define PI 3.1416f


TRobo::TRobo(float x, float y, float z, float ang, TBlockContainer *balde, TCamera *camera, char* name) {
               odometro = 0;
               velocidade = 0;
               angulo = 0;
               angulo_atual = ang;
               bussola = -ang;
               strcpy(robot_name, name);
	       elapsed_time = 0;
               for (int i = 0; i < SIM_NUM_LASERS; i++) {
                   laser[i] = 255;
               }
               this->camera = camera;
               
               robo_lego[0] = new TBlockSingle(balde->GetBlock("robo/perna.bri"), x, y, z);
               robo_lego[1] = new TBlockSingle(balde->GetBlock("robo/corpo.bri"), x, y + 3/DIVISAO, z);
               robo_lego[2] = new TBlockSingle(balde->GetBlock("robo/cabeca.bri"), x, y + 6/DIVISAO, z);

               GetCor(COR_CINZA, &robo_lego[0]->cor);
               GetCor(COR_AZUL, &robo_lego[1]->cor);
               GetCor(COR_AMARELO, &robo_lego[2]->cor);
               
               robo_lego[0]->ry = ang;
               robo_lego[1]->ry = ang;
               robo_lego[2]->ry = ang;
               
               this->x = (int) ((x*DIVISAO + 0.5)/2);
               this->z = (int) ((z*DIVISAO + 0.5)/2);
               this->y = (int) (y*DIVISAO + 0.5);

               
}

void TRobo::sim_loop(double t, bool display) {
                  this->elapsed_time += t;
                  /*
                  
                  fazer a movimentacao das pecas
                  
                  */     
                  if (angulo != 0) { //rotacao
		     float a = t*50;
		     if (a > fabs(angulo)) a = fabs(angulo);
                     robo_lego[0]->ry -= a*(angulo/fabs(angulo));
                     robo_lego[1]->ry -= a*(angulo/fabs(angulo));
                     robo_lego[2]->ry -= a*(angulo/fabs(angulo));

                     angulo_atual = robo_lego[0]->ry;
                     angulo -= a*(angulo/fabs(angulo));
                  }

                  
                  float nx = robo_lego[0]->x - velocidade*t*sin(angulo_atual*PI/180); // novas coordenadas x e z do robo
                  float nz = robo_lego[0]->z + velocidade*t*cos(angulo_atual*PI/180);
                  
                  bool colisao = false;  // verificar colisao
                  for (int altura = 1; altura < SIM_ALTURA_ROBO; altura++) {
		        //colisao original, um bloco somente
                        colisao = colisao ||  Sim->mundo->volumeMap[(int) ((nx*DIVISAO + 0.5)/2)][(int) (robo_lego[0]->y*DIVISAO + 0.5 + altura) ][(int) ((nz*DIVISAO + 0.5)/2)] != 0;
		    
		      /*
		        //2 blocks
			int mult_x = (int (fabs(cos(angulo_atual*PI/180)) + 0.5)); //1 se angulo prox de 0 ou 180
			int mult_z = (int (fabs(sin(angulo_atual*PI/180)) + 0.5)); //1 se angulo prox de 90 ou 270
		    
			//perna esquerda
			colisao = colisao ||  Sim->mundo->volumeMap[(int) ((nx*DIVISAO + 0.5 + 0.6*mult_z)/2)][(int) (robo_lego[0]->y*DIVISAO + 1 + altura) ][(int) ((nz*DIVISAO + 0.5 - 1.5*mult_x)/2)] != 0; //tras
			colisao = colisao ||  Sim->mundo->volumeMap[(int) ((nx*DIVISAO + 0.5 - 2.0*mult_z)/2)][(int) (robo_lego[0]->y*DIVISAO + 1 + altura) ][(int) ((nz*DIVISAO + 0.5 + 1.0*mult_x)/2)] != 0; //frente
			
			
			//perna direita
			colisao = colisao ||  Sim->mundo->volumeMap[(int) ((nx*DIVISAO + 0.5 + 0.6*mult_z)/2) - 1*mult_x][(int) (robo_lego[0]->y*DIVISAO + 1 + altura) ][(int) ((nz*DIVISAO + 0.5 - 1.5*mult_x)/2) - 1*mult_z] != 0; //tras
			colisao = colisao ||  Sim->mundo->volumeMap[(int) ((nx*DIVISAO + 0.5 - 2.0*mult_z)/2) - 1*mult_x][(int) (robo_lego[0]->y*DIVISAO + 1 + altura) ][(int) ((nz*DIVISAO + 0.5 + 1.0*mult_x)/2) - 1*mult_z] != 0; //frente
		    */
                  }
                  
                  //colliding with other robots?
                  colisao = colisao || hitOtherRobots(nx, robo_lego[0]->y + 1/DIVISAO, nz);
                  
                  if (!colisao) { // se nao for colidir, andar
                      robo_lego[0]->x = nx;
                      robo_lego[0]->z = nz;
                      robo_lego[1]->x = nx;
                      robo_lego[1]->z = nz;
                      robo_lego[2]->x = nx;
                      robo_lego[2]->z = nz;
                  }
                  
                  
		  /* 
		  colocar o robo no chao
		  */
		  int px = (int) ((robo_lego[0]->x*DIVISAO + 0.5)/2);
		  int pz = (int) ((robo_lego[0]->z*DIVISAO + 0.5)/2);
		  int py = (int) (robo_lego[0]->y*DIVISAO + 0.5);
		  if (Sim->mundo->volumeMap[px][py][pz] != 0 && Sim->mundo->volumeMap[px][py+2][pz] == 0) py = py + 2; //sobe um degrau, se necessario
		  if (Sim->mundo->volumeMap[px][py - 1][pz] == 0) py--; //cair se necessario
		  robo_lego[0]->y = py/DIVISAO;
		  robo_lego[1]->y = (py + 3)/DIVISAO;
		  robo_lego[2]->y = (py + 6)/DIVISAO;
			  
                            
                  
                  
                  
                  /* 
                  
                  atualizar marcadores: odometro, laser
                  
                  */
                  
                  //if (!colisao) odometro += fabs(velocidade*t)*DIVISAO/2 ;
		  if (!colisao) odometro += velocidade*t*DIVISAO/2 ;
                  //if (colisao) velocidade = 0;
		  
		  
               
                  
                  for (int laser_num = 0; laser_num < SIM_NUM_LASERS; laser_num++) {
                      float d = 0;
                      int px = (int) ((robo_lego[0]->x*DIVISAO + 0.5)/2);
                      int pz = (int) ((robo_lego[0]->z*DIVISAO + 0.5)/2);
                      int py = (int) (robo_lego[0]->y*DIVISAO + 0.5) + 2; // laser eh medido na metade da altura da perna (altura 1)
                      
                      
                      float x_t = robo_lego[0]->x; float y_t = robo_lego[0]->y + 1/DIVISAO; float z_t = robo_lego[0]->z; //variaveis para testar colisao do laser com outros robos
                      while (Sim->mundo->volumeMap[px][py][pz] == 0 && hitOtherRobots(x_t, y_t, z_t) == false && d < 10) {
                            d += 0.01;
                            px = (int) (( (robo_lego[0]->x - d*sin(angulo_atual*PI/180 + PI/2 - laser_num*PI/(SIM_NUM_LASERS - 1) ))*DIVISAO + 0.5)/2);
                            pz = (int) (( (robo_lego[0]->z + d*cos(angulo_atual*PI/180 + PI/2 - laser_num*PI/(SIM_NUM_LASERS - 1)))*DIVISAO + 0.5)/2);
			    
			    x_t = (robo_lego[0]->x - d*sin(angulo_atual*PI/180 + PI/2 - laser_num*PI/(SIM_NUM_LASERS - 1) ));
			    z_t = (robo_lego[0]->z + d*cos(angulo_atual*PI/180 + PI/2 - laser_num*PI/(SIM_NUM_LASERS - 1)));
			    
                      }
                      laser[laser_num] = d*((float) DIVISAO)/2.0f;    
                      if (laser[laser_num] > 10) laser[laser_num] = 10;
                  //    if (d < 10) printf("%d : %f\n", laser_num, laser[laser_num] );
                  }
                  this->x = (int) ((robo_lego[0]->x*DIVISAO + 0.5)/2); // coordenadas do robo
                  this->z = (int) ((robo_lego[0]->z*DIVISAO + 0.5)/2);
                  this->y = (int) (robo_lego[0]->y*DIVISAO + 0.5);
                  

                  bussola = -angulo_atual;



                  
                  /*
                  
                  desenhar as pecas
                      
                  */ 
		  if (display == true) {
		    robo_lego[0]->Draw(camera);
		    robo_lego[1]->Draw(camera);
		    robo_lego[2]->Draw(camera);
		  }
                  
                  
                  
         
                  
                  
}

bool TRobo::hitOtherRobots(float x_t, float y_t, float z_t) { //verifica se as coordenadas colidem com os outros robos da simulacao
    for (int i = 0; i < Sim->num_available_clients; i++) { //for all robots
        TRobo *R = Sim->GetRobot(i);
	if (R != this) { //if this one isn't myself
	  if (R->insideHitBox(x_t, y_t, z_t) == true) return true;
	}
    }
    return false;
}

//verifica se as coordenadas fornecidas são internas ao volume do robô
bool TRobo::insideHitBox(float x_t, float y_t, float z_t) {
  bool x_hit = (x_t >= (robo_lego[0]->x - 2/DIVISAO) && (x_t <= robo_lego[0]->x + 2/DIVISAO));
  bool z_hit = (z_t >= (robo_lego[0]->z - 2/DIVISAO) && (z_t <= robo_lego[0]->z + 2/DIVISAO));
  bool y_hit = (y_t >= (robo_lego[0]->y)  && (y_t <= robo_lego[0]->y + SIM_ALTURA_ROBO/DIVISAO));
  
  
  bool hit = x_hit && y_hit && z_hit;
  
  //printf("%d x_t = %f , y_t = %f , z_t = %f   |  rx = %f, ry = %f, rz = %f\n", hit, x_t, y_t, z_t, robo_lego[0]->x , robo_lego[0]->y , robo_lego[0]->z );
  
  
  
  return (hit);
}



void TRobo::Teleport(int xt, int yt, int zt) {
  robo_lego[0]->x = ((float) xt*2)/DIVISAO; robo_lego[0]->y = ((float) yt)/DIVISAO;     robo_lego[0]->z = ((float) zt*2)/DIVISAO;
  robo_lego[1]->x = ((float) xt*2)/DIVISAO; robo_lego[1]->y = (((float) yt)+3)/DIVISAO; robo_lego[1]->z = ((float) zt*2)/DIVISAO;
  robo_lego[2]->x = ((float) xt*2)/DIVISAO; robo_lego[2]->y = (((float) yt)+6)/DIVISAO; robo_lego[2]->z = ((float) zt*2)/DIVISAO;
}


TSimulador::TSimulador(TWorld* mundo) {
               start = false;
               num_robos = 0;
               num_available_clients = 0;
               clientlist_update = false;
               this->mundo = mundo;
               ta = 0;
	       sim_real_time = true;
	       elapsed_time = 0;
	       force_fps = 10;
}

void TSimulador::Simulador_loop(double rtc, bool display) {
     if (sim_real_time == true) {
	if (ta == 0) ta = rtc;
	if (ta > rtc) ta = rtc;
	elapsed_time = rtc;
     } else {
        rtc = 1.0/force_fps;
	ta = 0;
	elapsed_time += (1.0/force_fps);
     }
     
     for (int i = 0; i < num_robos; i++) {
         robos[i]->sim_loop(rtc - ta, display);
     }
     ta = rtc;
}

void TSimulador::Adicionar_Robo(TRobo *robo, TBlockSingle* peca_robo) {
     robos[num_robos] = robo; 
     robos[num_robos]->Sim = this;
     pecas_robos[num_robos] = peca_robo;
     num_robos++;
}

void TSimulador::Start() {
     start = true;
     for (int i = 0; i < num_robos; i++) {
         mundo->RemoveBlock(pecas_robos[i]);
         delete(pecas_robos[i]);
         StartRobot(robos[i]); //function located in server_thread
     }
}

void TSimulador::Stop() {
     start = false;
     for (int i = 0; i < num_robos; i++) {
         StopRobot(robos[i]); //function located in server_thread
     }
}

TRobo* TSimulador::GetRobot(char *robot_name) {
	int i = 0;
	while (i < num_robos && strcmp(robotnames[i], robot_name) != 0) i++;
	if (i < num_robos) return robos[i];
	else return 0;
}

TRobo* TSimulador::GetRobot(int i) {
	if (i < num_robos) return robos[i];
	else return 0;
}
