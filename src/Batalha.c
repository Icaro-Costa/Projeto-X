#include "../include/Batalha.h"
#include "../include/screen.h"
#include "../include/Dado.h"
#include "../include/game_config.h"

#include <stdio.h>
#include <unistd.h>

int realizarDefesa() {
    int resultadoDado = rolarDado20();

    screenGotoxy(OPCOES_BOX_START_X, OPCOES_BOX_START_Y + OPCOES_BOX_HEIGHT + 1);
    screenSetColor(WHITE, BLACK);

    screenUpdate();
    usleep(50000);

    // Retorna a rolagem do dado. A lógica de aplicar a defesa (redução de dano)
    // e o bônus de defesa ativa é feita em GerenciarTurnoJogador.c
    return resultadoDado;
}

int realizarAtaque() {

    int danoBruto = 0; // Variável para armazenar o dano antes das defesas
    int dado = rolarDado20(); // Rola o dado para o ataque

    screenGotoxy(OPCOES_BOX_START_X, OPCOES_BOX_START_Y + OPCOES_BOX_HEIGHT + 1);
    screenSetColor(WHITE, BLACK);

    // --- Calcular o dano bruto do ataque usando o ataque base do jogador e o dado ---
    // O dano bruto será o ataque base do jogador mais o resultado da rolagem do dado.
    danoBruto = ataque_Base_Do_Jogador + dado;
    // -----------------------------------------------------------------------------

    printf("Você atacou! Dano Bruto: %d (Ataque Base: %d + Dado: %d).", danoBruto, ataque_Base_Do_Jogador, dado); // Mensagem atualizada
    screenUpdate();
    usleep(50000); // Pequena pausa
    return danoBruto; // Retorna o dano bruto calculado (antes das defesas do inimigo)
}


int realizarCura() {
    int quantidadeCura = 0; // Variável para armazenar a quantidade de cura
    int dado = rolarDado20(); // Rola o dado para determinar a efetividade da cura

    screenGotoxy(OPCOES_BOX_START_X, OPCOES_BOX_START_Y + OPCOES_BOX_HEIGHT + 1);
    screenSetColor(WHITE, BLACK);

    // --- Calcular a quantidade de cura ---
    // Ex: Cura base + um bônus baseado no dado
    quantidadeCura = 10 + (dado / 2); // Cura base de 10 mais metade do dado
    // Você pode ajustar a fórmula de cura aqui

    printf("Você se curou em %d! (Dado: %d)          ", quantidadeCura, dado); // Mensagem atualizada
    screenUpdate();
    usleep(50000);
    return quantidadeCura; // Retorna a quantidade de cura aplicada
}

//--------------------------------------------------

int realizarIrritar() {
    int resultadoDado = rolarDado20();

    screenGotoxy(OPCOES_BOX_START_X, OPCOES_BOX_START_Y + OPCOES_BOX_HEIGHT + 1);
    screenSetColor(WHITE, BLACK);
    printf("Você tenta provocar o inimigo... [%d]", resultadoDado);
    screenUpdate();
    usleep(50000);

    return resultadoDado; // Retorna o resultado do dado para a lógica em GerenciarTurnoJogador/Inimigo
}
