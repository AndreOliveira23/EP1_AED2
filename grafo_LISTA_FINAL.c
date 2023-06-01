#include <stdio.h>
#include <stdlib.h>
#include "grafo_listaadj.h"

//Variáveis globais para usar no algoritmo de kruskal
int numero_de_vertices;
float* alturas_finais_arquivo;
int teste_contador = 1;
int tamanho_arvore;


bool verificaValidadeVertice(int v, Grafo* grafo){
	if(v > grafo->numVertices){
		fprintf(stderr, "Erro: Numero do vertice (%d) maior que o número total de vertices (%d).\n",v,grafo->numVertices);
		return false;
	}
	if(v<0){
		fprintf(stderr, "ERRO: Número do vértice (%d) deve ser positivo.\n",v);
		return false;
	}
	return true;
}

bool inicializaGrafo(Grafo *grafo, int nv){
	int i;
	if(nv <= 0){
		fprintf(stderr, "ERRO na chamada de inicializaGrafo: Número de vértices deve ser positivo.\n");
		return false;
	}

	grafo->numVertices = nv;
	if(!(grafo->listaAdj = (Apontador*) calloc(nv+1, sizeof(Apontador)))){
		fprintf(stderr, "ERRO: Falha de alocação de memória na função 'inicalizaGrafo\n");
		return false;
	}

	grafo->vetor_de_pais = (int*) malloc(nv*sizeof(int));//Make Set

	for(i=0;i<nv;i++){
		grafo->vetor_de_pais[i] = i;
	}

	grafo->rank = (int*)malloc(nv*sizeof(int));//Makeset
	grafo->numArestas = 0;
	return true;
}

bool listaAdjVazia(int v,Grafo* grafo){
	if(!verificaValidadeVertice(v,grafo))
		return false;
	return (grafo->listaAdj[v]==NULL);
}

Apontador proxListaAdj(int v, Grafo* grafo, Apontador atual){
	if(atual == NULL){
		fprintf(stderr, "atual  == NULL\n");
		return VERTICE_INVALIDO;
	}
	return(atual->prox);
}

Aresta* primeiroListaAdj(int v, Grafo* grafo){
	return(grafo->listaAdj[v]);
}

bool existeAresta(int v1,int v2, Grafo* grafo){
	Apontador q;

	if(! (verificaValidadeVertice(v1,grafo) && verificaValidadeVertice(v2,grafo)))
		return false;

	q = grafo->listaAdj[v1];
	while((q) && (q->vdest != v2))
		q = q->prox;
	if(q != NULL) return true;
	return false;

}

void insereAresta(int v1,int v2, Peso peso, Grafo* grafo){
	Apontador p;

	if(! (verificaValidadeVertice(v1,grafo) && verificaValidadeVertice(v2,grafo)))
		return;

	if(!(p = (Apontador) calloc(1,sizeof(Aresta)) )){
		fprintf(stderr, "ERRO: Falha na alocação de memória na função InsereAresta\n");
	}

	p->vdest = v2;
	p->peso = peso;
	p->prox = grafo->listaAdj[v1];
	grafo->listaAdj[v1] = p;
	grafo->numArestas++;
}

bool removeArestaObtendoPeso(int v1, int v2, Peso* peso, Grafo* grafo){
		Apontador q, ant;

	if(! (verificaValidadeVertice(v1,grafo) && verificaValidadeVertice(v2, grafo)))
		return AN;

	q = grafo->listaAdj[v1];
	while((q != NULL) && (q->vdest != v2)){
		ant = q;
		q = q->prox;
	}

	if( q!= NULL){
		if(grafo->listaAdj[v1] == q)
			grafo->listaAdj[v1] = q->prox;
		else ant->prox = q->prox;
		*peso = q->peso;
		q->prox = NULL;
		free(q);
		q = NULL;
		return true;
	}

	return false;
}

void liberaGrafo( Grafo* grafo) {
	int v;
	Apontador p;

	for(v=0;v<= grafo->numVertices; v++){
		p = grafo->listaAdj[v];
		while((p != NULL)) {
			grafo->listaAdj[v] = p->prox;
			p->prox = NULL;
			free(p);
		}
	}
	grafo->numVertices = 0;

	free(grafo->listaAdj);
	grafo->listaAdj = NULL;
}

