#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define maxCartasPegas 10
#define tamBaralho 52

int varTamBaralho = 52; // Vari�vel do tam do baralho q pode alterar no decorrer do codigo
char opcao1; // Escolhas do jogador
char opcao2;
int soma=0; //valor pra soma de cartas

const char *ranks[] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
const char *naipes[] = {"Copas", "Ouro", "Espada", "Paus",};
//retirei um "E" que tinha, o baralho só tem 4 naipes

typedef struct { // Struct que ter� as informa��es de cada carta
    const char *rank;
    const char *naipe;
} Carta;

Carta deckAlvo [tamBaralho];
Carta numCartasAgora [tamBaralho];

void geradorBaralho(Carta *baralho) { // Fun��o que ir� gerar todo o baralho
    int indiceBlh = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 13; j++) {
            baralho[indiceBlh].rank = ranks[j];
            baralho[indiceBlh].naipe = naipes[i];
            indiceBlh++;
        }
    }
}

void embaralharBaralho(Carta *baralho) { // Fun��o para embaralhar cartas
    for (int i = 0; i < tamBaralho; i++) {
        int r = rand() % tamBaralho;
        Carta temp = baralho[i]; // temp: struct temporario pro armazenamento das cartas q vao ser geradas numa ordem aleatoria
        baralho[i] = baralho[r];
        baralho[r] = temp;
    }
}

void pegarCarta(Carta *deckAlvo, int *numCartasAgora, Carta **baralho, int *tamanhoBaralho) {
    deckAlvo[(*numCartasAgora)] = (*baralho)[0]; // O deck alvo recebe a primeira carta do baralho
    (*numCartasAgora)++; // Troca do �ndice do deck alvo

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
int somaCartas(Carta *deckAlvo, int *numCartasAgora) {
    int soma = 0;
    int valor;
    for (int i = 0; i < (*numCartasAgora); i++) {
        if (strcmp(deckAlvo[i].rank, "A") == 0) {
            if(soma<=10){
                soma+=11;
            }else{
                soma+=1;
            }
        } else if (strcmp(deckAlvo[i].rank, "J") == 0 || strcmp(deckAlvo[i].rank, "Q") == 0 || strcmp(deckAlvo[i].rank, "K") == 0) {
            soma += 10;
        } else {
            soma += atoi(deckAlvo[i].rank); // Converte a string para número
        }
    }
    return soma;
}


int jogoPlayer(Carta *baralho) {
    int iniCartasPlayer = 2; // N�mero inicial de cartas para o jogador
    int totCartasPlayer = 0; // O jogador comeca com 0

    int valorTotal=0;

    printf("\t\t\tJogo de Black Jack\t\t\t\n\n");

    // O n�mero de cartas pode alterar, entao coloquei uma aloca��o de mem�ria aqui pra Vitor ver q sabemos usar (ou nao)
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
        printf("\n\t\t\tNaipe: %s // Rank: %s\n", cartasPlayer[i].naipe, cartasPlayer[i].rank);
    }
    // La�o de repeti��o para adicionar cartas a mao do Jogador
    do {
        if(somaCartas(cartasPlayer,&totCartasPlayer) == 21){
            printf("\n\t\t\tVocê Venceu!\n");
            return 1;
        }
        printf("\n\n\t\tDeseja adicionar mais cartas? y/n ");
        scanf(" %c", &opcao2);
        printf("\x1b[1F\x1b[2K"); // Limpa a linha anterior

        if (opcao2 == 'y') {
            if (totCartasPlayer < maxCartasPegas) {
                pegarCarta(cartasPlayer, &totCartasPlayer, &baralho, &varTamBaralho);
                printf("\t\t\tNaipe: %s // Rank: %s", cartasPlayer[totCartasPlayer - 1].naipe, cartasPlayer[totCartasPlayer - 1].rank);
                if (somaCartas(cartasPlayer,&totCartasPlayer) > 21){
                    printf("\n\t\t\tVocê perdeu!\n");
                    return -1;
                }
                else{
                    if (somaCartas(cartasPlayer,&totCartasPlayer) == 21){
                        printf("\n\t\t\tVocê Venceu!\n");
                        return 1;
                    }
                }
            }
            else {
                printf ("\t\t\tJogador atingiu o maximo de cartas pegas por turno.");
                printf ("\n\t\t\tAgora e a vez do Dealer...\n\n");
                return somaCartas(cartasPlayer,&totCartasPlayer);
            }
        } else if (opcao2 == 'n') {
            printf ("\n\t\t\tAgora e a vez do Dealer...\n\n");
            return somaCartas(cartasPlayer,&totCartasPlayer);
        } else {
            printf("Input Inv�lido.\n");
        }
    } while (1);

    printf("\n\n");

    // Libera��o de mem�ria
    free(cartasPlayer);
}

