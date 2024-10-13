#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define tamBaralho 52

int varTamBaralho = 52; // Variável do tam do baralho q pode alterar no decorrer do codigo
int *ptrVarTamBaralho = &varTamBaralho; // Ponteiro pra poder alterar o valor por funções

char opcao1; //escolhas do jogador
char opcao2;
int i; // variavel i pro for, nao fa�o a m�nima ideia do pq ele quer tanto q eu declare ela fora dos parametros do for, ent deixa ela aq msm

char *ranks [] = {'A','2','3','4','5','6','7','8','9','10','J','Q','K'};
char *naipes [] = {'C','O','E','P'};

typedef struct { //struct que ter� as inform��es de cara carta naipe e rank
    char *rank;
    char *naipe;
}Carta;

int indiceBlh = 0; //indice que ir� idicar cada carta de 0-52

void geradorBaralho (Carta *baralho){ // Fun��o q ir� gerar todo o baralho
    for (int i=0; i<4; i++){
        for (int j=0; j<13; j++){
            baralho[indiceBlh].rank = ranks [j];
            baralho[indiceBlh].naipe = naipes[i];
            indiceBlh++;
        }
    }

}

void embaralharBaralho ( Carta *baralho ){ //adicionado embaralhador de cartas
    for (i; i < tamBaralho; i++){ //desgra�ado do i q quer pq quer ser declarado fora dos par�metros do for
        int r = rand () % tamBaralho;
        Carta temp = baralho [i]; // temp: struct temporario pro armazenamento das cartas q v�o ser geradas numa ordem aleat�ria
        baralho [i] = baralho [r];
        baralho [r] = temp;
    }

}

void pegarCarta( Carta *deckAlvo, int *numCartasAgora, Carta *baralho , int *tamanhoBaralho ){
    deckAlvo[(*numCartasAgora)] = baralho[0]; // O deck alvo recebe a primeira carta do baralho
    (*numCartasAgora)++; // Troca do índice do deck alvo
    for (int i = 0; i < *tamanhoBaralho; i++){
        baralho[i] = baralho[i + 1]; // Substitui o primeiro elemento pelo q vem depois, e assim vai
    }
    (*tamanhoBaralho)--;
    baralho = (Carta*) realloc(baralho, ((*tamanhoBaralho) - 1) * sizeof(Carta));
}


int menu (){ //Fun��o de menu
    printf ("Bem-vindo ao menu, deseja jogar? y/n ");
    scanf (" %c", &opcao1);
    if (opcao1 == 'y'){
        system("cls");
        jogo ();
    }
    else if (opcao1 == 'n'){
        system ("cls");
        printf ("Fechando...");
        return 1;
    }
    else {
        printf ("Comando invalido... \n");
        menu ();
    }

}

int turno (){ //fun��o para andamento de turno
    printf ("\n\n\t\tDeseja adicionar mais cartas? y/n ");
    scanf ("%c ", &opcao2);
}

int jogo (){ //GameManager do jogo, onde ser�o chamadas as fun��es
    int valor = 0;

    int initCartasDealer = 1, initCartasPlayer = 2; // Número inicial de cartas para cada um 
    int totCartasDealer = 0, totCartasPlayer = 0; // Ambos começam com 0 cartas
    
    int *ptr_totCartasDealer = &totCartasDealer; // Ponteiros só p poder alterar o valor por meio de funções
    int *ptr_totCartasPlayer = &totCartasPlayer; 
    
    int sizeofCarta = sizeof(Carta); // Tamanho da struct, só pra não repetir o sizeof nos malloc logo abaixo
    
    printf ("\t\t\tJogo de Black Jack\t\t\t\n\n");
    Carta baralho [tamBaralho]; //array do struct carta
    geradorBaralho(baralho);
    embaralharBaralho(baralho);

    // O número de cartas pode alterar, entao coloquei uma alocação de memória aqui pra Vitor ver q sabemos usar (ou nao)
    Carta *cartasDealer = (Carta*) malloc(initCartasDealer * sizeofCarta);
    Carta *cartasPlayer = (Carta*) malloc(initCartasPlayer * sizeofCarta);
    
    if (cartasDealer == NULL || cartasPlayer == NULL){
        printf("Ocorreu um erro na memoria...");
        exit(1);
    }
    printf("\nBaralho completo: \n");
    for (int i = 0; i < varTamBaralho; i++)
    {
        printf("%c%c ", baralho[i].naipe, baralho[i].rank);
    }

    pegarCarta(cartasPlayer, ptr_totCartasPlayer, baralho, ptrVarTamBaralho);
    pegarCarta(cartasPlayer, ptr_totCartasPlayer, baralho, ptrVarTamBaralho);
    
    printf("\nBaralho completo - 1: \n");
    for (int i = 0; i < varTamBaralho; i++){
        printf("%c%c ", baralho[i].naipe, baralho[i].rank);
    }

    printf("\nCartas Player:\n");
    for(int i = 0; i < initCartasPlayer; i++){
        printf("\nNaipe: %c // Rank: %c", cartasPlayer[i].naipe, cartasPlayer[i].rank);
    }

    turno ();
    printf ("\n\n");

    free(cartasDealer);
    free(cartasPlayer);
}

int main() { //fun��o main
    srand (time(NULL));//fun��o que criar� uma semente aleat�ria para fun��o rand (), que ser� ultilizada para gerar valores int aleat�rios, no momento n�o est� sendo ultilizada no c�digo
    menu();
    return 0;
}
