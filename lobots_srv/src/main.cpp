/*
 * Lobots - Robot Simulator - Server
 *
 */

#include <GL/freeglut.h>
#define PI 3.1416f
#define J_LARG 800
#define J_ALT 600 

#include <stdlib.h>
#include <math.h>

#include <stdio.h>
#include <pthread.h>

#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>


#include "blocks/camera.h"
#include "blocks/balde.h"
#include "blocks/world.h"
#include "blocks/menupecas.h"
#include "blocks/loadsave.h"
#include "blocks/tutorial.h"
#include "simulator/simulador.h"
#include "widget/widgets.h"
#include "janelas.h"
#include "simulator/server_thread.h"
#include "argumentos.h"
#include "blocks/blockmultiple.h"
#include "cronometer.h"

#define ENGLISH 1
#define PORTUGUES 0

#define PORT_LOCAL 9456
#define PORT_SERVER 9123


using namespace std;

int getdir (string dir, vector<string> &files) // pega arquivos no diretorio
{
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        cout << "Error(" << errno << ") opening " << dir << endl;
        return errno;
    }

    while ((dirp = readdir(dp)) != NULL) {
        if (string(dirp->d_name) != ".." && string(dirp->d_name) != ".")
        files.push_back(string(dirp->d_name));
    }
    closedir(dp);
    return 0;
}


int janela_principal; // utilizada pelo glut
//GLUI_StaticText *Gui_Nome;


static int estadoMouse;
static int specialKey;


TCamera *Camera;
TBlockContainer *balde, *balde_esp;
TWorld *mundo, *mundotut;
TMenuPecas *Menu;
//TCaixaEntrada *Caixa;
TTutorial *Tutorial;


static TBlock *Peca_Mao = 0;
static TBlock *Blink = 0;
static bool mao = false;
static float rotacaomenu;
static bool draw_animation = true;
static bool noClip = false;
static bool entrada = false;
static bool load = false;
static bool loadtut = false;
static bool wire = true;
static bool grade = true;
static bool tut = false;
static bool selecao = false;
static bool adicionando_robo = false;
static bool adicionando_obj = false;
static TSimulador *Sim;
static TInterface *Interface;
//static TConexao *Conexao;
static double t = 0; // Elapsed time in seconds;
static int selected_robot = 0; // selected robot in listbox
static bool display = true; //UI
static TCronometer *Cronometer;

int AG_populacao = -1;


void ShowLoadWindow() {
    TWindow *WindowLoad = (TWindow*) Interface->Display->GetWidgetByName("WLoad");
    TListBox *LoadList = (TListBox*) WindowLoad->GetChildByName("WLoad_LoadList");
    LoadList->Items = 0;
    string dir = string("./save");
    vector<string> files = vector<string>();
    getdir(dir,files);
    for (unsigned int i = 0;i < files.size();i++) {
        LoadList->AddItem(files[i].c_str());
    }
    WindowLoad->Show();                                                                     
    Interface->Display->BringOver(WindowLoad);    
    ((TScrollBar*) Interface->Display->GetWidgetByName("WLoad_LoadListVScrollbar"))->RefreshPosition();
}

void Carregar(char *nome) {
    delete(mundo);  
    //glutSetCursor(GLUT_CURSOR_WAIT);  
    Abrir(nome, &mundo, balde, Camera);
    //glutSetCursor(GLUT_CURSOR_CROSSHAIR);  
    if (tut) delete(Tutorial);
    tut = false;     
}

void CallbackGrade() {
     grade = grade^1;
}

void CallbackWire() {
     wire = wire^1;
}

void CarregarTut(char *nome) {
    delete(mundo);  
    mundo = new TWorld();   
    Abrir(nome, &mundotut, balde, Camera);   
    if (tut) delete(Tutorial);
    Tutorial = new TTutorial(mundotut);
    tut = true;      
}

void TutNext() {
     Tutorial->Avancar();
     }