void Dealer(Carta *baralho, int somaPlayer){
    int iniCartasDealer = 1; // Número inicial de cartas para o computador/dealer
    int totCartasDealer = 0; // O dealer comeca com 0
    int totalDealer = 0;
    Carta *cartasDealer = malloc(maxCartasPegas * sizeof(Carta));
    if (cartasDealer == NULL) {
        printf("Ocorreu um erro na memoria...");
        exit(1);
    }

    // O dealer pega carta uma vez
    pegarCarta(cartasDealer, &totCartasDealer, &baralho, &varTamBaralho);
    totalDealer = somaCartas(cartasDealer, &totCartasDealer);
    printf("\n\t\t\tNaipe: %s // Rank: %s\n",cartasDealer[totCartasDealer-1].naipe,cartasDealer[totCartasDealer-1].rank);

    while(totalDealer<17 && totalDealer<somaPlayer){
        pegarCarta(cartasDealer, &totCartasDealer, &baralho, &varTamBaralho);
        totalDealer = somaCartas(cartasDealer, &totCartasDealer);
        printf("\n\t\t\tNaipe: %s // Rank: %s\n",cartasDealer[totCartasDealer-1].naipe,cartasDealer[totCartasDealer-1].rank);
    }

    if(totalDealer>21){
        printf("\n\t\t\tVocê venceu! O Dealer estourou com: %d pontos\n",totalDealer);
    }else{
        if(totalDealer>somaPlayer){
            printf("\n\t\t\tVocê perdeu! O Dealer conseguiu: %d pontos\n",totalDealer);
        }else{
            if(totalDealer==somaPlayer){
                printf("\n\t\t\tVocê empatou! Ambos ficaram com: %d pontos\n",totalDealer);
            }else{
                printf("\n\t\t\tVocê perdeu! O Dealer ficou com: %d pontos\n",totalDealer);
            }
        }
    }


    free(cartasDealer);
}

int menu() { // Fun��o de menu
    int somaPlayer;

    printf("\t\tBem-vindo ao menu, deseja jogar? y/n ");
    scanf(" %c", &opcao1);
    if (opcao1 == 'y') {
        system("cls");
        Carta *baralho = malloc(tamBaralho * sizeof(Carta));
        if (baralho == NULL) {
            printf("Erro ao alocar mem�ria para o baralho.\n");
            exit(1);
        }
        geradorBaralho(baralho);
        embaralharBaralho(baralho);

        somaPlayer=jogoPlayer(baralho);
        if(somaPlayer!=1 && somaPlayer!=-1){
            Dealer(baralho,somaPlayer);
            return 1;
        }
        free(baralho);
    } else if (opcao1 == 'n') {
        system("cls");
        printf("Fechando...\n");
        return 1;
    } else {
        printf("Comando inv�lido... \n");
        return menu();
    }
    return 0;
}

int main() { // Fun��o main
    srand(time(NULL)); // Cria uma semente aleat�ria
    menu();
    return 0;
}