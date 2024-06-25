#include "arquivo.c"

//  MAIN
int main(int argc, char *argv[]) {
    struct Matriz matrizA, matrizB, matrizC, matrizD;
    int *matrizA_elementos, *matrizB_elementos, *matrizD_elementos, *matrizC_elementos, *matrizE_elementos;
    //variáveis de tempo
    clock_t start, end;

    //definições
    if (argc != 8) {
        printf("Uso: %s <quant. threads> <tamanho da matriz> arqA.dat arqB.dat arqC.dat arqD.dat arqE.dat\n", argv[0]);
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
    // Verificação do tamanho das matrizes 
    if ((TamanhoMatriz(arqA) == -1) || (TamanhoMatriz(arqB) == -1) || (TamanhoMatriz(arqC) == -1)) {
        return 1; // Retorna 1 se houver erro ao abrir o arquivo
    } 
    if ((TamanhoMatriz(arqA) < tamanho_matriz) || (TamanhoMatriz(arqB) < tamanho_matriz) || (TamanhoMatriz(arqC) < tamanho_matriz)) {
        printf("O tamanho fornecido excede o numero de linhas e colunas presentes no arquivo.\n");
        return 1;
    }
    // Alocando matrizes
    matrizA_elementos = AlocarMatriz(matrizA.linha, matrizA.coluna);
    matrizB_elementos = AlocarMatriz(matrizB.linha, matrizB.coluna);
    matrizC_elementos = AlocarMatriz(matrizC.linha, matrizC.coluna);
    // Ler os elementos de cada matriz
    ThreadLeituraInfo infoA = {arqA, matrizA_elementos, matrizA.linha,  matrizA.coluna};
    ThreadLeituraInfo infoB = {arqB, matrizB_elementos, matrizB.linha, matrizB.coluna};
    ThreadLeituraInfo infoC = {arqC, matrizC_elementos, matrizC.linha, matrizC.coluna};
    // Criar threads de leitura
    pthread_t threadA, threadB, threadC;
    criarThread(&threadA, LerMatriz, &infoA, "leitura para o arquivo A");
    criarThread(&threadB, LerMatriz, &infoB, "leitura para o arquivo B");
    criarThread(&threadC, LerMatriz, &infoC, "leitura para o arquivo C");
    aguardarThread(threadA, "leitura para o arquivo A");
    aguardarThread(threadB, "leitura para o arquivo B");
    aguardarThread(threadC, "leitura para o arquivo C");

    // Realizar a soma das matrizes A e B
    matrizD_elementos = SomarMatrizes(matrizA_elementos, matrizB_elementos, matrizA.linha, matrizA.coluna, num_threads);
    // Criar a estrutura de informações para a thread de escrita
    ThreadEscritaInfo infoEscritaD = {arqD, matrizD_elementos, matrizD.linha, matrizD.coluna};
    // Criar a thread de escrita
    pthread_t threadEscritaD;
    criarThread(&threadEscritaD, LerMatriz, &infoEscritaD, "leitura para o arquivo D");
    aguardarThread(threadEscritaD, "leitura para o arquivo D");
    
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
        criarThread(&thread_escritaE, LerMatriz, &infoE, "leitura para o arquivo E");
        aguardarThread(thread_escritaE, "leitura para o arquivo E");
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
