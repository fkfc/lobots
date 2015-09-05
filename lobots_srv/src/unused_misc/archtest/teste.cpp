#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int IsBigEndian(void)
{
    union {
        uint32_t i;
        char c[4];
    } bint = {0x01020304};

    return bint.c[0] == 1; 
}


int main(int argc, char *argv[]) {
   printf("float = %d bits\nint = %d bits\nlong = %d bits\ndouble = %d bits\nlong double = %d bits\n", sizeof(float)*8, sizeof(int)*8, sizeof(long)*8, sizeof(double)*8, sizeof(long double)*8);	
   if (IsBigEndian()) {
      printf("Arquitetura Big Endian\n");
   } else {
      printf("Arquitetura Little Endian\n");
   }
}
