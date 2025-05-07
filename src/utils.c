#include "../include/screen.h"
#include "../include/keyboard.h"
#include "../include/timer.h"
#include "../include/Batalha.h"
#include "../include/visual.h"
#include "../include/inimigo.h"
#include "../include/barradevida.h"
#include "../include/utils.h"
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

int gerenciarTurnoJogador(Inimigo *inimigo, int *vidaJogador, int *vidaInimigo) {
    int key = 0;
    bool jogadorAgir = true; // Controla se é o turno do jogador de agir
    int vidaMaximaJogador = 100; // Valor fixo para a vida máxima do jogador nesta implementação
    int defesaAtivaJogador = 4;  // Bônus de defesa do jogador obtido pela ação 'W'

    // Garante que os status ATUAIS do inimigo sejam resetados para os BASE no início da batalha.
    // Importante se a mesma struct Inimigo for reutilizada sem chamar inicializarInimigo novamente.
    inimigo->ataqueAtual = inimigo->ataqueBase;
    inimigo->defesaAtual = inimigo->defesaBase;
    inimigo->bonusDefesaAtiva = 0;
    // Nota: A raiva do inimigo (nivelRaiva, turnosDeRaivaRestantes) não é resetada aqui.
    // Isso permite que a raiva persista entre chamadas a esta função, se desejado,
    // ou deve ser resetada explicitamente antes de iniciar uma nova batalha.

    while (*vidaJogador > 0 && *vidaInimigo > 0) { // Loop principal da batalha
        desenharBoxOpcoes(); // Desenha a UI de opções do jogador

        // Limpa as linhas de status da tela antes de exibir novas mensagens
        screenGotoxy(OPCOES_BOX_START_X, OPCOES_BOX_START_Y + OPCOES_BOX_HEIGHT + 1); // Linha de status do jogador
        printf("                                                                 ");
        screenGotoxy(OPCOES_BOX_START_X, OPCOES_BOX_START_Y + OPCOES_BOX_HEIGHT + 2); // Linha de status do inimigo
        printf("                                                                 ");
        screenGotoxy(OPCOES_BOX_START_X, OPCOES_BOX_START_Y + OPCOES_BOX_HEIGHT + 3); // Linha extra para mensagens
        printf("                                                                 ");

        desenharBarraDeVida(2, 14, *vidaJogador, vidaMaximaJogador);
        desenharBarraDeVida(51, 14, *vidaInimigo, inimigo->vidaMaxima);
        screenUpdate(); // Atualiza a tela com as barras e a box de opções

        if (jogadorAgir) { // Turno do Jogador
            screenGotoxy(OPCOES_BOX_START_X, OPCOES_BOX_START_Y + OPCOES_BOX_HEIGHT + 1);
            screenSetColor(WHITE, BLACK);
            printf("Sua vez! (Q:Atk, W:Def, E:Cura, R:Irrita, X:Sair): ");
            screenUpdate();

            key = 0;
            while (key == 0) { // Espera por uma tecla de ação válida
                if (keyhit()) {
                    key = readch();
                    screenGotoxy(OPCOES_BOX_START_X, OPCOES_BOX_START_Y + OPCOES_BOX_HEIGHT + 1); // Prepara para exibir mensagem da ação
                    screenSetColor(WHITE, BLACK);

                    switch (key) {
                        case 'q': case 'Q': // Ação: Ataque
                        {
                            int danoBrutoJogador = realizarAtaque(); // Dano baseado no D20 retornado pela função
                            // Redução de dano pelo inimigo: passiva (baseada na defesaAtual) + ativa (bônus da ação de defesa)
                            int reducaoPassivaInimigo = inimigo->defesaAtual / 5; // defesaAtual é afetada pela raiva
                            int reducaoTotalPeloInimigo = reducaoPassivaInimigo + inimigo->bonusDefesaAtiva;
                            int danoLiquidoAoInimigo = danoBrutoJogador - reducaoTotalPeloInimigo;
                            if (danoLiquidoAoInimigo < 0) danoLiquidoAoInimigo = 0; // Dano não pode ser negativo

                            *vidaInimigo -= danoLiquidoAoInimigo;
                            if (*vidaInimigo < 0) *vidaInimigo = 0; // Vida não pode ser negativa

                            printf("Você atacou! Dano: %d (Inimigo Reduziu: %d). Vida Inimigo: %d/%d",
                                   danoLiquidoAoInimigo, reducaoTotalPeloInimigo, *vidaInimigo, inimigo->vidaMaxima);

                            inimigo->bonusDefesaAtiva = 0; // Bônus de defesa ativa do inimigo é consumido.
                            defesaAtivaJogador = 0;    // Jogador não está mais ativamente defendendo após atacar.
                            jogadorAgir = false;       // Fim do turno do jogador
                            getchar();                 // Pausa para o jogador ler a mensagem
                        }
                        break;

                        case 'w': case 'W': // Ação: Defender
                        {
                            int rolagemDefesa = realizarDefesa(); // Retorna D20
                            defesaAtivaJogador = rolagemDefesa / 4; // Converte D20 em bônus de redução (0-5)
                            if (defesaAtivaJogador > 5) defesaAtivaJogador = 5; // Limita o bônus

                            printf("Você se defende! Redução de dano: %d no próximo ataque.", defesaAtivaJogador);
                            // O bônus de defesa do inimigo (inimigo->bonusDefesaAtiva) não é resetado aqui,
                            // pois o jogador não atacou para consumi-lo. Ele persiste se o inimigo estava defendendo.
                            jogadorAgir = false;
                            getchar();
                        }
                        break;

                        case 'e': case 'E': // Ação: Curar
                        {
                            int curaRecebida = realizarCura(); // Quantidade de cura baseada no D20
                            *vidaJogador += curaRecebida;
                            if (*vidaJogador > vidaMaximaJogador) *vidaJogador = vidaMaximaJogador; // Não exceder vida máxima

                            printf("Você se curou em %d! Vida atual: %d/%d                     ", curaRecebida, *vidaJogador, vidaMaximaJogador);
                            defesaAtivaJogador = 0;    // Jogador não está mais ativamente defendendo.
                            inimigo->bonusDefesaAtiva = 0;
                            jogadorAgir = false;
                            getchar();
                        }
                        break;

                        case 'r': case 'R': // Ação: Irritar
                        {
                            int sucessoIrritar = realizarIrritar(); // Retorna D20 para avaliar o sucesso
                            if (sucessoIrritar > 10) { // Limiar para provocação bem-sucedida
                                int aumentoRaiva = (sucessoIrritar - 10) / 2; // Calcula aumento da raiva (0-5)
                                if (aumentoRaiva < 1) aumentoRaiva = 1;       // Garante aumento mínimo de 1 se sucesso
                                inimigo->nivelRaiva += aumentoRaiva;
                                if (inimigo->nivelRaiva > 20) inimigo->nivelRaiva = 20; // Limite máximo de raiva
                                inimigo->turnosDeRaivaRestantes = 3;                    // Duração dos efeitos da raiva
                                printf("Você irritou o inimigo! Raiva dele: %d. Efeitos por %d turnos.", inimigo->nivelRaiva, inimigo->turnosDeRaivaRestantes);
                            } else {
                                printf("Sua provocação falhou em irritar o inimigo significativamente.");
                            }
                            defesaAtivaJogador = 0;
                            inimigo->bonusDefesaAtiva = 0;
                            jogadorAgir = false;
                            getchar();
                        }
                        break;

                        case 'x': case 'X': // Sair da batalha
                            printf("Saindo da batalha...                                       ");
                            screenUpdate(); getchar(); // Pausa para ver mensagem antes do usleep
                            usleep(500000); // Pequena pausa antes de efetivamente sair
                            return 0;       // Retorna 0 para indicar saída da batalha

                        default: // Tecla inválida
                            key = 0; // Permanece no loop de espera por input
                            printf("Tecla inválida! Use Q, W, E, R ou X.                       ");
                            screenUpdate(); getchar(); // Pausa para ler mensagem de erro
                            // Re-exibe o prompt de ação
                            screenGotoxy(OPCOES_BOX_START_X, OPCOES_BOX_START_Y + OPCOES_BOX_HEIGHT + 1);
                            screenSetColor(WHITE, BLACK);
                            printf("Sua vez! (Q:Atk, W:Def, E:Cura, R:Irrita, X:Sair): ");
                            screenUpdate();
                            break;
                    }
                }
                usleep(50000); // Pequena pausa para não sobrecarregar CPU no loop de input
            }
        }

        if (*vidaInimigo <= 0 || *vidaJogador <= 0) break; // Verifica condição de fim de jogo

        if (!jogadorAgir) { // Turno do Inimigo
            executarTurnoInimigo(inimigo, vidaJogador, vidaMaximaJogador, &defesaAtivaJogador);
            jogadorAgir = true; // Devolve o turno para o jogador
        }

        if (*vidaInimigo <= 0 || *vidaJogador <= 0) break; // Verifica condição de fim de jogo
    }
    // --- Fim da Batalha ---
    screenGotoxy(OPCOES_BOX_START_X, OPCOES_BOX_START_Y + OPCOES_BOX_HEIGHT + 3);
    screenSetColor(YELLOW, BLACK); // Destaca mensagem final
    if (*vidaJogador <= 0) {
        printf("VOCÊ FOI DERROTADO! Pressione Enter para sair.                 ");
    } else if (*vidaInimigo <= 0) {
        printf("VOCÊ VENCEU A BATALHA! Pressione Enter para sair.              ");
    } else { // Caso de saída prematura por 'X
        printf("Batalha interrompida. Pressione Enter para sair.             ");
    }
    screenUpdate();

    // Limpa o buffer de entrada para evitar que inputs residuais afetem o próximo estado do jogo
    char tempChar;
    do {
        tempChar = readch();
    } while (tempChar != '\n' && tempChar != EOF);

    return (*vidaJogador > 0 && *vidaInimigo <= 0) ? 1 : 0; // Retorna 1 se jogador venceu, 0 caso contrário
}


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
