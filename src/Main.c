#include "../include/screen.h"
#include "../include/keyboard.h"
#include "../include/inimigo.h"
#include "../include/game_config.h"
#include "../include/barradevida.h"
#include "../include/bonecos.h"
#include "../include/GerenciarTurnoJogador.h"
#include "../include/Menu.h"
#include "../include/gerenciar_dados.h"
#include "../include/gerenciar_xp.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <termios.h>
#include <sys/ioctl.h>

// a tela tem que ta em 162 por 52 px

void menu() {
    screenClear();
    screenSetColor( BLUE, BLACK);

    int posicao_X_ProjetoX_Menu = 60;
    int posicao_Y_ProjetoX_Menu = 15;

    ProjetoX(posicao_X_ProjetoX_Menu, posicao_Y_ProjetoX_Menu);

    int posicao_X_Start_Menu = 50;
    int posicao_Y_Start_Menu = 25;

    Start(posicao_X_Start_Menu, posicao_Y_Start_Menu);

    screenGotoxy(75, 32); printf("Aperte Enter");

    char ch;
    do {
        ch = readch();
    } while (ch != '\n' && ch != '\r');

    screenSetColor( WHITE, BLACK);
}

void pedirNomeJogador() {
    struct termios original_settings;
    struct termios new_settings;

    tcgetattr(STDIN_FILENO, &original_settings);

    new_settings = original_settings;

    new_settings.c_lflag |= (ECHO | ICANON);

    tcsetattr(STDIN_FILENO, TCSANOW, &new_settings);

    screenClear();
    screenSetColor(WHITE, BLACK);
    screenGotoxy(10, 10);

    printf("Digite o nome do seu heroi (sem espacos, max %d caracteres): ", MAX_NOME_LENGTH - 1);
    screenShowCursor();
    if (fgets(nome_Do_jogador, sizeof(nome_Do_jogador), stdin) != NULL) {

        nome_Do_jogador[strcspn(nome_Do_jogador, "\n")] = '\0';
    } else {

        strcpy(nome_Do_jogador, "Heroi");
        screenGotoxy(10, 12);
        screenSetColor(RED, BLACK);
        printf("Erro ao ler o nome. Usando nome padrao: Heroi\n");
        screenUpdate();

    }

    tcsetattr(STDIN_FILENO, TCSANOW, &original_settings);

    screenHideCursor();
}


int main() {
    screenInit(0);
    keyboardInit();
    srand(time(NULL));

    pedirNomeJogador(); // Sempre pede o nome no início
    carregarDadosPorNome(); // Carrega os dados com base no nome digitado

    menu(); // Exibe o menu

    screenClear(); // Limpa a tela antes da batalha

    jogadorboneco(posicao_X_jogador_boneco, posicao_Y_jogador_boneco);

    Inimigo inimigo1;
    inicializarInimigo(&inimigo1, 150, 10, 10); // HP, Ataque, Defesa iniciais do inimigo

    inimigoboneco(posicao_X_inimigo_boneco, posicao_Y_inimigo_boneco);

    // Inicia o gerenciamento do turno do jogador para a batalha
    int resultadoBatalha = gerenciarTurnoJogador(&inimigo1, &vida_Do_jogador, &vida_do_inimigo);

    screenGotoxy(1, MAXY);
    screenSetColor(WHITE, BLACK);
    if (resultadoBatalha == 1) {
        printf("Fim de jogo. Voce venceu a batalha.                                                                                  ");
    } else if (vida_Do_jogador <= 0) {
        printf("Fim de jogo. Voce foi derrotado.                                                                                       ");
    }
    else {
        printf("Batalha encerrada.                                                                                                 ");
    }

    screenGotoxy(1, MAXY + 1);
    printf("Pressione Enter para sair.                                                                                  ");

    while (keyhit()) {
        readch();
    }
    getchar();

    screenDestroy();
    keyboardDestroy();

    return 0;
}
