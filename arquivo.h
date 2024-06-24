#ifndef ARQUIVO_H
#define ARQUIVO_H

#include <pthread.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

// Estrutura da matriz
struct Matriz {
    int linha; // Número de linhas da matriz
    int coluna; // Número de colunas da matriz
};

//Estrutura para encapsular as inforamções que cada thread precisa
typedef struct {
    const char *nomeArquivo;    
    int *matriz;
    int linhas;
    int colunas;      // Número de colunas e linhas nas matrizes
} ThreadLeituraInfo;
typedef struct{
    int *matrizA;
    int *matrizB;
    int *matrizD;
    int linhas;
    int colunas;
    int thread_id;     // ID da thread (para identificar qual parte da matriz esta thread processará)
    int num_threads;   // Número total de threads
    double tempo_soma;
} ThreadSomaInfo;
typedef struct{
    int *matrizC;
    int *matrizD;
    int *matrizE;
    int linhas;
    int colunas1;
    int colunas2;
    int thread_id;
    int num_threads;
    double tempo_mult;
} ThreadMultInfo;
typedef struct{
    const char *nomeArquivo;
    int *matriz;
    int linhas;
    int colunas;
}ThreadEscritaInfo;
typedef struct{
    int *matrizE;
    int linhas;
    int colunas;
    int resultado;
    int thread_id;
    int num_threads;
    double tempo_reducao;
}ThreadReducaoInfo;

double medirtempo(clock_t start, clock_t end);
int *AlocarMatriz(int m, int n);
void LerElementosMatrizes(const char *nomeArquivo, int *matriz, int linhas, int colunas);
void *LerMatriz(void *args);
void *ThreadSomarMatrizes(void *args);
int *SomarMatrizes(int *matrizA, int *matrizB, int linhas, int colunas, int num_threads);
void EscreverMatrizResultado(const char *nomeArquivo, int *matriz, int linhas, int colunas);
void *Threadescrevenamatriz(void *args);
void *ThreadMultiplicarMatrizes(void *arg);
int *MultiplicarMatrizes(int *matrizC, int *matrizD, int linhas, int colunas1, int colunas2, int num_threads);
void *ThreadReduzir(void *arg);
int Reduzir(int *matrizE, int linhas, int colunas, int num_threads);
int TamanhoMatriz(const char *nomeArquivo);
void criarThread(pthread_t *thread, void *(*func)(void *), void *args, const char *erroMsg);
void aguardarThread(pthread_t thread, const char *erroMsg);

#endif 
