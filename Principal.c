#pragma once
#define _CRT_SECURE_NO_WARNINGS 1
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1

#pragma comment(lib,"pthreadVC2.lib")

#define HAVE_STRUCT_TIMESPEC
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h> 

// Definindo constantes para a matriz e o macrobloqueio
#define MATRIZ_LINHAS 10000
#define MATRIZ_COLUNAS 10000
#define MACROBLOCO_ALTURA 1000
#define MACROBLOCO_LARGURA 1000


int** matriz;

void* threadFunc(void*);

int main(int argc, char* argv[]) {
	pthread_t thread;

	matriz = malloc(MATRIZ_LINHAS * sizeof(int*)); // Alocando espaço para os ponteiros
	int* dados = malloc(MATRIZ_LINHAS * MATRIZ_COLUNAS * sizeof(int)); // Alocando todos os elementos

	if (matriz == NULL || dados == NULL) { 
		perror("Erro ao alocar memória para matriz");
		exit(1);
	}

	// Inicializando os ponteiros da matriz para apontar para os dados alocados
	for (int i = 0; i < MATRIZ_LINHAS; i++) { 
		matriz[i] = &dados[i * MATRIZ_COLUNAS];
	}

	srand(42); // Semente para números aleatórios

	// Preenchendo a matriz com números aleatórios entre 0 e 3200
	for (int i = 0; i < MATRIZ_LINHAS; i++) {
		for (int j = 0; j < MATRIZ_COLUNAS; j++) {
			matriz[i][j] = rand() % 32000;
		}
	}

	if (pthread_create(&thread, NULL, threadFunc, NULL) != 0) {
		perror("Pthread_create falhou!");
		exit(1);
	}
	if (pthread_join(thread, NULL) != 0) {
		perror("Pthread_join falhou!");
		exit(1);
	}
	printf("\oi");
	return 0;
}
void* threadFunc(void* nenhum) {
	printf("Print da Thread\n");
}