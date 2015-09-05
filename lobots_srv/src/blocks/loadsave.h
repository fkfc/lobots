/* loadsave.h
*
* Funções para abrir e salvar uma construção em um arquivo
*/

#ifndef LoadsaveH
#define LoadsaveH

#include "world.h"
#include "balde.h"
#include "camera.h"
//#include "conexao.h"

#if defined(unix) || defined(__APPLE__)
  #define DIR_SEP '/'
#else 
  #define DIR_SEP '\\'
#endif

void Salvar(char *nome_ar, TWorld *Mundo, TCamera *Camera);
void Abrir(char *nome_ar, TWorld **Mundo, TBlockContainer* Balde, TCamera *Camera);

#endif
