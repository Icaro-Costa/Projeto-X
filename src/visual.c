#include "../include/screen.h"
#include <stdio.h>

#include "../include/game_config.h"

// Função para desenhar a box
void desenharBoxOpcoes() {
    // --- Desenhar a Borda da Box ---
    screenSetColor(LIGHTGRAY, BLACK); // Cor da borda
    screenBoxEnable(); // Habilita o desenho de caracteres de caixa

    // Desenha os cantos
    screenGotoxy(OPCOES_BOX_START_X, OPCOES_BOX_START_Y);
    printf("%c", BOX_UPLEFT);
    screenGotoxy(OPCOES_BOX_START_X + OPCOES_BOX_WIDTH - 1, OPCOES_BOX_START_Y);
    printf("%c", BOX_UPRIGHT);
    screenGotoxy(OPCOES_BOX_START_X, OPCOES_BOX_START_Y + OPCOES_BOX_HEIGHT - 1);
    printf("%c", BOX_DWNLEFT);
    screenGotoxy(OPCOES_BOX_START_X + OPCOES_BOX_WIDTH - 1, OPCOES_BOX_START_Y + OPCOES_BOX_HEIGHT - 1);
    printf("%c", BOX_DWNRIGHT);

    // Desenha as linhas horizontais (entre os cantos)
    for (int x = 1; x < OPCOES_BOX_WIDTH - 1; x++) {
        screenGotoxy(OPCOES_BOX_START_X + x, OPCOES_BOX_START_Y);
        printf("%c", BOX_HLINE);
        screenGotoxy(OPCOES_BOX_START_X + x, OPCOES_BOX_START_Y + OPCOES_BOX_HEIGHT - 1);
        printf("%c", BOX_HLINE);
    }

    // Desenha as linhas verticais (entre os cantos)
    for (int y = 1; y < OPCOES_BOX_HEIGHT - 1; y++) {
        screenGotoxy(OPCOES_BOX_START_X, OPCOES_BOX_START_Y + y);
        printf("%c", BOX_VLINE);
        screenGotoxy(OPCOES_BOX_START_X + OPCOES_BOX_WIDTH - 1, OPCOES_BOX_START_Y + y);
        printf("%c", BOX_VLINE);
    }

    screenBoxDisable(); // Desabilita o desenho de caracteres de caixa
    screenSetColor(WHITE, BLACK); // Reseta a cor para desenhar o interior

    // --- Preencher a Área Interna da Box ---
    screenSetColor(WHITE, BLACK); // Texto branco, fundo preto para a área interna
    for (int y = 0; y < OPCAO_INNER_HEIGHT; y++) {
        screenGotoxy(OPCAO_INNER_START_X, OPCAO_INNER_START_Y + y);
        for (int x = 0; x < OPCAO_INNER_WIDTH; x++) {
            printf(" "); // Imprime espaços com fundo preto para limpar/preencher
        }
    }

    // --- Posicionar e Escrever o Texto das Opções ---
    screenSetColor(RED, WHITE);

    screenGotoxy(OPCAO_INNER_START_X + OPCAO1_INNER_REL_X, OPCAO_INNER_START_Y + OPCAO1_INNER_REL_Y);
    printf("Aperte Q para atk");

    screenGotoxy(OPCAO_INNER_START_X + OPCAO2_INNER_REL_X, OPCAO_INNER_START_Y + OPCAO2_INNER_REL_Y);
    printf("Aperte W para Defender");

    screenGotoxy(OPCAO_INNER_START_X + OPCAO3_INNER_REL_X, OPCAO_INNER_START_Y + OPCAO3_INNER_REL_Y);
    printf("Aperte E para Curar");

    screenGotoxy(OPCAO_INNER_START_X + OPCAO4_INNER_REL_X, OPCAO_INNER_START_Y + OPCAO4_INNER_REL_Y);
    printf("Aperte R para Irritar");

    screenSetColor(WHITE, BLACK);

}
