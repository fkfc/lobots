#ifndef simuladorH
#define simuladorH

#include "../blocks/balde.h"
#include "../blocks/world.h"
#define SIM_NUM_LASERS 19
#define SIM_MAX_ROBOS 50
#define SIM_ALTURA_ROBO 6

class TSimulador;

class TRobo {
      public:     
          TRobo(float x, float y, float z, float ang, TBlockContainer *balde, TCamera* camera, char* name ); // funcao_robo((void*) TRobo);
          float odometro; // distancia percorrida 
          float bussola;
          float velocidade;
          float angulo; 
          float laser[SIM_NUM_LASERS];
          void (*algoritmo_loop)(void*);
          void sim_loop(double deltat, bool display);
          TSimulador *Sim;
          int x, y, z; // coordenadas do robo
          double elapsed_time; //seconds
          char robot_name[150];
	  bool insideHitBox(float x_t, float y_t, float z_t); //verifica se as coordenadas fornecidas são internas ao volume do robô
	  bool hitOtherRobots(float x_t, float y_t, float z_t); //verifica se as coordenadas colidem com os outros robos da simulacao
	  void Teleport(int xt, int yt, int zt);
      private:
          float angulo_atual; // usado como referencia para desenhar    
          TBlockSingle *robo_lego[3];
          TCamera *camera;
};


class TSimulador {
      public:
           TSimulador(TWorld* mundo);
           bool start;  
           void Simulador_loop(double rtc, bool display);
           void Adicionar_Robo(TRobo *robo, TBlockSingle *peca_robo);
           void Start();
	   void Stop();
           TWorld *mundo;
           TRobo* GetRobot(char *robot_name);
	   TRobo* GetRobot(int i);
           int num_available_clients; // number of robots connected, to be made available on the interface for the user to add
           char robotnames[SIM_MAX_ROBOS][150]; // connected to num_available_clients
           bool clientlist_update; // is set to true when a new client connects, then the interface is updated and variable is set to false again
           bool sim_real_time;
	   double elapsed_time; //seconds
	   double force_fps;
      private:
           int num_robos;   // robots that are actually inserterd in the simulation
           TRobo *robos[SIM_MAX_ROBOS];   // connected to num_robos
           TBlockSingle *pecas_robos[SIM_MAX_ROBOS];
           double ta; // tempo anterior
           
           
};


#endif
