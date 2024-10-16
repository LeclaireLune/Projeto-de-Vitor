#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define maxCartasPegas 10
#define tamBaralho 52

int varTamBaralho = 52; // Vari�vel do tam do baralho q pode alterar no decorrer do codigo
char opcao1;            // Escolhas do jogador
char opcao2;
int soma = 0; // valor pra soma de cartas
char *clearTerminal = "cls";

const char *ranks[] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
const char *naipes[] = {
    "Copas",
    "Ouro",
    "Espada",
    "Paus"
};
// retirei um "E" que tinha, o baralho só tem 4 naipes

typedef struct{ // Struct que ter� as informa��es de cada carta
    const char *rank;
    const char *naipe;
} Carta;

void desenharCartasLadoALado(Carta *cartas, int numCartas){
    for (int linha = 0; linha < 7; linha++){ // Alterado para 7, pois o desenho tem 7 linhas (contando a borda inferior)
        for (int i = 0; i < numCartas; i++){
            switch (linha){
            case 0:
                printf("\t\xDA\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xBF ");
                break;
            case 1:
                printf("\t\xB3 %-2s    \xB3 ", cartas[i].rank);
                break;
            case 2:
                printf("\t\xB3       \xB3 "); // Linha vazia
                break;
            case 3:
                printf("\t\xB3 %-6s\xB3 ", cartas[i].naipe); // Exibe o naipe da carta
                break;
            case 4:
                printf("\t\xB3       \xB3 "); // Linha vazia
                break;
            case 5:
                printf("\t\xB3    %-2s \xB3 ", cartas[i].rank); // Exibe o valor da carta (base direita)
                break;
            case 6:
                printf("\t\xC0\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xD9 "); // Linha inferior
                break;
            }
        }
        printf("\n"); // Pula para a próxima linha após imprimir todas as cartas na linha atual
    }
}

Carta deckAlvo[tamBaralho];
Carta numCartasAgora[tamBaralho];

void geradorBaralho(Carta *baralho){ // Fun��o que ir� gerar todo o baralho
    int indiceBlh = 0;
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 13; j++){
            baralho[indiceBlh].rank = ranks[j];
            baralho[indiceBlh].naipe = naipes[i];
            indiceBlh++;
        }
    }
}

void embaralharBaralho(Carta *baralho){ // Fun��o para embaralhar cartas
    for (int i = 0; i < tamBaralho; i++){
        int r = rand() % tamBaralho;
        Carta temp = baralho[i]; // temp: struct temporario pro armazenamento das cartas q vao ser geradas numa ordem aleatoria
        baralho[i] = baralho[r];
        baralho[r] = temp;
    }
}

void pegarCarta(Carta *deckAlvo, int *numCartasAgora, Carta **baralho, int *tamanhoBaralho){
    deckAlvo[(*numCartasAgora)] = (*baralho)[0]; // O deck alvo recebe a primeira carta do baralho
    (*numCartasAgora)++;                         // Troca do �ndice do deck alvo

    for (int i = 0; i < *tamanhoBaralho - 1; i++){
        (*baralho)[i] = (*baralho)[i + 1]; // Substitui o primeiro elemento pelo q vem depois, e assim vai
    }
    (*tamanhoBaralho)--;

    // Reallocate baralho
    Carta *newBaralho = realloc(*baralho, (*tamanhoBaralho) * sizeof(Carta));
    if (newBaralho == NULL && *tamanhoBaralho > 0){
        fprintf(stderr, "Erro na realloc.\n");
        exit(1);
    }
    *baralho = newBaralho; // Atualiza o ponteiro do baralho
}
int somaCartas(Carta *deckAlvo, int *numCartasAgora){
    int soma = 0, num_A = 0;
    for (int i = 0; i < (*numCartasAgora); i++){
        if (strcmp(deckAlvo[i].rank, "A") == 0)
        {
            soma += 11;
            num_A++;
        }
        else{ 
            if (strcmp(deckAlvo[i].rank, "J") == 0 || strcmp(deckAlvo[i].rank, "Q") == 0 || strcmp(deckAlvo[i].rank, "K") == 0){
                soma += 10;
            }else{
                soma += atoi(deckAlvo[i].rank); // Converte a string para número
            }
        }
    }

    while (num_A > 0 && soma > 21){
        soma -= 10;
        num_A--;
    }

    return soma;
}