void TutPrev() {
     Tutorial->Voltar();
}

void CallbackSalvar(char *nome){
   Salvar(nome, mundo, Camera);
}     

void CallbackNew() {
   delete(mundo);  
    mundo = new TWorld();         
    if (tut) delete(Tutorial);
    tut = false;     
}

/*
void CallbackConnect(char *srv) {
    int port_local = atoi(Interface->TextPortLocal->Text);
    int port_remote = atoi(Interface->TextPortRemote->Text);
    Conexao = new TConexao(port_local, port_remote, srv, Interface->Display, mundo, balde);         
}
*/

void CallbackGradeUp() {
     mundo->floor_y++;
     
}

void CallbackGradeDown() {
     mundo->floor_y--;
}


void Callback_Adicionar(int id) {
     mao = true;
     
    float xp = -Camera->transx + 6*cos(Camera->rotx*PI/180)*sin(Camera->roty*PI/180);
    float yp = -Camera->transy - 6*sin(Camera->rotx*PI/180);
    float zp = -Camera->transz - 6*cos(Camera->rotx*PI/180)*cos(Camera->roty*PI/180);
    
    xp = ((int) (xp*DIVISAO/2))*2/DIVISAO;
    zp = ((int) (zp*DIVISAO/2))*2/DIVISAO;
    yp = ((int) (yp*DIVISAO))/DIVISAO;
    
    
    //Peca_Mao = new TBlockSingle(balde->GetBlock("robo\\robo.bri"), xp, yp, zp);
    TBlockSingle *PecaRobo = new TBlockSingle(balde->GetBlock("robo/robo.bri"), xp, yp, zp);
    GetCor(COR_CINZA, &PecaRobo->cor);
    Peca_Mao = PecaRobo;
    adicionando_robo = true;
}
void Callback_Iniciar(int id) {
     if (Sim->start == false)
       Sim->Start();
     else 
       Sim->Stop();
}
void Callback_Adicionar_Obj(int id) {
     mao = true;
     
    float xp = -Camera->transx + 6*cos(Camera->rotx*PI/180)*sin(Camera->roty*PI/180);
    float yp = -Camera->transy - 6*sin(Camera->rotx*PI/180);
    float zp = -Camera->transz - 6*cos(Camera->rotx*PI/180)*cos(Camera->roty*PI/180);
    
    xp = ((int) (xp*DIVISAO/2))*2/DIVISAO;
    zp = ((int) (zp*DIVISAO/2))*2/DIVISAO;
    yp = ((int) (yp*DIVISAO))/DIVISAO;
    
    TBlockSingle *PecaObjetivo = new TBlockSingle(balde->GetBlock("robo\\obj.bri"), xp, yp, zp);
    GetCor(COR_VERDE, &PecaObjetivo->cor);
    Peca_Mao = PecaObjetivo;
    adicionando_obj = true;
}

void UI_UpdateRobotList() {
	if (Sim->clientlist_update) {
		int i;
		TListBox* RoboList = ((TListBox*) Interface->Display->GetWidgetByName("RoboList"));
		//clean the list
		for (i = 0; i < Sim->num_available_clients + 1; i++) {
			RoboList->RemoveItem(i);
		}
		if ( Sim->num_available_clients == 0 ) {
			RoboList->AddItem("null");
			RoboList->SelectedItem = 0;
		} else {
			for (i = 0; i < Sim->num_available_clients; i++) {
				RoboList->AddItem(Sim->robotnames[i]);
			}
			RoboList->SelectedItem = RoboList->Items - 1;
		}
		
		Sim->clientlist_update = false;
	}

}

void CallbackSair() {
  exit(0);
}

void CallbackSelecionarRobo(int i) {
  selected_robot = i + 1;
}

/* Callbacks do Glut */

static void 
resize(int width, int height)
{
    

    Camera->janela_largura = width;
    Camera->janela_altura = height;
    Interface->Display->SetSize(width, height);
    

    
    const float ar = (float) width / (float) height;
  
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}


