#ifndef INIMIGO_H
#define INIMIGO_H

#include <stdbool.h>

typedef struct {
    int vida;
    int vidaMaxima;

    int ataqueBase;         // Ataque original/base do inimigo
    int defesaBase;         // Defesa original/base do inimigo

    int ataqueAtual;        // Ataque modificado por efeitos (ex: raiva)
    int defesaAtual;        // Defesa modificada por efeitos

    int bonusDefesaAtiva;   // Bônus da ação "Defender" do inimigo

    int nivelRaiva;
    int turnosDeRaivaRestantes;

    int acoesDisponiveis[4];
    int numAcoesDisponiveis;
    bool podeUsarIrritar;
} Inimigo;

void inicializarInimigo(Inimigo *inimigo, int vida, int ataque, int defesa);
int escolherAcaoInimigo(Inimigo *inimigo);

#endif
