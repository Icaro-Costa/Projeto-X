#include "../include/gerenciar_dados.h"
#include "../include/game_config.h"
#include "../include/screen.h"

#include <stdio.h>
#include <stdlib.h> // Necessário para atoi
#include <string.h> // Necessário para strcmp, strcpy, strncpy, strtok, strcspn
#include <stdbool.h>

#define ARQUIVO_DADOS "dados.txt"
#define MAX_JOGADORES 100
#define MAX_NOME_LENGTH 50

// --- Definições movidas para o início do arquivo ---

// Estrutura para armazenar os dados de um jogador salvo
typedef struct {
    char nome[MAX_NOME_LENGTH];
    int xp;
    int inimigosDerrotados;
    int nivel;
    int ataqueBase;
    int defesaBase;
    // A vidaMaxima NÃO ESTÁ NESTA ESTRUTURA,
    // vamos calculá-la com base no nível carregado.
} DadosJogadorSalvo;

// Array para armazenar temporariamente os dados de todos os jogadores lidos do arquivo
DadosJogadorSalvo jogadoresSalvos[MAX_JOGADORES];
// Contador de jogadores lidos/armazenados no array jogadoresSalvos
int numJogadoresSalvos = 0;

// Declarações das funções internas (para que possam ser usadas antes de suas definições completas)
void lerTodosDadosDoArquivo();
void escreverTodosDadosParaArquivo();

// --- Fim das Definições movidas ---


// Função interna para ler todos os dados do arquivo para a memória
// MODIFICADA para ler o formato delimitado por pipe (|)
void lerTodosDadosDoArquivo() {
    FILE *arquivo;
    arquivo = fopen(ARQUIVO_DADOS, "r"); // Abre para leitura

    numJogadoresSalvos = 0; // Resetar o contador de jogadores lidos antes de ler

    if (arquivo == NULL) {
        // Arquivo não existe, não há dados para ler. Isso é normal para o primeiro jogo.
        return;
    }

    char linha[256]; // Buffer para ler cada linha do arquivo (ajuste o tamanho se necessário)

    // Lê cada linha do arquivo usando fgets e processa o formato delimitado por pipe
    while (numJogadoresSalvos < MAX_JOGADORES && fgets(linha, sizeof(linha), arquivo) != NULL) {
        // Remove o caractere de nova linha '\n' lido por fgets, se existir.
        linha[strcspn(linha, "\n")] = 0;

        // Ignora linhas vazias ou que contenham apenas a nova linha
        if (strlen(linha) == 0) {
            continue;
        }

        // Usa strtok para dividir a linha nos campos usando o pipe "|" como delimitador
        char *token;

        // Campo 1: Nome
        token = strtok(linha, "|");
        if (token == NULL) { // Se o primeiro campo (nome) estiver faltando, ignora a linha
            // Opcional: Adicionar log de erro para linhas mal formatadas
            continue;
        }
        // Copia o nome lido para a estrutura, garantindo não exceder o buffer
        strncpy(jogadoresSalvos[numJogadoresSalvos].nome, token, MAX_NOME_LENGTH - 1);
        jogadoresSalvos[numJogadoresSalvos].nome[MAX_NOME_LENGTH - 1] = '\0'; // Garante terminação nula

        // Campo 2: XP
        token = strtok(NULL, "|"); // Continua a partir de onde parou, procurando o próximo "|"
        if (token == NULL) continue; // Se o campo estiver faltando, ignora a linha
        jogadoresSalvos[numJogadoresSalvos].xp = atoi(token); // Converte a string para inteiro

        // Campo 3: Inimigos Derrotados
        token = strtok(NULL, "|");
        if (token == NULL) continue;
        jogadoresSalvos[numJogadoresSalvos].inimigosDerrotados = atoi(token);

        // Campo 4: Nivel
        token = strtok(NULL, "|");
        if (token == NULL) continue;
        jogadoresSalvos[numJogadoresSalvos].nivel = atoi(token);

        // Campo 5: Ataque Base
        token = strtok(NULL, "|");
        if (token == NULL) continue;
        jogadoresSalvos[numJogadoresSalvos].ataqueBase = atoi(token);

        // Campo 6: Defesa Base
        token = strtok(NULL, "|");
        if (token == NULL) continue;
        jogadoresSalvos[numJogadoresSalvos].defesaBase = atoi(token);

        // Se todos os 6 campos foram lidos e processados para esta linha, incrementa o contador de jogadores
        // strtok(NULL, "|") retornaria NULL se houver mais pipes do que o esperado,
        // mas a lógica acima trata cada campo individualmente.
        // Podemos adicionar uma verificação extra se esperamos *exatamente* 6 campos,
        // mas para este formato simples, verificar se cada token não é NULL já é suficiente.

        numJogadoresSalvos++; // A linha foi lida e os dados foram armazenados

    } // Fim do loop while (fgets)

    fclose(arquivo); // Fecha o arquivo
}

