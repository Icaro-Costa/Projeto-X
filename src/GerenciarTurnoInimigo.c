#include "../include/screen.h"
#include "../include/keyboard.h"
#include "../include/timer.h"
#include "../include/Batalha.h"
#include "../include/visual.h"
#include "../include/inimigo.h"
#include "../include/barradevida.h"
#include "../include/GerenciarTurnoInimigo.h"
#include "../include/Dado.h"

#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define OPCOES_BOX_START_X 5
#define OPCOES_BOX_START_Y 16
#define OPCOES_BOX_WIDTH   70
#define OPCOES_BOX_HEIGHT  7

void executarTurnoInimigo(Inimigo *inimigo, int *vidaJogador, int vidaMaximaJogador, int *defesaAplicadaPeloJogador) {
    char msgEfeitoRaiva[100] = ""; // Buffer para mensagens sobre o estado de raiva/calma do inimigo

    // --- Gerenciamento dos Efeitos da Raiva (antes da ação do inimigo) ---
    if (inimigo->turnosDeRaivaRestantes > 0) {
        // Aplica modificadores de status devido à raiva
        inimigo->ataqueAtual = inimigo->ataqueBase + (inimigo->nivelRaiva / 3); // Raiva aumenta ataque
        inimigo->defesaAtual = inimigo->defesaBase - (inimigo->nivelRaiva / 2); // Raiva diminui defesa
        if (inimigo->defesaAtual < 0) inimigo->defesaAtual = 0; // Defesa não pode ser negativa

        inimigo->turnosDeRaivaRestantes--; // Decrementa a duração do efeito

        if (inimigo->turnosDeRaivaRestantes == 0) { // Efeitos da raiva acabaram neste turno
            inimigo->ataqueAtual = inimigo->ataqueBase; // Reverte para status base
            inimigo->defesaAtual = inimigo->defesaBase;
            int raivaQueAcabou = inimigo->nivelRaiva;
            inimigo->nivelRaiva = 0; // Zera o nível de raiva
            if (raivaQueAcabou > 0) { // Só mostra mensagem se realmente estava com raiva
                sprintf(msgEfeitoRaiva, "O inimigo se acalmou. ");
            }
        } else { // Raiva ainda ativa
            sprintf(msgEfeitoRaiva, "(Raiva: %d! Atk:%d Def:%d [%d t]) ", inimigo->nivelRaiva, inimigo->ataqueAtual, inimigo->defesaAtual, inimigo->turnosDeRaivaRestantes);
        }
    } else { // Sem turnos de raiva restantes, garante status base
        inimigo->ataqueAtual = inimigo->ataqueBase;
        inimigo->defesaAtual = inimigo->defesaBase;
        if (inimigo->nivelRaiva > 0) { // Se havia nível de raiva residual sem turnos
            inimigo->nivelRaiva = 0;   // Zera completamente
        }
    }

    int acaoInimigo = escolherAcaoInimigo(inimigo); //Decide a ação
    int danoBrutoInimigo = 0;

    // Se o inimigo não for se defender novamente, perde o bônus de defesa ativa que tinha.
    // Ações que não são "Defender" (código 1) fazem o inimigo perder esse bônus.
    if (acaoInimigo != 1) {
        inimigo->bonusDefesaAtiva = 0;
    }

    screenGotoxy(OPCOES_BOX_START_X, OPCOES_BOX_START_Y + OPCOES_BOX_HEIGHT + 2); // Linha de status do inimigo
    screenSetColor(LIGHTRED, BLACK);

    switch (acaoInimigo) {
        case 0: // Ação do Inimigo: Ataque
        {
            // Dano bruto do inimigo: ataqueAtual (modificado por raiva) + variabilidade do dado
            int bonusDadoAtaqueInimigo = rolarDado20() / 4; // D20/4 = bônus de 0-5
            danoBrutoInimigo = inimigo->ataqueAtual + bonusDadoAtaqueInimigo;

            // Aplica a defesa ativa do jogador (se houver)
            int danoFinalRecebidoPeloJogador = danoBrutoInimigo - *defesaAplicadaPeloJogador;
            if (danoFinalRecebidoPeloJogador < 0) danoFinalRecebidoPeloJogador = 0;

            *vidaJogador -= danoFinalRecebidoPeloJogador;
            if (*vidaJogador < 0) *vidaJogador = 0;

            printf("Inimigo ataca %s! Dano: %d (Dado: %d, Reduzido: %d). Sua Vida: %d/%d",
                   msgEfeitoRaiva, // Inclui estado de raiva na mensagem, se houver
                   danoFinalRecebidoPeloJogador, bonusDadoAtaqueInimigo, *defesaAplicadaPeloJogador, *vidaJogador, vidaMaximaJogador);

            *defesaAplicadaPeloJogador = 0; // Bônus de defesa do jogador é consumido após o ataque.
            getchar();
        }
        break;

        case 1: // Ação do Inimigo: Defender
        {
            int rolagemDefesaInimigo = rolarDado20();
            inimigo->bonusDefesaAtiva = rolagemDefesaInimigo / 4; // Bônus de 0-5
            if (inimigo->bonusDefesaAtiva > 5) inimigo->bonusDefesaAtiva = 5; // Limita o bônus

            printf("Inimigo se defende %s! Reduzirá dano em %d.",
                   msgEfeitoRaiva, // Inclui estado de raiva
                   inimigo->bonusDefesaAtiva);
            // A defesa ativa do jogador (*defesaAplicadaPeloJogador) não é consumida,
            // pois o inimigo não atacou. Ela persiste para uma próxima oportunidade.
            getchar();
        }
        break;

        case 2: // Ação do Inimigo: Curar
        {
            int curaBaseInimigo = inimigo->vidaMaxima / 10; // Cura base (ex: 10% da vida máxima)
            int bonusCuraDado = rolarDado20() / 2;        // Bônus de cura do dado (0-10)
            int curaTotalInimigo = curaBaseInimigo + bonusCuraDado;

            inimigo->vida += curaTotalInimigo;
            if (inimigo->vida > inimigo->vidaMaxima) { // Não exceder vida máxima
                inimigo->vida = inimigo->vidaMaxima;
            }
            printf("Inimigo %s se curou em %d! Vida Inimigo: %d/%d",
                   msgEfeitoRaiva, // Inclui estado de raiva
                   curaTotalInimigo, inimigo->vida, inimigo->vidaMaxima);
            // Defesa ativa do jogador não é consumida.
            getchar();
        }
        break;

        default: // Ação desconhecida ou inimigo "passa o turno"
            printf("Inimigo %s observa... (Ação: %d)                           ",
                   msgEfeitoRaiva, // Inclui estado de raiva
                   acaoInimigo);
            getchar();
            break;
    }
    // Exibe mensagem de "acalmar" numa linha separada se aplicável e se não foi já mostrada com a ação.
    // Condição: Havia mensagem de calma, e a ação do inimigo não foi uma que já incorporaria essa mensagem.
    if (strlen(msgEfeitoRaiva) > 0 && strstr(msgEfeitoRaiva, "acalmou") &&
        (acaoInimigo != 0 && acaoInimigo !=1 && acaoInimigo !=2 )) { // Evita mostrar se já foi parte da msg de atk/def/cura
            screenGotoxy(OPCOES_BOX_START_X, OPCOES_BOX_START_Y + OPCOES_BOX_HEIGHT + 3);
            screenSetColor(CYAN, BLACK);
            printf("%s", msgEfeitoRaiva); // Mostra a mensagem "O inimigo se acalmou."
        }
}
