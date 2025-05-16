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

typedef struct {
    char nome[MAX_NOME_LENGTH];
    int xp;
    int inimigosDerrotados;
    int nivel;
    int ataqueBase;
    int defesaBase;
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

    // Lê cada linha do arquivo no NOVO formato: Nome XP Kills Nivel AtaqueBase DefesaBase
    while (numJogadoresSalvos < MAX_JOGADORES &&
        fscanf(arquivo, "%s %d %d %d %d %d", // Modificado para ler 6 valores
               jogadoresSalvos[numJogadoresSalvos].nome,
               &jogadoresSalvos[numJogadoresSalvos].xp,
               &jogadoresSalvos[numJogadoresSalvos].inimigosDerrotados,
               &jogadoresSalvos[numJogadoresSalvos].nivel,
               &jogadoresSalvos[numJogadoresSalvos].ataqueBase, // Lê o ataque base
               &jogadoresSalvos[numJogadoresSalvos].defesaBase) == 6) { // Lê a defesa base e verifica se 6 itens foram lidos
                   numJogadoresSalvos++;
               }

               fclose(arquivo);
}

// Função interna para escrever todos os dados da memória de volta para o arquivo
void escreverTodosDadosParaArquivo() {
    FILE *arquivo;
    arquivo = fopen(ARQUIVO_DADOS, "w"); // Abre para escrita (sobrescreve o arquivo)

    if (arquivo == NULL) {
        screenGotoxy(1, MAXY - 1);
        screenSetColor(RED, BLACK);
        printf("Erro fatal: Nao foi possivel salvar os dados do jogador!\n");
        screenUpdate();
        return;
    }

    // Escreve cada registro de volta no arquivo no NOVO formato
    for (int i = 0; i < numJogadoresSalvos; i++) {
        fprintf(arquivo, "%s %d %d %d %d %d\n", jogadoresSalvos[i].nome,
                jogadoresSalvos[i].xp, jogadoresSalvos[i].inimigosDerrotados, jogadoresSalvos[i].nivel,
                jogadoresSalvos[i].ataqueBase, jogadoresSalvos[i].defesaBase); // Escreve ataque e defesa base
    }

    fclose(arquivo);
}


void carregarDadosPorNome() {
    lerTodosDadosDoArquivo(); // Lê todos os registros existentes do arquivo para a memória

    bool encontrado = false;
    // Procura pelo nome do jogador atual nos dados lidos para a memória
    for (int i = 0; i < numJogadoresSalvos; i++) {
        if (strcmp(nome_Do_jogador, jogadoresSalvos[i].nome) == 0) {
            // Nome encontrado na memória, carrega os dados para as variáveis globais
            xp_Do_jogador = jogadoresSalvos[i].xp;
            inimigos_Derrotados = jogadoresSalvos[i].inimigosDerrotados;
            nivel_Do_jogador = jogadoresSalvos[i].nivel; // Carrega o nível
            ataque_Base_Do_Jogador = jogadoresSalvos[i].ataqueBase; // Carrega o ataque base
            defesa_Base_Do_Jogador = jogadoresSalvos[i].defesaBase; // Carrega a defesa base
            encontrado = true;

            // Mensagem de boas-vindas e dados carregados
            screenClear();
            screenSetColor(WHITE, BLACK);
            screenGotoxy(10, 10);
            printf("Bem-vindo de volta, %s!", nome_Do_jogador);
            screenGotoxy(10, 11);
            printf("Dados carregados: Nivel: %d, XP Total: %d, Inimigos Derrotados: %d", nivel_Do_jogador, xp_Do_jogador, inimigos_Derrotados);
            screenGotoxy(10, 12); // Nova linha para mostrar atributos base
            printf("Atk Base: %d, Def Base: %d", ataque_Base_Do_Jogador, defesa_Base_Do_Jogador);
            screenGotoxy(10, 14); // Ajusta a posição do prompt
            printf("Pressione Enter para iniciar a sessao.");
            screenUpdate();
            getchar(); // Espera o jogador pressionar Enter

            break; // Sai do loop assim que encontrar
        }
    }

    if (!encontrado) {
        // Nome não encontrado, é um novo jogador.
        // Inicia as variáveis globais com valores padrão/zerados.
        xp_Do_jogador = 0;
        inimigos_Derrotados = 0;
        nivel_Do_jogador = 1; // Novo jogador começa no nível 1
        // ataque_Base_Do_Jogador e defesa_Base_Do_Jogador já têm seus valores iniciais
        // definidos em game_config.c ao iniciar o programa, então não precisamos
        // redefini-los aqui para um novo jogador. Eles começarão com os valores padrão.

        // Mensagem de boas-vindas para novo jogador
        screenClear();
        screenSetColor(WHITE, BLACK);
        screenGotoxy(10, 10);
        printf("Novo heroi detectado! Bem-vindo, %s!", nome_Do_jogador);
        screenGotoxy(10, 12);
        printf("Pressione Enter para iniciar sua jornada.");
        screenUpdate();
        getchar(); // Espera o jogador pressionar Enter
    }
    // As variáveis globais (XP, Nível, Kills, Ataque Base, Defesa Base)
    // agora contêm os valores corretos para esta sessão (carregados ou iniciais).
}

void salvarDadosPorNome() {
    lerTodosDadosDoArquivo(); // Lê todos os registros existentes para a memória (para não perdê-los)

    bool encontrado = false;
    // Procura pelo nome do jogador atual na memória para atualizar seus dados
    for (int i = 0; i < numJogadoresSalvos; i++) {
        if (strcmp(nome_Do_jogador, jogadoresSalvos[i].nome) == 0) {
            // Nome encontrado na memória, atualiza os dados na memória com os valores atuais das variáveis globais
            jogadoresSalvos[i].xp = xp_Do_jogador;
            jogadoresSalvos[i].inimigosDerrotados = inimigos_Derrotados;
            jogadoresSalvos[i].nivel = nivel_Do_jogador; // Salva o nível atual
            jogadoresSalvos[i].ataqueBase = ataque_Base_Do_Jogador; // Salva o ataque base atual
            jogadoresSalvos[i].defesaBase = defesa_Base_Do_Jogador; // Salva a defesa base atual
            encontrado = true;
            break; // Sai do loop assim que encontrar
        }
    }

    if (!encontrado) {
        // Nome não encontrado na memória (primeira vez salvando este jogador), adiciona como novo registro na memória
        if (numJogadoresSalvos < MAX_JOGADORES) {
            strcpy(jogadoresSalvos[numJogadoresSalvos].nome, nome_Do_jogador);
            jogadoresSalvos[numJogadoresSalvos].xp = xp_Do_jogador;
            jogadoresSalvos[numJogadoresSalvos].inimigosDerrotados = inimigos_Derrotados;
            jogadoresSalvos[numJogadoresSalvos].nivel = nivel_Do_jogador; // Salva o nível atual
            jogadoresSalvos[numJogadoresSalvos].ataqueBase = ataque_Base_Do_Jogador; // Salva o ataque base atual
            jogadoresSalvos[numJogadoresSalvos].defesaBase = defesa_Base_Do_Jogador; // Salva a defesa base atual
            numJogadoresSalvos++; // Incrementa o contador de jogadores na memória
        } else {

            screenGotoxy(1, MAXY - 1);
            screenSetColor(RED, BLACK);
            printf("Limite de jogadores salvos atingido. Nao foi possivel adicionar novo registro.\n");
            screenUpdate();
            getchar();
        }
    }

    escreverTodosDadosParaArquivo();
}
