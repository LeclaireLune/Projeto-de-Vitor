#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define maxCartasPegas 10
#define tamBaralho 52

int varTamBaralho = 52; // Vari�vel do tam do baralho q pode alterar no decorrer do codigo
char opcao1; // Escolhas do jogador
char opcao2;

const char *ranks[] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
const char *naipes[] = {"C", "O", "E", "P",}; // Including "E" as "Espadas"
//retirei um "E" que tinha, o baralho só tem 4 naipes

typedef struct { // Struct que ter� as informa��es de cada carta
    const char *rank;
    const char *naipe;
} Carta;

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

void jogoPlayer() {

    int iniCartasPlayer = 2; // N�mero inicial de cartas para o jogador
    int totCartasPlayer = 0; //O jogador comeca com 0
    int valorTotal=0;

    printf("\t\t\tJogo de Black Jack\t\t\t\n\n");

    // Aloca��o de mem�ria para o baralho e cartas do jogador
    Carta *baralho = malloc(tamBaralho * sizeof(Carta));
    if (baralho == NULL) {
        printf("Erro ao alocar mem�ria para o baralho.\n");
        exit(1);
    }

    geradorBaralho(baralho);
    embaralharBaralho(baralho);

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
        printf("\n\t\t\tNaipe: %s // Rank: %s", cartasPlayer[i].naipe, cartasPlayer[i].rank);
    }
    // La�o de repeti��o para adicionar cartas a mao do Jogador
    do {
        printf("\n\n\t\tDeseja adicionar mais cartas? y/n ");
        scanf(" %c", &opcao2);
        printf("\x1b[1F\x1b[2K"); // Limpa a linha anterior

        if (opcao2 == 'y') {
            if (totCartasPlayer < maxCartasPegas) {
                pegarCarta(cartasPlayer, &totCartasPlayer, &baralho, &varTamBaralho);
                printf("\t\t\tNaipe: %s // Rank: %s", cartasPlayer[totCartasPlayer - 1].naipe, cartasPlayer[totCartasPlayer - 1].rank);
                valorTotal=somaCartas(cartasPlayer, &totCartasPlayer);
                if(valorTotal==-1){
                    printf("\n\t\t\tVocê perdeu!\n");
                    break;
                }else{
                    if(valorTotal==21){
                        printf("\n\t\t\tVocê Venceu!\n");
                        break;
                    }
                }
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
            printf("Input Inv�lido.\n");
        }
    } while (1);

    printf("\n\n");

    // Libera��o de mem�ria
    free(cartasPlayer);
    free(baralho);
}

int somaCartas(Carta *deckAlvo, int *numCartasAgora){
    int soma=0, x;
    int *valorDeck = malloc((*numCartasAgora) * sizeof(int));

    for(int i=0; i<*numCartasAgora; i++){
        for(int j=1; j=10; j++){
            if(deckAlvo[i].rank == 'J' || deckAlvo[i].rank == 'Q' || deckAlvo[i].rank == 'K'){
                valorDeck[i]=10;
            }else{
                if(deckAlvo[i].rank == 'j'){
                    if(deckAlvo[i].rank == '1'){
                        do{
                            printf("Desejas usar está carta como 1 ou 11? (1/11)\n");
                            scanf(" %d", &x);
                            if(x==1){
                                valorDeck[i]=1;
                            }else{
                                if(x==11){
                                    valorDeck[i]=11;
                                }else{
                                    printf("Valor inválido!\nTente novamente.\n");
                                }
                            }
                        }while(x!=1 || x!=11);
                    }else{
                        valorDeck[i]=j;
                    }
                }
            }
        }
    }

    for(int i=0; i<*numCartasAgora; i++){
        soma=soma+valorDeck[i];
        if(soma==21){
            return soma;
        }else{
            if(soma>21){
                return -1
            }else{
                return soma;
            }
        }
    }
    free(valorDeck);
}

int menu() { // Fun��o de menu
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