int jogoPlayer(Carta **cartasPlayer, int *totCartasPlayer, Carta *baralho){

    printf("\t\t----------------------------------\n");
    printf("\t\t\tJogo de Black Jack\t\t\t\n\n");
    printf("\t\t----------------------------------\n");

    // O n�mero de cartas pode alterar, entao coloquei uma aloca��o de mem�ria aqui pra Vitor ver q sabemos usar (ou nao)
    *cartasPlayer = malloc(maxCartasPegas * sizeof(Carta));
    if (*cartasPlayer == NULL){
        printf("Ocorreu um erro na memória...");
        exit(1);
    }

    // O player pega carta duas vezes
    pegarCarta(*cartasPlayer, totCartasPlayer, &baralho, &varTamBaralho);
    pegarCarta(*cartasPlayer, totCartasPlayer, &baralho, &varTamBaralho);

    printf("\n\t\t\tCartas do Jogador:\n");
    desenharCartasLadoALado(*cartasPlayer, *totCartasPlayer);
    printf("\n\t\t     Você tem %d pontos.\n", somaCartas(*cartasPlayer, totCartasPlayer));

    // La�o de repeti��o para adicionar cartas a mao do Jogador
    do{
        if (somaCartas(*cartasPlayer, totCartasPlayer) == 21){
            printf("\n\t\t     Você obteve %d pontos.\n", somaCartas(*cartasPlayer, totCartasPlayer));
            printf("\n\t\t\tAgora e a vez do Dealer...\n\n");
            return somaCartas(*cartasPlayer, totCartasPlayer);
        }
        printf("\n\n\t\tDeseja adicionar mais cartas? y/n ");
        scanf(" %c", &opcao2);

        if (opcao2 == 'y'){
            if (*totCartasPlayer < maxCartasPegas){
                pegarCarta(*cartasPlayer, totCartasPlayer, &baralho, &varTamBaralho);
                system(clearTerminal); // Limpa a tela (use "cls" se estiver no Windows)

                printf("\t\t----------------------------------\n");
                printf("\t\t\tJogo de Black Jack\t\t\t\n\n");
                printf("\t\t----------------------------------\n");

                printf("\n\t\t\tCartas do Jogador:\n");
                desenharCartasLadoALado(*cartasPlayer, *totCartasPlayer);
                printf("\n");
                printf("\n\t\t     Você tem %d pontos.\n", somaCartas(*cartasPlayer, totCartasPlayer));
                if (somaCartas(*cartasPlayer, totCartasPlayer) > 21){
                    system(clearTerminal);
                    desenharCartasLadoALado(*cartasPlayer, *totCartasPlayer);
                    printf("\n\t\t\t  Você perdeu!\n");
                    printf("\n\t\t     Você obteve %d pontos.\n", somaCartas(*cartasPlayer, totCartasPlayer));
                    return -1;
                }
                else{
                    if (somaCartas(*cartasPlayer, totCartasPlayer) == 21){
                        printf("\n\t\t\tAgora e a vez do Dealer...\n\n");
                        return somaCartas(*cartasPlayer, totCartasPlayer);
                    }
                }
            }
            else{
                printf("\t\t\tJogador atingiu o maximo de cartas pegas por turno.");
                printf("\n\t\t\tAgora e a vez do Dealer...\n\n");
                return somaCartas(*cartasPlayer, totCartasPlayer);
            }
        }
        else if (opcao2 == 'n'){
            printf("\n\t\t\tAgora e a vez do Dealer...\n\n");
            return somaCartas(*cartasPlayer, totCartasPlayer);
        }
        else{
            printf("Input Inv�lido.\n");
        }
    } while (1);

    printf("\n\n");

    // Libera��o de mem�ria
    free(cartasPlayer);
}

