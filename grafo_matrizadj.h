#include <stdbool.h>

#define MAXNUMVERTICES 100
#define AN -1 //Aresta nula
#define VERTICE_INVALIDO -1

typedef float Peso;

typedef struct{
	Peso mat[MAXNUMVERTICES + 1][MAXNUMVERTICES + 1];
	int numVertices;
	int numArestas;
    int* vetor_de_pais;
    int* rank;
}Grafo;

/*Para algoritmo de kruskal*/
typedef struct {
    int origem;
    int destino;
    float peso;
} Aresta;

typedef int Apontador;