static void key(unsigned char key, int x, int y);

static void DisplayLoop(void) { 
    
    // Simulation batch ---------------------------------------
      if (AG_populacao > 0) {
	if (Sim->start == false && Sim->num_available_clients == AG_populacao) { //inicia automaticamente simulação quando todos os clientes conectarem
	    for (int i = 0; i < Sim->num_available_clients; i++) { //adiciona os robos
	      TBlockSingle *PecaRobo = new TBlockSingle(balde->GetBlock("robo/robo.bri"), 1, 1, 1);
	      
	      Sim->Adicionar_Robo(new TRobo(PecaRobo->x, PecaRobo->y, PecaRobo->z, PecaRobo->ry, balde, Camera, Sim->robotnames[i]), PecaRobo);
	      

	    }
	    Sim->Start(); //inicia a simulacao
	    mao = false;
	    Peca_Mao = 0;
	    adicionando_obj = false;
	    adicionando_robo = false;
	} 
      }
    // ------------------------------------------
    
    
    
    //t: Elapsed time in seconds
    t = Cronometer->GetTime()/1000.0;
    
    if (display == true) {  
	rotacaomenu = t*90.0;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // fundo azul
      
      
	Menu->Draw(rotacaomenu, mao, selecao); // desenha o menu superior
    
      if (noClip) Raster::ImprimeTexto(5,10,GLUT_BITMAP_8_BY_13,"N", Camera->janela_largura, Camera->janela_altura);
      
      char textotela[255];
      float dist = 0;
      if (mao) dist = Peca_Mao->Distance(Camera);

      if (tut) Interface->JTut->Visible = true;
      else Interface->JTut->Visible = false;
      
      //display hand piece coordinates
      if (mao) {
	  char coordinates[20];
	  int x = (int) ((Peca_Mao->x*DIVISAO + 0.5)/2);
	  int z = (int) ((Peca_Mao->z*DIVISAO + 0.5)/2);
	  int y = (int) (Peca_Mao->y*DIVISAO + 0.5);
	  sprintf(coordinates, "%3d,%3d,%3d", x, y, z);
	  Interface->LabelCoord->SetText(coordinates);
      } else {
	  Interface->LabelCoord->SetText("");
      }
      
      
      
      
      if (draw_animation == true) {
	  Interface->Display->Draw(); 
	  
	  if (grade == true) {  
	      float xc =  MAX_X/DIVISAO;
	      float yc =  mundo->floor_y/DIVISAO;
	      float zc =  MAX_Z/DIVISAO;
	      glPushMatrix();
		    glRotated(Camera->rotx,1,0,0);   // rotacao de camera  x   
		    glRotated(Camera->roty,0,1,0);   // rotacao de camera  y  
		    glRotated(Camera->rotz,0,0,1);   // rotacao de camera  z  
		    glTranslated(Camera->transx, Camera->transy, Camera->transz); // translacao da camera              
		    glColor3b(60, 60, 127); // cor
	      
		      glBegin(GL_LINES); 
		      
		      for (float i = 0; i < 60; i++)
		      {           
			      glVertex3f( xc - 6, yc, zc + i*2/DIVISAO - 6);
			      glVertex3f( xc + 6, yc, zc + i*2/DIVISAO - 6);
			      
			      glVertex3f( xc + (i + 1)*2/DIVISAO - 6, yc, zc  - 6);
			      glVertex3f( xc + (i + 1)*2/DIVISAO - 6, yc, zc + 6);
				
		      }
		      glEnd();
	      glPopMatrix();   
	  }
      
	  glColor3b(0,0,0); 
	    
	  if (tut) {
		    if (Tutorial->prox) Tutorial->ColocarProx(t, Camera);
		    if (!Tutorial->fim) {
			Tutorial->Animar(t);
			glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
			Tutorial->Peca_wire->Draw(Camera);
		    }
	  }         
	  
	  bool repeat = false;
	  do {
	      glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
	      if (wire) {
			repeat = !repeat;
			if (repeat) {
				//    glDisable(GL_LIGHTING);
				    glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
			} else {
			      // glEnable(GL_LIGHTING);
			}
	      }
	      
	      if (mao && !selecao) { 
			  if (!noClip) mundo->EvalCollision((TBlockSingle*) Peca_Mao);
			  Peca_Mao->DrawBlinking(Camera, t*400);
	    
	      }
		
	      if (selecao) {
		      Peca_Mao->DrawBlinking(Camera, t*200);     
		      Blink = 0;
		      Blink = mundo->GetCollision((TBlockSingle*) Peca_Mao);
		      if (Blink) {
				Blink->DrawBlinking(Camera, t*400);
				}
		      
		      }
	      
	      for (int i = 0; i < mundo->blockCount; i++) {
			mundo->blockList[i]->Draw(Camera, repeat); 
	      }
	    } while (repeat);
      }
	
      
	  
	if (Sim->start) {
	      Sim->Simulador_loop(t, display);            
	}
	UI_UpdateRobotList();
	
	glutSwapBuffers();
    } else { // display == false
	Sim->Simulador_loop(t, display);
      
    }
   
}

