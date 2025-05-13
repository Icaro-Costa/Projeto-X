#include "../include/screen.h"
#include <stdio.h>


void desenharBarraDeVida(int x, int y, int vidaAtual, int vidaMaxima) {
    int comprimento = 20;
    float proporcao = (float)vidaAtual / vidaMaxima;
    int preenchido = (int)(comprimento * proporcao);
    screenGotoxy(x, y);

    screenColor cor; // Variável para armazenar a cor

    if (vidaAtual > 70) {
        cor = GREEN;
    } else if (vidaAtual > 30) {
        cor = YELLOW;
    } else {
        cor = RED;
    }

    screenSetColor(cor, BLACK); // Define a cor da barra

    for (int i = 0; i < preenchido; i++) {
        printf("█");
    }

    screenSetColor(WHITE, BLACK); // Reseta a cor para o texto
    for (int i = preenchido; i < comprimento; i++) {
        printf(" ");
    }

    printf(" %d/%d", vidaAtual, vidaMaxima);
}