// Função interna para escrever todos os dados da memória de volta para o arquivo
// MODIFICADA para escrever no formato delimitado por pipe (|)
void escreverTodosDadosParaArquivo() {
    FILE *arquivo;
    arquivo = fopen(ARQUIVO_DADOS, "w"); // Abre para escrita (sobrescreve o arquivo existente)

    if (arquivo == NULL) {
        // Trata o erro se o arquivo não puder ser aberto para escrita
        screenGotoxy(1, MAXY - 1); // Usa a posição MAXY (parte inferior da tela)
        screenSetColor(RED, BLACK);
        printf("Erro fatal: Nao foi possivel salvar os dados do jogador!\n");
        screenUpdate();
        // Poderíamos adicionar uma pausa aqui, mas seguindo o padrão existente, apenas imprime a mensagem.
        return;
    }

    // Escreve cada registro de volta no arquivo no NOVO formato: Nome|XP|Kills|Nivel|AtaqueBase|DefesaBase\n
    for (int i = 0; i < numJogadoresSalvos; i++) {
        fprintf(arquivo, "%s|%d|%d|%d|%d|%d\n",
                jogadoresSalvos[i].nome,
                jogadoresSalvos[i].xp,
                jogadoresSalvos[i].inimigosDerrotados,
                jogadoresSalvos[i].nivel,
                jogadoresSalvos[i].ataqueBase,
                jogadoresSalvos[i].defesaBase);
    }

    fclose(arquivo); // Fecha o arquivo após escrever
}


void carregarDadosPorNome() {
    // Chama a função interna para ler todos os dados do arquivo para a memória
    lerTodosDadosDoArquivo();

    bool encontrado = false;
    // Procura pelo nome do jogador atual nos dados lidos para a memória
    for (int i = 0; i < numJogadoresSalvos; i++) {
        if (strcmp(nome_Do_jogador, jogadoresSalvos[i].nome) == 0) {
            // Nome encontrado na memória, carrega os dados para as variáveis globais
            xp_Do_jogador = jogadoresSalvos[i].xp;
            inimigos_Derrotados = jogadoresSalvos[i].inimigosDerrotados;
            nivel_Do_jogador = jogadoresSalvos[i].nivel; // Carrega o nível

            // --- CORREÇÃO: Recalcula e define a vida máxima com base no nível carregado ---
            // A vida base inicial é 100. Cada nível acima do 1º adiciona 15 HP.
            vida_Maxima_Do_jogador = 100 + (nivel_Do_jogador - 1) * 15;
            // --- Fim da Correção ---

            // --- CORREÇÃO: Define a vida atual do jogador como a vida máxima carregada ---
            vida_Do_jogador = vida_Maxima_Do_jogador;
            // --- Fim da Correção ---

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
            screenGotoxy(10, 12);
            // Incluímos a exibição da vida máxima carregada na mensagem de boas-vindas para confirmação
            printf("Atk Base: %d, Def Base: %d, Vida Maxima: %d", ataque_Base_Do_Jogador, defesa_Base_Do_Jogador, vida_Maxima_Do_jogador);
            screenGotoxy(10, 14);
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

        // --- CORREÇÃO: Define a vida máxima e atual inicial para um novo jogador ---
        vida_Maxima_Do_jogador = 100; // Define a vida máxima inicial
        vida_Do_jogador = vida_Maxima_Do_jogador; // Define a vida atual como a máxima inicial
        // --- Fim da Correção ---

        // ataque_Base_Do_Jogador e defesa_Base_Do_Jogador já têm seus valores iniciais
        // definidos em game_config.c ao iniciar o programa.

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
    // As variáveis globais (XP, Nível, Kills, Ataque Base, Defesa Base, VIDA MAXIMA, VIDA ATUAL)
    // agora contêm os valores corretos para esta sessão (carregados e recalculados ou iniciais).
}

void salvarDadosPorNome() {
    // Chama a função interna para ler todos os registros existentes para a memória (para não perdê-los)
    lerTodosDadosDoArquivo();

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
            getchar(); // Pausa para o usuário ver a mensagem de erro
        }
    }

    // Chama a função interna para escrever todos os dados (existentes + atualizado/novo) de volta para o arquivo
    escreverTodosDadosParaArquivo();
}
