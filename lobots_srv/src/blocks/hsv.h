/* hsv.h
* 
* Fornece m�todos para a obten��o de valores H, S e V a partir do sistema
* de cor RGB, bem como um m�todo para converter um conjunto HSV para RGB
*/

#ifndef HsvH
#define HsvH

#include "tipos.h"

void HSV(tcor* cor, float H, float S, float V);
       // 0 <= H <= 360; 0 <= S <= 1; 0 <= V <= 1);
float GetHValue(tcor *cor);
float GetSValue(tcor *cor);
float GetVValue(tcor *cor);

#endif
