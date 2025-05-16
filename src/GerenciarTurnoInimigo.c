#include "../include/screen.h"
#include "../include/inimigo.h"
#include "../include/GerenciarTurnoInimigo.h"
#include "../include/Dado.h"
#include "../include/game_config.h"

void executarTurnoInimigo(Inimigo *inimigo, int *vidaJogador, int vidaMaximaJogadorLocal, int *defesaAplicadaPeloJogador) {
    char msgEfeitoRaiva[100] = "";

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

    if (acaoInimigo != 1) {
        inimigo->bonusDefesaAtiva = 0;
    }

    int linhaStatusInimigo = OPCOES_BOX_START_Y + OPCOES_BOX_HEIGHT + 1;

    switch (acaoInimigo) {
        case 0: // Ataque
        {
            int bonusDadoAtaqueInimigo = rolarDado20() / 4;
            danoBrutoInimigo = inimigo->ataqueAtual + bonusDadoAtaqueInimigo;

            int reducaoTotalPeloJogador = defesa_Base_Do_Jogador + *defesaAplicadaPeloJogador;

            int danoFinalRecebidoPeloJogador = danoBrutoInimigo - reducaoTotalPeloJogador;
            if (danoFinalRecebidoPeloJogador < 0) danoFinalRecebidoPeloJogador = 0;

            *vidaJogador -= danoFinalRecebidoPeloJogador;
            if (*vidaJogador < 0) *vidaJogador = 0;

            screenGotoxy(OPCOES_BOX_START_X, linhaStatusInimigo);
            screenSetColor(LIGHTRED, BLACK);

            printf("Inimigo ataca %s! Dano Bruto: %d, Reduzido por voce: %d. Dano Final: %d. Sua Vida: %d/%d                                          ",
                   msgEfeitoRaiva,
                   danoBrutoInimigo, reducaoTotalPeloJogador, danoFinalRecebidoPeloJogador, *vidaJogador, vida_Maxima_Do_jogador);

            *defesaAplicadaPeloJogador = 0;
            getchar();
        }
        break;

        case 1: // Defender
        {
            int rolagemDefesaInimigo = rolarDado20();
            inimigo->bonusDefesaAtiva = rolagemDefesaInimigo / 4;
            if (inimigo->bonusDefesaAtiva > 5) inimigo->bonusDefesaAtiva = 5;

            screenGotoxy(OPCOES_BOX_START_X, linhaStatusInimigo);
            screenSetColor(LIGHTRED, BLACK);

            printf("Inimigo se defende %s! Reduzira dano em %d.                                                                    ",
                   msgEfeitoRaiva,
                   inimigo->bonusDefesaAtiva);
            getchar();
        }
        break;

        case 2: // Curar
        {
            int curaBaseInimigo = inimigo->vidaMaxima / 10;
            int bonusCuraDado = rolarDado20() / 2;
            int curaTotalInimigo = curaBaseInimigo + bonusCuraDado;

            inimigo->vida += curaTotalInimigo;
            if (inimigo->vida > inimigo->vidaMaxima) {
                inimigo->vida = inimigo->vidaMaxima;
            }
            screenGotoxy(OPCOES_BOX_START_X, linhaStatusInimigo);
            screenSetColor(LIGHTRED, BLACK);

            printf("Inimigo %s se curou em %d! Vida Inimigo: %d/%d                                                            ",
                   msgEfeitoRaiva,
                   curaTotalInimigo, inimigo->vida, inimigo->vidaMaxima);
            getchar();
        }
        break;

        default: // Ação desconhecida
            screenGotoxy(OPCOES_BOX_START_X, linhaStatusInimigo);
            screenSetColor(LIGHTRED, BLACK);
            printf("Inimigo %s observa... (Acao: %d)                                                                    ",
                   msgEfeitoRaiva,
                   acaoInimigo);
            getchar();
            break;
    }

    if (strlen(msgEfeitoRaiva) > 0 && strstr(msgEfeitoRaiva, "acalmou") != NULL &&
        (acaoInimigo != 0 && acaoInimigo != 1 && acaoInimigo != 2 )) {
        screenGotoxy(OPCOES_BOX_START_X, OPCOES_BOX_START_Y + OPCOES_BOX_HEIGHT + 2);
    screenSetColor(CYAN, BLACK);
    printf("%s                                                                     ", msgEfeitoRaiva);
        }
}
