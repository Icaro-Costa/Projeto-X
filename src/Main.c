#include "../include/screen.h"
#include "../include/keyboard.h"
#include "../include/inimigo.h"
#include "../include/game_config.h"
#include "../include/barradevida.h"
#include "../include/bonecos.h"
#include "../include/GerenciarTurnoJogador.h"
#include "../include/Menu.h"
#include "../include/gerenciar_dados.h"
#include "../include/gerenciar_xp.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <stdbool.h> // Para usar o tipo bool
#include <math.h> // Para usar a função pow para o scaling do inimigo

// A tela ideal para este jogo com base nas definições de game_config é de pelo menos 190 colunas por 50 linhas.
// Verifique as constantes MAXX e MAXY em screen.h.

void menu() {
    screenClear(); // Limpa toda a tela
    screenSetColor( BLUE, BLACK); // Define a cor para o menu

    // Posições para os logos do menu
    int posicao_X_ProjetoX_Menu = 60;
    int posicao_Y_ProjetoX_Menu = 15;
    ProjetoX(posicao_X_ProjetoX_Menu, posicao_Y_ProjetoX_Menu); // Desenha o logo "Projeto-X"

    int posicao_X_Start_Menu = 50;
    int posicao_Y_Start_Menu = 25;
    Start(posicao_X_Start_Menu, posicao_Y_Start_Menu); // Desenha o logo "Start"

    // Mensagem para iniciar o jogo
    screenGotoxy(75, 32); printf("Aperte Enter");

    screenUpdate(); // Garante que todo o conteúdo desenhado seja exibido no terminal imediatamente

    char ch;
    // Limpa o buffer de entrada de forma segura usando keyhit().
    // Isso remove quaisquer teclas pressionadas antes do menu aparecer,
    // evitando que um Enter antigo inicie o jogo automaticamente.
    while (keyhit()) {
        readch(); // Lê e descarta o caractere do buffer
    }

    // Espera que o usuário pressione Enter (\n ou \r) para sair do menu
    do {
        ch = readch();
    } while (ch != '\n' && ch != '\r');

    screenSetColor( WHITE, BLACK); // Restaura as cores padrão
}

void pedirNomeJogador() {
    struct termios original_settings; // Estrutura para armazenar configurações originais do terminal
    struct termios new_settings;     // Estrutura para novas configurações

    // Obtém as configurações atuais do terminal (stdin = 0)
    tcgetattr(STDIN_FILENO, &original_settings);

    // Copia as configurações atuais para modificar
    new_settings = original_settings;

    // Habilita o eco de caracteres digitados (ECHO) e o modo canônico (ICANON).
    // No modo canônico, a entrada é bufferizada linha por linha, e a leitura só
    // ocorre após o Enter, permitindo editar a linha digitada. Isso é bom para ler strings.
    new_settings.c_lflag |= (ECHO | ICANON);

    // Aplica as novas configurações imediatamente (TCSANOW)
    tcsetattr(STDIN_FILENO, TCSANOW, &new_settings);

    screenClear(); // Limpa a tela
    screenSetColor(WHITE, BLACK); // Define cor para o texto
    screenGotoxy(10, 10); // Posiciona o cursor

    // Mensagem para pedir o nome. Avisa sobre não usar espaços devido à forma de salvamento.
    printf("Digite o nome do seu heroi (sem espacos, max %d caracteres): ", MAX_NOME_LENGTH - 1);
    screenShowCursor(); // Mostra o cursor para o usuário digitar

    // Usa scanf para ler uma única palavra (sem espaços) para o nome.
    // Isso é compatível com o formato de salvamento atual que usa %s.
    char input_nome[MAX_NOME_LENGTH];
    if (scanf("%s", input_nome) == 1) {
        // Copia o nome lido para a variável global, garantindo que não exceda o tamanho
        strncpy(nome_Do_jogador, input_nome, MAX_NOME_LENGTH - 1);
        nome_Do_jogador[MAX_NOME_LENGTH - 1] = '\0'; // Garante terminação nula na string
    } else {
        // Em caso de erro na leitura, define um nome padrão
        strcpy(nome_Do_jogador, "Heroi");
        screenGotoxy(10, 12);
        screenSetColor(RED, BLACK);
        printf("Erro ao ler o nome ou nome invalido. Usando nome padrao: Heroi\n");
        screenUpdate();
    }

    // Limpa o restante da linha de entrada (o que ficou no buffer após o scanf)
    // Isso é importante para não afetar a próxima leitura de input no menu ou jogo.
    int c;
    while ((c = getchar()) != '\n' && c != EOF);


    // Restaura as configurações originais do terminal (SEM ECHO e SEM ICANON)
    tcsetattr(STDIN_FILENO, TCSANOW, &original_settings);

    screenHideCursor(); // Esconde o cursor novamente
}


