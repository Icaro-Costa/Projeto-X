#include "../include/screen.h"
#include "../include/inimigo.h"
#include "../include/GerenciarTurnoInimigo.h"
#include "../include/Dado.h"
#include "../include/game_config.h"
#include "../include/bonecos.h"

#include <stdio.h>
#include <string.h>

void executarTurnoInimigo(Inimigo *inimigo, int *vidaJogador, int vidaMaximaJogadorLocal, int *defesaAplicadaPeloJogador) {
    char msgEfeitoRaiva[100] = "";

    // Lógica para gerenciar os efeitos da raiva do inimigo
    if (inimigo->turnosDeRaivaRestantes > 0) {
        inimigo->ataqueAtual = inimigo->ataqueBase + (inimigo->nivelRaiva / 3);
        inimigo->defesaAtual = inimigo->defesaBase - (inimigo->nivelRaiva / 2);
        if (inimigo->defesaAtual < 0) inimigo->defesaAtual = 0; // Garante que a defesa não fica negativa

        inimigo->turnosDeRaivaRestantes--;

        if (inimigo->turnosDeRaivaRestantes == 0) {
            // Reseta atributos para os base quando a raiva acaba
            inimigo->ataqueAtual = inimigo->ataqueBase;
            inimigo->defesaAtual = inimigo->defesaBase;
            int raivaQueAcabou = inimigo->nivelRaiva; // Guarda o nível de raiva antes de resetar para a mensagem
            inimigo->nivelRaiva = 0; // Reseta o nível de raiva
            if (raivaQueAcabou > 0) {
                sprintf(msgEfeitoRaiva, "O inimigo se acalmou. ");
            }
        } else {
            // Mensagem indicando os efeitos da raiva ativa
            sprintf(msgEfeitoRaiva, "(Raiva: %d! Atk:%d Def:%d [%d t]) ", inimigo->nivelRaiva, inimigo->ataqueAtual, inimigo->defesaAtual, inimigo->turnosDeRaivaRestantes);
        }
    } else {
        // Garante que os atributos estão nos valores base se não houver raiva ativa
        inimigo->ataqueAtual = inimigo->ataqueBase;
        inimigo->defesaAtual = inimigo->defesaBase;
        // Reseta o nível de raiva se os turnos acabaram e o nível ainda estava marcado
        if (inimigo->nivelRaiva > 0) {
            inimigo->nivelRaiva = 0;
        }
    }

    // Escolhe a ação do inimigo com base na lógica definida em inimigo.c
    int acaoInimigo = escolherAcaoInimigo(inimigo);

    // Reseta o bônus de defesa ativa do inimigo se ele não for defender neste turno
    if (acaoInimigo != 1) {
        inimigo->bonusDefesaAtiva = 0;
    }

    // Define a linha na tela onde as mensagens de status do inimigo serão exibidas
    int linhaStatusInimigo = OPCOES_BOX_START_Y + OPCOES_BOX_HEIGHT + 1;

    // --- Executa a ação escolhida pelo inimigo ---
    switch (acaoInimigo) {
        case 0: // Ataque
        {
            inimigo_bonecoAtk(posicao_X_inimigo_boneco, posicao_Y_inimigo_boneco);
            // Calcula o dano bruto do ataque do inimigo (Ataque Atual + bônus do dado)
            int bonusDadoAtaqueInimigo = rolarDado20() / 4;
            int danoBrutoInimigo = inimigo->ataqueAtual + bonusDadoAtaqueInimigo; // Declarado e usado localmente

            // Calcula a redução total de dano pelo jogador (Defesa Base + bônus de Defesa Ativa)
            int reducaoTotalPeloJogador = defesa_Base_Do_Jogador + *defesaAplicadaPeloJogador;

            // Calcula o dano final que o jogador recebe
            int danoFinalRecebidoPeloJogador = danoBrutoInimigo - reducaoTotalPeloJogador;
            if (danoFinalRecebidoPeloJogador < 0) danoFinalRecebidoPeloJogador = 0; // O dano final não pode ser negativo

            // Aplica o dano na vida do jogador
            *vidaJogador -= danoFinalRecebidoPeloJogador;
            if (*vidaJogador < 0) *vidaJogador = 0; // A vida do jogador não pode ser negativa

            screenGotoxy(OPCOES_BOX_START_X, linhaStatusInimigo);

         //   printf("Inimigo ataca %s! Dano Bruto: %d, Reduzido por voce: %d. Dano Final: %d. Sua Vida: %d/%d                                          ",
         //          msgEfeitoRaiva, // Inclui mensagem de efeito de raiva, se houver
          //         danoBrutoInimigo, reducaoTotalPeloJogador, danoFinalRecebidoPeloJogador, *vidaJogador, vida_Maxima_Do_jogador);

            *defesaAplicadaPeloJogador = 0; // O bônus de defesa ativa do jogador é consumido após o ataque inimigo.
            getchar(); // Pausa para o jogador ler a mensagem
            inimigoboneco(posicao_X_inimigo_boneco, posicao_Y_inimigo_boneco);
        }
        break;

        case 1: // Defender
        {
            inimigo_bonecoDef(posicao_X_inimigo_boneco, posicao_Y_inimigo_boneco);
            int rolagemDefesaInimigo = rolarDado20();
            inimigo->bonusDefesaAtiva = rolagemDefesaInimigo / 4;
            if (inimigo->bonusDefesaAtiva > 5) inimigo->bonusDefesaAtiva = 5;

            screenGotoxy(OPCOES_BOX_START_X, linhaStatusInimigo);

        //    printf("Inimigo se defende %s! Reduzira dano em %d no seu proximo ataque.                                                                    ",
        //           msgEfeitoRaiva,
        //           inimigo->bonusDefesaAtiva);
            getchar();
            inimigoboneco(posicao_X_inimigo_boneco, posicao_Y_inimigo_boneco);
        }
        break;

        case 2: // Curar
        {
            inimigoboneco(posicao_X_inimigo_boneco, posicao_Y_inimigo_boneco);
            int curaBaseInimigo = 0;

            if (inimigo->vidaMaxima > 0) {

                curaBaseInimigo = inimigo->vidaMaxima / 10;
            }
            int bonusCuraDado = rolarDado20() / 2;
            int curaTotalInimigo = curaBaseInimigo + bonusCuraDado;

            inimigo->vida += curaTotalInimigo;
            if (inimigo->vida > inimigo->vidaMaxima) {
                inimigo->vida = inimigo->vidaMaxima;
            }

            screenGotoxy(OPCOES_BOX_START_X, linhaStatusInimigo);

           // printf("Inimigo %s se curou em %d! Vida Inimigo: %d/%d                                                            ",
           //        msgEfeitoRaiva,
          //         curaTotalInimigo, inimigo->vida, inimigo->vidaMaxima);
            getchar();
        }
        break;

        default:
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

        screenUpdate();
}
