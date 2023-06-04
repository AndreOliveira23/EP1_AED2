ep1_matriz_12608772.exe: main.o grafo_matrizadj.o
	gcc -o ep1_matriz_12608772.exe main.o grafo_matrizadj.o

ep1_lista_12608772.exe: main.o grafo_listaadj.o
	gcc -o ep1_lista_12608772.exe main.o grafo_listaadj.o

main.o: main.c grafo_listaadj.h grafo_matrizadj.h
	gcc -c main.c

grafo_matrizadj.o: grafo_matrizadj.c grafo_matrizadj.h
	gcc -c grafo_matrizadj.c

grafo_listaadj.o: grafo_listaadj.c grafo_listaadj.h
	gcc -c grafo_listaadj.c


	