void imprimeGrafo(Grafo* grafo){
	int i;
	for(i=1;i<=	 grafo->numVertices;i++){
		printf("V[%d]:",i);
		Aresta* atual = grafo->listaAdj[i];
		while(atual != NULL	){
			printf("(%d, peso: %.1f)",atual->vdest,atual->peso);
			atual = atual->prox;
		}
	printf("\n");
	}
}


/*Variáveis e Funções para o algoritmo de Kruskal*/


void make_set(Grafo* grafo,int x) {
    grafo->vetor_de_pais[x] = x;
    grafo->rank[x] = 0;
}

int find_set(Grafo* grafo, int x) {
   // printf("execução find-set!\n x=%d\n",x);
    if ( x != grafo->vetor_de_pais[x]) {
       //printf("entrou no if...\n");
       // printf("grafo->vetor_de_pais[%d] = %d\n",x,grafo->vetor_de_pais[x]);
        grafo->vetor_de_pais[x] = find_set(grafo,grafo->vetor_de_pais[x]);
    }
   // printf("vai retornar: %d\n",grafo->vetor_de_pais[x]);
    return grafo->vetor_de_pais[x];
}

void link(Grafo* grafo,int x, int y){
    if(grafo->rank[x] > grafo->rank[y]){
        grafo->vetor_de_pais[y] = x;
    }else{
        grafo->vetor_de_pais[x]= y;
    }

    if(grafo->rank[x] == grafo->rank[y]){
        grafo->rank[y]++;
    }
}

void union_sets(Grafo* grafo,int x, int y){
    link(grafo,find_set(grafo,x),find_set(grafo,y));
}

void insertionSortAresta(Aresta array[], int numero_de_rotas){
    int i, j;
    Aresta key;

    for (i = 1; i < numero_de_rotas; i++) {
        key = array[i];
        j = i - 1;
        while (j >= 0 && array[j].peso < key.peso) {
            array[j + 1] = array[j];
            j = j - 1;
        }
        array[j + 1] = key;
    }

}

Aresta*  AgmKruskal(Grafo* grafo, int numero_de_rotas, Aresta* array_de_arestas){


    int i;


    Aresta *ArvoreGeradoraMaxima = malloc(numero_de_rotas * sizeof(Aresta));



    insertionSortAresta(array_de_arestas, numero_de_rotas); //Ordenando edges por ordem não-crescente



    printf("Aresta[0] = %d -> %d : %.1f\n",array_de_arestas[0].origem,array_de_arestas[0].vdest,array_de_arestas[0].peso);

    for(i = 0; i < numero_de_rotas; i++){
        if(find_set(grafo, array_de_arestas[i].origem) != find_set(grafo, array_de_arestas[i].vdest)){
              ArvoreGeradoraMaxima[tamanho_arvore] = array_de_arestas[i];
              tamanho_arvore++;
              union_sets(grafo, array_de_arestas[i].origem, array_de_arestas[i].vdest);
              //printf("Aresta %d->%d: %.1f é segura e será colocada na AGM.\n", array_de_arestas[i].origem, array_de_arestas[i].vdest, array_de_arestas[i].peso);
       	 } else {
              //printf("Aresta %d->%d: %.1f NÃO é segura e NÃO será colocada na AGM.\n", array_de_arestas[i].origem, array_de_arestas[i].vdest, array_de_arestas[i].peso);       
       }
    }

    printf("AGM:\n");
    for(i = 0; i < tamanho_arvore; i++){
       	printf("Aresta %d->%d: %.1f\n", ArvoreGeradoraMaxima[i].origem, ArvoreGeradoraMaxima[i].vdest, ArvoreGeradoraMaxima[i].peso);
    }

    return ArvoreGeradoraMaxima;
}

