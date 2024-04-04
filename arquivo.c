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



// Função para ler as dimensões das matrizes A e B do arquivo de texto

void LerMatrizes(FILE *arquivo, struct Matriz *matrizA, struct Matriz *matrizB) {

    // Ler as dimensões da primeira matriz do arquivo e armazenar nos membros da estrutura primeira_matriz

    fscanf(arquivo, "%d %d", &matrizA->linha, &matrizA->coluna);

    // Verificar se o arquivo contém outra matriz

    if (!feof(arquivo)) { //Feof é uma função em C que verifica se o indicador de fim de arquivo foi acionado

    // Se houver, ler as dimensões da segunda matriz do arquivo e armazenar nos membros da estrutura segunda_matriz

    fscanf(arquivo, "%d %d", &matrizB->linha, &matrizB->coluna);
    }

    // Se o indicador de fim de arquivo for acionado, significa que só há uma matriz no arquivo
} /* Esta função lê as dimensões das duas matrizes do arquivo de texto.
* É necessário passar a primeira matriz como um ponteiro para que as dimensões lidas do arquivo
* possam ser armazenadas corretamente nos membros linha e coluna da estrutura primeira_matriz. */



// Alocação dinâmica de uma matriz com m linhas e n colunas - objetivo de armazenar os elementos da matriz

float **AlocarMatriz(int m, int n) {
    // Alocar memória para um vetor de ponteiros para as linhas da matriz, onde cada ponteiro aponta para um array de floats
    float **mat = (float **)malloc(m * sizeof(float *));
    // Verificar se a alocação foi bem sucedida
    if (mat == NULL) {
        printf("Erro ao alocar memória para a matriz.\n");
        exit(1); // Encerra o programa
    }
    // Alocar memória para cada linha da matriz
    for (int i = 0; i < m; i++) {
        mat[i] = (float *)malloc(n * sizeof(float));
        // Verificar se a alocação foi bem sucedida
        if (mat[i] == NULL) {
            printf("Erro ao alocar memória para a matriz.\n");
            exit(1); // Encerra o programa
        }
    }
    return mat; // Retornar um ponteiro para um array de ponteiros que apontam para as linhas da matriz alocada
}



// Leitura dos elementos das matrizes do arquivo

void LerElementosMatrizes(FILE *arquivo, float **matriz, int linhas, int colunas) {
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            fscanf(arquivo, "%f", &matriz[i][j]); //  Ler cada elemento da matriz do arquivo e armazenar na posição correspondente na matriz
        }
    }
}



// Função para somar duas matrizes e armazenar o resultado em uma terceira matriz

float **SomarMatrizes(float **matrizA, float **matrizB, int linhas, int colunas) {
    // A função recebe duas matrizes como parâmetros (matrizA e matrizB), cada uma representada por um ponteiro para um array de ponteiros.
    // Além disso, ela recebe o número de linhas (linhas) e colunas (colunas) das matrizes como argumentos.

    // Aloca memória para armazenar a matriz resultante (matrizD) com o mesmo número de linhas e colunas das matrizes de entrada.
    float **matrizD = AlocarMatriz(linhas, colunas);

    // Realiza a soma elemento por elemento das matrizes de entrada (matrizA e matrizB) e armazena o resultado na matrizD.
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            // Soma os elementos correspondentes das matrizes matrizA e matrizB e atribui o resultado à matrizD na mesma posição (linha i, coluna j).
            matrizD[i][j] = matrizA[i][j] + matrizB[i][j];
        }
    }

    // Retorna a matriz resultante (matrizD) após a soma.
    return matrizD;
}



// Escrever a matriz resultante em um arquivo

void EscreverMatrizResultado(FILE *arquivo, float **matrizD, int linhas, int colunas) {

    // Escrever as dimensões da matriz resultante no arquivo

    fprintf(arquivo, "%d %d\n", linhas, colunas);

    // Escrever os elementos da matriz resultante no arquivo

    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            fprintf(arquivo, "%.2f ", matrizD[i][j]);
        }
        fprintf(arquivo, "\n");
    }
}

// Função para multiplicar duas matrizes e armazenar o resultado em uma terceira matriz

float **MultiplicarMatrizes(float **matrizC, float **matrizD, int linhas, int colunas1, int colunas2){
    /* Parâmetros:
     * - matrizC: Matriz C a ser multiplicada (array de ponteiros para floats).
     * - matrizD: Matriz D a ser multiplicada (array de ponteiros para floats).
     * - linhas: Número de linhas da matriz C e matriz E.
     * - colunas1: Número de colunas da matriz C (deve ser igual ao número de linhas da matriz D).
     * - colunas2: Número de colunas da matriz D e matriz E. */

    // Aloca memória para armazenar a matriz resultante (matrizE) com o mesmo número de linhas e colunas das matrizes de entrada.

    float **matrizE = AlocarMatriz(linhas, colunas2);

    //Realizar a multiplicação das matrizes de entrada (matrizC e matrizD) e armazena o resultado na matrizE.

    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas2; j++) {
            matrizE[i][j] = 0;
            for (int k = 0; k < colunas1; k++) {
                matrizE[i][j] += matrizC[i][k] * matrizD[k][j];
            }
        }
    }

    // Retorna a matriz resultante (matrizE) após a multiplicação.

    return matrizE;
}

