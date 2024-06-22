/*
Grupo: Operadoras matriciais
Maria Eduarda de Souza Gomes 260844
Tammy Kojima 213792
*/


#include <stdio.h>
#include <stdlib.h>


// Estrutura da matriz
struct Matriz {
    int linha; // Número de linhas da matriz
    int coluna; // Número de colunas da matriz
};


// Alocação dinâmica de uma matriz com m linhas e n colunas - objetivo de armazenar os elementos da matriz
float *AlocarMatriz(int m, int n) {
    // Alocar memória para a matriz inteira de uma vez
    float *mat = (float *)malloc(m * n * sizeof(float));
    // Verificar se a alocação foi bem sucedida
    if (mat == NULL) {
        printf("Erro ao alocar memória para a matriz.\n");
        exit(1); // Encerra o programa
    }
    return mat; // Retornar um ponteiro para o bloco contínuo de memória alocada
}
// aqui quando for para acessar o elemento na linha i e coluna j, vc precisa chamar assim: mat[i * n + j], sendo n o numero de colunas 
// mudei aqui pq tem q ser alocação em uma unica etapa


// Leitura dos elementos das matrizes do arquivo
void LerElementosMatrizes(const char *nomeArquivo, float *matriz, int linhas, int colunas) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s.\n", nomeArquivo);
        exit(1); // Encerra o programa em caso de erro na abertura do arquivo
    }

    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            fscanf(arquivo, "%f", &matriz[i * colunas + j]); // Ler cada elemento da matriz do arquivo e armazenar na posição correspondente na matriz
        }
    }

    fclose(arquivo);
}


// Função para somar duas matrizes e armazenar o resultado em uma terceira matriz

float *SomarMatrizes(float *matrizA, float *matrizB, int linhas, int colunas) {
    // A função recebe duas matrizes como parâmetros (matrizA e matrizB), cada uma representada por um ponteiro para um array de ponteiros.
    // Além disso, ela recebe o número de linhas (linhas) e colunas (colunas) das matrizes como argumentos.

    // Aloca memória para armazenar a matriz resultante (matrizD) com o mesmo número de linhas e colunas das matrizes de entrada.
    float *matrizD = AlocarMatriz(linhas, colunas);

    // Realiza a soma elemento por elemento das matrizes de entrada (matrizA e matrizB) e armazena o resultado na matrizD.
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            // Soma os elementos correspondentes das matrizes matrizA e matrizB e atribui o resultado à matrizD na mesma posição (linha i, coluna j).
            matrizD[i * colunas + j] = matrizA[i * colunas + j] + matrizB[i * colunas + j];
        }
    }

    // Retorna a matriz resultante (matrizD) após a soma.
    return matrizD;
}


// Escrever a matriz resultante em um arquivo
void EscreverMatrizResultado(const char *nomeArquivo, float *matriz, int linhas, int colunas) {
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

// Função para multiplicar duas matrizes e armazenar o resultado em uma terceira matriz
float *MultiplicarMatrizes(float *matrizC, float *matrizD, int linhas, int colunas1, int colunas2) {
    /* Parâmetros:
     * - matrizC: Matriz C a ser multiplicada (array de ponteiros para floats).
     * - matrizD: Matriz D a ser multiplicada (array de ponteiros para floats).
     * - linhas: Número de linhas da matriz C e matriz E.
     * - colunas1: Número de colunas da matriz C (deve ser igual ao número de linhas da matriz D).
     * - colunas2: Número de colunas da matriz D e matriz E. */

    // Aloca memória para armazenar a matriz resultante (matrizE) com o mesmo número de linhas e colunas das matrizes de entrada.
    float *matrizE = AlocarMatriz(linhas, colunas2);

    // Realizar a multiplicação das matrizes de entrada (matrizC e matrizD) e armazena o resultado na matrizE.
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas2; j++) {
            matrizE[i * colunas2 + j] = 0;
            for (int k = 0; k < colunas1; k++) {
                matrizE[i * colunas2 + j] += matrizC[i * colunas1 + k] * matrizD[k * colunas2 + j];
            }
        }
    }

    // Retorna a matriz resultante (matrizE) após a multiplicação.
    return matrizE;
}