int dfs(int vertice, int destino, int* visitado, Aresta* arestas, int numArestas, float *menorPeso){
	if(vertice == destino){
		return 1;
	}

	visitado[vertice]=1;

	int i;
	for(i=0;i<numArestas;i++){
		if(arestas[i].origem == vertice && !visitado[arestas[i].vdest]){
		//Se a aresta parte do vértice atual e o destino ainda não foi visitado
			if(dfs(arestas[i].vdest,destino,visitado,arestas,numArestas,menorPeso)){
			//Se encontrou o destino a partir desse vértice, verifica se o peso da aresta é menor
				if(*menorPeso == -1 || arestas[i].peso <*menorPeso){
					*menorPeso = arestas[i	].peso;
				}
			return 1;
			}
		}else if(arestas[i].vdest == vertice && !visitado[arestas[i].origem]){
			//Se a aresta chega ao vértice atual e a origem ainda não foi visitada
			if(dfs(arestas[i].origem,destino,visitado,arestas,numArestas,menorPeso)){
			//Se encontrou o destino a partir desse vértice, verifica se o peso da aresta é menor
				if(*menorPeso == -1 || arestas[i].peso <*menorPeso){
					*menorPeso = arestas[i].peso;
				}
			return 1;
			}
		}
	}
	return 0; //Não encontrou o destino a patir desse vértice

}
/*****************************************************************************************************************************************************/
int main(int argc, char **argv){

    FILE* file;
    file =  fopen(argv[1],"r");

    int numero_de_centros_de_distribuicao, numero_de_rotas, quantidade_de_consultas;
    int centro_de_origem, centro_de_destino,origem_consulta,destino_consulta;
    float altura_maxima_carreta;
    int altura_dos_baus[] = {2.5,3.0,2.5,4.0,4.5};
    int i,j,x,y,z;
    float c;
    Grafo g1;



    /*Lê a primeira linha*/
    fscanf(file,"%d %d %d",&numero_de_centros_de_distribuicao, &numero_de_rotas, &quantidade_de_consultas);
    numero_de_vertices = numero_de_rotas;
    printf("%d %d %d\n",numero_de_centros_de_distribuicao, numero_de_rotas, quantidade_de_consultas);
    if(inicializaGrafo(&g1,numero_de_centros_de_distribuicao)) printf("Grafo Criado!\n");
    imprimeGrafo(&g1);

   Aresta array[numero_de_rotas];

    /*Lê as arestas e pesos*/
    for(i=0;i<numero_de_rotas;i++){
        fscanf(file,"%d %d %f",&centro_de_origem, &centro_de_destino, &altura_maxima_carreta);
        insereAresta(centro_de_origem,centro_de_destino,altura_maxima_carreta,&g1);
	array[i].origem = centro_de_origem;
	array[i].vdest = centro_de_destino;
        array[i].peso = altura_maxima_carreta;
	insereAresta(centro_de_destino,centro_de_origem,altura_maxima_carreta,&g1);
	printf("%d %d %.1f\n",centro_de_origem, centro_de_destino, altura_maxima_carreta);
    }

    imprimeGrafo(&g1);

    printf("numArestas: %d\n",g1.numArestas/2);

    Aresta* teste = AgmKruskal(&g1,g1.numArestas/2,array);

    /*Lendo consultas*/
    alturas_finais_arquivo = (float*) realloc(alturas_finais_arquivo,quantidade_de_consultas * sizeof(float) + 1);

    int *visitado = (int*) calloc(g1.numVertices, sizeof(int));


    for(i=0;i<quantidade_de_consultas;i++){
       float altura_final = 0.0;
       fscanf(file,"%d %d",&origem_consulta, &destino_consulta);
       printf("Consulta: Origem: %d-> Destino: %d\n",origem_consulta, destino_consulta);
        int *visitadoTemp = (int*) calloc(g1.numVertices, sizeof(int));
        float menorPeso = -1;
        if (dfs(origem_consulta, destino_consulta, visitadoTemp, teste, g1.numArestas, &menorPeso)) {
            printf("Existe um caminho entre %d e %d. A aresta de menor peso tem peso %.1f.\n", origem_consulta, destino_consulta, menorPeso);
            //Comparar pesos para carreta
	        float altura_dos_baus[] = {2.5,3.0,3.5,4.0,4.5};
	        float altura_final ;
	        int z=0;
	        for(z=0;z<=4;z++){
		        if(altura_dos_baus[z] <= menorPeso){
			        altura_final = altura_dos_baus[z];
		        }
	        }
	        printf("Altura final: %.1f \n",altura_final);
            alturas_finais_arquivo[i] = altura_final;
        } else {
            printf("Não existe um caminho entre %d e %d.\n", origem_consulta, destino_consulta);
        }

        free(visitadoTemp);
       printf("########################################################################################\n");

      }

        free(visitado);





	    //Escrevendo arquivo de saída

          FILE *pont_arq;
          pont_arq = fopen("saida.txt", "a");
          for(i=0; i < quantidade_de_consultas; i++){
     		fprintf(pont_arq,"%.1f\n",alturas_finais_arquivo[i]);
    	   }

    return 0;
}
