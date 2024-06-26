# Operadoras-matriciais
  O projeto consiste em um programa em C desenvolvido no ambiente Linux que realiza uma sequência de operações com matrizes de números inteiros utilizando threads. Dessa forma, a partir de arquivos de entrada, o programa realiza operações de soma e multiplicação de matrizes, gravando posteriormente os resultados em arquivos de saída.

  Para inicializar o programa, ao compilar o código, são definidos a quantidade de threads e o tamanho das matrizes que serão manipuladas. Assim, com base nos arquivos A, B e C (arqA.dat, arqB.dat e arqC.dat), os dados necessários são lidos, e o tempo de execução é cronometrado. Após a leitura dos dados, o programa soma as matrizes A e B, armazenando o resultado na matriz D, que é escrita em um arquivo (arqD.dat).

  Antes de realizar a multiplicação das matrizes C e D, o programa verifica se o número de colunas da matriz C é igual ao número de linhas da matriz D, um pré-requisito para a multiplicação de matrizes. Se não forem compatíveis, o programa exibe uma mensagem de erro e encerra. Caso sejam compatíveis, o programa multiplica as matrizes C e D, armazenando o resultado na matriz E.

  Por fim, o programa reduz a matriz E a um único valor somando todos os seus elementos, e o tempo total de execução é calculado e exibido. Portanto, este programa tem como fim estudar o uso de múltiplas threads e como seu funcionamento afeta o tempo de execução.
