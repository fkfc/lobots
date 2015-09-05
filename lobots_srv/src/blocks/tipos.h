/* tipos.h
*
* Define os tipos e estruturas de dados base para armazenar os objetos 3D.
* Encontram-se aqui também as constantes básicas utilizadas pelo programa
*/

#ifndef TiposH
#define TiposH

// Máximo para cada objeto
#define MAX_VERTICES 600 
#define MAX_POLIGONOS 500

#define DIVISAO 10.0f // diminui objetos na exibicao

//cores
#define COR_VERMELHO  0
#define COR_AMARELO 1
#define COR_AZUL 2
#define COR_VERDE 3
#define COR_PRETO 4
#define COR_BRANCO 5
#define COR_CINZA 6
#define COR_MARROM 7


typedef enum { PLANO_XY, PLANO_YZ, PLANO_ZX } tplano;

typedef struct{  // vértice
    float x,y,z;
}tvertice;

typedef struct{  // polígono = quadrado - aponta 4 vertices
    int a,b,c,d;
}tpoligono;

typedef struct{
	char r, g, b;
}tcor;

typedef struct {  // objeto - armazena os vertices e poligonos
    char nome[50];
    tvertice vertice[MAX_VERTICES]; 
    tpoligono poligono[MAX_POLIGONOS];
    int num_poligonos;
} tobj, *ptobj;

ptobj LoadObject(const char *fileName);

void normalizar(float *x, float *y, float *z);
void normal (float ax,float ay,float az,float bx,float by,float bz,float cx,float cy,float cz,float *x, float *y, float *z);
void GetCor(int n_cor, tcor* cor);



#endif