// Função para reduzir o tamanho da matriz E em um único valor
float Reduzir(float *matrizE, int linhas, int colunas) {
    float numeroreduzido = 0;
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            numeroreduzido += matrizE[i * colunas + j]; // Soma dos elementos da matriz E
        }
    }
    return numeroreduzido;
}



//  MAIN
int main(int argc, char *argv[]) {
    //FILE *arqA, *arqB, *arqC, *arqD, *arqE;
    struct Matriz matrizA, matrizB, matrizC, matrizD, matrizE;
    float *matrizA_elementos; // Isso declara uma variável chamada primeira_matriz_elementos que é um ponteiro para um ponteiro para float. 
    // Usado para armazenar a matriz de elementos da primeira matriz lida do arquivo 
    float *matrizB_elementos;
    float *matrizD_elementos;
    float *matrizC_elementos;
    float *matrizE_elementos;
    float resultadodareducao;

    //definições
    if (argc < 8) {
        printf("Uso: %s <tamanho_matriz> 100 arqA.dat arqB.dat arqC.dat arqD.dat arqE.dat\n", argv[0]);
        return 1;
    }
    int tamanho_threads = atoi(argv[1]); // Número de threads 
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
    matrizE.linha = tamanho_matriz;
    matrizE.coluna = tamanho_matriz;
 
    // Matriz A e B
    // Alocar memória para as matrizes
    // O tipo de retorno é float ** porque uma matriz em C é um ponteiro para um array de ponteiros
    matrizA_elementos = AlocarMatriz(matrizA.linha, matrizA.coluna);
    matrizB_elementos = AlocarMatriz(matrizB.linha, matrizB.coluna);
    // Ler os elementos de cada matriz
    LerElementosMatrizes(arqA, matrizA_elementos, matrizA.linha, matrizA.coluna);
    LerElementosMatrizes(arqB, matrizB_elementos, matrizB.linha, matrizB.coluna);

    // Realizar a soma das matrizes A e B
    matrizD_elementos = SomarMatrizes(matrizA_elementos, matrizB_elementos, matrizA.linha, matrizA.coluna);
    // Abrir arquivo para escrita da matriz D resultante da soma de A e B
    EscreverMatrizResultado(arqD, matrizD_elementos, matrizA.linha, matrizA.coluna);

    // Alocar memória para a matriz C
    matrizC_elementos = AlocarMatriz(matrizC.linha, matrizC.coluna);
    // Ler os elementos da matriz C
    LerElementosMatrizes(arqC, matrizC_elementos, matrizC.linha, matrizC.coluna);
    // Verificar se o número de colunas da matriz C é igual ao número de linhas da matriz D
    if (matrizC.coluna != matrizD.linha) {
        printf("Erro: O número de colunas da matrizC não é igual ao número de linhas da matrizD. A multiplicação não é permitida.\n");
        return 1; // Encerra o programa
    }
    else{
        // Realizar a multiplicação da Matriz C pela D
        matrizE_elementos = MultiplicarMatrizes(matrizC_elementos, matrizD_elementos, matrizC.linha, matrizC.coluna, matrizD.coluna);
    }
    // Abrir arquivo para escrita da matriz E resultante da multiplicação de C e D
    EscreverMatrizResultado(arqE, matrizE_elementos, matrizC.linha, matrizD.coluna);

    // Reduzir a matriz E e obter um único valor
    resultadodareducao = Reduzir(matrizE_elementos, matrizC.linha, matrizD.coluna);
    // Imprimir o valor obtido da redução da matriz E
    printf("O valor resultante da reducao da matriz E eh: %.1f\n", resultadodareducao);

    // Liberar memória alocada para as matrizes
    free(matrizA_elementos);
    free(matrizB_elementos);
    free(matrizC_elementos);
    free(matrizD_elementos);
    free(matrizE_elementos);

    return 0;
};