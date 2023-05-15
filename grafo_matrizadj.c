#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "grafo_matrizadj.h"

//Variáveis globais para usar no algoritmo de kruskal
int numero_de_vertices;


bool inicializaGrafo(Grafo* grafo, int nv){
	int i,j;
	if(nv > MAXNUMVERTICES){
		fprintf(stderr,"ERRO na chamada de inicializaGrafo: Numero de vértices maior que o máximo permitido de %d.\n",MAXNUMVERTICES);
		return false;
	}
	if(nv <=0){
		fprintf(stderr,"ERRO Na chamada de inicializaGrafo: Numero de vértices deve ser positivo.\n");
		return false;
	}

	grafo->numVertices = nv;
	grafo->numArestas = 0;
    grafo->vetor_de_pais = (int*) malloc(nv * sizeof(int));//Make set
    grafo->rank = (int*) malloc(nv * sizeof(int));//Make sET
	for( i = 0; i<= grafo->numVertices; i++){
		for( j = 0; j<= grafo->numVertices; j++){
			grafo->mat[i][j] = AN;
		}
	}
}

bool verificaValidadeVertice(int v, Grafo* grafo){
	if(v > grafo->numVertices){
		fprintf(stderr, "Erro: Numero do vertice (%d) maior que o número total de vertices (%d).\n",v,grafo->numVertices);
		return false;
	}
	if(v<=0){
		fprintf(stderr, "ERRO: Número do vértice (%d) deve ser positivo.\n",v);
		return false;
	}
	return true;
}

void insereAresta(int v1, int v2, Peso peso, Grafo* grafo){
	if(! ( verificaValidadeVertice(v1, grafo) && verificaValidadeVertice(v2, grafo)))
	 return;


    /*Verifica se o caminho de volta já existe)*/
    if((grafo->mat[v1][v2] != AN) && (v1 > v2)){
        printf("Erro! -> A ordem do número dos centros de distribuição deve ser crescente: ");
        grafo->mat[v1][v2] = AN;
        //grafo->numArestas--; 
        return;
    }else{
	    grafo->mat[v1][v2] = peso;
        if(grafo->mat[v2][v1] != AN){
	        
        }else{
            grafo->numArestas++;        
        }
    }
}

bool existeAresta(int v1, int v2, Grafo* grafo){
	if(! ( verificaValidadeVertice(v1, grafo) && verificaValidadeVertice(v2, grafo)))
	 return false;
	if(grafo->mat[v1][v2] != AN) return true;
		else return false;
}


bool removeAresta(int v1, int v2, Peso* peso, Grafo* grafo){
	if(! ( verificaValidadeVertice(v1, grafo) && verificaValidadeVertice(v2, grafo)))
	 return false;

	if(grafo->mat[v1][v2] != AN){
		*peso = grafo->mat[v1][v2];
		grafo->mat[v1][v2];
		grafo->numArestas--;
		return true;
	}

	return false;
}

bool listaADjVazia(int v, Grafo* grafo){
	if(!verificaValidadeVertice(v, grafo))
	  return true;

	int i;
	for(i=1; i<=grafo->numVertices; i++){
		if(grafo->mat[v][i] != AN) return false;
	return true;
	}

}

Apontador proxListaAdj(int v, Grafo* grafo, Apontador atual){
	if( !verificaValidadeVertice(v, grafo))
		return VERTICE_INVALIDO;

	atual++;
	while ((atual <= grafo->numVertices) && (grafo->mat[v][atual] == AN))
		atual++;
	if(atual > grafo->numVertices){
		return VERTICE_INVALIDO;
	}
	return atual;
}

void liberaGrafo(Grafo* grafo){
}

void imprimeGrafo(Grafo* grafo){
	int i,j;
	for(i=1;i<=grafo->numVertices;i++){
		for(j=1;j<=grafo->numVertices;j++){
			printf("[%d][%d] = %.1f | ",i,j,grafo->mat[i][j]);
		}
	printf("\n");
	}
    printf("---------------------------------------------------------------------------------------------------------- \n");
}

/*Variáveis e Funções para o algoritmo de Kruskal*/


void make_set(Grafo* grafo,int x) {
    grafo->vetor_de_pais[x] = x;
    grafo->rank[x] = 0;
}

int find_set(Grafo* grafo, int x) {
    printf("execução find-set!\n x=%d\n",x);
    if ( x != grafo->vetor_de_pais[x]) {
        printf("entrou no if...\n");
        printf("grafo->vetor_de_pais[%d] = %d\n",x,grafo->vetor_de_pais[x]);
        grafo->vetor_de_pais[x] = find_set(grafo,grafo->vetor_de_pais[x]);
    }
    printf("vai retornar: %d\n",grafo->vetor_de_pais[x]);
    return grafo->vetor_de_pais[x];
}




