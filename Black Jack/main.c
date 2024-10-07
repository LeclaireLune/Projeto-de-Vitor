#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define tamBaralho 52

char opcao1; //escolhas do jogador
char opcao2;

struct Carta{ //struct que ter� as inform��es de cara carta naipe e rank
    int rank;
    char *naipe;
};

struct Carta baralho [tamBaralho]; //array do struct carta
int indiceBlh = 0; //indice que ir� idicar cada carta de 0-52

int geradorBaralho (){ // Fun��o q ir� gerar todo o baralho
    char rank [] = {'A','2','3','4','5','6','7','8','9','10','J','Q','K'};
    char naipe [] = {'C','O','E','P'};

    for (int i=0; i<4; i++){
        for (int j=0; j<13; j++){
            baralho[indiceBlh].rank = rank [j];
            baralho[indiceBlh].naipe = naipe[i];
            indiceBlh++;
        }
    }

}

struct Carta pegarCarta (){ //fun��o "pegarCarta" que retorna o struct "Carta"
    return baralho [indiceBlh++];
}


int turno (){ //fun��o para cada turno
    printf ("\n\n\t\tDeseja adicionar mais cartas? y/n ");
    scanf ("%c ", &opcao2);
}

int jogo (){ //GameManager do jogo, onde ser�o chamadas as fun��es
    int valor = 0;
    printf ("\t\t\tJogo de Black Jack\t\t\t\n\n");
    geradorBaralho();
    pegarCarta();
    turno ();
    printf ("\n\n");
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


int main() { //fun��o main
    srand (time(NULL));//fun��o que criar� uma semente aleat�ria para fun��o rand (), que ser� ultilizada para gerar valores int aleat�rios, no momento n�o est� sendo ultilizada no c�digo
    menu();
    return 0;
}
