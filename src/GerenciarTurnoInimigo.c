#include "../include/screen.h"
#include "../include/inimigo.h"
#include "../include/GerenciarTurnoInimigo.h"
#include "../include/Dado.h"
#include "../include/game_config.h"

#include <stdio.h>
#include <string.h>

void executarTurnoInimigo(Inimigo *inimigo, int *vidaJogador, int vidaMaximaJogador, int *defesaAplicadaPeloJogador) {
    char msgEfeitoRaiva[100] = ""; // Buffer para mensagens sobre o estado de raiva/calma do inimigo

    // --- Gerenciamento dos Efeitos da Raiva (antes da ação do inimigo) ---
    if (inimigo->turnosDeRaivaRestantes > 0) {
        inimigo->ataqueAtual = inimigo->ataqueBase + (inimigo->nivelRaiva / 3);
        inimigo->defesaAtual = inimigo->defesaBase - (inimigo->nivelRaiva / 2);
        if (inimigo->defesaAtual < 0) inimigo->defesaAtual = 0;

        inimigo->turnosDeRaivaRestantes--;

        if (inimigo->turnosDeRaivaRestantes == 0) {
            inimigo->ataqueAtual = inimigo->ataqueBase;
            inimigo->defesaAtual = inimigo->defesaBase;
            int raivaQueAcabou = inimigo->nivelRaiva;
            inimigo->nivelRaiva = 0;
            if (raivaQueAcabou > 0) {
                sprintf(msgEfeitoRaiva, "O inimigo se acalmou. ");
            }
        } else {
            sprintf(msgEfeitoRaiva, "(Raiva: %d! Atk:%d Def:%d [%d t]) ", inimigo->nivelRaiva, inimigo->ataqueAtual, inimigo->defesaAtual, inimigo->turnosDeRaivaRestantes);
        }
    } else {
        inimigo->ataqueAtual = inimigo->ataqueBase;
        inimigo->defesaAtual = inimigo->defesaBase;
        if (inimigo->nivelRaiva > 0) {
            inimigo->nivelRaiva = 0;
        }
    }

    int acaoInimigo = escolherAcaoInimigo(inimigo);
    int danoBrutoInimigo = 0;

    if (acaoInimigo != 1) { // Se o inimigo não defender, ele perde o bônus de defesa ativa anterior.
        inimigo->bonusDefesaAtiva = 0;
    }

    // Todas as mensagens de ação do inimigo serão posicionadas nesta linha
    // para sobrescrever a mensagem da acao do jogador.
    int linhaStatusInimigo = OPCOES_BOX_START_Y + OPCOES_BOX_HEIGHT + 1;


    switch (acaoInimigo) {
        case 0: // Ação do Inimigo: Ataque
        {
            int bonusDadoAtaqueInimigo = rolarDado20() / 4;
            danoBrutoInimigo = inimigo->ataqueAtual + bonusDadoAtaqueInimigo;

            int danoFinalRecebidoPeloJogador = danoBrutoInimigo - *defesaAplicadaPeloJogador;
            if (danoFinalRecebidoPeloJogador < 0) danoFinalRecebidoPeloJogador = 0;

            *vidaJogador -= danoFinalRecebidoPeloJogador;
            if (*vidaJogador < 0) *vidaJogador = 0;

            screenGotoxy(OPCOES_BOX_START_X, linhaStatusInimigo); // Posiciona na linha de status
            screenSetColor(LIGHTRED, BLACK);

            // Mensagem de Ataque
            printf("Inimigo ataca %s! Dano: %d (Dado: %d, Reduzido: %d). Sua Vida: %d/%d                          ", // Espacos para limpar
                   msgEfeitoRaiva,
                   danoFinalRecebidoPeloJogador, bonusDadoAtaqueInimigo, *defesaAplicadaPeloJogador, *vidaJogador, vidaMaximaJogador);

            *defesaAplicadaPeloJogador = 0;
            getchar();
        }
        break;

        case 1: // Ação do Inimigo: Defender
        {
            int rolagemDefesaInimigo = rolarDado20();
            inimigo->bonusDefesaAtiva = rolagemDefesaInimigo / 4;
            if (inimigo->bonusDefesaAtiva > 5) inimigo->bonusDefesaAtiva = 5;

            screenGotoxy(OPCOES_BOX_START_X, linhaStatusInimigo); // Posiciona na linha de status
            screenSetColor(LIGHTRED, BLACK);

            // Mensagem de Defesa
            printf("Inimigo se defende %s! Reduzira dano em %d.                                        ", // Espacos para limpar
                   msgEfeitoRaiva,
                   inimigo->bonusDefesaAtiva);
            getchar();
        }
        break;

        case 2: // Ação do Inimigo: Curar
        {
            int curaBaseInimigo = inimigo->vidaMaxima / 10;
            int bonusCuraDado = rolarDado20() / 2;
            int curaTotalInimigo = curaBaseInimigo + bonusCuraDado;

            inimigo->vida += curaTotalInimigo;
            if (inimigo->vida > inimigo->vidaMaxima) {
                inimigo->vida = inimigo->vidaMaxima;
            }
            screenGotoxy(OPCOES_BOX_START_X, linhaStatusInimigo); // Posiciona na linha de status
            screenSetColor(LIGHTRED, BLACK);

            // Mensagem de Cura
            printf("Inimigo %s se curou em %d! Vida Inimigo: %d/%d                                       ", // Espacos para limpar
                   msgEfeitoRaiva,
                   curaTotalInimigo, inimigo->vida, inimigo->vidaMaxima);
            getchar();
        }
        break;

        default: // Ação desconhecida ou inimigo "passa o turno"
            screenGotoxy(OPCOES_BOX_START_X, linhaStatusInimigo); // Posiciona na linha de status
            screenSetColor(LIGHTRED, BLACK);
            printf("Inimigo %s observa... (Acao: %d)                                                        ", // Espacos para limpar
                   msgEfeitoRaiva,
                   acaoInimigo);
            getchar();
            break;
    }

    // Exibe mensagem de "acalmar" numa linha separada se aplicável e se não foi já mostrada com a ação.
    if (strlen(msgEfeitoRaiva) > 0 && strstr(msgEfeitoRaiva, "acalmou") &&
        (acaoInimigo != 0 && acaoInimigo !=1 && acaoInimigo !=2 )) {
            screenGotoxy(OPCOES_BOX_START_X, OPCOES_BOX_START_Y + OPCOES_BOX_HEIGHT + 2); // Linha abaixo da de status
            screenSetColor(CYAN, BLACK);
            printf("%s                                                                    ", msgEfeitoRaiva); // Espacos para limpar
        }
}