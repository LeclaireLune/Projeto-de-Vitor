#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define tamBaralho 52

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

int geradorBaralho (Carta *baralho){ // Fun��o q ir� gerar todo o baralho
    for (int i=0; i<4; i++){
        for (int j=0; j<13; j++){
            baralho[indiceBlh].rank = ranks [j];
            baralho[indiceBlh].naipe = naipes[i];
            indiceBlh++;
        }
    }

}

int embaralharBaralho ( Carta *baralho ){ //adicionado embaralhador de cartas
    for (i; i < tamBaralho; i++){ //desgra�ado do i q quer pq quer ser declarado fora dos par�metros do for
        int r = rand () % tamBaralho;
        Carta temp = baralho [i]; // temp: struct temporario pro armazenamento das cartas q v�o ser geradas numa ordem aleat�ria
        baralho [i] = baralho [r];
        baralho [r] = temp;
    }

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
    printf ("\t\t\tJogo de Black Jack\t\t\t\n\n");
    Carta baralho [tamBaralho]; //array do struct carta
    geradorBaralho(baralho);
    turno ();
    printf ("\n\n");
}

int main() { //fun��o main
    srand (time(NULL));//fun��o que criar� uma semente aleat�ria para fun��o rand (), que ser� ultilizada para gerar valores int aleat�rios, no momento n�o est� sendo ultilizada no c�digo
    menu();
    return 0;
}
