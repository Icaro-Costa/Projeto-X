#include "../include/inimigo.h"
#include <stdlib.h>
#include <stdbool.h>

void inicializarInimigo(Inimigo *inimigo, int vida, int ataque, int defesa) {
    inimigo->vida = vida;
    inimigo->vidaMaxima = vida;

    inimigo->ataqueBase = ataque;
    inimigo->defesaBase = defesa;
    inimigo->ataqueAtual = ataque;
    inimigo->defesaAtual = defesa;

    inimigo->bonusDefesaAtiva = 0;

    inimigo->nivelRaiva = 0;
    inimigo->turnosDeRaivaRestantes = 0;

    // Define as ações disponíveis e seus códigos numéricos
    inimigo->acoesDisponiveis[0] = 0; // 0: Ataque
    inimigo->acoesDisponiveis[1] = 1; // 1: Defesa
    inimigo->acoesDisponiveis[2] = 2; // 2: Cura
    inimigo->numAcoesDisponiveis = 3;

    inimigo->podeUsarIrritar = false; // Flag para habilidade de irritar do próprio inimigo (não usada atualmente)
}

int escolherAcaoInimigo(Inimigo *inimigo) {
    if (inimigo->numAcoesDisponiveis == 0) {
        return -1; // Nenhuma ação possível
    }

    // --- Lógica de Cura ---

    float porcentagemVida = (float)inimigo->vida / inimigo->vidaMaxima;
    if (porcentagemVida < 0.4f /*&& inimigo->nivelRaiva < 10*/) {
        int chanceDeCurar = rand() % 100;
        if (chanceDeCurar < 60) { // 60% de chance de escolher curar se as condições de vida forem atendidas
            return 2; // Código para Cura
        }
    }

    // --- Lógica de Ataque/Defesa (influenciada pela raiva) ---
    int chance = rand() % 100; // 0-99

    // Se estiver com raiva e os efeitos ainda estiverem ativos
    if (inimigo->nivelRaiva > 10 && inimigo->turnosDeRaivaRestantes > 0) {
        if (chance < 90) { // 90% de chance de atacar (com raiva alta)
            return 0; // Código para Ataque
        } else { // 10% de chance de defender (com raiva alta)
            return 1; // Código para Defesa
        }
    } else { // Comportamento com raiva baixa/normal ou sem turnos de raiva restantes
        if (chance < 65) { // 65% de chance de atacar (normal)
            return 0; // Código para Ataque
        } else { // 35% de chance de defender (normal)
            return 1; // Código para Defesa
        }
    }
    // A lógica acima deve cobrir todos os casos se numAcoesDisponiveis > 0.
    // A linha abaixo é um fallback caso a lógica precise ser depurada ou alterada.
    // return inimigo->acoesDisponiveis[rand() % inimigo->numAcoesDisponiveis];
}
