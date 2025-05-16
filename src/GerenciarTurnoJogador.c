#include "../include/screen.h"
#include "../include/keyboard.h"
#include "../include/Batalha.h"
#include "../include/visual.h"
#include "../include/inimigo.h"
#include "../include/barradevida.h"
#include "../include/gerenciar_xp.h"

#include "../include/GerenciarTurnoInimigo.h"
#include "../include/GerenciarTurnoJogador.h"
#include "../include/game_config.h" // Incluir para usar as variáveis globais, incluindo vida_Maxima_Do_jogador e os atributos base do jogador
#include "../include/gerenciar_dados.h" // Incluir se necessário (provavelmente não para esta função, mas incluído anteriormente)

#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>


int gerenciarTurnoJogador(Inimigo *inimigo, int *vidaJogador, int *vidaInimigo) {
    int key = 0;
    bool jogadorAgir = true; // Controla se é o turno do jogador de agir
    // int vidaMaximaJogador = 100; // REMOVIDO: Usamos a variável global vida_Maxima_Do_jogador agora
    int defesaAtivaJogador = 0;  // Bônus de defesa do jogador obtido pela ação 'W' (Inicialmente 0)


    // Garante que os status ATUAIS do inimigo sejam resetados para os BASE no início da batalha.
    // Importante se a mesma struct Inimigo for reutilizada sem chamar inicializarInimigo novamente.
    inimigo->ataqueAtual = inimigo->ataqueBase;
    inimigo->defesaAtual = inimigo->defesaBase;
    inimigo->bonusDefesaAtiva = 0;
    // Nota: A raiva do inimigo (nivelRaiva, turnosDeRaivaRestantes) não é resetada aqui.
    // Isso permite que a raiva persista entre chamadas a esta função.

    // Todas as mensagens de ação e status do jogador serão posicionadas nesta linha
    int linhaStatusJogador = OPCOES_BOX_START_Y + OPCOES_BOX_HEIGHT + 1;


    while (*vidaJogador > 0 && *vidaInimigo > 0) { // Loop principal da batalha
        desenharBoxOpcoes(); // Desenha a UI de opções do jogador

        // Desenha as barras de vida
        // Usa a variável global vida_Maxima_Do_jogador para a barra de vida do jogador
        desenharBarraDeVida(vida_X_Jogador, vida_Y_Jogador, *vidaJogador, vida_Maxima_Do_jogador);
        desenharBarraDeVida(vida_X_Inimigo, vida_Y_Inimigo, *vidaInimigo, inimigo->vidaMaxima);

        exibirStatusXP(); // Exibe o nível, XP e inimigos derrotados do jogador

        screenUpdate(); // Atualiza a tela com as barras, a box de opções e o status de XP

        if (jogadorAgir) { // Turno do Jogador
            screenGotoxy(linhaStatusJogador, linhaStatusJogador); // Posiciona para a mensagem de status/ação
            screenSetColor(WHITE, BLACK);
            // O prompt da ação é desenhado pela desenharBoxOpcoes(), não precisa repetir aqui.
            // Limpa a linha de status antes de exibir a nova mensagem
            printf("                                                                    ");
            screenGotoxy(linhaStatusJogador, linhaStatusJogador); // Retorna para a posição

            key = 0;
            while (key == 0) { // Espera por uma tecla de ação válida
                if (keyhit()) {
                    key = readch();

                    screenGotoxy(linhaStatusJogador, linhaStatusJogador); // Prepara para exibir mensagem da ação
                    screenSetColor(WHITE, BLACK);
                    // Limpa a linha de status antes de imprimir a nova mensagem
                    printf("                                                                    ");
                    screenGotoxy(linhaStatusJogador, linhaStatusJogador); // Retorna para a posição

                    switch (key) {
                        case 'q': case 'Q': // Ação: Ataque
                        {
                            int danoBrutoJogador = realizarAtaque(); // Dano bruto calculado em Batalha.c (Ataque Base + Dado)
                            // Redução de dano pelo inimigo: passiva (baseada na defesaAtual do inimigo) + ativa (bônus da ação de defesa do inimigo)
                            int reducaoPassivaInimigo = inimigo->defesaAtual / 5; // Exemplo de redução baseada na defesa atual do inimigo
                            int reducaoTotalPeloInimigo = reducaoPassivaInimigo + inimigo->bonusDefesaAtiva;
                            int danoLiquidoAoInimigo = danoBrutoJogador - reducaoTotalPeloInimigo;
                            if (danoLiquidoAoInimigo < 0) danoLiquidoAoInimigo = 0; // Dano não pode ser negativo

                            *vidaInimigo -= danoLiquidoAoInimigo; // Aplica o dano no inimigo
                            if (*vidaInimigo < 0) *vidaInimigo = 0; // Vida do inimigo não pode ser negativa

                            printf("Você atacou! Dano: %d (Inimigo Reduziu: %d). Vida Inimigo: %d/%d",
                                   danoLiquidoAoInimigo, reducaoTotalPeloInimigo, *vidaInimigo, inimigo->vidaMaxima);

                            inimigo->bonusDefesaAtiva = 0; // Bônus de defesa ativa do inimigo é consumido após o ataque.
                            defesaAtivaJogador = 0;    // Jogador não está mais ativamente defendendo após atacar.
                            jogadorAgir = false;       // Fim do turno do jogador
                            getchar();                 // Pausa para o jogador ler a mensagem
                        }
                        break;

                        case 'w': case 'W': // Ação: Defender
                        {
                            int rolagemDefesa = realizarDefesa(); // Retorna D20
                            defesaAtivaJogador = rolagemDefesa / 4; // Converte D20 em bônus de redução (ex: 0-5)
                            if (defesaAtivaJogador > 5) defesaAtivaJogador = 5; // Limita o bônus de defesa ativa do jogador

                            printf("Você se defende! Redução de dano: %d no próximo ataque.", defesaAtivaJogador);
                            // O bônus de defesa ativa do inimigo (inimigo->bonusDefesaAtiva) não é resetado aqui.
                            jogadorAgir = false; // Fim do turno do jogador
                            getchar(); // Pausa para o jogador ler
                        }
                        break;

                        case 'e': case 'E': // Ação: Curar
                        {
                            int curaRecebida = realizarCura(); // Quantidade de cura baseada no D20 e fórmula em Batalha.c
                            *vidaJogador += curaRecebida; // Aplica a cura na vida do jogador
                            if (*vidaJogador > vida_Maxima_Do_jogador) *vidaJogador = vida_Maxima_Do_jogador; // Não exceder vida máxima global

                            printf("Você se curou em %d! Vida atual: %d/%d                     ", curaRecebida, *vidaJogador, vida_Maxima_Do_jogador); // Usa vida máxima global na mensagem
                            defesaAtivaJogador = 0;    // Jogador não está mais ativamente defendendo.
                            inimigo->bonusDefesaAtiva = 0; // O bônus de defesa ativa do inimigo é consumido pela ação do jogador (mesmo que não seja ataque direto).
                            jogadorAgir = false; // Fim do turno do jogador
                            getchar(); // Pausa para o jogador ler
                        }
                        break;

                        case 'r': case 'R': // Ação: Irritar
                        {
                            int sucessoIrritar = realizarIrritar(); // Retorna D20 para avaliar o sucesso da provocação
                            if (sucessoIrritar > 10) { // Limiar de sucesso para irritar (exemplo: rolagem > 10)
                                int aumentoRaiva = (sucessoIrritar - 10) / 2; // Calcula aumento da raiva (ex: 0-5)
                                if (aumentoRaiva < 1) aumentoRaiva = 1;       // Garante aumento mínimo de 1 se sucesso
                                inimigo->nivelRaiva += aumentoRaiva; // Aumenta o nível de raiva do inimigo
                                if (inimigo->nivelRaiva > 20) inimigo->nivelRaiva = 20; // Limite máximo de raiva
                                inimigo->turnosDeRaivaRestantes = 3;                    // Define a duração dos efeitos da raiva (ex: 3 turnos)
                                printf("Você irritou o inimigo! Raiva dele: %d. Efeitos por %d turnos.", inimigo->nivelRaiva, inimigo->turnosDeRaivaRestantes);
                            } else {
                                printf("Sua provocação falhou em irritar o inimigo significativamente.");
                            }
                            defesaAtivaJogador = 0; // Jogador não está mais ativamente defendendo.
                            inimigo->bonusDefesaAtiva = 0; // O bônus de defesa ativa do inimigo é consumido.
                            jogadorAgir = false; // Fim do turno do jogador
                            getchar(); // Pausa para o jogador ler
                        }
                        break;

                        case 'x': case 'X': // Sair da batalha
                            screenGotoxy(linhaStatusJogador, linhaStatusJogador); // Posiciona para mensagem de saída
                            screenSetColor(WHITE, BLACK);
                            printf("Saindo da batalha...                                       ");
                            screenUpdate(); // Exibe a mensagem
                            usleep(500000); // Pequena pausa antes de efetivamente sair
                            // Note: Ao sair com 'X', o loop da batalha termina, e a lógica em Main.c
                            // detectará que o inimigo *não* morreu e tratará como interrupção.
                            return 0; // Retorna 0 para indicar que a batalha terminou sem vitória/derrota por vida
                            break; // break é redundante após return
                            // return 0; // Return para sair da batalha (Main.c verificará as vidas)

                        default: // Tecla inválida
                            key = 0; // Permanece no loop de espera por input válido

                            // Posiciona na última linha da tela (MAXY) para a mensagem de erro
                            screenGotoxy(1, MAXY); // Ajuste a posição se MAXY não for o local ideal para mensagens de erro
                            screenSetColor(RED, WHITE); // Cor para erro
                            printf("Tecla inválida! Use Q, W, E, R ou X.                       "); // A mensagem
                            screenUpdate(); // Exibe a mensagem de erro

                            usleep(1000000); // Pausa para o usuário ver o erro
                            screenGotoxy(1, MAXY);
                            printf("                                                                    "); // Limpa a linha de erro

                            break; // Volta para o loop de espera por input
                    }
                }
                usleep(50000); // Pequena pausa para não sobrecarregar CPU no loop de input
            }
        }

        // --- Verifica condição de fim de jogo após a ação do jogador ---
        if (*vidaInimigo <= 0) { // Verifica se o inimigo foi derrotado
            inimigos_Derrotados++; // Incrementa a contagem global de inimigos derrotados
            adicionarXP(50 + inimigo->vidaMaxima / 10); // Adiciona XP ao derrotar (mais XP para inimigos mais fortes)

            // A mensagem de vitória e XP ganho é exibida dentro de Main.c agora.
            // Esta função simplesmente retorna.
            // A lógica de pausa e prompt para continuar está em Main.c

            // Não precisamos de mensagem de vitória aqui, Main.c lida com isso.
            return 1; // Retorna 1 para indicar que o jogador venceu a batalha
        }

        if (*vidaJogador <= 0) { // Verifica se o jogador foi derrotado
            // A lógica de derrota e mensagem é tratada em Main.c
            // Esta função simplesmente retorna.
            return 0; // Retorna 0 para indicar que o jogador perdeu a batalha ou saiu
        }


        if (!jogadorAgir) { // Turno do Inimigo (se o jogador já agiu)
            // Executa a ação do inimigo. Passa o endereço da vida do jogador,
            // o valor da vida máxima global do jogador, e o endereço do bônus de defesa ativa do jogador.
            executarTurnoInimigo(inimigo, vidaJogador, vida_Maxima_Do_jogador, &defesaAtivaJogador); // Passa vida_Maxima_Do_jogador global

            jogadorAgir = true; // Devolve o turno para o jogador

            // Verifica condição de fim de jogo após a ação do inimigo
            if (*vidaInimigo <= 0 || *vidaJogador <= 0) {
                // A lógica de vitória/derrota e mensagens é tratada em Main.c
                // A função simplesmente retorna.
                return (*vidaJogador > 0 && *vidaInimigo <= 0) ? 1 : 0; // Retorna 1 se vitória, 0 se derrota
            }
        }
    }
    // O loop da batalha terminou (vida de jogador ou inimigo chegou a 0, ou jogador saiu com 'X')

    // Se o loop terminou e nem a vida do jogador nem do inimigo chegou a 0, significa que o jogador saiu com 'X'.
    // A lógica para isso está em Main.c, que verifica o estado das vidas após o retorno desta função.
    return (*vidaJogador > 0 && *vidaInimigo <= 0) ? 1 : 0; // Retorna 1 se jogador venceu, 0 caso contrário (derrota ou saída por 'X')
}