void keyspecial(int key, int x, int y) {
    if (mao) { 
        	Peca_Mao->Move(key, Camera);
        	//mundo->SelecionaTecla(key, &Peca_Mao);
    } else {
           switch (key) {
                  case GLUT_KEY_UP:
                       Camera->CameraY(Camera->XMouse, Camera->YMouse, Camera->XMouse, Camera->YMouse-1);
                       break;
                  case GLUT_KEY_DOWN:
                       Camera->CameraY(Camera->XMouse, Camera->YMouse, Camera->XMouse, Camera->YMouse+1);
                       break;
                  case GLUT_KEY_LEFT:
                       Camera->CameraXZ(Camera->XMouse, Camera->YMouse, Camera->XMouse+1, Camera->YMouse);
                       break;
                  case GLUT_KEY_RIGHT:
                       Camera->CameraXZ(Camera->XMouse, Camera->YMouse, Camera->XMouse-1, Camera->YMouse);
                       break;
           }
    
    }
    
    if (!entrada) {
           switch(key) {
                case GLUT_KEY_F5:
                    Interface->JSave->Visible = true;
                    Interface->Display->BringOver(Interface->JSave);
                    break;
                case GLUT_KEY_F8:
                    ShowLoadWindow();
                    break;
                case GLUT_KEY_F9:
                    ShowLoadWindow();
                    break;
           }
   }

}
static void key(unsigned char key, int x, int y) {
     if (Interface->Display->IsVisible() && Interface->Display->HasFocus()) {
      Interface->Display->KeyPress(key); 
      return; 
   } 
   

  	if ( glutGetModifiers() == GLUT_ACTIVE_CTRL ) key += 96 ;

    switch (key) 
    {
        case 27 : 
             if (mao) {
                      delete(Peca_Mao);
                      Blink = 0;
                      selecao = false;
                      mao = false;
                      }	  
        case 'q':
            exit(0);
            break;


        case 'o':
            if (tut) Tutorial->Voltar();
            break;    
        case 'p':
            if (tut) Tutorial->Avancar();
            break;        
        case 'z': // cancela peca ou ctrl + z
            if (glutGetModifiers() == GLUT_ACTIVE_CTRL) {
                   Peca_Mao = mundo->blockList[mundo->blockCount - 1];
                    if (Peca_Mao) {
                       mao = true; 
                       mundo->RemoveBlock((TBlockSingle*) Peca_Mao);
		       //if (Conexao->connected == true) Conexao->Send((TBlockSingle*) Peca_Mao, true);
                    } else  mao = false;
                                   
            } else {                       
                    if (mao) delete(Peca_Mao);
                    Peca_Mao = 0;
                    mao = false;
                    adicionando_robo = false;
            }
            break;            
        case 'x': // rotaciona peça no sentido anti-horario
            if (mao) {
            Peca_Mao->RotateCCW();
             }
            break;                 
       case 'c': // rotaciona peça no sentido horario
            if (mao) Peca_Mao->RotateCW();
            break;
       case 'n': // noClip
            noClip = noClip^1;
            break;
        case 'f': // sobe peça
            if (mao) Peca_Mao->MoveUp();
            break;
        case 'v': // desce peça
            if (mao) Peca_Mao->MoveDown();
            break;
        case ' ': // insere peça que esta na mao 
            if (mao) {
                     if (adicionando_robo) {
					    char *robotname = Sim->robotnames[selected_robot - 1];
					    printf("%d robot %s added\n", selected_robot, robotname);
					    TBlockSingle *PecaRobo = (TBlockSingle*) Peca_Mao;
					    Sim->Adicionar_Robo(new TRobo(PecaRobo->x, PecaRobo->y, PecaRobo->z, PecaRobo->ry, balde, Camera, robotname), PecaRobo);
                                           
                                           }
                     else if (adicionando_obj) {
                                            int x_obj = (int) ((Peca_Mao->x*DIVISAO + 0.5)/2);
                                            int z_obj = (int) ((Peca_Mao->z*DIVISAO + 0.5)/2);
                                            int y_obj = (int) (Peca_Mao->y*DIVISAO + 0.5);    
                                            SendWaypoint(Sim->GetRobot(Sim->robotnames[selected_robot - 1]), x_obj, y_obj, z_obj);
                                            
                          }
                     //adicionando objetivo ou robo
                     if (adicionando_obj || adicionando_robo) {
			  Peca_Mao->AddToWorld(mundo);
			  mao = false;
			  Peca_Mao = 0;
			  adicionando_obj = false;
			  adicionando_robo = false;
		     } else { //peças normais
			    if (mao && !selecao) {
				    Peca_Mao->AddToWorld(mundo);
				    //if (Conexao->connected == true) Conexao->Send(Peca_Mao);
				    mao = false;
				    Peca_Mao = 0;
				    if (tut) {
					      Tutorial->prox = true;
					      }
			    }
			    if (mao && selecao) {
				    delete(Peca_Mao);
				    mao = false;
				    selecao = false;
				    if (Blink) {
					      mundo->RemoveBlock((TBlockSingle *) Blink);
					      //if (Conexao->connected == true) Conexao->Send(Blink, true);
					      Peca_Mao = Blink;
					      mao = true;
					      }
			    }		       
		     }
		     
                     
            }
            glutSetCursor(GLUT_CURSOR_CROSSHAIR);
            break;     
            
            
        case 's': // modo de seleção de peça
             if (mao) { 
			 Peca_Mao->AddToWorld(mundo);
                         mao = false;
                         Peca_Mao = 0;
            }
            Camera->iniciado = false;
            {
		float xp = -Camera->transx + 6*cos(Camera->rotx*PI/180)*sin(Camera->roty*PI/180);
		float yp = -Camera->transy - 6*sin(Camera->rotx*PI/180);
		float zp = -Camera->transz - 6*cos(Camera->rotx*PI/180)*cos(Camera->roty*PI/180);  
		
		xp = ((int) (xp*DIVISAO/2))*2/DIVISAO;
		zp = ((int) (zp*DIVISAO/2))*2/DIVISAO;
		yp = ((int) (yp*DIVISAO))/DIVISAO;  
		Peca_Mao =  new TBlockSingle(balde_esp->GetBlock(0), xp, yp, zp);
		mao = true;
		selecao = true;
            }
            break;      
	case 'w':
	      wire ^= 1;   
	      break;
        case 'l':
	      draw_animation ^= 1;
	      break;
	case 't':  //teste com múltiplas peças
	  
		//Peca_Mao = new TBlockMultiple( balde->GetBlock(0);
		float xp = -Camera->transx + 6*cos(Camera->rotx*PI/180)*sin(Camera->roty*PI/180);
		float yp = -Camera->transy - 6*sin(Camera->rotx*PI/180);
		float zp = -Camera->transz - 6*cos(Camera->rotx*PI/180)*cos(Camera->roty*PI/180);
		
		xp = ((int) (xp*DIVISAO/2))*2/DIVISAO;
		zp = ((int) (zp*DIVISAO/2))*2/DIVISAO;
		yp = ((int) (yp*DIVISAO))/DIVISAO;
		
		Peca_Mao = new TBlockMultiple(xp, yp, zp);
		((TBlockMultiple*) Peca_Mao)->AddBlock( new TBlockSingle(balde->GetBlock(0)), 0, 0, 0);
		((TBlockMultiple*) Peca_Mao)->AddBlock( new TBlockSingle(balde->GetBlock(0)), 0, 1, 1);
		
		mao = true;
		
	      break;      


    }

    glutPostRedisplay();
}

