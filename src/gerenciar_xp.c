#include "../include/gerenciar_xp.h"
#include "../include/game_config.h" // Para acessar as variáveis globais de XP e Nível
#include "../include/screen.h"     // Para exibir mensagens na tela
#include "../include/keyboard.h"    // Para o getchar()

#include <stdio.h>

void adicionarXP(int quantidadeXP) {
    xp_Do_jogador += quantidadeXP;
    inimigos_Derrotados++; // Assumindo que adicionarXP é chamado ao derrotar um inimigo

    // Posição para mensagens de level up ou XP ganho
    int mensagem_y = OPCOES_BOX_START_Y + OPCOES_BOX_HEIGHT + 4;

    screenGotoxy(OPCOES_BOX_START_X, mensagem_y);
    screenSetColor(YELLOW, BLACK);
    printf("Ganhou %d XP! Total: %d XP.                                          ", quantidadeXP, xp_Do_jogador);
    screenUpdate();
    getchar(); // Pausa para o jogador ler a mensagem

    // Limpa a mensagem de ganho de XP
    screenGotoxy(OPCOES_BOX_START_X, mensagem_y);
    printf("                                                                    ");


    // Verifica se subiu de nível
    if (xp_Do_jogador >= 100) {
        nivel_Do_jogador++;
        xp_Do_jogador -= 100; // Ou xp_Do_jogador %= 100; para manter o restante

        screenGotoxy(OPCOES_BOX_START_X, mensagem_y);
        screenSetColor(CYAN, BLACK);
        printf("VOCÊ SUBIU PARA O NÍVEL %d!                                          ", nivel_Do_jogador);
        screenUpdate();
        getchar(); // Pausa para o jogador ler a mensagem de level up

        // Limpa a mensagem de level up
        screenGotoxy(OPCOES_BOX_START_X, mensagem_y);
        printf("                                                                  ");
    }
}

void exibirStatusXP() {
    // Posição para exibir Nível e XP. Ajuste conforme o layout da sua tela.
    // Usaremos uma posição fixa abaixo da barra de vida do jogador como exemplo.
    int status_x = vida_X_Jogador;
    int status_y = vida_Y_Jogador + 1;

    screenGotoxy(status_x, status_y);
    screenSetColor(WHITE, BLACK);
    printf("Nível: %d  XP: %d      ", nivel_Do_jogador, xp_Do_jogador); // Espaços para limpar
    screenUpdate();
}
