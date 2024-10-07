#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define tamBaralho 52

char opcao1; //escolhas do jogador
char opcao2;

struct Carta{ //struct que terá as informções de cara carta naipe e rank
    int rank;
    char *naipe;
};

struct Carta baralho [tamBaralho]; //array do struct carta
int indiceBlh = 0; //indice que irá idicar cada carta de 0-52

int geradorBaralho (){ // Função q irá gerar todo o baralho
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

struct Carta pegarCarta (){ //função "pegarCarta" que retorna o struct "Carta"
    return baralho [indiceBlh++];
}


int turno (){ //função para cada turno
    printf ("\n\n\t\tDeseja adicionar mais cartas? y/n ");
    scanf ("%c ", &opcao2);
}

int jogo (){ //GameManager do jogo, onde serão chamadas as funções
    int valor = 0;
    printf ("\t\t\tJogo de Black Jack\t\t\t\n\n");
    geradorBaralho();
    pegarCarta();
    turno ();
    printf ("\n\n");
}

int menu (){ //Função de menu
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


int main() { //função main
    srand (time(NULL));//função que criará uma semente aleatória para função rand (), que será ultilizada para gerar valores int aleatórios, no momento não está sendo ultilizada no código
    menu();
    return 0;
}
