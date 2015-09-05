#include "janelas.h"
#include "widget/widgets.h"
#include "widget/loadwindow.h"
#include "interface/simulator_window.h"
#include "interface/loadmap_window.h"

#include <GL/freeglut.h>
#include "widget/support/raster.h"

TInterface *Interface;


void BotaoUpGradeClick(TButton *Button) {
     Interface->CallbackGradeUp();
}

void BotaoDownGradeClick(TButton *Button) {
     Interface->CallbackGradeDown();
}


void WLoad_ListClick(TListBox *ListBox) {
     if (ListBox->SelectedItem != -1)
         ((TTextBox*) ListBox->Display->GetWidgetByName("WLoad_MapName"))->SetText(ListBox->List[ListBox->SelectedItem]);
}

void BotaoTutNext(TButton *Button) {
     Interface->CallbackTutNext();
}
void BotaoTutPrev(TButton *Button) {
     Interface->CallbackTutPrev();
}

void Botao2LoadClick(TButton *Button) {
    Interface->CallbackCarregarTut(
       ((TTextBox*) Interface->Display->GetWidgetByName("WLoad_MapName"))->Text
    );
    
    Interface->Display->GetWidgetByName("WLoad")->Hide();
}


void WLoad_ButtonLoadClick(TButton *Button) {
   Interface->CallbackCarregar(
       ((TTextBox*) Interface->Display->GetWidgetByName("WLoad_MapName"))->Text
   );
   
   Interface->Display->GetWidgetByName("WLoad")->Hide();
}

void BotaoSaveClick(TButton *Button) {
     Interface->CallbackSalvar(Interface->TextSave->Text);
     Interface->JSave->Visible = false;
}

void BotaoFecharClick(TButton *Button) {
     Button->Parent->Visible = false;
}


void BotaoNew(TButton *Button) {
     Interface->CallbackNew();
     Interface->JNew->Visible = false;
}

void BotaoGrade(TButton *Button) {
     Interface->CallbackGrade();
}

void BotaoWire(TButton *Button) {
     Interface->CallbackWire();
}




void BrickButton1Press(TButton *Button) {
    printf("BrickButton saving... ");
    
    
    long bufferSize = sizeof(unsigned char)*4*Button->Display->GetWidth()*Button->Display->GetHeight();
    unsigned char *data = (unsigned char*) malloc(bufferSize);
    char fileName[255];
    
    
    glReadPixels(0, 0, Button->Display->GetWidth(), Button->Display->GetHeight(), GL_RGBA, GL_UNSIGNED_BYTE, data);
    sprintf(fileName, "FramebufferDump_RGBA_%d_%d_before.raw", Button->Display->GetWidth(), Button->Display->GetHeight());
    FILE *arq = fopen(fileName, "wb");
    fwrite (data , sizeof(unsigned char), bufferSize, arq);
    fclose(arq);
    
    std::fill_n(data, bufferSize, 255);
    Raster::setOrthographicProjection(Button->Display->GetWidth(), Button->Display->GetHeight());
        glPushMatrix();
            glLoadIdentity();            
            
            glRasterPos2f(0 , 0);
            glDrawPixels(Button->Display->GetWidth(), Button->Display->GetHeight(), GL_RGBA, GL_UNSIGNED_BYTE, data);
            
        
        glPopMatrix();
    Raster::resetPerpectiveProjection();
    
    
    
    glutPostRedisplay();
    
    
    glReadPixels(0, 0, Button->Display->GetWidth(), Button->Display->GetHeight(), GL_RGBA, GL_UNSIGNED_BYTE, data);
    sprintf(fileName, "FramebufferDump_RGBA_%d_%d_after.raw", Button->Display->GetWidth(), Button->Display->GetHeight());
    arq = fopen(fileName, "wb");
    fwrite (data , sizeof(unsigned char), bufferSize, arq);
    fclose(arq);
    
    
    printf("done\n");
    free(data);
    
    
}

TInterface::TInterface(int w, int h) {
     Display = new TDisplay (w, h);
     Interface = this;
     LoadMapWindow(Display);
     JanelaSave(Display);
     JanelaNew(Display);
     JanelaTut(Display);
     JanelaOpcoes(Display);
     JanelaCoordinates(Display);
     SimulatorWindow(Display);
     
     //TWindow *W = LoadWindowFromFile("src/interface/unused_demonstration_window.xml");
     //Display->AddWidget(W);
     //((TButton*) W->GetChildByName("BrickButton1"))->OnClick = BrickButton1Press;
     
     
    
}

