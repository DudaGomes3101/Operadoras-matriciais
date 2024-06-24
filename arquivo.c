/*
Grupo: Operadoras matriciais
Maria Eduarda de Souza Gomes 260844
Tammy Kojima 213792
*/

#include "arquivo.h"

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
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            fscanf(arquivo, "%d", &matriz[i * linhas + j]); // Ler cada elemento da matriz do arquivo e armazenar na posição correspondente na matriz
        }
    }
    fclose(arquivo);
}

//Thread le a matriz
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
            fprintf(arquivo, "%d", matriz[i * colunas + j]);
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

// função que exibirá a quantidade de linhas do arquivo para depois ser possível verificar se a quantidade que for fornecida na hora de copilação é maior ou não
int TamanhoMatriz(const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    int contagem = 0;
    char linha[10000]; // Tamanho máximo de uma linha
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return -1; // Retorna -1 em caso de erro
    }
    // Conta as linhas do arquivo
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        contagem++;
    }
    fclose(arquivo);
    return contagem;
}

void criarThread(pthread_t *thread, void *(*func)(void *), void *args, const char *erroMsg) {
    if (pthread_create(thread, NULL, func, args) != 0) {
        fprintf(stderr, "Erro ao criar a thread: %s.\n", erroMsg);
        exit(EXIT_FAILURE);
    }
}

void aguardarThread(pthread_t thread, const char *erroMsg) {
    if (pthread_join(thread, NULL) != 0) {
        fprintf(stderr, "Erro ao aguardar a thread: %s.\n", erroMsg);
        exit(EXIT_FAILURE);
    }
}