int main() {
    screenInit(0); // Inicializa a tela (sem bordas, limpa a tela, esconde cursor)
    keyboardInit(); // Inicializa o modo de leitura de teclado (sem buffer, sem echo, leitura imediata)
    srand(time(NULL)); // Inicializa a semente para a função rand() com base no tempo atual

    pedirNomeJogador(); // Solicita e lê o nome do jogador
    carregarDadosPorNome(); // Carrega os dados salvos para o nome digitado (ou inicia novos dados se não encontrar)

    menu(); // Exibe o menu principal e espera pelo Enter para iniciar

    screenClear(); // Limpa a tela antes de começar a primeira batalha

    int batalhaAtual = 0; // Contador para saber em qual batalha o jogador está nesta sessão
    bool continuarBatalha = true; // Flag para controlar se o loop de batalhas continua

    // Define os atributos base INICIAIS do inimigo para usar como referência para o scaling percentual.
    // Estes são os atributos do inimigo da Batalha 1.
    int inimigoInitialBaseVida = 150; // Valor inicial da vida base do inimigo
    int inimigoInitialBaseAtaque = 10; // Valor inicial do ataque base do inimigo
    int inimigoInitialBaseDefesa = 10; // Valor inicial da defesa base do inimigo


    // --- Loop Principal do Jogo (Batalhas Sequenciais) ---
    // O loop continua enquanto o jogador tiver mais de 0 de vida E a flag continuarBatalha for verdadeira.
    while (vida_Do_jogador > 0 && continuarBatalha) {
        batalhaAtual++; // Incrementa o número da batalha atual (começa em 1)

        screenClear(); // Limpa a tela para preparar o cenário da nova batalha

        // Desenha o boneco do jogador na sua posição definida em game_config.h
        jogadorboneco(posicao_X_jogador_boneco, posicao_Y_jogador_boneco);

        // --- Configura o inimigo para a batalha atual com scaling composto de 10% ---
        Inimigo inimigoAtual; // Cria uma nova instância da estrutura Inimigo para esta batalha
        // Calcula os atributos base do inimigo para esta batalha.
        // O fator de aumento é 1.10 elevado à potência (batalhaAtual - 1).
        // Isso aplica um aumento aproximado de 10% composto a cada batalha subsequente.
        // Usamos pow() de <math.h> e convertemos o resultado (double) para int.
        int inimigoBaseVidaCalculado = (int)(inimigoInitialBaseVida * pow(1.10, batalhaAtual - 1));
        int inimigoBaseAtaqueCalculado = (int)(inimigoInitialBaseAtaque * pow(1.10, batalhaAtual - 1));
        int inimigoBaseDefesaCalculado = (int)(inimigoInitialBaseDefesa * pow(1.10, batalhaAtual - 1));

        // Opcional: Garantir que os atributos mínimos sejam pelo menos os iniciais (para evitar problemas de arredondamento com pow se necessário, embora pow(x, 0)=1 resolva para batalha 1).
        // if (inimigoBaseVidaCalculado < inimigoInitialBaseVida) inimigoBaseVidaCalculado = inimigoInitialBaseVida;
        // if (inimigoBaseAtaqueCalculado < inimigoInitialBaseAtaque) inimigoBaseAtaqueCalculado = inimigoInitialBaseAtaque;
        // if (inimigoBaseDefesaCalculado < inimigoInitialBaseDefesa) inimigoBaseDefesaCalculado = inimigoInitialBaseDefesa;

        // Inicializa a estrutura do inimigo para a batalha atual com os atributos calculados
        inicializarInimigo(&inimigoAtual, inimigoBaseVidaCalculado, inimigoBaseAtaqueCalculado, inimigoBaseDefesaCalculado);

        // Atualiza as variáveis globais de vida máxima e atual do inimigo.
        // Isso é usado pelas funções de desenho da barra de vida e lógica da batalha.
        vida_maxima_do_inimigo = inimigoAtual.vidaMaxima;
        vida_do_inimigo = inimigoAtual.vida; // A vida do inimigo começa cheia a cada nova batalha

        // Desenha o boneco do inimigo na sua posição definida
        inimigoboneco(posicao_X_inimigo_boneco, posicao_Y_inimigo_boneco);

        // --- Inicia a batalha contra o inimigo atual ---
        // Chama a função que gerencia a batalha turno a turno.
        // Passamos os endereços das variáveis globais de vida do jogador e do inimigo
        // para que gerenciarTurnoJogador possa modificar diretamente a vida deles.
        // Não precisamos armazenar o retorno em uma variável local (removido o aviso "unused variable").
        gerenciarTurnoJogador(&inimigoAtual, &vida_Do_jogador, &vida_do_inimigo);

        // --- Lógica pós-batalha ---
        // Após gerenciarTurnoJogador retornar, verificamos o estado das vidas
        // para determinar o resultado da batalha e se o jogo continua.

        if (vida_Do_jogador <= 0) {
            // --- Cenário 1: Jogador foi derrotado ---
            continuarBatalha = false; // Define a flag como falsa para sair do loop principal após esta iteração
            screenClear(); // Limpa a tela
            screenSetColor(RED, BLACK); // Define a cor para mensagens de derrota
            // A função perda() em Menu.c tem um erro de posicionamento (todos printf usam y + 0).
            // Se for usar um logo de "Você Perdeu", corrija as posições y dentro dela.
            // Por enquanto, exibimos uma mensagem simples de texto:
            screenGotoxy(60, 15);
            printf("Voce foi derrotado na Batalha %d!", batalhaAtual);
            screenGotoxy(60, 17);
            printf("Seu progresso final: Nivel %d, XP %d, Inimigos Derrotados %d", nivel_Do_jogador, xp_Do_jogador, inimigos_Derrotados);
            screenUpdate(); // Exibe as mensagens de derrota e progresso final
            // Limpa o buffer do teclado para garantir que o próximo getchar espere uma nova tecla
            while (keyhit()) { readch(); }
            getchar(); // Espera o jogador pressionar Enter para reconhecer a derrota
        } else if (vida_do_inimigo <= 0) {
            // --- Cenário 2: Jogador venceu a batalha atual ---
            // A lógica de adicionar XP e subir de nível (se aplicável) já acontece dentro de gerenciarTurnoJogador
            // quando a vida do inimigo chega a zero.

            screenClear(); // Limpa a tela para a mensagem de vitória
            screenSetColor(GREEN, BLACK); // Define a cor para mensagens de vitória
            screenGotoxy(60, 15);
            printf("Voce venceu a Batalha %d!", batalhaAtual);
            screenGotoxy(60, 17);
            printf("Seu progresso atual: Nivel %d, XP %d, Inimigos Derrotados %d", nivel_Do_jogador, xp_Do_jogador, inimigos_Derrotados);
            screenUpdate(); // Exibe as mensagens de vitória e progresso atual
            // Limpa o buffer do teclado para garantir que o próximo getchar espere uma nova tecla
            while (keyhit()) { readch(); }
            getchar(); // Pausa para o jogador ler a mensagem de vitória

            // Pergunta se o jogador quer continuar para a próxima batalha APOS ler a mensagem de vitória
            screenClear(); // Limpa a tela novamente para o prompt de continuar
            screenSetColor(WHITE, BLACK); // Define a cor para o prompt
            screenGotoxy(60, 15);
            printf("Deseja continuar para a proxima batalha? (S/N)");
            screenUpdate(); // Exibe o prompt

            char choice;
            // Limpa o buffer do teclado para garantir que a leitura da escolha (S/N) seja precisa
            while (keyhit()) { readch(); }

            // Espera que o usuário digite 'S', 's', 'N' ou 'n'
            do {
                choice = readch();
                // Ignora caracteres de nova linha ou retorno de carro que possam ter ficado no buffer
                if (choice == '\n' || choice == '\r') choice = 0;
            } while (choice != 's' && choice != 'S' && choice != 'n' && choice != 'N');

                // Se a escolha for 'n' ou 'N', define a flag para sair do loop principal
                if (choice == 'n' || choice == 'N') {
                    continuarBatalha = false;
                }
                // Se a escolha for 's' ou 'S', continuarBatalha permanece true e o loop continua para a próxima iteração
        } else {
            // --- Cenário 3: Batalha terminou por outro motivo (ex: jogador pressionou 'X' para sair) ---
            continuarBatalha = false; // Define a flag para sair do loop principal
            screenClear(); // Limpa a tela
            screenSetColor(WHITE, BLACK); // Define a cor para a mensagem
            screenGotoxy(60, 15);
            printf("Batalha interrompida.");
            screenGotoxy(60, 17);
            printf("Seu progresso final: Nivel %d, XP %d, Inimigos Derrotados %d", nivel_Do_jogador, xp_Do_jogador, inimigos_Derrotados);
            screenUpdate(); // Exibe a mensagem de interrupção e o progresso final
            // Limpa o buffer do teclado antes de esperar getchar
            while (keyhit()) { readch(); }
            getchar(); // Espera o jogador pressionar Enter
        }

        // --- Salvamento de Progresso ---
        // Salva o progresso do jogador no arquivo após cada batalha (vitória, derrota ou interrupção).
        // Isso garante que o progresso seja salvo antes do loop principal decidir se continua ou termina.
        salvarDadosPorNome();
    }

    // --- Fim do Jogo ---
    // O loop principal terminou porque o jogador perdeu, escolheu não continuar
    // após uma vitória, ou interrompeu uma batalha.

    screenClear(); // Limpa a tela para a mensagem final do jogo
    screenSetColor(WHITE, BLACK); // Define a cor padrão para a mensagem final
    screenGotoxy(10, 10);
    printf("Fim de jogo. Seu progresso final foi salvo.");
    screenGotoxy(10, 12);
    printf("Detalhes: Nivel %d, XP %d, Inimigos Derrotados %d", nivel_Do_jogador, xp_Do_jogador, inimigos_Derrotados);
    screenGotoxy(10, 14);
    printf("Pressione Enter para sair.");
    screenUpdate(); // Exibe a mensagem final

    // Limpa o buffer do teclado antes de esperar o Enter final
    while (keyhit()) { readch(); }
    getchar(); // Espera pelo Enter final para fechar o programa

    screenDestroy(); // Restaura as configurações originais do terminal (cores, cursor)
    keyboardDestroy(); // Restaura as configurações originais do teclado

    return 0; // Finaliza o programa com código de sucesso
}