void TInterface::LoadMapWindow(TDisplay *Display) {
   TWindow *W = LoadWindowFromString((char*) src_interface_loadmap_window_xml);
   Display->AddWidget(W);
   
   ((TButton*) Display->GetWidgetByName("Wload_ButtonClose"))->OnClick = BotaoFecharClick;
   ((TButton*) Display->GetWidgetByName("Wload_ButtonLoad"))->OnClick = WLoad_ButtonLoadClick;
   ((TButton*) Display->GetWidgetByName("Wload_ButtonStep"))->OnClick = Botao2LoadClick;
   ((TListBox*) Display->GetWidgetByName("WLoad_LoadList"))->OnClick = WLoad_ListClick;
   
   W->Hide();
     
}

void TInterface::JanelaOpcoes(TDisplay *Display) {
   JOpc = new TWindow();
   JOpc->SetName("JOpc");
   JOpc->Width = 100;
   JOpc->Height = 43;
   JOpc->X = 50;
   JOpc->Y = 50;
   JOpc->Visible = false;
   Display->AddWidget(JOpc);

   TButtonUp *BotaoUp = new TButtonUp(JOpc);
   BotaoUp->SetText("");
   BotaoUp->X = 17;
   BotaoUp->Y = 2;
   BotaoUp->Width = 18;
   BotaoUp->Height = 7; 
   BotaoUp->OnClick = BotaoUpGradeClick;   
   
   TButtonDown *BotaoDown = new TButtonDown(JOpc);
   BotaoDown->SetText("");
   BotaoDown->X = 17;
   BotaoDown->Y = 33;
   BotaoDown->Width = 18;
   BotaoDown->Height = 7; 
   BotaoDown->OnClick = BotaoDownGradeClick;
   
   TButton *Botao = new TButton(JOpc);
   Botao->SetText("Grid");
   Botao->X = 5;
   Botao->Y = 12;
   Botao->Width = 40;
   Botao->Height = 15;     
   Botao->OnClick = BotaoGrade;     
   
   TButton *Botao2 = new TButton(JOpc);
   Botao2->SetText("Wire");
   Botao2->X = 55;
   Botao2->Y = 12;
   Botao2->Width = 40;
   Botao2->Height = 15;     
   Botao2->OnClick = BotaoWire;    
     
}



void TInterface::JanelaNew(TDisplay *Display) {
   
   JNew = new TWindow();
   JNew->Width = 200;
   JNew->Height = 100;
   JNew->X = Display->GetWidth() / 2 - 150;
   JNew->Y = Display->GetHeight() / 2 - 50;
   JNew->Visible = false;
   Display->AddWidget(JNew);

   TButton *BotaoS = new TButton(JNew);
   BotaoS->SetText("Sim");
   BotaoS->X = 10;
   BotaoS->Y = 50;
   BotaoS->Width = 30;
   BotaoS->Height = 30;     
   BotaoS->OnClick = BotaoNew;   
   
   
  TButton *BotaoN = new TButton(JNew);
   BotaoN->SetText("Não");
   BotaoN->X = 150;
   BotaoN->Y = 50;
   BotaoN->Width = 30;
   BotaoN->Height = 30;     
   BotaoN->OnClick = BotaoFecharClick;   

  
   TLabel *Label1 = new TLabel(JNew);
   Label1->SetText("Limpar tela?");
   Label1->X = 10;
   Label1->Y = 25;
           
     
}


void TInterface::JanelaSave(TDisplay *Display) {
   
   
   JSave = new TWindow();
   JSave->Width = 300;
   JSave->Height = 100;
   JSave->X = Display->GetWidth() / 2 - 150;
   JSave->Y = Display->GetHeight() / 2 - 50;
   JSave->Visible = false;
   Display->AddWidget(JSave);

   TButton *Botao = new TButton(JSave);
   Botao->SetText("Fechar");
   Botao->X = 10;
   Botao->Y = 50;
   Botao->Width = 30;
   Botao->Height = 30;     
   Botao->OnClick = BotaoFecharClick;   
   
   
   TButton *Botao3 = new TButton(JSave);
   Botao3->SetText("Salvar");
   Botao3->X = 235;
   Botao3->Y = 50;
   Botao3->Width = 50;
   Botao3->Height = 30; 
   Botao3->OnClick = BotaoSaveClick;

   TextSave = new TTextBox(JSave);
   TextSave->SetText("");
   TextSave->X = 55;
   TextSave->Y = 15;
   TextSave->Width = 230;
   TextSave->Height = 20;    
  
   TLabel *Label1 = new TLabel(JSave);
   Label1->SetText("Nome:");
   Label1->X = 10;
   Label1->Y = 25;
           
     
}

