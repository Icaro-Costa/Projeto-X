#include "../include/gerenciar_xp.h"
#include "../include/game_config.h"
#include "../include/screen.h"
#include "../include/keyboard.h"

#include <stdio.h>

void adicionarXP(int quantidadeXP) {
    xp_Do_jogador += quantidadeXP;
    // inimigos_Derrotados++; // Esta contagem é incrementada em GerenciarTurnoJogador ao derrotar um inimigo


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
    // Calcula o potencial novo nível com base no XP total
    int novoNivelCalculado = (xp_Do_jogador / 100) + 1;

    // Loop para garantir múltiplos níveis se ganhou muito XP de uma vez
    while (novoNivelCalculado > nivel_Do_jogador) {
        nivel_Do_jogador++; // Incrementa o nível

        // --- AUMENTO DE ATRIBUTOS AO SUBIR DE NÍVEL ---
        vida_Maxima_Do_jogador += 15; // Aumenta vida máxima em 15 por nível
        vida_Do_jogador = vida_Maxima_Do_jogador; // Restaura a vida cheia do jogador
        ataque_Base_Do_Jogador += 2; // Aumenta ataque base em 2 por nível
        defesa_Base_Do_Jogador += 1; //Aumenta defesa base em 1 por nível

        screenGotoxy(OPCOES_BOX_START_X, mensagem_y); // Usa a mesma linha para a mensagem de level up
        screenSetColor(CYAN, BLACK);
        printf("VOCÊ SUBIU PARA O NÍVEL %d! Atributos melhorados!                     ", nivel_Do_jogador); // Mensagem atualizada
        screenUpdate();
        getchar(); // Pausa para o jogador ler a mensagem de level up

        // Limpa a mensagem de level up
        screenGotoxy(OPCOES_BOX_START_X, mensagem_y);
        printf("                                                                  ");

        // Recalcula o potencial novo nível caso tenha XP suficiente para subir mais de um nível de uma vez
        novoNivelCalculado = (xp_Do_jogador / 100) + 1;
    }
}

void exibirStatusXP() {

    int status_x = vida_X_Jogador;
    int status_y = vida_Y_Jogador + 1;

    screenGotoxy(status_x, status_y);
    screenSetColor(WHITE, BLACK);
    // Exibe Nível, XP total e Inimigos Derrotados para feedback completo
    printf("Nivel: %d  XP: %d  Inimigos: %d      ", nivel_Do_jogador, xp_Do_jogador, inimigos_Derrotados);
    screenUpdate();
}
