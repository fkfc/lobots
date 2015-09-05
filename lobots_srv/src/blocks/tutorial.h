#ifndef tutorialH
#define tutorialH

#include "world.h"
#include "balde.h"
#include "camera.h"

class TTutorial {
      public:
             TTutorial(TWorld *mundo);
             ~TTutorial();
             void ColocarProx(double t, TCamera *camera);
             void Avancar();
             void Voltar();
             void Animar(double t);
             TBlockSingle *Peca_wire;
             bool prox;
             bool fim;
      private:
              float ox, oy, oz;
              float ory;
              TWorld *mundo_ref;
              int atual;
              float passox, passoy, passoz, passory;
              float tempo;
              float distancia;
};

#endif