void link(Grafo* grafo,int x, int y){
    if(grafo->rank[x] > grafo->rank[y]){
        grafo->vetor_de_pais[y] = x;    
    }else{
        grafo->vetor_de_pais[x] = y;    
    }
    
    if(grafo->rank[x] == grafo->rank[y]){
        grafo->rank[y]++;    
    }
}

void union_sets(Grafo* grafo,int x, int y){
    link(grafo,find_set(grafo,x),find_set(grafo,y));
}

void insertionSortAresta(Aresta array[], int numero_de_rotas){
    float i, j;
    Aresta key;
    for (i = 1; i < numero_de_rotas; i++) {
        key = array[(int)i];
        j = i - 1;
        while (j >= 0 && array[(int)j].peso > key.peso) {
            array[(int)j + 1] = array[(int)j];
            j = j - 1;
        }
        array[(int)j + 1] = key;
    }
}

void AgmKruskal(Grafo* grafo, int numero_de_rotas, Aresta* array_de_arestas){
int i;
Aresta ArvoreGeradoraMinima[numero_de_rotas];
//MakeSet feito na inicialização do grafo

insertionSortAresta(array_de_arestas,numero_de_rotas); //Ordenando edges por ordem não-decrescente

    
    for(i=0;i<numero_de_rotas;i++){
        printf("Array[%d].origem = %d\n",i,array_de_arestas[i].origem);
        if(find_set(grafo,array_de_arestas[i].origem) != find_set(grafo,array_de_arestas[i].destino)){
            ArvoreGeradoraMinima[i] = array_de_arestas[i];
            union_sets(grafo,array_de_arestas[i].origem,array_de_arestas[i].destino);
            printf("D\n");

            printf("Aresta %d-> %d:  %.1f é segura e será colocada na AGM.\n",array_de_arestas[i].origem,array_de_arestas[i].destino,array_de_arestas[i].peso);
        }else{
      printf("Aresta %d-> %d:  %.1f NÃO é segura e NÃO será colocada na AGM.\n",array_de_arestas[i].origem,array_de_arestas[i].destino,array_de_arestas[i].peso);       
        }
    }
    
    
    for(i=0;i<numero_de_rotas;i++){
       printf("Aresta %d-> %d:  %.1f.\n",array_de_arestas[i].origem,array_de_arestas[i].destino,array_de_arestas[i].peso);
    }








}

/*****************************************************************************************************************************************************/
int main(int argc, char **argv){
    
    FILE* file;
    file =  fopen(argv[1],"r");
    
    int numero_de_centros_de_distribuicao, numero_de_rotas, quantidade_de_consultas;
    int centro_de_origem, centro_de_destino;
    float altura_maxima_carreta;
     
    int i,j,x,y,z;
    float c;
    Grafo g1;



    /*Lê a primeira linha*/
    fscanf(file,"%d %d %d",&numero_de_centros_de_distribuicao, &numero_de_rotas, &quantidade_de_consultas);
    numero_de_vertices = numero_de_rotas;
    printf("%d %d %d\n",numero_de_centros_de_distribuicao, numero_de_rotas, quantidade_de_consultas);
    inicializaGrafo(&g1,numero_de_centros_de_distribuicao);
    imprimeGrafo(&g1);
    Aresta array[numero_de_rotas];
    /*Lê as arestas e pesos*/
    for(i=0;i<numero_de_rotas;i++){
        fscanf(file,"%d %d %f",&centro_de_origem, &centro_de_destino, &altura_maxima_carreta);
        insereAresta(centro_de_origem,centro_de_destino,altura_maxima_carreta,&g1);
        insereAresta(centro_de_destino,centro_de_origem,altura_maxima_carreta,&g1);
        array[i].origem = centro_de_origem;
        array[i].destino = centro_de_destino;
        array[i].peso = altura_maxima_carreta;
        printf("%d %d %.1f\n",centro_de_origem, centro_de_destino, altura_maxima_carreta);
    }    
    imprimeGrafo(&g1);
    printf("numArestas: %d\n",g1.numArestas);
    AgmKruskal(&g1,g1.numArestas,array);
    
    
  

















    /*Lê os caminhos desejados
     for(i=0;i<=2;i++){
        fscanf(file,"%d %d",&x,&y);
        printf("%d %d\n",x,y);
    }
    fclose(file);
    
    Grafo g1;
    if(inicializaGrafo(&g1,10)) printf("true");*/
    return 0;
}