void processMouse(int button, int state, int x, int y) {
   if (Interface->Display->IsVisible()) {
                                       
      if (button == GLUT_LEFT_BUTTON) {                       
         if (state == GLUT_UP) Interface->Display->ClickUp(x, y);  
         else Interface->Display->ClickDown(x, y);
      }  
   }// else {
     specialKey = glutGetModifiers();
     estadoMouse = button;
     if (state != GLUT_UP) {
               Camera->IniciaMovimento(x,y);
     }
     
     if (state == GLUT_UP) {
               glutSetCursor(GLUT_CURSOR_CROSSHAIR);
               }
     bool mao_ant = mao;
     if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
             if (Menu->MouseAtivo(x, y, &Peca_Mao)) {
                     if (Menu->ferramentaAtiva == 0) {
                         key('s', 0, 0);                      
                     } else if (Menu->ferramentaAtiva == 1) {
                            key('c', 0, 0);                      
                     } else if (Menu->ferramentaAtiva == 2) {
                            key('x', 0, 0);
                     } else if (Menu->ferramentaAtiva == 3) {
                            key(' ', 0, 0);              
                     } else if (Menu->ferramentaAtiva == 4) {
                            key('z', 0, 0);       
                     } else if (Menu->ferramentaAtiva == 12) {
			    ShowLoadWindow();
                     } else if (Menu->ferramentaAtiva == 11) {
                            keyspecial(GLUT_KEY_F5, 0, 0);
                     } else if (Menu->ferramentaAtiva == 9) {
                            Interface->JNew->Visible = true;
                            Interface->Display->BringOver(Interface->JNew);
                     } /*else if (Menu->ferramentaAtiva == 7) {
                            Interface->JCon->Visible = true;
                            Interface->Display->BringOver(Interface->JCon);
                     }*/
                     
                     else {
                         if (Peca_Mao) {
                             mao = true;
                             if (!mao_ant && Peca_Mao->tipo == TBLOCKSINGLE) mundo->DropBlock((TBlockSingle*) Peca_Mao);
                         } else mao = false;
                     }
             }
             
     }



	
