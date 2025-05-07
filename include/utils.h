#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include "../include/inimigo.h"

int gerenciarTurnoJogador(Inimigo *inimigo, int *vidaJogador, int *vidaInimigo);

void executarTurnoInimigo(Inimigo *inimigo, int *vidaJogador, int vidaMaximaJogador, int *defesaAplicadaJogador);

#endif
