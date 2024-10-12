#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define tamBaralho 52

char opcao1; //escolhas do jogador
char opcao2;
int i; // variavel i pro for, nao faço a mínima ideia do pq ele quer tanto q eu declare ela fora dos parametros do for, ent deixa ela aq msm

char *ranks [] = {'A','2','3','4','5','6','7','8','9','10','J','Q','K'};
char *naipes [] = {'C','O','E','P'};

typedef struct { //struct que terá as informções de cara carta naipe e rank
    char *rank;
    char *naipe;
}Carta;

int indiceBlh = 0; //indice que irá idicar cada carta de 0-52

int geradorBaralho (Carta *baralho){ // Função q irá gerar todo o baralho
    for (int i=0; i<4; i++){
        for (int j=0; j<13; j++){
            baralho[indiceBlh].rank = ranks [j];
            baralho[indiceBlh].naipe = naipes[i];
            indiceBlh++;
        }
    }

}

int embaralharBaralho ( Carta *baralho ){ //adicionado embaralhador de cartas
    for (i; i < tamBaralho; i++){ //desgraçado do i q quer pq quer ser declarado fora dos parâmetros do for
        int r = rand () % tamBaralho;
        Carta temp = baralho [i]; // temp: struct temporario pro armazenamento das cartas q vão ser geradas numa ordem aleatória
        baralho [i] = baralho [r];
        baralho [r] = temp;
    }

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

int turno (){ //função para andamento de turno
    printf ("\n\n\t\tDeseja adicionar mais cartas? y/n ");
    scanf ("%c ", &opcao2);
}

int jogo (){ //GameManager do jogo, onde serão chamadas as funções
    int valor = 0;
    printf ("\t\t\tJogo de Black Jack\t\t\t\n\n");
    Carta baralho [tamBaralho]; //array do struct carta
    geradorBaralho(baralho);
    turno ();
    printf ("\n\n");
}

int main() { //função main
    srand (time(NULL));//função que criará uma semente aleatória para função rand (), que será ultilizada para gerar valores int aleatórios, no momento não está sendo ultilizada no código
    menu();
    return 0;
}