int Dealer(Carta *baralho, int somaPlayer, Carta *cartasPlayer, int *totCartasPlayer){
    int totCartasDealer = 0; // O dealer comeca com 0
    int totalDealer = 0;
    Carta *cartasDealer = malloc(maxCartasPegas * sizeof(Carta));
    if (cartasDealer == NULL){
        printf("Ocorreu um erro na memoria...");
        exit(1);
    }

    // O dealer pega carta uma vez
    pegarCarta(cartasDealer, &totCartasDealer, &baralho, &varTamBaralho);
    totalDealer = somaCartas(cartasDealer, &totCartasDealer);

    while (totalDealer < 17 && totalDealer < somaPlayer){
        pegarCarta(cartasDealer, &totCartasDealer, &baralho, &varTamBaralho);
        totalDealer = somaCartas(cartasDealer, &totCartasDealer);
        printf("\n");

        // Limpa a linha do dealer e reimprime as cartas do jogador
        printf("\x1b[2J"); // Limpa a tela
        printf("\x1b[H");  // Move o cursor para o topo da tela

        printf("\t\t----------------------------------\n");
        printf("\t\t\tJogo de Black Jack\t\t\t\n\n");
        printf("\t\t----------------------------------\n");

        printf("\n\t\t\tCartas do Player:\n");
        desenharCartasLadoALado(cartasPlayer, *totCartasPlayer);
        printf("\n\t\t     Você obteve %d pontos.\n", somaCartas(cartasPlayer, totCartasPlayer));

        printf("\n\t\t\tCartas do Dealer:\n");
        desenharCartasLadoALado(cartasDealer, totCartasDealer);
    }

    if (totalDealer > 21 || totalDealer < somaPlayer){
        printf("\n\t\tVocê venceu! O Dealer ficou com: %d pontos\n", totalDealer);
        return 1;
    }
    else{
        if (totalDealer > somaPlayer){
            printf("\n\t    Você perdeu! O Dealer conseguiu: %d pontos\n", totalDealer);
            return -1;
        }else{
            if (totalDealer == somaPlayer){
                printf("\n\t\tVocê empatou! Ambos ficaram com: %d pontos\n", totalDealer);
                return 0;
            }else{
                printf("\n\t\tVocê perdeu! O Dealer ficou com: %d pontos\n", totalDealer);
                return -1;
            }
        }
    }

    free(cartasDealer);
}

int V_Player = 0, V_Dealer = 0, Empates = 0;

void relatorio(int V_Player, int V_Dealer, int Empates){
    FILE *arq_relat = fopen("relatorio.txt", "w");

    fprintf(arq_relat, "\t\t----------------------------------\n");
    fprintf(arq_relat, "\t\t\tRelatório do Black Jack\t\t\t\n\n");
    fprintf(arq_relat, "\t\t----------------------------------\n");

    fprintf(arq_relat, "\t\tO número de vitórias do player foram de: %d\n", V_Player);
    fprintf(arq_relat, "\t   O número de derrotas para o Dealer foram de: %d\n", V_Dealer);
    fprintf(arq_relat, "\t\tO número de empates foram de: %d\n", Empates);
    fprintf(arq_relat, "\n\n");

    printf("\t\t----------------------------------\n");
    printf("\t\t\tRelatório do Black Jack\t\t\t\n\n");
    printf("\t\t----------------------------------\n");

    printf("\t\tO número de vitórias do player foram de: %d\n",V_Player);
    printf("\t   O número de derrotas para o Dealer foram de: %d\n",V_Dealer);
    printf("\t\tO número de empates foram de: %d\n",Empates);
    printf("\n\n");

    fclose(arq_relat);
}

int menu(){ // Fun��o de menu
    int somaPlayer, resultDealer=0;
    char respPlayer,respPlayer_2, respPlayer_3;
    
    printf("\t      Bem-vindo ao menu, deseja jogar? y/n ");
    scanf(" %c", &opcao1);
    if (opcao1 == 'y'){
        system(clearTerminal);
        Carta *baralho = malloc(tamBaralho * sizeof(Carta));
        if (baralho == NULL){
            printf("Erro ao alocar mem�ria para o baralho.\n");
            exit(1);
        }
        geradorBaralho(baralho);
        embaralharBaralho(baralho);

        int totCartasPlayer = 0;
        Carta *cartasPlayer = NULL;
        somaPlayer = jogoPlayer(&cartasPlayer, &totCartasPlayer, baralho);

        if(somaPlayer == -1){
            V_Dealer++;
        }else{
                if (somaPlayer != -1){
                resultDealer=Dealer(baralho, somaPlayer, cartasPlayer, &totCartasPlayer);
                if(resultDealer == 1){
                    V_Player++;
                }else{
                    if(resultDealer == -1){
                        V_Dealer++;
                    }else{
                        Empates++;
                    }
                }
            }
        }
        
        printf("\n\t\t   Deseja jogar novamente? y/n ");
        scanf(" %c", &respPlayer);
        printf("\n");
        if (respPlayer == 'y'){
            return menu();
        }
        else {
            printf("Fechando...");
            relatorio(V_Player, V_Dealer, Empates);
            return 1;
        }
        free(baralho);
    }
    else{
        if (opcao1 == 'n'){
            system(clearTerminal);
            printf("Fechando...\n");
            return 1;
        }else{
            printf("Comando inv�lido... \n");
            return menu();
        }
    }
    return 0;
}

int main(){                      // Fun��o main
    srand(time(NULL)); // Cria uma semente aleat�ria
    menu();
    return 0;
}
