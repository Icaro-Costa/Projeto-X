#ifndef __GAME_CONFIG_H__
#define __GAME_CONFIG_H__


// -- Definicoes de Posicao e Tamanho da Box de Opcoes ---
#define OPCOES_BOX_START_X 5
#define OPCOES_BOX_START_Y 16
#define OPCOES_BOX_WIDTH 70
#define OPCOES_BOX_HEIGHT 7

// Definicoes para a area interna da box
#define OPCAO_INNER_START_X (OPCOES_BOX_START_X + 1)
#define OPCAO_INNER_START_Y (OPCOES_BOX_START_Y + 1)
#define OPCAO_INNER_WIDTH   (OPCOES_BOX_WIDTH - 2)
#define OPCAO_INNER_HEIGHT  (OPCOES_BOX_HEIGHT - 2)

// Definicoes para posicionamento do texto dentro da area interna
#define INNER_PADDING_X 2
#define INNER_PADDING_Y 1

#define OPCAO1_INNER_REL_X INNER_PADDING_X
#define OPCAO1_INNER_REL_Y INNER_PADDING_Y

#define OPCAO2_INNER_REL_X (OPCAO_INNER_WIDTH / 2 + INNER_PADDING_X)
#define OPCAO2_INNER_REL_Y INNER_PADDING_Y

#define OPCAO3_INNER_REL_X INNER_PADDING_X
#define OPCAO3_INNER_REL_Y (OPCAO_INNER_HEIGHT / 2 + INNER_PADDING_Y)

#define OPCAO4_INNER_REL_X (OPCAO_INNER_WIDTH / 2 + INNER_PADDING_X)
#define OPCAO4_INNER_REL_Y (OPCAO_INNER_HEIGHT / 2 + INNER_PADDING_Y)

// --- Declaracoes de Variaceis Globais
// Extern e para declara a variaveis existem em outro lugar

extern int posicao_X_jogador;
extern int posicao_Y_jogador;
extern int vida_Do_jogador;
extern int vida_Maxima_Do_jogador;
extern int posicao_X_jogador_boneco;
extern int posicao_Y_jogador_boneco;

extern int posicao_X_inimigo;
extern int posicao_Y_inimigo;
extern int vida_do_inimigo;
extern int vida_maxima_do_inimigo;
extern int posicao_X_inimigo_boneco;
extern int posicao_Y_inimigo_boneco;

#endif