//   }
}
void passivoMouse(int x, int y) {
     if (Interface->Display->IsVisible() && Interface->Display->MouseOver(x, y) == true) {
                                          Interface->Display->MouseMove(x, y);
                                          return;
                                          }

             glutSetCursor(GLUT_CURSOR_CROSSHAIR);
             Menu->MousePassivo(x, y);
  
     
}



void motionMouse(int x, int y) { 
     if (Interface->Display->IsVisible() && Interface->Display->MouseOver(x, y) == true) {
                                          Interface->Display->MouseMove(x, y);
                                          return;
                                          }
     glutSetCursor(GLUT_CURSOR_CROSSHAIR);
     
     if (mao && estadoMouse == GLUT_LEFT_BUTTON ) { // arrasta a peça ativa
        glutSetCursor(GLUT_CURSOR_NONE);
        int xa = Camera->XMouse;
        int ya = Camera->YMouse;
        float px = Peca_Mao->x;
        float py = Peca_Mao->y;
        float pz = Peca_Mao->z;
        float dist = Peca_Mao->Distance(Camera) / 20 ;
        int difX = (x - xa) * dist * dist;
        int difY = (y - ya) * dist * dist;
    
        if (difX != 0) Camera->XMouse = x;
        if (difY != 0) Camera->YMouse = y;
        
        int arrowkeyX, arrowkeyY;
        if (difX > 0) arrowkeyX = GLUT_KEY_RIGHT; else arrowkeyX = GLUT_KEY_LEFT;
        if (difY < 0) arrowkeyY = GLUT_KEY_UP; else arrowkeyY = GLUT_KEY_DOWN;
        
        if (specialKey != GLUT_ACTIVE_SHIFT) { 
                if (((int) Camera->rotx)%360 < 20 || ((int) Camera->rotx)%360 > 340) { // visao de frente
                   float aux;
                   difY = - difY;
                   float cr = cos(Camera->roty*PI/180.0);
                   float sr = sin(Camera->roty*PI/180.0);
    
                   int passos = difY;
                   Peca_Mao->y += passos * 1/DIVISAO;
                   if (difX != 0) Peca_Mao->Move(arrowkeyX, Camera);
           
                } else { // visao de cima
               
                    if (difX != 0) Peca_Mao->Move(arrowkeyX, Camera);
                    if (difY != 0) Peca_Mao->Move(arrowkeyY, Camera);
                }
        } else if (specialKey == GLUT_ACTIVE_SHIFT) {
                if (((int) Camera->rotx)%360 < 20 || ((int) Camera->rotx)%360 > 340) { // visao de frente
                
                    if (difX != 0) Peca_Mao->Move(arrowkeyX, Camera);
                    if (difY != 0) Peca_Mao->Move(arrowkeyY, Camera);
                
                } else { // visao de cima
                   float aux;
                   difY = - difY;
                   float cr = cos(Camera->roty*PI/180.0);
                   float sr = sin(Camera->roty*PI/180.0);
                   int passos = difY;
                   Peca_Mao->y += passos * 1/DIVISAO;
                   if (difX != 0) Peca_Mao->Move(arrowkeyX, Camera);
                }               
        }     
     }
     else {     
         if (specialKey != GLUT_ACTIVE_SHIFT && estadoMouse == GLUT_LEFT_BUTTON) {  // L -> arrasta
            Camera->CameraXZ(Camera->XMouse, Camera->YMouse, x, y); 
         }
         if (specialKey == GLUT_ACTIVE_SHIFT && estadoMouse == GLUT_LEFT_BUTTON) { // shift + L -> cima/baixo
            if (!mao) Camera->CameraXY(Camera->XMouse, Camera->YMouse, x, y);
         }
         if (estadoMouse == GLUT_RIGHT_BUTTON) { // R -> olha para os lados
            Camera->CameraRotXY(Camera->XMouse, Camera->YMouse, x, y); 
         }
         if (estadoMouse == GLUT_MIDDLE_BUTTON) { // sobe e desce a camera
            Camera->CameraY(Camera->XMouse, Camera->YMouse , x, y);
         }
     }

}


