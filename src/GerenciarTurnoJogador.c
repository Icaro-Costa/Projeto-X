#include "../include/screen.h"
#include "../include/keyboard.h"
#include "../include/Batalha.h"
#include "../include/visual.h"
#include "../include/inimigo.h"
#include "../include/barradevida.h"
#include "../include/gerenciar_xp.h"

#include "../include/GerenciarTurnoInimigo.h"
#include "../include/GerenciarTurnoJogador.h"
#include "../include/game_config.h"
#include "../include/gerenciar_dados.h"

#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>


int gerenciarTurnoJogador(Inimigo *inimigo, int *vidaJogador, int *vidaInimigo) {
    int key = 0;
    bool jogadorAgir = true; // Controla se é o turno do jogador de agir
    int vidaMaximaJogador = 100; // Valor fixo para a vida máxima do jogador nesta implementação
    int defesaAtivaJogador = 0;  // Bônus de defesa do jogador obtido pela ação 'W' (Inicialmente 0)


    // Garante que os status ATUAIS do inimigo sejam resetados para os BASE no início da batalha.
    // Importante se a mesma struct Inimigo for reutilizada sem chamar inicializarInimigo novamente.
    inimigo->ataqueAtual = inimigo->ataqueBase;
    inimigo->defesaAtual = inimigo->defesaBase;
    inimigo->bonusDefesaAtiva = 0;
    // Nota: A raiva do inimigo (nivelRaiva, turnosDeRaivaRestantes) não é resetada aqui.
    // Isso permite que a raiva persista entre chamadas a esta função.

    while (*vidaJogador > 0 && *vidaInimigo > 0) { // Loop principal da batalha
        desenharBoxOpcoes(); // Desenha a UI de opções do jogador

        // Desenha as barras de vida
        desenharBarraDeVida(vida_X_Jogador, vida_Y_Jogador, *vidaJogador, vidaMaximaJogador);
        desenharBarraDeVida(vida_X_Inimigo, vida_Y_Inimigo, *vidaInimigo, inimigo->vidaMaxima);

        exibirStatusXP(); // Adicionado: Exibe o nível e XP do jogador

        screenUpdate(); // Atualiza a tela com as barras, a box de opções e o status de XP

        if (jogadorAgir) { // Turno do Jogador
            screenGotoxy(OPCOES_BOX_START_X, OPCOES_BOX_START_Y + OPCOES_BOX_HEIGHT + 1);
            screenSetColor(WHITE, BLACK);
            // O prompt da ação é desenhado pela desenharBoxOpcoes(), não precisa repetir aqui.
            screenUpdate();

            key = 0;
            while (key == 0) { // Espera por uma tecla de ação válida
                if (keyhit()) {
                    key = readch();
                    screenGotoxy(OPCOES_BOX_START_X, OPCOES_BOX_START_Y + OPCOES_BOX_HEIGHT + 1); // Prepara para exibir mensagem da ação
                    screenSetColor(WHITE, BLACK);

                    // Limpa a linha de status antes de imprimir a nova mensagem
                    printf("                                                                    ");
                    screenGotoxy(OPCOES_BOX_START_X, OPCOES_BOX_START_Y + OPCOES_BOX_HEIGHT + 1); // Retorna para a posição

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
                            inimigo->bonusDefesaAtiva = 0; // Inimigo não está mais defendendo ativamente.
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
                            inimigo->bonusDefesaAtiva = 0; // Inimigo não está mais defendendo ativamente.
                            jogadorAgir = false;
                            getchar();
                        }
                        break;

                        case 'x': case 'X': // Sair da batalha
                            screenGotoxy(OPCOES_BOX_START_X, OPCOES_BOX_START_Y + OPCOES_BOX_HEIGHT + 1);
                            screenSetColor(WHITE, BLACK);
                            printf("Saindo da batalha...                                       ");
                            screenUpdate();
                            usleep(500000); // Pequena pausa antes de efetivamente sair
                            return 0;       // Retorna 0 para indicar saída da batalha

                        default: // Tecla inválida
                            key = 0; // Permanece no loop de espera por input

                            // Posiciona na última linha da tela (MAXY) para a mensagem de erro
                            screenGotoxy(1, MAXY);
                            screenSetColor(RED, WHITE); // Sugestão: usar uma cor diferente para erro
                            printf("Tecla inválida! Use Q, W, E, R ou X.                       "); // A mensagem
                            screenUpdate();

                            usleep(1000000);
                            screenGotoxy(1, MAXY);
                            printf("                                                                    "); // Limpa a linha de erro

                            break;
                    }
                }
                usleep(50000); // Pequena pausa para não sobrecarregar CPU no loop de input
            }
        }

        // Verifica condição de fim de jogo após a ação do jogador
        if (*vidaInimigo <= 0) { // Verifica se o inimigo foi derrotado
            adicionarXP(25); // Chama a nova função para gerenciar o XP e o nível

            screenGotoxy(OPCOES_BOX_START_X, OPCOES_BOX_START_Y + OPCOES_BOX_HEIGHT + 3);
            screenSetColor(YELLOW, BLACK);
            printf("VOCÊ VENCEU A BATALHA!                                            ");
            screenUpdate();
            getchar(); // Pausa adicional se necessário

            break; // Sai do loop da batalha já que o inimigo foi derrotado
        }

        if (*vidaJogador <= 0) { // Verifica se o jogador foi derrotado
            // Lógica de derrota já existente ou a ser adicionada no Main
            break; // Sai do loop da batalha
        }


        if (!jogadorAgir) { // Turno do Inimigo
            executarTurnoInimigo(inimigo, vidaJogador, vidaMaximaJogador, &defesaAtivaJogador);
            jogadorAgir = true; // Devolve o turno para o jogador

            // Verifica condição de fim de jogo após a ação do inimigo
            if (*vidaInimigo <= 0 || *vidaJogador <= 0) break;
        }


    }
    // --- Fim da Batalha ---
    screenGotoxy(OPCOES_BOX_START_X, OPCOES_BOX_START_Y + OPCOES_BOX_HEIGHT + 3);
    screenSetColor(YELLOW, BLACK); // Destaca mensagem final
    if (*vidaJogador <= 0) {
        printf("VOCÊ FOI DERROTADO! Pressione Enter para continuar.                 ");
    } else if (*vidaInimigo <= 0) {
        // A mensagem de vitória e XP ganho já foi mostrada antes do break.
        // Limpa a linha ou mostra uma mensagem final simples.
        printf("Batalha encerrada. Pressione Enter para continuar.                 ");
    } else { // Caso de saída prematura por 'X'
        printf("Batalha interrompida. Pressione Enter para continuar.             ");
    }
    screenUpdate();

    while (keyhit()) {
        readch();
    }


    return (*vidaJogador > 0 && *vidaInimigo <= 0) ? 1 : 0; // Retorna 1 se jogador venceu, 0 caso contrário
}
