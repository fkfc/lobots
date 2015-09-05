#include "reverse_endianness.h"
#include <stdint.h>

//PowerPC é big endian, e precisa inverter ao ler um arquivo binário
int isBigEndian(void)
{
    union {
        uint32_t i;
        char c[4];
    } bint = {0x01020304};

    return bint.c[0] == 1; 
}

float ReverseFloat( const float inFloat ) {
   if (isBigEndian()) {
      float retVal;
      char *floatToConvert = ( char* ) & inFloat;
      char *returnFloat = ( char* ) & retVal;

      // swap the bytes into a temporary buffer
      returnFloat[0] = floatToConvert[3];
      returnFloat[1] = floatToConvert[2];
      returnFloat[2] = floatToConvert[1];
      returnFloat[3] = floatToConvert[0];

      return retVal;
   } else return inFloat;
}


int ReverseInt( const int inInt ) {
   if (isBigEndian()) {
      int retVal;
      char *intToConvert = ( char* ) & inInt;
      char *returnInt = ( char* ) & retVal;

      // swap the bytes into a temporary buffer
      returnInt[0] = intToConvert[3];
      returnInt[1] = intToConvert[2];
      returnInt[2] = intToConvert[1];
      returnInt[3] = intToConvert[0];

      return retVal;
   } else return inInt;
}



double ReverseDouble( const double inDouble ) {
   if (isBigEndian()) {
      double retVal;
      char *doubleToConvert = ( char* ) & inDouble;
      char *returnDouble = ( char* ) & retVal;

      // swap the bytes into a temporary buffer
      returnDouble[0] = doubleToConvert[7];
      returnDouble[1] = doubleToConvert[6];
      returnDouble[2] = doubleToConvert[5];
      returnDouble[3] = doubleToConvert[4];
      returnDouble[4] = doubleToConvert[3];
      returnDouble[5] = doubleToConvert[2];
      returnDouble[6] = doubleToConvert[1];
      returnDouble[7] = doubleToConvert[0];
     
      return retVal;
   } else return inDouble;
}