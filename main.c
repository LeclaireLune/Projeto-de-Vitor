#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define maxCartasPegas 10
#define tamBaralho 52

int varTamBaralho = 52; // Variável do tam do baralho q pode alterar no decorrer do codigo
char opcao1; // Escolhas do jogador
char opcao2;

const char *ranks[] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
const char *naipes[] = {"C", "O", "P", "E"};

typedef struct { // Struct que terá as informações de cada carta
    const char *rank;
    const char *naipe;
} Carta;

void geradorBaralho(Carta *baralho) { // Função que irá gerar todo o baralho
    int indiceBlh = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 13; j++) {
            baralho[indiceBlh].rank = ranks[j];
            baralho[indiceBlh].naipe = naipes[i];
            indiceBlh++;
        }
    }
}

void embaralharBaralho(Carta *baralho) { // Função para embaralhar cartas
    for (int i = 0; i < tamBaralho; i++) {
        int r = rand() % tamBaralho;
        Carta temp = baralho[i]; // temp: struct temporario pro armazenamento das cartas q vao ser geradas numa ordem aleatoria
        baralho[i] = baralho[r];
        baralho[r] = temp;
    }
}

void pegarCarta(Carta *deckAlvo, int *numCartasAgora, Carta **baralho, int *tamanhoBaralho) {
    deckAlvo[(*numCartasAgora)] = (*baralho)[0]; // O deck alvo recebe a primeira carta do baralho
    (*numCartasAgora)++; // Troca do índice do deck alvo

    for (int i = 0; i < *tamanhoBaralho - 1; i++) {
        (*baralho)[i] = (*baralho)[i + 1]; // Substitui o primeiro elemento pelo q vem depois, e assim vai

    }
    (*tamanhoBaralho)--;

    // Reallocate baralho
    Carta *newBaralho = realloc(*baralho, (*tamanhoBaralho) * sizeof(Carta));
    if (newBaralho == NULL && *tamanhoBaralho > 0) {
        fprintf(stderr, "Erro na realloc.\n");
        exit(1);
    }
    *baralho = newBaralho; // Atualiza o ponteiro do baralho
}

void jogoPlayer() {

    int iniCartasPlayer = 2; // Número inicial de cartas para o jogador
    int totCartasPlayer = 0; //O jogador comeca com 0

    printf("\t\t\tJogo de Black Jack\t\t\t\n\n");

    // Alocação de memória para o baralho e cartas do jogador
    Carta *baralho = malloc(tamBaralho * sizeof(Carta));
    if (baralho == NULL) {
        printf("Erro ao alocar memória para o baralho.\n");
        exit(1);
    }

    geradorBaralho(baralho);
    embaralharBaralho(baralho);

    // O número de cartas pode alterar, entao coloquei uma alocação de memória aqui pra Vitor ver q sabemos usar (ou nao)
    Carta *cartasPlayer = malloc(maxCartasPegas * sizeof(Carta));
    if (cartasPlayer == NULL) {
        printf("Ocorreu um erro na memoria...");
        exit(1);
    }

    // O player pega carta duas vezes
    pegarCarta(cartasPlayer, &totCartasPlayer, &baralho, &varTamBaralho);
    pegarCarta(cartasPlayer, &totCartasPlayer, &baralho, &varTamBaralho);

    printf("\n\t\t\tCartas do Jogador:\n");
    for (int i = 0; i < iniCartasPlayer; i++) {
        printf("\n\t\t\tNaipe: %s // Rank: %s", cartasPlayer[i].naipe, cartasPlayer[i].rank);
    }
    // Laço de repetição para adicionar cartas a mao do Jogador
    do {
        printf("\n\n\t\tDeseja adicionar mais cartas? y/n ");
        scanf(" %c", &opcao2);
        printf("\x1b[1F\x1b[2K"); // Limpa a linha anterior

        if (opcao2 == 'y') {
            if (totCartasPlayer < maxCartasPegas) {
                pegarCarta(cartasPlayer, &totCartasPlayer, &baralho, &varTamBaralho);
                printf("\t\t\tNaipe: %s // Rank: %s", cartasPlayer[totCartasPlayer - 1].naipe, cartasPlayer[totCartasPlayer - 1].rank);
            }
            else {
                printf ("\t\t\tJogador atingiu o maximo de cartas pegas por turno.");
                printf ("\n\t\t\tAgora e a vez do Dealer...\n\n");
                break;
            }
        } else if (opcao2 == 'n') {
            printf ("\n\t\t\tAgora e a vez do Dealer...\n\n");
            break;
        } else {
            printf("Input Inválido.\n");
        }
    } while (1);

    printf("\n\n");

    // Liberação de memória
    free(cartasPlayer);
    free(baralho);
}

int menu() { // Função de menu
    printf("Bem-vindo ao menu, deseja jogar? y/n ");
    scanf(" %c", &opcao1);
    if (opcao1 == 'y') {
        system("cls");
        jogoPlayer();
    } else if (opcao1 == 'n') {
        system("cls");
        printf("Fechando...\n");
        return 1;
    } else {
        printf("Comando inválido... \n");
        return menu();
    }
    return 0;
}

int main() { // Função main
    srand(time(NULL)); // Cria uma semente aleatória
    menu();
    return 0;
}