static void idle(void) {
  if ( glutGetWindow() != janela_principal ) 
    glutSetWindow(janela_principal);  
    
    glutPostRedisplay();
}



const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 0.0f };
const GLfloat light_position[] = { -1.5f, 1.0f, -0.0f, 1.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };


/* main */

int main(int argc, char *argv[]) {
      
         
    balde = new TBlockContainer("pecas.txt");
    balde_esp = new TBlockContainer("pecas_esp.txt");
    
    mundo = new TWorld();
    Camera = new TCamera (- MAX_X/DIVISAO, - MAX_Y/DIVISAO/2,-MAX_Z/DIVISAO, 41, 60, 0, J_LARG, J_ALT);
    Menu = new TMenuPecas(balde, balde_esp, Camera);
    Sim = new TSimulador(mundo);
    Cronometer = new TCronometer();
    
    
    
   Interface = new TInterface(Camera->janela_largura, Camera->janela_altura);
   Interface->CallbackCarregar = Carregar;
   Interface->CallbackCarregarTut = CarregarTut;
   Interface->CallbackSalvar = CallbackSalvar;
   Interface->CallbackNew = CallbackNew;
   Interface->CallbackTutPrev = TutPrev;
   Interface->CallbackTutNext = TutNext;
   //Interface->CallbackConnect = CallbackConnect;
   Interface->CallbackGrade = CallbackGrade;
   Interface->CallbackWire = CallbackWire;
   Interface->CallbackGradeUp = CallbackGradeUp;
   Interface->CallbackGradeDown = CallbackGradeDown;
   Interface->JOpc->Visible = true;
   Interface->CallbackSair = CallbackSair;
   Interface->CallbackIniciar = Callback_Iniciar;
   Interface->CallbackAdicionar = Callback_Adicionar;
   Interface->CallbackAdicionarWaypoint = Callback_Adicionar_Obj;
   Interface->CallbackSelecionarRobo = CallbackSelecionarRobo;
   
   TListBox *RoboList = ((TListBox*) Interface->Display->GetWidgetByName("RoboList"));
   RoboList->AddItem("null");
   RoboList->SelectedItem = 0;
    
    
   //Conexao = new TConexao();
   
   
   TArgumentos* Argumentos = new TArgumentos(argc, argv);  
   
  //*****************************
  // Starting the server thread
  pthread_t server_th;
  int execute_thread;
  ptsrv_thread_args thread_args = (ptsrv_thread_args) malloc (sizeof(tsrv_thread_args));
  
  if (Argumentos->isSet("port")) {
      thread_args->port = atoi(Argumentos->getVal("port"));
  } else {
      thread_args->port = 7123; 
  }
  
  thread_args->sim = Sim;
  
  execute_thread = pthread_create(&server_th,NULL,srv_thread_function,(void *) thread_args);
  
  
  
  
  
  //Load map from command line
  if (Argumentos->isSet("")) Carregar(Argumentos->getVal(""));
  
  //AG - set populacao
  if (Argumentos->isSet("pop")) AG_populacao = atoi(Argumentos->getVal("pop"));
  
  //Desliga clock do tempo real
  if (Argumentos->isSet("no_clock")) Sim->sim_real_time = false;
  
  //não desenha mapa por default
  if (Argumentos->isSet("skip_draw")) draw_animation = false;
  
  //não desenha interface gráfica
  if (Argumentos->isSet("no_display")) display = false;
  
  //forces clock FPS (not real-time)
  if (Argumentos->isSet("clock_fps")) Sim->force_fps = atoi(Argumentos->getVal("clock_fps"));
  
  
  Cronometer->Start();
  
  if (display) {  
    
    glutInit(&argc, argv);
    
    
    glutInitWindowSize(Camera->janela_largura,Camera->janela_altura);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);

    janela_principal = glutCreateWindow("Lobots");

    glutReshapeFunc(resize);
    glutDisplayFunc(DisplayLoop);
    glutKeyboardFunc(key);
    glutSpecialFunc(keyspecial);
    glutMotionFunc(motionMouse);
    glutPassiveMotionFunc(passivoMouse);
    glutMouseFunc(processMouse);
    glutIdleFunc(idle);

    glClearColor(0.80,0.95,1,1);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);


    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);


    glutSetCursor(GLUT_CURSOR_CROSSHAIR);
    
    
    glutMainLoop();    
  } else {
    for (;;) {
      DisplayLoop();
    }
  }

    
    
    
    
    
  
 
  
    

  
  //*** exit
  pthread_exit(NULL);
  return EXIT_SUCCESS;
}

