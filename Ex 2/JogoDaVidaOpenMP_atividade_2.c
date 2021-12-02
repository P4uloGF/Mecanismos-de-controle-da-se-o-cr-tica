/*
Programacao Concorrente e Distribuida
2/2021
Autoria:
- Henrique Yoshiaki Okuhara RA:112326
-
-
-

Jogo da vida e highLife em C - openMP
Para alterar o tipo do jogo, alterar o valor da variavel 'qualJogo' para gameOfLife ou highLife
É possível alterar o tipo de saida no console, atraves de uma representacao visual ou apenas com a geracao e a quantidade de celulas vidas, deve-se alterar 'printaGrafico' para true ou false
*/
#include <stdlib.h>
#include <omp.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>
#include <windows.h>

#define nThreads 1
#define vivo 1
#define morto 0
#define true 1
#define false 0
#define gameOfLife 0
#define highLife 1

/*
Alterar aqui o tamanho do grid e da quantidade de iteracoes, caso queira a representacao visual,
coloque um valor do grid que seja possivel apresentar todas as celulas no console (no meu caso foi 60),
ou 2048 para o resultado esperado pelo exercicio.
*/
#define tamanhoGrid 4096
#define iteracoes 20

int getNeighbors(int** grid, int linha, int coluna){
    int i, j, quantidadeVizinhosVivos = 0, posX, posY;
    for(i=-1; i<=1; i++){
        for(j=-1; j<=1; j++){
            if(i !=0 || j!=0){
                posX=i+linha;
                posY=j+coluna;
                if(posX<0) posX = tamanhoGrid-1;
                else if(posX==tamanhoGrid)posX = 0;
                if(posY<0) posY = tamanhoGrid-1;
                else if(posY==tamanhoGrid) posY = 0;
                quantidadeVizinhosVivos+=grid[posX][posY];
            }
        }
    }
    return quantidadeVizinhosVivos;
}

void printaBordaTabela(){
    int i;
    for (i=0; i<tamanhoGrid; i++){
        printf("=");
    }
}

void printTabela(int** newGrid){
    int i, f, g, quantidade = 0;
    for(f=0;f<tamanhoGrid;f++){
        for(g=0;g<tamanhoGrid;g++){
        printf("%d", newGrid[f][g]);
        }
        printf("\n");
    }
}

void insereEstadoInicial(int** grid){
    int lin = 1, col = 1;
    grid[lin  ][col+1] = vivo;
    grid[lin+1][col+2] = vivo;
    grid[lin+2][col  ] = vivo;
    grid[lin+2][col+1] = vivo;
    grid[lin+2][col+2] = vivo;

    lin = 10;
    col = 30;
    grid[lin  ][col+1] = vivo;
    grid[lin  ][col+2] = vivo;
    grid[lin+1][col  ] = vivo;
    grid[lin+1][col+1] = vivo;
    grid[lin+2][col+1] = vivo;
}

void iniciaGrid(int** grid, int** newGrid){
    int i, j;
    for (i = 0; i < tamanhoGrid; ++i) {
        for (j = 0; j < tamanhoGrid; ++j) {
            grid[i][j] = morto;
            newGrid[i][j] = morto;
        }
    }
}

void printTabelaEQuantidade(int** grid, int iteracao, int print){
    if(print == true){
        int i, f, g, quantidade = 0;
        printf("\n");
        for(f=0;f<tamanhoGrid;f++){
            printf("|");
            for(g=0;g<tamanhoGrid;g++){
                if((grid[f][g])%2==0){
                        printf(" |");
                }
                else{
                        printf("#|");
                        quantidade++;
                }
            }
            printf("\n");
        }
        printf("\n Iteracao: %d, Total de vivos: %d\n", iteracao, quantidade);
        Sleep(100);
        system("cls");
    }
    else{
            if(iteracao == 0){
                int f, g, quantidade = 0;
                for(f=0;f<tamanhoGrid;f++){
                    for(g=0;g<tamanhoGrid;g++){
                        if((grid[f][g])%2==1)
                            quantidade++;
                    }
                }
                    printf("Iteracao: %d, Total de vivos: %d\n", iteracao, quantidade);
            }
            else if(iteracao==iteracoes){
                int f, g, quantidade = 0;
                for(f=0;f<tamanhoGrid;f++){
                    for(g=0;g<tamanhoGrid;g++){
                        if((grid[f][g])%2==1)
                            quantidade++;
                    }
                }
                    printf("Ultima iteracao - Total de vivos: %d\n", quantidade);
            }
    }
}

int main() {
    double startSerial, endSerial;
    startSerial = omp_get_wtime();

    int qualJogo = gameOfLife;
    int printaGrafico = false;

	int** grid;
	int** newGrid;
	int l, i, a, b, vizinhosVivos, quantidade;
    double start, end;

	//Cria dois arrays 2d
	grid = (int**)malloc(tamanhoGrid * sizeof(int*));
	newGrid = (int**)malloc(tamanhoGrid * sizeof(int*));
	for (l = 0; l < tamanhoGrid; ++l) {
        grid[l] = (int*)malloc(tamanhoGrid * sizeof(int));
        newGrid[l] = (int*)malloc(tamanhoGrid * sizeof(int));
	}

    //Inicia as duas tabelas apenas com 0
    iniciaGrid(grid, newGrid);
    insereEstadoInicial(grid);

    if(printaGrafico == 1)Sleep(2500);
    printTabelaEQuantidade(grid, 0, printaGrafico);
    endSerial = omp_get_wtime();
    start = omp_get_wtime();
    for (i = 1; i <= iteracoes; i++){
        quantidade = 0;
        #pragma omp parallel num_threads(nThreads)
        {
            #pragma omp for private(a, b, vizinhosVivos)
            for(a = 0; a<tamanhoGrid; a++){
                for(b = 0; b<tamanhoGrid; b++){
                    vizinhosVivos = getNeighbors(grid, a, b);
                    if(grid[a][b] == vivo && (vizinhosVivos == 2 ||vizinhosVivos == 3))
                        newGrid[a][b] = vivo;
                    else if(grid[a][b] == morto && (vizinhosVivos == 3 || (qualJogo==highLife && vizinhosVivos == 6)))
                        newGrid[a][b] = vivo;
                    else
                        newGrid[a][b] = morto;
                }
            }
            printTabelaEQuantidade(newGrid, i, printaGrafico);
            #pragma omp for private(a, b)
            for(a = 0; a<tamanhoGrid; a++){
                for(b = 0; b<tamanhoGrid; b++){
                    grid[a][b] = newGrid[a][b];
                }
            }
        }
    }

    end = omp_get_wtime();

    //printf("Geracao %d: %d\n", iteracoes, quantidade);
    //printf("Serial: took %f seconds.\n", endSerial-startSerial);
    printf("took %f seconds.\n", end-start);
    free(newGrid);
    free(grid);
}
