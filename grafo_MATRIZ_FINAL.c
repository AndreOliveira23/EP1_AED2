#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include "grafo_matrizadj.h"

//Variáveis globais para usar no algoritmo de kruskal
int numero_de_vertices;
float *alturas_finais_arquivo;	
int teste_contador=1;
int tamanho_arvore;

bool inicializaGrafo(Grafo* grafo, int nv){
	int i,j;
	if(nv > MAXNUMVERTICES){
		fprintf(stderr,"ERRO na chamada de inicializaGrafo: Numero de vértices maior que o máximo permitido de %d.\n",MAXNUMVERTICES);
		return false;
	}
	if(nv <0){
		fprintf(stderr,"ERRO Na chamada de inicializaGrafo: Numero de vértices deve ser positivo.\n");
		return false;
	}

	grafo->numVertices = nv;
	grafo->numArestas = 0;
    grafo->vetor_de_pais = (int*) malloc(nv * sizeof(int));//Make set
    for (int i = 0; i < nv; i++) {
        grafo->vetor_de_pais[i] = i; // atribui o índice atual a cada posição do vetor
    }
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
	if(v<0){
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
    //printf("execução find-set!\n x=%d\n",x);
    if ( x != grafo->vetor_de_pais[x]) {
        //printf("entrou no if...\n");
        //printf("grafo->vetor_de_pais[%d] = %d\n",x,grafo->vetor_de_pais[x]);
        grafo->vetor_de_pais[x] = find_set(grafo,grafo->vetor_de_pais[x]);
    }
    //printf("vai retornar: %d\n",grafo->vetor_de_pais[x]);
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

void insertionSortPorOrigemAresta(Aresta array[], int tamanho_do_array){
    int i, j;
    Aresta key;

    for (i = 1; i <tamanho_do_array; i++) {
        key = array[i];
        j = i - 1;
        while (j >= 0 && array[j].origem> key.origem) {
            array[j + 1] = array[j];
            j = j - 1;
        }
        array[j + 1] = key;
    }
}


Aresta* AgmKruskal(Grafo* grafo, int numero_de_rotas, Aresta* array_de_arestas){
    int i;
    Aresta *ArvoreGeradoraMaxima = malloc(numero_de_rotas * sizeof(Aresta));
 

    insertionSortAresta(array_de_arestas, numero_de_rotas); //Ordenando edges por ordem não-crescente

    for(i = 0; i < numero_de_rotas; i++){
        if(find_set(grafo, array_de_arestas[i].origem) != find_set(grafo, array_de_arestas[i].destino)){
            ArvoreGeradoraMaxima[tamanho_arvore] = array_de_arestas[i];
            tamanho_arvore++;
            union_sets(grafo, array_de_arestas[i].origem, array_de_arestas[i].destino);
            //printf("Aresta %d->%d: %.1f é segura e será colocada na AGM.\n", array_de_arestas[i].origem, array_de_arestas[i].destino, array_de_arestas[i].peso);
        } else {
            //printf("Aresta %d->%d: %.1f NÃO é segura e NÃO será colocada na AGM.\n", array_de_arestas[i].origem, array_de_arestas[i].destino, array_de_arestas[i].peso);       
        }
    }
    
    printf("AGM:\n");
    for(i = 0; i < tamanho_arvore; i++){
       	printf("Aresta %d->%d: %.1f\n", ArvoreGeradoraMaxima[i].origem, ArvoreGeradoraMaxima[i].destino, ArvoreGeradoraMaxima[i].peso);
    }
    
    return ArvoreGeradoraMaxima;
}

/*
int dfs2(int vertice, int destino, int *visitado, Aresta *arestas, int numArestas) {
    if (vertice == destino) {
        return 1; // Encontrou o destino
    }
    
    visitado[vertice] = 1; // Marca o vértice como visitado
    
    int i;
    for (i = 0; i < numArestas; i++) {
        if (arestas[i].origem == vertice && !visitado[arestas[i].destino]) {
            // Se a aresta parte do vértice atual e o destino ainda não foi visitado
            if (dfs(arestas[i].destino, destino, visitado, arestas, numArestas)) {
                // Se encontrou o destino a partir desse vértice, retorna 1
                return 1;
            }
        }
        else if (arestas[i].destino == vertice && !visitado[arestas[i].origem]) {
            // Se a aresta chega ao vértice atual e a origem ainda não foi visitada
            if (dfs(arestas[i].origem, destino, visitado, arestas, numArestas)) {
                // Se encontrou o destino a partir desse vértice, retorna 1
                return 1;
            }
        }
    }
    
    return 0; // Não encontrou o destino a partir desse vértice
}

*/

int dfs(int vertice, int destino, int *visitado, Aresta *arestas, int numArestas, float *menorPeso) {
    if (vertice == destino) {
        return 1; // Encontrou o destino
    }
    
    visitado[vertice] = 1; // Marca o vértice como visitado
    
    int i;
    for (i = 0; i < numArestas; i++) {
        if (arestas[i].origem == vertice && !visitado[arestas[i].destino]) {
            // Se a aresta parte do vértice atual e o destino ainda não foi visitado
            if (dfs(arestas[i].destino, destino, visitado, arestas, numArestas, menorPeso)) {
                // Se encontrou o destino a partir desse vértice, verifica se o peso da aresta é menor
                if (*menorPeso == -1 || arestas[i].peso < *menorPeso) {
                    *menorPeso = arestas[i].peso;
                }
                return 1;
            }
        }
        else if (arestas[i].destino == vertice && !visitado[arestas[i].origem]) {
            // Se a aresta chega ao vértice atual e a origem ainda não foi visitada
            if (dfs(arestas[i].origem, destino, visitado, arestas, numArestas, menorPeso)) {
                // Se encontrou o destino a partir desse vértice, verifica se o peso da aresta é menor
                if (*menorPeso == -1 || arestas[i].peso < *menorPeso) {
                    *menorPeso = arestas[i].peso;
                }
                return 1;
            }
        }
    }
    
    return 0; // Não encontrou o destino a partir desse vértice
}


void buscaArvoreGeradoraMaxima( Aresta* ArvoreGeradoraMaxima, int vertice_origem, int vertice_destino, float menorPeso_backTracking){

     //printf("%d -> ",vertice_origem);

    int i=0;
    Aresta menorPeso;
    menorPeso.origem = -1;
    menorPeso.destino = -1;
    menorPeso.peso = menorPeso_backTracking;		

    Aresta atual = ArvoreGeradoraMaxima[0];	
	printf("Atual: %d -> %d : %.1f		\n",atual.origem,atual.destino,atual.peso);

    //Percorrendo arvore até achar a origem correta
    while(atual.origem != vertice_origem ){
	    atual = ArvoreGeradoraMaxima[i+1];
		i++;
        
        /*Se chegou no próximo if, quer dizer que não existe um vértice que parte do vértice dado como origem. Precisa procurar a aresta que chega no vértice e fazer um backtracking na AGM*/
		if(i == tamanho_arvore){
		        printf("não existe vértice com essa origem na AGM. Fazendo backtracking...\n");
                return;
        }    

        printf("Atual: %d -> %d : %.1f		\n",atual.origem,atual.destino,atual.peso);
    }

    //Acha o destino certo recursivamente
    while(atual.destino != vertice_destino){
        if (atual.peso <  menorPeso.peso) {
            menorPeso 	=   atual;
        }	
        buscaArvoreGeradoraMaxima(ArvoreGeradoraMaxima,atual.destino,vertice_destino,menorPeso.peso);
    }


}
/*    
//Percorrendo arvore até achar a origem correta
       while(atual.origem != vertice_origem ){
		atual = ArvoreGeradoraMaxima[i+1];
		i++;

			i=0;			
			atual = ArvoreGeradoraMaxima[0];
			
			//Ao sair do while, encontrou a aresta que tem  a origem dada como destino;
			while(atual.destino != vertice_origem){
				atual = ArvoreGeradoraMaxima[i+1];
				i++;
			}
			printf("Atual no backtracking: %d -> %d : %.1f		\n",atual.origem,atual.destino,atual.peso);
			menorPeso=atual;
			printf("Menor peso no backtracking (antes de chamar a função) : %d -> %d : %.1f		\n",menorPeso.origem,menorPeso.destino,menorPeso.peso);
			buscaArvoreGeradoraMaxima( ArvoreGeradoraMaxima, atual.origem, vertice_destino, menorPeso.peso);
			return;
		}
	}		
	printf("Atual antes de percorrer destino: %d -> %d : %.1f		\n",atual.origem,atual.destino,atual.peso);
		i=0;
 //Partindo da origem correta até chegar no destino		
    while(atual.destino != vertice_destino){
	printf("MenorPeso %d -> %d: %.1f \n",menorPeso.origem,menorPeso.destino,menorPeso.peso);		
	printf("Atual %d -> %d: %.1f \n",atual.origem,atual.destino,atual.peso	);		
	if (atual.peso <  menorPeso.peso) {
            menorPeso 	=   atual;
        }	        
	atual = ArvoreGeradoraMaxima[i+1];
        i++;
    }	
	

		//Chegou na aresta de destino certo
	if (atual.peso <  menorPeso.peso) {
            menorPeso 	=   atual;
        }			
	printf("Atual final:		 %d -> %d: %.1f \n",atual.origem,atual.destino,atual.peso	)	;		
	printf("Aresta de menor peso: %d -> %d: %.1f\n",menorPeso.origem,menorPeso.destino,menorPeso.peso);
	
	//Comparar pesos para carreta
	
	float altura_dos_baus[] = {2.5,3.0,3.5,4.0,4.5};
	float altura_final ;
	int z=0;
	for(z=0;z<=4;z++){
		if(altura_dos_baus[z] <= menorPeso.peso){
			altura_final = altura_dos_baus[z];	
		}
	}	
	printf("Altura final: %.1f \n",altura_final);		
	alturas_finais_arquivo[teste_contador] = altura_final;
	teste_contador++;	

printf("/***********************************************************************************************************\n		");			

}
*/

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

    Aresta* teste = AgmKruskal(&g1,g1.numArestas,array);

    
    /*Lendo consultas*/

	
    alturas_finais_arquivo = (float*) realloc(alturas_finais_arquivo,quantidade_de_consultas * sizeof(float) + 1);

	alturas_finais_arquivo[0]=0;

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
            alturas_finais_arquivo[i+1] = altura_final;            
            alturas_finais_arquivo[0]++;
        } else {
            printf("Não existe um caminho entre %d e %d.\n", origem_consulta, destino_consulta);
        }
        
        free(visitadoTemp);
    }
    
        free(visitado);
    
        //buscaArvoreGeradoraMaxima(teste, origem_consulta, destino_consulta, 10.0); 

       printf("########################################################################################\n");
    


	//Escrevendo arquivo de saída

      FILE *pont_arq;
      pont_arq = fopen("saida.txt", "a");
      for(i=1; i <= quantidade_de_consultas; i++){
 		fprintf(pont_arq,"%.1f\n",alturas_finais_arquivo[i]);  
	}

    return 0;
}
			
