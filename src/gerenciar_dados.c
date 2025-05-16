#include "../include/gerenciar_dados.h"
#include "../include/game_config.h"
#include "../include/screen.h"
#include "../include/keyboard.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ARQUIVO_DADOS "dados.txt"
#define MAX_JOGADORES 100
#define MAX_NOME_LENGTH 50

// Estrutura temporária para armazenar dados lidos do arquivo
typedef struct {
    char nome[MAX_NOME_LENGTH];
    int xp;
    int inimigosDerrotados;
} DadosJogadorSalvo;

// Array para armazenar temporariamente os dados de todos os jogadores
DadosJogadorSalvo jogadoresSalvos[MAX_JOGADORES];
int numJogadoresSalvos = 0; // Contador de jogadores lidos/armazenados

// Função interna para ler todos os dados do arquivo para a memória
void lerTodosDadosDoArquivo() {
    FILE *arquivo;
    arquivo = fopen(ARQUIVO_DADOS, "r");
    numJogadoresSalvos = 0; // Resetar o contador

    if (arquivo == NULL) {
        // Arquivo não existe, não há dados para ler.
        return;
    }

    // Lê cada linha do arquivo
    while (numJogadoresSalvos < MAX_JOGADORES &&
        fscanf(arquivo, "%s %d %d", jogadoresSalvos[numJogadoresSalvos].nome,
               &jogadoresSalvos[numJogadoresSalvos].xp,
               &jogadoresSalvos[numJogadoresSalvos].inimigosDerrotados) == 3) {
        numJogadoresSalvos++;
               }

               fclose(arquivo);
}

// Função interna para escrever todos os dados da memória de volta para o arquivo
void escreverTodosDadosParaArquivo() {
    FILE *arquivo;
    arquivo = fopen(ARQUIVO_DADOS, "w"); // Abre para escrita (sobrescreve o arquivo)

    if (arquivo == NULL) {
        // Erro fatal: Não foi possível salvar
        screenGotoxy(1, MAXY - 1);
        screenSetColor(RED, BLACK);
        printf("Erro fatal: Nao foi possivel salvar os dados do jogador!\n");
        screenUpdate();
        // exit(1); // Considerar encerrar o programa se não puder salvar
        return;
    }

    // Escreve cada registro de volta no arquivo
    for (int i = 0; i < numJogadoresSalvos; i++) {
        fprintf(arquivo, "%s %d %d\n", jogadoresSalvos[i].nome,
                jogadoresSalvos[i].xp, jogadoresSalvos[i].inimigosDerrotados);
    }

    fclose(arquivo);
}


void carregarDadosPorNome() {
    lerTodosDadosDoArquivo(); // Lê todos os registros existentes

    bool encontrado = false;
    // Procura pelo nome do jogador atual nos dados lidos
    for (int i = 0; i < numJogadoresSalvos; i++) {
        if (strcmp(nome_Do_jogador, jogadoresSalvos[i].nome) == 0) {
            // Nome encontrado, carrega os dados
            xp_Do_jogador = jogadoresSalvos[i].xp;
            inimigos_Derrotados = jogadoresSalvos[i].inimigosDerrotados;
            encontrado = true;

            // Opcional: Mensagem de boas-vindas e dados carregados
            screenClear();
            screenSetColor(WHITE, BLACK);
            screenGotoxy(10, 10);
            printf("Bem-vindo de volta, %s!", nome_Do_jogador);
            screenGotoxy(10, 11);
            printf("Dados carregados: XP Total: %d, Inimigos Derrotados: %d", xp_Do_jogador, inimigos_Derrotados);
            screenGotoxy(10, 13);
            printf("Pressione Enter para iniciar a sessao.");
            screenUpdate();
            getchar(); // Espera o jogador pressionar Enter

            break; // Sai do loop assim que encontrar
        }
    }

    if (!encontrado) {
        // Nome não encontrado, iniciar com dados zerados para este jogador
        xp_Do_jogador = 0;
        inimigos_Derrotados = 0;
        // A variável global nome_Do_jogador já contém o nome digitado.

        screenClear();
        screenSetColor(WHITE, BLACK);
        screenGotoxy(10, 10);
        printf("Novo heroi detectado! Bem-vindo, %s!", nome_Do_jogador);
        screenGotoxy(10, 12);
        printf("Pressione Enter para iniciar sua jornada.");
        screenUpdate();
        getchar(); // Espera o jogador pressionar Enter
    }

    // O nível para a *exibição* nesta sessão começará no Nível 1,
    // mas o XP_Do_jogador guarda o total para o cálculo do nível real salvo.
    // a partir do total de XP carregado/iniciado.
    nivel_Do_jogador = (xp_Do_jogador / 100) + 1;

    // As variáveis globais xp_Do_jogador, inimigos_Derrotados e nivel_Do_jogador
    // agora contêm os valores iniciais para esta sessão, baseados no carregamento ou novo jogo.
}

void salvarDadosPorNome() {
    lerTodosDadosDoArquivo(); // Lê todos os registros para não perdê-los

    bool encontrado = false;
    // Procura pelo nome do jogador atual para atualizar seus dados
    for (int i = 0; i < numJogadoresSalvos; i++) {
        if (strcmp(nome_Do_jogador, jogadoresSalvos[i].nome) == 0) {
            // Nome encontrado, atualiza os dados com os valores atuais das variáveis globais
            jogadoresSalvos[i].xp = xp_Do_jogador;
            jogadoresSalvos[i].inimigosDerrotados = inimigos_Derrotados;
            encontrado = true;
            break; // Sai do loop
        }
    }

    if (!encontrado) {
        // Nome não encontrado (primeira vez salvando este jogador), adiciona como novo registro
        if (numJogadoresSalvos < MAX_JOGADORES) {
            strcpy(jogadoresSalvos[numJogadoresSalvos].nome, nome_Do_jogador);
            jogadoresSalvos[numJogadoresSalvos].xp = xp_Do_jogador;
            jogadoresSalvos[numJogadoresSalvos].inimigosDerrotados = inimigos_Derrotados;
            numJogadoresSalvos++;
        } else {
            screenGotoxy(1, MAXY - 1);
            screenSetColor(RED, BLACK);
            printf("Limite de jogadores salvos atingido. Nao foi possivel adicionar novo registro.\n");
            screenUpdate();
            getchar(); // Pausa opcional
        }
    }
    escreverTodosDadosParaArquivo();
}
