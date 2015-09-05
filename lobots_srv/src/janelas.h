#ifndef janelas_H
#define janelas_H
#include "widget/widgets.h"

class TInterface {
      
      public:
      TInterface(int w, int h);
      TDisplay* Display;
      void LoadMapWindow(TDisplay *Display);
      void JanelaSave(TDisplay *Display);
      void JanelaNew(TDisplay *Display);
      void JanelaTut(TDisplay *Display);
      void JanelaOpcoes(TDisplay *Display);
      void JanelaCoordinates(TDisplay *Display);
      void SimulatorWindow(TDisplay *Display);

      TWindow *JSave;
      TWindow *JNew;
      TWindow *JTut;
      TWindow *JOpc;
      TTextBox *TextSave;
      
      TWindow *JCoord;
      TLabel *LabelCoord;
      
      void (*CallbackSair)();
      void (*CallbackIniciar)(int id);
      void (*CallbackAdicionar)(int id);
      void (*CallbackAdicionarWaypoint)(int id);
      void (*CallbackSelecionarRobo)(int i);
      
      void (*CallbackCarregar)(char* nome);
      void (*CallbackCarregarTut)(char* nome);
      void (*CallbackSalvar)(char* nome);
      void (*CallbackNew)();
      void (*CallbackTutNext)();
      void (*CallbackTutPrev)();
      void (*CallbackGrade)();
      void (*CallbackWire)();
      void (*CallbackGradeUp)();
      void (*CallbackGradeDown)();
      
      void (*CallbackConnect)(char *srv);
};



#endif
