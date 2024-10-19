#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ncurses.h>
#define maxCartasPegas 10
#define tamBaralho 52

void iniciar_ncurses() {
    initscr();      // Inicializando a tela com ncurses
    cbreak();       // Desabilita o buffering da linha
    noecho();       // Não mostra os caracteres digitados no terminal
    keypad(stdscr, TRUE); // Habilita o uso de teclas especiais
}

int varTamBaralho = 52; // Vari�vel do tam do baralho q pode alterar no decorrer do codigo
// Escolhas do jogador
int opcao2=-1;
int soma = 0; // valor pra soma de cartas

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

void cabecalho(){
    printw("\t\t\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\n");
    printw("\t\t\tJogo de Black Jack\t\t\t\n\n");
    printw("\t\t\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\n"); refresh();
}

void draw_button(WINDOW *win, int coord_y, int coord_x, const char *palavra){
    mvwprintw(win, coord_y, coord_x, "\xDA\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xBF");      // Parte superior do botão
    mvwprintw(win, coord_y+1, coord_x, "\xB3 %-8s \xB3", palavra); // Texto do botão
    mvwprintw(win, coord_y+2, coord_x, "\xC0\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xD9");      // Parte inferior do botão
    wrefresh(win);
}

int clique_button(int mouse_x, int mouse_y, int button_y, int button_x, int button_width, int button_height) {
    return (mouse_x >= button_x && mouse_x <= button_x + button_width &&
            mouse_y >= button_y && mouse_y <= button_y + button_height);
}

int botao_sn(MEVENT *mousevent, int coord_x, int coord_y,const char *button_1, const char *button_2) {
    int resp_button = -1, value_button = 0;

    draw_button(stdscr, coord_y, coord_x, button_1);  // Corrigido para usar button_1
    draw_button(stdscr, coord_y, coord_x+15, button_2); // Corrigido para usar button_2
    refresh();

    while (1){
        value_button = getch();

        if (value_button == KEY_MOUSE){
            if (getmouse(mousevent) == OK){
                if (mousevent->bstate & BUTTON1_CLICKED){
                    printw("Mouse clicked at (%d, %d)\n", mousevent->x, mousevent->y); refresh();
                    if (clique_button(mousevent->x, mousevent->y, coord_y, coord_x, 13, 3)){
                        resp_button = 1;
                        printw("Botão 'Sim' clicado.\n");
                        refresh();
                        break;
                    }else{
                        if (clique_button(mousevent->x, mousevent->y, coord_y, coord_x+15, 13, 3)) {
                            resp_button = 0;
                            printw("Botão 'Não' clicado.\n");
                            refresh();
                            break;
                        }
                    }
                }
            }
        }
    }
    return resp_button;
}

