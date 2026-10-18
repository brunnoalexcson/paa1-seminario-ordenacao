// Implementação do Quick Sort
// Responsável: Brunno Alexcson de Carvalho

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX 1000000

static int original[MAX];  // gerado uma vez por teste
static int trabalho[MAX];  // copia de original a cada ordenacao

// particiona e quickSort: fixas, copiadas do README (Cormen 7.1)
int particiona(int vetor[], int inicio, int fim){
    int valorPivo = vetor[fim];
    int i = inicio;

    for (int j = i; j <= fim - 1; j++){
        if (vetor[j] <= valorPivo){
            int aux = vetor[i];
            vetor[i] = vetor[j];
            vetor[j] = aux;

            i++;
        }
    }

    int aux = vetor[i];
    vetor[i] = vetor[fim];
    vetor[fim] = aux;

    return i;
}

void quickSort(int vetor[], int inicio, int fim){
    if (inicio < fim){
        int pivo = particiona(vetor, inicio, fim);
        quickSort(vetor, inicio, pivo-1);
        quickSort(vetor, pivo+1, fim);
    }
}

// preenche v[ini..fim] com menor, menor+1, ... de modo que o pivo
// caia sempre na mediana do trecho
void montaMelhor(int v[], int ini, int fim, int menor){
    int m = fim - ini + 1;
    if (m <= 0)
        return;

    int k = (m - 1) / 2;
    montaMelhor(v, ini, ini + k - 1, menor);          // menores
    montaMelhor(v, ini + k, fim - 1, menor + k + 1);  // maiores

    // gira v[ini+k..fim-1] uma posicao para a direita
    // (compensa a troca final da particiona)
    for (int i = fim - 1; i > ini + k; i--){
        int aux = v[i];
        v[i] = v[i - 1];
        v[i - 1] = aux;
    }
    v[fim] = menor + k;  // mediana no fim
}

// gera o vetor original do caso c (0 aleatorio, 1 melhor, 2 pior)
void geraOriginal(int c, int n){
    if (c == 0){
        // aleatorio: dois rand() de 15 bits (no Windows vai so ate 32767)
        for (int i = 0; i < n; i++)
            original[i] = ((rand() & 0x7FFF) << 15) | (rand() & 0x7FFF);
    } else if (c == 1){
        // melhor
        montaMelhor(original, 0, n - 1, 1);
    } else {
        // pior: 1, 2, ..., n
        for (int i = 0; i < n; i++)
            original[i] = i + 1;
    }
}

// tempo de uma ordenacao de n elementos, em segundos
double medeTempo(int n){
    int R = 1;
    double total;
    clock_t t0;

    // copia e ordena R vezes; abaixo de 0,5 s, dobra R e mede de novo
    while (1){
        t0 = clock();
        for (int r = 0; r < R; r++){
            memcpy(trabalho, original, n * sizeof(int));
            quickSort(trabalho, 0, n - 1);
        }
        total = (double)(clock() - t0) / CLOCKS_PER_SEC;
        if (total >= 0.5)
            break;
        R *= 2;
    }

    // confere se ficou ordenado
    for (int i = 1; i < n; i++){
        if (trabalho[i - 1] > trabalho[i]){
            printf("ERRO: vetor nao ordenado\n");
            exit(1);
        }
    }

    // mede so as R copias, para descontar do total
    t0 = clock();
    for (int r = 0; r < R; r++)
        memcpy(trabalho, original, n * sizeof(int));
    double copias = (double)(clock() - t0) / CLOCKS_PER_SEC;

    return (total - copias) / R;
}

// mostra o rotulo e os n valores de v
void mostraVetor(const char *rotulo, int v[], int n){
    printf("%s", rotulo);
    for (int i = 0; i < n; i++)
        printf(" %d", v[i]);
    printf("\n");
    fflush(stdout);
}

// item b: ordena 10 valores aleatorios de 0 a 99
void demonstracao(void){
    int v[10];

    for (int i = 0; i < 10; i++)
        v[i] = rand() % 100;

    printf("Demonstracao (10 valores de 0 a 99)\n");
    mostraVetor("antes: ", v, 10);
    quickSort(v, 0, 9);
    mostraVetor("depois:", v, 10);
    printf("\n");
    fflush(stdout);
}

int main(int argc, char *argv[]){
    const char *casos[3] = {"aleatorio", "melhor", "pior"};
    int tamanhos[9] = {10, 100, 1000, 5000, 10000, 50000,
                       100000, 500000, 1000000};
    int escolhido = -1;  // -1: todos os casos

    if (argc == 2){
        for (int c = 0; c < 3; c++)
            if (strcmp(argv[1], casos[c]) == 0)
                escolhido = c;
    }
    if (argc > 2 || (argc == 2 && escolhido == -1)){
        printf("uso: quicksort [aleatorio|melhor|pior]\n");
        return 1;
    }

    printf("O resultados.csv fica na pasta de onde o programa e executado.\n");
    printf("Na IDE, pode ser a pasta do projeto.\n\n");
    fflush(stdout);

    FILE *csv = fopen("resultados.csv", "a");
    if (csv == NULL){
        printf("feche o resultados.csv\n");
        return 1;
    }

    // cabecalho so se o arquivo estiver vazio (novo ou sobrado vazio);
    // o fseek vai ao fim porque no Windows o modo "a" comeca na posicao 0
    fseek(csv, 0, SEEK_END);
    if (ftell(csv) == 0){
        fprintf(csv, "caso;n;tempo_s\n");
        fflush(csv);
    }

    demonstracao();

    printf("%-10s %-10s %s\n", "caso", "n", "tempo_s");
    fflush(stdout);

    for (int c = 0; c < 3; c++){
        if (escolhido != -1 && c != escolhido)
            continue;
        if (c == 0)
            srand(42);  // semente fixa no inicio do caso aleatorio

        for (int t = 0; t < 9; t++){
            int n = tamanhos[t];
            char tempo[32];

            // caso e n antes de ordenar: se a pilha estourar, ficam na tela
            printf("%-10s %-10d ", casos[c], n);
            fflush(stdout);

            geraOriginal(c, n);

            // 9 casas decimais, com virgula
            snprintf(tempo, sizeof tempo, "%.9f", medeTempo(n));
            char *ponto = strchr(tempo, '.');
            if (ponto != NULL)
                *ponto = ',';

            printf("%s\n", tempo);
            fflush(stdout);
            fprintf(csv, "%s;%d;%s\n", casos[c], n, tempo);
            fflush(csv);
        }
    }

    fclose(csv);
    return 0;
}