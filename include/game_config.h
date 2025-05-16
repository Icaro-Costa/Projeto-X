#ifndef __GAME_CONFIG_H__
#define __GAME_CONFIG_H__

// -- Definicoes de Posicao e Tamanho da Box de Opcoes ---
// Ajustadas para uma tela de 196x55
#define OPCOES_BOX_START_X 10   // Margem a partir da esquerda
#define OPCOES_BOX_START_Y 40   // Posicionada mais abaixo na tela
#define OPCOES_BOX_WIDTH 150    // Caixa mais larga
#define OPCOES_BOX_HEIGHT 10    // Caixa mais alta

// Definicoes para a area interna da box (calculadas automaticamente com base nas novas dimensoes)
#define OPCAO_INNER_START_X (OPCOES_BOX_START_X + 10)
#define OPCAO_INNER_START_Y (OPCOES_BOX_START_Y + 1)
#define OPCAO_INNER_WIDTH   (OPCOES_BOX_WIDTH - 20)
#define OPCAO_INNER_HEIGHT  (OPCOES_BOX_HEIGHT - 2)

// Definicoes para posicionamento do texto dentro da area interna
#define INNER_PADDING_X 20 // Padding horizontal
#define INNER_PADDING_Y 1 // Padding vertical

// Posicoes relativas do texto dentro da area interna (Inner)
#define OPCAO1_INNER_REL_X INNER_PADDING_X
#define OPCAO1_INNER_REL_Y INNER_PADDING_Y // Primeira linha

#define OPCAO2_INNER_REL_X (OPCAO_INNER_WIDTH / 2 + INNER_PADDING_X) // Metade da largura interna para a segunda coluna
#define OPCAO2_INNER_REL_Y INNER_PADDING_Y // Primeira linha

#define OPCAO3_INNER_REL_X INNER_PADDING_X
#define OPCAO3_INNER_REL_Y (OPCAO_INNER_HEIGHT / 2 + INNER_PADDING_Y) // Metade da altura interna para a segunda linha

#define OPCAO4_INNER_REL_X (OPCAO_INNER_WIDTH / 2 + INNER_PADDING_X) // Metade da largura interna para a segunda coluna
#define OPCAO4_INNER_REL_Y (OPCAO_INNER_HEIGHT / 2 + INNER_PADDING_Y) // Metade da altura interna para a segunda linha

// --- Declaracoes de Variaceis Globais
// Extern e para declara a variaveis existem em outro lugar

extern char nome_Do_jogador[50];

extern int posicao_X_jogador;
extern int posicao_Y_jogador;
extern int vida_Do_jogador;
extern int vida_Maxima_Do_jogador;
extern int posicao_X_jogador_boneco;
extern int posicao_Y_jogador_boneco;
extern int vida_X_Jogador;
extern int vida_Y_Jogador;
extern int xp_Do_jogador;
extern int nivel_Do_jogador;
extern int inimigos_Derrotados;

extern int posicao_X_inimigo;
extern int posicao_Y_inimigo;
extern int vida_do_inimigo;
extern int vida_maxima_do_inimigo;
extern int posicao_X_inimigo_boneco;
extern int posicao_Y_inimigo_boneco;
extern int vida_X_Inimigo;
extern int vida_Y_Inimigo;

#endif
