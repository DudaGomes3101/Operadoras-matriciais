/*
Grupo: Operadoras matriciais
Maria Eduarda de Souza Gomes 260844
Tammy Kojima 213792
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

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

//Usar biblioteca clock
double medirtempo(clock_t start, clock_t end){
    if (end < start) {
        fprintf(stderr, "Erro: end deve ser maior ou igual a start.\n");
        return -1.0; // Caso erro
    }
    double tempo = ((double)(end - start)) / CLOCKS_PER_SEC;
    return tempo;
}

// Alocação dinâmica de uma matriz com m linhas e n colunas 
int *AlocarMatriz(int m, int n) {
    // Alocar memória para a matriz inteira de uma vez
    int *mat = (int *)malloc(m * n * sizeof(int));
    // Verificar se a alocação foi bem sucedida
    if (mat == NULL) {
        printf("Erro ao alocar memória para a matriz.\n");
        exit(1); // Encerra o programa
    }
    return mat; // Retornar um ponteiro para o bloco contínuo de memória alocada
}
// aqui quando for para acessar o elemento na linha i e coluna j, vc precisa chamar assim: mat[i * n + j], sendo n o numero de colunas 

// Leitura dos elementos das matrizes do arquivo de forma simultânea
void LerElementosMatrizes(const char *nomeArquivo, int *matriz, int linhas, int colunas) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        fprintf(stderr,"Erro ao abrir o arquivo %s.\n", nomeArquivo);
        exit(1); // Encerra o programa em caso de erro na abertura do arquivo
    }

    // Ler os valores de linhas e colunas (primeira linha)
    int linhasArquivo, colunasArquivo;
    fscanf(arquivo, "%d %d", &linhasArquivo, &colunasArquivo);

    // Verificar se os valores lidos correspondem aos parâmetros passados
    if(linhasArquivo != linhas || colunasArquivo != colunas) {
        fprintf(stderr, "Erro: O tamanho da matriz no arquivo %s não corresponde ao tamanho esperado.\n", nomeArquivo);
        fclose(arquivo);
        exit(1); // Encerra o programa
    }

    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            fscanf(arquivo, "%f", &matriz[i * linhas + j]); // Ler cada elemento da matriz do arquivo e armazenar na posição correspondente na matriz
        }
    }

    fclose(arquivo);
}

//Thread le a matriz A
void * LerMatriz(void *args){
    ThreadLeituraInfo* info = (ThreadLeituraInfo*) args;
    LerElementosMatrizes(info->nomeArquivo, info->matriz, info->linhas, info->colunas);
    pthread_exit(NULL);
}

// Função para somar duas matrizes e armazenar o resultado em uma terceira matriz
void ThreadSomarMatrizes(void *args){
    ThreadSomaInfo*info = (ThreadSomaInfo*)args;
    clock_t start = clock(); //Inicio da soma
    int linhas_por_thread = info->linhas / info->num_threads;
    int start_index = info->thread_id * linhas_por_thread;
    int end_index = (info->thread_id == info->num_threads - 1) ? info->linhas : start_index +linhas_por_thread;

    for(int i=start_index; i<end_index;i++){
        for(int j=0; j<info->colunas;j++){
            info->matrizD[i * info->colunas + j] = info->matrizA[i*info->colunas + j] + info->matrizB[i*info->colunas+j];
        }
    }

    clock_t end = clock(); //Fim da soma
    info->tempo_soma = (double)(end-start) /CLOCKS_PER_SEC;
    pthread_exit(NULL);
}
int *SomarMatrizes(int *matrizA, int *matrizB, int linhas, int colunas, int num_threads) {
    // A função recebe duas matrizes como parâmetros (matrizA e matrizB), cada uma representada por um ponteiro para um array de ponteiros.
    // Além disso, ela recebe o número de linhas (linhas) e colunas (colunas) das matrizes como argumentos.

    // Aloca memória para armazenar a matriz resultante (matrizD) com o mesmo número de linhas e colunas das matrizes de entrada.
    int *matrizD = AlocarMatriz(linhas, colunas);
    pthread_t threads[num_threads];
    ThreadSomaInfo thread_info[num_threads];

    // Realiza a soma elemento por elemento das matrizes de entrada (matrizA e matrizB) e armazena o resultado na matrizD.
    for (int i = 0; i < num_threads; i++) {
        thread_info[i].matrizA = matrizA;
        thread_info[i].matrizB = matrizB;
        thread_info[i].matrizD = matrizD;
        thread_info[i].linhas = linhas;
        thread_info[i].colunas = colunas;
        thread_info[i].thread_id = i;
        thread_info[i].num_threads = num_threads;
        // Chamada corrigida para pthread_create
        if (pthread_create(&threads[i], NULL, (void *)(void*)ThreadSomarMatrizes, (void *)&thread_info[i]) != 0) {
            fprintf(stderr, "Erro ao criar a thread %d.\n", i);
            exit(EXIT_FAILURE); // Tratar erro ao criar a thread, se necessário
        }
    }

    double tempo_total = 0; //// Variável para armazenar o tempo total de soma
    // Aguarda o término de todas as threads e coleta o tempo de execução de cada uma
    for (int i = 0; i < num_threads; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            fprintf(stderr, "Erro ao aguardar a thread %d.\n", i);
            exit(EXIT_FAILURE); // Tratar erro ao aguardar a thread, se necessário
        }
        // Atualiza o tempo total com o maior tempo entre as threads
        if (thread_info[i].tempo_soma > tempo_total) {
            tempo_total = thread_info[i].tempo_soma;
        }
    }

    // Impressão do tempo total de soma
    printf("Tempo de soma: %.6f segundos\n", tempo_total);
    
    // Retorna a matriz resultante (matrizD) após a soma.
    return matrizD;
}

// Escrever a matriz resultante em um arquivo
void EscreverMatrizResultado(const char *nomeArquivo, int *matriz, int linhas, int colunas) {
    // Abrir arquivo e verificação de erro
    FILE *arquivo = fopen(nomeArquivo, "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s para escrita.\n", nomeArquivo);
        exit(1);
    }

    // Escrever os elementos da matriz resultante no arquivo
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            fprintf(arquivo, "%.2f ", matriz[i * colunas + j]);
        }
        fprintf(arquivo, "\n");
    }

    fclose(arquivo);
}

// Função para escrever a matriz resultante em um arquivo (usada pela thread)
void *Threadescrevenamatriz(void *args) {
    ThreadEscritaInfo *info = (ThreadEscritaInfo *)args;
    EscreverMatrizResultado(info->nomeArquivo, info->matriz, info->linhas, info->colunas);
    pthread_exit(NULL);
}

void *ThreadMultiplicarMatrizes(void *arg) {
    ThreadMultInfo *info = (ThreadMultInfo *)arg;
    clock_t start = clock(); //Inicio da Multiplicação
    int linhas_por_thread = info->linhas / info->num_threads;
    int inicio_index = info->thread_id * linhas_por_thread;
    int fim_index = (info->thread_id == info->num_threads - 1) ? info->linhas : inicio_index + linhas_por_thread;

    for (int i = inicio_index; i < fim_index; i++) {
        for (int j = 0; j < info->colunas2; j++) {
            info->matrizE[i * info->colunas2 + j] = 0;
            for (int k = 0; k < info->colunas1; k++) {
                info->matrizE[i * info->colunas2 + j] += info->matrizC[i * info->colunas1 + k] * info->matrizD[k * info->colunas2 + j];
            }
        }
    }
    clock_t end = clock(); //Fim da Multiplicação
    info->tempo_mult=(double)(end-start)/CLOCKS_PER_SEC;
    pthread_exit(NULL);
}

// Função para multiplicar duas matrizes e armazenar o resultado em uma terceira matriz
int *MultiplicarMatrizes(int *matrizC, int *matrizD, int linhas, int colunas1, int colunas2, int num_threads) {
    // Aloca memória para armazenar a matriz resultante (matrizE) com o mesmo número de linhas e colunas das matrizes de entrada.
    int *matrizE = AlocarMatriz(linhas, colunas2);
    pthread_t threads[num_threads];
    ThreadMultInfo thread_info[num_threads];

    // Realizar a multiplicação das matrizes de entrada (matrizC e matrizD) e armazena o resultado na matrizE.
    for (int i = 0; i < num_threads; i++) {
        thread_info[i].matrizC = matrizC;
        thread_info[i].matrizD = matrizD;
        thread_info[i].matrizE = matrizE;
        thread_info[i].linhas = linhas;
        thread_info[i].colunas1 = colunas1;
        thread_info[i].colunas2 = colunas2;
        thread_info[i].thread_id = i;
        thread_info[i].num_threads = num_threads;

        // Criação das threads
        if (pthread_create(&threads[i], NULL, ThreadMultiplicarMatrizes, (void *)&thread_info[i]) != 0) {
            fprintf(stderr, "Erro ao criar a thread %d.\n", i);
            exit(EXIT_FAILURE); // Tratar erro ao criar a thread, se necessário
        }
    }

    double tempo_total = 0; //// Variável para armazenar o tempo total de multiplicação
    // Aguarda o término de todas as threads e coleta o tempo de execução de cada uma
    for (int i = 0; i < num_threads; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            fprintf(stderr, "Erro ao aguardar a thread %d.\n", i);
            exit(EXIT_FAILURE); // Tratar erro ao aguardar a thread, se necessário
        }
        // Atualiza o tempo total com o maior tempo entre as threads
        if (thread_info[i].tempo_mult > tempo_total) {
            tempo_total = thread_info[i].tempo_mult;
        }
    }

    // Impressão do tempo total de multiplicação
    printf("Tempo de multiplicação: %.6f segundos\n", tempo_total);

    return matrizE;
}

void *ThreadReduzir(void *arg) {
    ThreadReducaoInfo *info = (ThreadReducaoInfo *)arg;
    clock_t start = clock(); // Início da redução

    info->resultado = 0;
    int elementos_por_thread = (info->linhas * info->colunas) / info->num_threads;
    int inicio_index = info->thread_id * elementos_por_thread;
    int fim_index = (info->thread_id == info->num_threads - 1) ? (info->linhas * info->colunas) : inicio_index + elementos_por_thread;

    for (int i = inicio_index; i < fim_index; i++) {
        info->resultado += info->matrizE[i];
    }

    clock_t end = clock(); // Captura o tempo no fim da execução
    info->tempo_reducao = (double)(end - start) / CLOCKS_PER_SEC;
    pthread_exit(NULL);
}

// Função para reduzir o tamanho da matriz E em um único valor
int Reduzir(int *matrizE, int linhas, int colunas,int num_threads) {
    pthread_t threads[num_threads];
    ThreadReducaoInfo thread_info[num_threads];
    float numeroreduzido = 0;
    double tempo_total = 0;

    // Realizar a redução da matrizE.
    for (int i = 0; i < num_threads; i++) {
        thread_info[i].matrizE = matrizE;
        thread_info[i].linhas = linhas;
        thread_info[i].colunas = colunas;
        thread_info[i].thread_id = i;
        thread_info[i].num_threads = num_threads;

        // Criação das threads
        if (pthread_create(&threads[i], NULL, ThreadReduzir, (void *)&thread_info[i]) != 0) {
            fprintf(stderr, "Erro ao criar a thread %d.\n", i);
            exit(EXIT_FAILURE); // Tratar erro ao criar a thread, se necessário
        }
    }

    // Aguarda o término de todas as threads e acumula o valor reduzido
    for (int i = 0; i < num_threads; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            fprintf(stderr, "Erro ao aguardar a thread %d.\n", i);
            exit(EXIT_FAILURE); // Tratar erro ao aguardar a thread, se necessário
        }
        numeroreduzido += thread_info[i].resultado;
        // Atualiza o tempo total com o maior tempo entre as threads
        if (thread_info[i].tempo_reducao > tempo_total) {
            tempo_total = thread_info[i].tempo_reducao;
        }
    }
    // Impressão do tempo total de multiplicação
    printf("Tempo de redução: %.6f segundos\n", tempo_total);
    printf("O valor resultante da reducao da matriz E eh: %.1f\n", numeroreduzido);
    return numeroreduzido;
}

//  MAIN
int main(int argc, char *argv[]) {
    struct Matriz matrizA, matrizB, matrizC, matrizD;
    int *matrizA_elementos, *matrizB_elementos, *matrizD_elementos, *matrizC_elementos, *matrizE_elementos;
    //variáveis de tempo
    clock_t start, end;

    //definições
    if (argc < 8) {
        printf("Uso: %s <tamanho_matriz> 100 arqA.dat arqB.dat arqC.dat arqD.dat arqE.dat\n", argv[0]);
        return 1;
    }
    int num_threads = atoi(argv[1]); // Número de threads 
    int tamanho_matriz = atoi(argv[2]);  // Tamanho da matriz (n)
    const char *arqA = argv[3];          
    const char *arqB = argv[4];          
    const char *arqC = argv[5];          
    const char *arqD = argv[6];          
    const char *arqE = argv[7]; 
    //definição do tamanho da matriz
    matrizA.linha = tamanho_matriz;
    matrizA.coluna = tamanho_matriz;
    matrizB.linha = tamanho_matriz;
    matrizB.coluna = tamanho_matriz;
    matrizC.linha = tamanho_matriz;
    matrizC.coluna = tamanho_matriz;
    matrizD.linha = tamanho_matriz;
    matrizD.coluna = tamanho_matriz;

    //início das operações
    start = clock(); 

    // Matriz A, B e C
    matrizA_elementos = AlocarMatriz(matrizA.linha, matrizA.coluna);
    matrizB_elementos = AlocarMatriz(matrizB.linha, matrizB.coluna);
    matrizC_elementos = AlocarMatriz(matrizC.linha, matrizC.coluna);
    // Ler os elementos de cada matriz
    ThreadLeituraInfo infoA = {arqA, matrizA_elementos, matrizA.linha,  matrizA.coluna};
    ThreadLeituraInfo infoB = {arqB, matrizB_elementos, matrizB.linha, matrizB.coluna};
    ThreadLeituraInfo infoC = {arqC, matrizC_elementos, matrizC.linha, matrizC.coluna};
    // Criar threads de leitura
    pthread_t threadA, threadB, threadC;
    if (pthread_create(&threadA, NULL, LerMatriz, (void *)&infoA) != 0) {
        fprintf(stderr, "Erro ao criar a thread de leitura para o arquivo A.\n");
        exit(EXIT_FAILURE);
    }
    if (pthread_create(&threadB, NULL, LerMatriz, (void *)&infoB) != 0) {
        fprintf(stderr, "Erro ao criar a thread de leitura para o arquivo B.\n");
        exit(EXIT_FAILURE);
    }
    if (pthread_create(&threadC, NULL, LerMatriz, (void *)&infoC) != 0) {
        fprintf(stderr, "Erro ao criar a thread de leitura para o arquivo C.\n");
        exit(EXIT_FAILURE);
    }
    // Aguardar término das threads
    if (pthread_join(threadA, NULL) != 0) {
        fprintf(stderr, "Erro ao aguardar a thread de leitura para o arquivo A.\n");
        exit(EXIT_FAILURE);
    }
    if (pthread_join(threadB, NULL) != 0) {
        fprintf(stderr, "Erro ao aguardar a thread de leitura para o arquivo B.\n");
        exit(EXIT_FAILURE);
    }
    if (pthread_join(threadC, NULL) != 0) {
        fprintf(stderr, "Erro ao aguardar a thread de leitura para o arquivo C.\n");
        exit(EXIT_FAILURE);
    }

    // Realizar a soma das matrizes A e B
    matrizD_elementos = SomarMatrizes(matrizA_elementos, matrizB_elementos, matrizA.linha, matrizA.coluna, num_threads);
    // Criar a estrutura de informações para a thread de escrita
    ThreadEscritaInfo infoEscritaD = {arqD, matrizD_elementos, matrizD.linha, matrizD.coluna};
    // Criar a thread de escrita
    pthread_t threadEscritaD;
    if (pthread_create(&threadEscritaD, NULL, Threadescrevenamatriz, (void *)&infoEscritaD) != 0) {
        fprintf(stderr, "Erro ao criar a thread de escrita para a matriz D.\n");
        exit(EXIT_FAILURE);
    }
    // Aguardar o término da thread de escrita
    if (pthread_join(threadEscritaD, NULL) != 0) {
        fprintf(stderr, "Erro ao aguardar a thread de escrita para a matriz D.\n");
        exit(EXIT_FAILURE);
    }
    
    // Verificar se o número de colunas da matriz C é igual ao número de linhas da matriz D
    if (matrizC.coluna != matrizD.linha) {
        printf("Erro: O número de colunas da matrizC não é igual ao número de linhas da matrizD. A multiplicação não é permitida.\n");
        return 1; // Encerra o programa
    }
    else{
        // Realizar a multiplicação da Matriz C pela D
        matrizE_elementos = MultiplicarMatrizes(matrizC_elementos, matrizD_elementos, matrizC.linha, matrizC.coluna, matrizD.coluna, num_threads);
        // Abrir arquivo para escrita da matriz E resultante da multiplicação de C e D
        pthread_t thread_escritaE;
        ThreadEscritaInfo infoE = {arqE, matrizE_elementos, tamanho_matriz, tamanho_matriz};
        pthread_create(&thread_escritaE, NULL, Threadescrevenamatriz, (void *)&infoE);
        pthread_join(thread_escritaE, NULL);
        // Reduzir a matriz E e obter um único valor
        Reduzir(matrizE_elementos, matrizC.linha, matrizD.coluna, num_threads);
    }

    end = clock(); //Fim do fluxo
    printf("Tempo total: %.6f segundos.\n", medirtempo(start, end));

    // Liberar memória alocada para as matrizes
    free(matrizA_elementos);
    free(matrizB_elementos);
    free(matrizC_elementos);
    free(matrizD_elementos);
    free(matrizE_elementos);

    return 0;
};