void desenharCartasLadoALado(Carta *cartas, int numCartas){
    for (int linha = 0; linha < 7; linha++){ // Alterado para 7, pois o desenho tem 7 linhas (contando a borda inferior)
        for (int i = 0; i < numCartas; i++){
            switch (linha){
            case 0:
                printw("\t\xDA\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xBF ");
                break;
            case 1:
                printw("\t\xB3 %-2s    \xB3 ", cartas[i].rank);
                break;
            case 2:
                printw("\t\xB3       \xB3 "); // Linha vazia
                break;
            case 3:
                printw("\t\xB3 %-6s\xB3 ", cartas[i].naipe); // Exibe o naipe da carta
                break;
            case 4:
                printw("\t\xB3       \xB3 "); // Linha vazia
                break;
            case 5:
                printw("\t\xB3    %-2s \xB3 ", cartas[i].rank); // Exibe o valor da carta (base direita)
                break;
            case 6:
                printw("\t\xC0\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xD9 "); // Linha inferior
                break;
            }
        }
        printw("\n"); // Pula para a próxima linha após imprimir todas as cartas na linha atual
    }
    refresh();
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

int jogoPlayer(Carta **cartasPlayer, int *totCartasPlayer, Carta *baralho, MEVENT *mousevent){

    cabecalho();

    // O n�mero de cartas pode alterar, entao coloquei uma aloca��o de mem�ria aqui pra Vitor ver q sabemos usar (ou nao)
    *cartasPlayer = malloc(maxCartasPegas * sizeof(Carta));
    if (*cartasPlayer == NULL){
        printw("Ocorreu um erro na memória..."); refresh();
        exit(1);
    }

    // O player pega carta duas vezes
    pegarCarta(*cartasPlayer, totCartasPlayer, &baralho, &varTamBaralho);
    pegarCarta(*cartasPlayer, totCartasPlayer, &baralho, &varTamBaralho);

    printw("\n\t\t\tCartas do Jogador:\n");
    desenharCartasLadoALado(*cartasPlayer, *totCartasPlayer);
    printw("\n\t\t     Você tem %d pontos.\n", somaCartas(*cartasPlayer, totCartasPlayer)); refresh();

    // La�o de repeti��o para adicionar cartas a mao do Jogador
    do{
        if (somaCartas(*cartasPlayer, totCartasPlayer) == 21){
            printw("\n\t\t     Você obteve %d pontos.\n", somaCartas(*cartasPlayer, totCartasPlayer));
            printw("\n\t\t\tAgora e a vez do Dealer...\n\n"); refresh();
            return somaCartas(*cartasPlayer, totCartasPlayer);
        }
        //adicionar os botões hit me e stand
        opcao2=botao_sn(mousevent, 16, 16,"hit me", "stand");//ordem: x -> y

        if (opcao2 == 1){
            if (*totCartasPlayer < maxCartasPegas){
                pegarCarta(*cartasPlayer, totCartasPlayer, &baralho, &varTamBaralho);
                clear();

                cabecalho();

                printw("\n\t\t\tCartas do Jogador:\n");
                desenharCartasLadoALado(*cartasPlayer, *totCartasPlayer);
                printw("\n");
                printw("\n\t\t     Você tem %d pontos.\n", somaCartas(*cartasPlayer, totCartasPlayer)); refresh();
                if (somaCartas(*cartasPlayer, totCartasPlayer) > 21){
                    clear();
                    cabecalho();
                    desenharCartasLadoALado(*cartasPlayer, *totCartasPlayer);
                    printw("\n\t\t\t  Você perdeu!\n");
                    printw("\n\t\t     Você obteve %d pontos.\n", somaCartas(*cartasPlayer, totCartasPlayer)); refresh();
                    return -1;
                }
                else{
                    if (somaCartas(*cartasPlayer, totCartasPlayer) == 21){
                        printw("\n\t\t\tAgora e a vez do Dealer...\n\n"); refresh();
                        return somaCartas(*cartasPlayer, totCartasPlayer);
                    }
                }
            }
            else{
                printw("\t\t\tJogador atingiu o maximo de cartas pegas por turno.");
                printw("\n\t\t\tAgora e a vez do Dealer...\n\n"); refresh();
                return somaCartas(*cartasPlayer, totCartasPlayer);
            }
        }
        else if (opcao2 == 0){
            printw("\n\t\t\tAgora e a vez do Dealer...\n\n"); refresh();
            return somaCartas(*cartasPlayer, totCartasPlayer);
        }
        else{
            printf("Input Inv�lido.\n");
        }
    } while (1);

    printw("\n\n"); refresh();

    // Libera��o de mem�ria
    free(cartasPlayer);
}

int Dealer(Carta *baralho, int somaPlayer, Carta *cartasPlayer, int *totCartasPlayer){
    int totCartasDealer = 0; // O dealer comeca com 0
    int totalDealer = 0;
    Carta *cartasDealer = malloc(maxCartasPegas * sizeof(Carta));
    if (cartasDealer == NULL){
        printw("Ocorreu um erro na memoria..."); refresh();
        exit(1);
    }

    // O dealer pega carta uma vez
    pegarCarta(cartasDealer, &totCartasDealer, &baralho, &varTamBaralho);
    totalDealer = somaCartas(cartasDealer, &totCartasDealer);

    while (totalDealer < 17 && totalDealer < somaPlayer){
        pegarCarta(cartasDealer, &totCartasDealer, &baralho, &varTamBaralho);
        totalDealer = somaCartas(cartasDealer, &totCartasDealer);
        printw("\n");

        clear(); refresh();

        cabecalho();

        printw("\n\t\t\tCartas do Player:\n");

        desenharCartasLadoALado(cartasPlayer, *totCartasPlayer);
        printw("\n\t\t     Você obteve %d pontos.\n", somaCartas(cartasPlayer, totCartasPlayer)); refresh();

        printw("\n\t\t\tCartas do Dealer:\n"); refresh();
        desenharCartasLadoALado(cartasDealer, totCartasDealer);
    }

    if (totalDealer > 21 || totalDealer < somaPlayer){
        printw("\n\t\tVocê venceu! O Dealer ficou com: %d pontos\n", totalDealer);refresh();
        return 1;
    }
    else{
        if (totalDealer > somaPlayer){
            printw("\n\t    Você perdeu! O Dealer conseguiu: %d pontos\n", totalDealer); refresh();
            return -1;
        }else{
            if (totalDealer == somaPlayer){
                printw("\n\t\tVocê empatou! Ambos ficaram com: %d pontos\n", totalDealer); refresh();
                return 0;
            }else{
                printw("\n\t\tVocê perdeu! O Dealer ficou com: %d pontos\n", totalDealer); refresh();
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

    cabecalho();

    printw("\t\tO número de vitórias do player foram de: %d\n",V_Player); refresh();
    printw("\t   O número de derrotas para o Dealer foram de: %d\n",V_Dealer); refresh();
    printw("\t\tO número de empates foram de: %d\n",Empates); refresh();
    printw("\n\n"); refresh();

    fclose(arq_relat);
}

int a=1, opcao1=-1;
int menu(MEVENT *mousevent, int *opcao1) {
    int somaPlayer, resultDealer = 0, *respPlayer;

    if (a == 1) {
        printw("\t      Bem-vindo ao menu, deseja jogar?\n");
        *opcao1 = botao_sn(mousevent, 15, 5,"sim", "não"); //ordem: x, y
        printw("valor do opcao1: %d",*opcao1);
        refresh();
        a += 1;
    }

    // Verifique se opcao1 foi setada

    if (*opcao1 == 1) {
        clear(); refresh();
        Carta *baralho = malloc(tamBaralho * sizeof(Carta));
        if (baralho == NULL) {
            printw("Erro ao alocar memória para o baralho.\n"); refresh();
            exit(1);
        }
        geradorBaralho(baralho);
        embaralharBaralho(baralho);

        int totCartasPlayer = 0;
        Carta *cartasPlayer = NULL;
        somaPlayer = jogoPlayer(&cartasPlayer, &totCartasPlayer, baralho, mousevent);

        // Verifique o resultado do jogo
        if (somaPlayer == -1) {
            V_Dealer++;
        } else if (somaPlayer >= 0) {
            resultDealer = Dealer(baralho, somaPlayer, cartasPlayer, &totCartasPlayer);
            if (resultDealer == 1) {
                V_Player++;
            } else if (resultDealer == -1) {
                V_Dealer++;
            } else {
                Empates++;
            }
        }

        printw("\n\t\t   Deseja jogar novamente?\n "); refresh();
        if(somaPlayer >= 0){
        *respPlayer=botao_sn(mousevent, 16, 25,"sim","não"); //ordem x, y
        printw("\n"); refresh();
        }else{
        *respPlayer=botao_sn(mousevent, 16, 17,"sim","não"); //ordem x, y
        printw("\n"); refresh();
        }

        if (*respPlayer == 1){
            return menu(mousevent, opcao1);
        }
        else {
            if(*respPlayer == 0){
                clear();
                printw("Fechando..."); refresh();
                relatorio(V_Player, V_Dealer, Empates);
                return 1;
            }
        }
        free(baralho);
    }
    else{
        if (*opcao1 == 0){
            printw("Fechando...\n"); refresh();
            return 1;
        }else{
            printw("Comando inválido... \n"); refresh();
            return menu(mousevent, opcao1);
        }
    }
    return 0;
}

int main(){                      // Fun��o main
    srand(time(NULL)); // Cria uma semente aleatória

    iniciar_ncurses();

    mousemask(ALL_MOUSE_EVENTS, NULL);// habilita os eventos com o mouse
    MEVENT mousevent;

    menu(&mousevent, &opcao1);
    endwin(); //finaliza a biblioteca
    return 0;
}