void TInterface::JanelaTut(TDisplay *Display) {
   
   JTut = new TWindow();
   JTut->Width = 160;
   JTut->Height = 70;
   JTut->X = 6*Display->GetWidth() / 10;
   JTut->Y = 2*Display->GetHeight() / 10;
   JTut->Visible = true;
   Display->AddWidget(JTut);
   
   TLabel *Label1 = new TLabel(JTut);
   Label1->SetText("Passo-a-passo");
   Label1->X = 5;
   Label1->Y = 10;
           

   TButton *BotaoA = new TButton(JTut);
   BotaoA->SetText("Anterior");
   BotaoA->X = 5;
   BotaoA->Y = 30;
   BotaoA->Width = 30;
   BotaoA->Height = 30;     
   BotaoA->OnClick = BotaoTutPrev;   
   
   
   TButton *BotaoP = new TButton(JTut);
   BotaoP->SetText("Próximo");
   BotaoP->X = 90;
   BotaoP->Y = 30;
   BotaoP->Width = 30;
   BotaoP->Height = 30;     
   BotaoP->OnClick = BotaoTutNext;   

  

     
}

void TInterface::JanelaCoordinates(TDisplay *Display) {
   JCoord = new TWindow();
   JCoord->X = 50;
   JCoord->Y = 100;
   
   JCoord->Width = 100;
   JCoord->Height = 20;
   
   JCoord->Visible = true;
   Display->AddWidget(JCoord);
   
   LabelCoord = new TLabel(JCoord);
   LabelCoord->SetText("0, 0");
   LabelCoord->X = 5;
   LabelCoord->Y = 10;
 
}




//Simulator Window -------------------------------------------------------------------------------------------

void ButtonMinimizeSWClick(TButton *Button) {
  static int previous_height;
  if (((TWindow*) Button->Parent)->Height != 15) {
    previous_height = ((TWindow*) Button->Parent)->Height;
    ((TWindow*) Button->Parent)->Height = 15;
  }
  else ((TWindow*) Button->Parent)->Height = previous_height;
}

void RoboListClick(TListBox *ListBox) {
    Interface->CallbackSelecionarRobo(((TListBox*) Interface->Display->GetWidgetByName("RoboList"))->SelectedItem);
}

void BotaoQuitClick(TButton *Button) {
    Interface->CallbackSair();
}

void ButtonSimStartStopClick(TButton *Button) {   
    Interface->CallbackIniciar(0);
}

void ButtonAddRobotClick(TButton *Button) {
    Interface->CallbackAdicionar(0);
}

void ButtonAddWaypointClick(TButton *Button) {
    Interface->CallbackAdicionarWaypoint(0);
}


void TInterface::SimulatorWindow(TDisplay *Display) {
   TWindow *W = LoadWindowFromString((char*) src_interface_simulator_window_xml);
  //TWindow *W = LoadWindowFromFile("src/interface/simulator_window.xml");
   
   Display->AddWidget(W);
   
   ((TButton*) W->GetChildByName("ButtonSWMinimize"))->OnClick = ButtonMinimizeSWClick;
   
   ((TButton*) W->GetChildByName("ButtonQuit"))->OnClick = BotaoQuitClick;
   
   ((TListBox*) W->GetChildByName("RoboList"))->OnClick = RoboListClick;
   
   ((TButton*) W->GetChildByName("ButtonAddRobot"))->OnClick = ButtonAddRobotClick;
   
   ((TButton*) W->GetChildByName("ButtonAddWaypoint"))->OnClick = ButtonAddWaypointClick;
   
   ((TButton*) W->GetChildByName("ButtonSimStartStop"))->OnClick = ButtonSimStartStopClick;
}




