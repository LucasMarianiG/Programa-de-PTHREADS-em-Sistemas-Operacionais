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
//#define MATRIZ_LINHAS 10000
//#define MATRIZ_COLUNAS 10000
//#define MACROBLOCO_ALTURA 1000
//#define MACROBLOCO_LARGURA 1000
#define MATRIZ_LINHAS 9
#define MATRIZ_COLUNAS 9
#define MACROBLOCO_ALTURA 3
#define MACROBLOCO_LARGURA 3
// Semente para números aleatórios
#define SEED 42 
// Máximo número aleatório a ser gerado
#define MAX_NUM_ALEATORIO 30
// Número de threads a serem criadas
#define NUM_THREADS 4
// Número de núcleos do processador
#define NUM_NUCLEOS 10 
// Número de núcleos lógicos do processador
#define NUM_NUCLEOS_LOGICOS 16


int** matriz;



void* threadFunc(void*);
//essa não tem muito o que falar, é apenas a declaração de uma função que verifica se um número é primo ou não : )
int ehPrimo(int n);
// Função que percorre a matriz de forma serial e conta os números primos
void buscaSerial();

void buscaParalela(int);

void criaMatriz();

void preencheMatriz();

void imprimeMatriz();

void limpaMatriz();

int main(int argc, char* argv[]) {
	pthread_t thread;

	// Cria a matriz dinamicamente 
	criaMatriz();

	// Preenche a matriz com números aleatórios
	preencheMatriz(); 

	// Imprime a matriz para verificação
	imprimeMatriz();

	buscaSerial();

	buscaParalela(NUM_THREADS);

	if (pthread_create(&thread, NULL, threadFunc, NULL) != 0) {
		perror("Pthread_create falhou!");
		exit(1);
	}
	if (pthread_join(thread, NULL) != 0) {
		perror("Pthread_join falhou!");
		exit(1);
	}
	printf("Print do Main");

	// Libera a memória alocada para a matriz
	limpaMatriz();

	return 0;
	 
}
// estamos crianndo uma matriz dinamicamente
void criaMatriz() {
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
}

void preencheMatriz() {
	srand(SEED); // Semente para números aleatórios

	// Preenchendo a matriz com números aleatórios entre 0 e 32000
	for (int i = 0; i < MATRIZ_LINHAS; i++) {
		for (int j = 0; j < MATRIZ_COLUNAS; j++) {
			matriz[i][j] = rand() % MAX_NUM_ALEATORIO;
		}
	}
}


//essa não tem muito o que falar, é apenas a declaração de uma função que verifica se um número é primo ou não : )
int ehPrimo(int n) {
	if (n < 2) return 0;
	int limiteVerificacao = (int)sqrt(n);


	for (int i = 2; i <= limiteVerificacao; i++) {
		if (n % i == 0) {
			return 0; // não é primo
		}
	}
	return 1; // é primo
}

// Função que percorre a matriz de forma serial e conta os números primos
void buscaSerial() {
	clock_t inicio = clock();

	int totalPrimos = 0;

	for (int i = 0; i < MATRIZ_LINHAS; i++) {
		for (int j = 0; j < MATRIZ_COLUNAS; j++) {
			int primo = ehPrimo(matriz[i][j]);
			totalPrimos+= primo;
		}
	}

	clock_t fim = clock();
	double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;

	printf("Busca Serial: %d primos encontrados\n", totalPrimos);
	printf("Tempo de execução (serial): %.2f segundos\n", tempo);
}

void buscaParalela() {
	clock_t inicio = clock();
	int totalPrimos = 0;
	pthread_t threads[NUM_THREADS];
	int thread_data[NUM_THREADS];

	// Dividindo o trabalho entre as threads
	for (int i = 0; i < NUM_THREADS; i++) {
		thread_data[i] = i;
		if (pthread_create(&threads[i], NULL, threadFunc, &thread_data[i]) != 0) {
			perror("Pthread_create falhou!");
			exit(1);
		}
	}
	// Aguardando todas as threads terminarem
	for (int i = 0; i < NUM_THREADS; i++) {
		if (pthread_join(threads[i], NULL) != 0) {
			perror("Pthread_join falhou!");
			exit(1);
		}
	}
	clock_t fim = clock();
	double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;
	printf("Busca Paralela: %d primos encontrados\n", totalPrimos);
	printf("Tempo de execução (paralela): %.2f segundos\n", tempo);
}
	

void* threadFunc(void* nenhum) {
	printf("Print da Thread\n");
}

void imprimeMatriz() {
	printf("Matriz %d x %d:\n \n", MATRIZ_LINHAS, MATRIZ_COLUNAS);
	for (int i = 0; i < MATRIZ_LINHAS; i++) {
		for (int j = 0; j < MATRIZ_COLUNAS; j++) {
			printf("%d ", matriz[i][j]);
		}
		printf("\n");
	}
}

void limpaMatriz(){
	if (matriz != NULL) {
		// Libera o espaço alocado para os dados
		free(matriz[0]); 
		// Libera o espaço alocado para os ponteiros
		free(matriz);
		// para ter certeza que não vamos usar mais a matriz, vamos zerar o ponteiro
		matriz = NULL; 
	}

}