// Função para reduzir o tamanho da matriz E em um único valor

float Reduzir(float **matrizE,int linhas, int colunas){
    float numeroreduzido = 0;
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            numeroreduzido = numeroreduzido + matrizE[i][j]; //Soma dos elementos da matriz E
        }
    }
    return numeroreduzido;
}

int main() {
    FILE *arquivo;
    struct Matriz matrizA, matrizB, matrizC, matrizD, matrizE;
    float *matrizA_elementos; / Isso declara uma variável chamada primeira_matriz_elementos que é um ponteiro para um ponteiro para float. */
    // Usado para armazenar a matriz de elementos da primeira matriz lida do arquivo //
    float **matrizB_elementos;
    float **matrizD_elementos;
    float **matrizC_elementos;
    float **matrizE_elementos;
    float resultadodareducao;

    // Abrir arquivo das matrizes A e B para leitura

    arquivo = fopen("arquivo.txt", "r");

    // Verificar se o arquivo foi aberto com sucesso
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1; // Encerra o programa;
    }

    // Ler as matrizes do arquivo
    
    LerMatrizes(arquivo, &matrizA, &matrizB);

    // Alocar memória para as matrizes
    // O tipo de retorno é float ** porque uma matriz em C é um ponteiro para um array de ponteiros

    matrizA_elementos = AlocarMatriz(matrizA.linha, matrizA.coluna);
    matrizB_elementos = AlocarMatriz(matrizB.linha, matrizB.coluna);

    // Ler os elementos de cada matriz

    LerElementosMatrizes(arquivo, matrizA_elementos, matrizA.linha, matrizA.coluna);
    LerElementosMatrizes(arquivo, matrizB_elementos, matrizB.linha, matrizB.coluna);

    // Fechar o arquivo

    fclose(arquivo);

    // Realizar a soma das matrizes A e B
 
    matrizD_elementos = SomarMatrizes(matrizA_elementos, matrizB_elementos, matrizA.linha, matrizA.coluna);

    // Abrir novo arquivo para leitura da matriz C

    arquivo = fopen("arquivo2.txt", "r");

    // Verificar se o arquivo foi aberto com sucesso
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1; // Encerra o programa;
    }

    // Ler as dimensões da matriz C do arquivo

    LerMatrizes(arquivo, &matrizC, 0); // O segundo argumento (0) indica que não queremos ler as dimensões da segunda matriz, uma vez que ela não existe.

    // Alocar memória para a matriz C

    matrizC_elementos = AlocarMatriz(matrizC.linha, matrizC.coluna);

    // Ler os elementos da matriz C

    LerElementosMatrizes(arquivo, matrizC_elementos, matrizC.linha, matrizC.coluna);

    // Fechar o arquivo

    fclose(arquivo);

    // Verificar se o número de colunas da matriz C é igual ao número de linhas da matriz D
    if (matrizC.coluna != matrizD.linha) {
        printf("Erro: O número de colunas da matrizC não é igual ao número de linhas da matrizD. A multiplicação não é permitida.\n");
        return 1; // Encerra o programa
    }
    else{

    // Realizar a multiplicação da Matriz C pela D

    matrizE_elementos = MultiplicarMatrizes(matrizC_elementos, matrizD_elementos, matrizC.linha, matrizC.coluna, matrizD.coluna);
    
    }
    //Criar um arquivo para guardar a matriz D resultante da soma de A e B

    arquivo = fopen("resultadosoma.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para escrita.\n");
        return 1;
    }
    
    // Escrever a matriz D resultante no arquivo

    EscreverMatrizResultado(arquivo, matrizD_elementos, matrizA.linha, matrizA.coluna);


    // Fechar o arquivo após a escrita

    fclose(arquivo);

    // Criar um arquivo para guardar a matriz E resultante da multiplicação de C e D

    arquivo = fopen("resultadomultiplicacao.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para escrita.\n");
        return 1;
    }
    
    // Escrever a matriz E resultante no arquivo

    EscreverMatrizResultado(arquivo, matrizE_elementos, matrizC.linha, matrizD.coluna);


    // Fechar o arquivo após a escrita

    fclose(arquivo);

    // Reduzir a matriz E e obter um único valor

    resultadodareducao = Reduzir(matrizE_elementos, matrizC.linha, matrizD.coluna);

    // Imprimir o valor obtido da redução da matriz E

    printf("O valor resultante da reducao da matriz E eh: %.1f\n", resultadodareducao);


    /*// Liberar memória alocada para as matrizes
    for (int i = 0; i < matrizA.linha; i++) {
        free(matrizA_elementos[i]);
        free(matrizB_elementos[i]);
        free(matrizD_elementos[i]);
    }
    free(matrizA_elementos);
    free(matrizB_elementos);
    free(matrizD_elementos);*/

    return 0;
};