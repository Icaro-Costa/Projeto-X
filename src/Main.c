#include "../include/screen.h"
#include "../include/keyboard.h"
#include "../include/utils.h"
#include "../include/inimigo.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include "../include/barradevida.h"
#include "../include/bonecos.h"

#include <stdbool.h>

int posicao_X_jogador = 2;
int posicao_Y_jogador = 14;
int vida_Do_jogador = 100;
int vida_Maxima_Do_jogador = 100;
int posicao_X_jogador_boneco = 2;
int posicao_Y_jogador_boneco = 0;

int posicao_X_inimigo = 51;
int posicao_Y_inimigo = 14;
int vida_do_inimigo = 150;
int vida_maxima_do_inimigo = 150;
int posicao_X_inimigo_boneco = 37;
int posicao_Y_inimigo_boneco = 1;

void jogador() {
    jogadorboneco(posicao_X_jogador_boneco, posicao_Y_jogador_boneco);
    desenharBarraDeVida(posicao_X_jogador, posicao_Y_jogador, vida_Do_jogador, vida_Maxima_Do_jogador);
}

void inimigo(Inimigo *inimigo) {
    inimigoboneco(posicao_X_inimigo_boneco, posicao_Y_inimigo_boneco);
    desenharBarraDeVida(posicao_X_inimigo, posicao_Y_inimigo, inimigo->vida, inimigo->vidaMaxima);
}

int main() {
    screenInit(0);
    keyboardInit();
    srand(time(NULL));

    jogador();

    Inimigo inimigo1;
    inicializarInimigo(&inimigo1, 150, 10, 10);

    inimigo(&inimigo1);

    int resultadoBatalha = gerenciarTurnoJogador(&inimigo1, &vida_Do_jogador, &vida_do_inimigo);

    if (resultadoBatalha == 1) {
        printf("\nBatalha encerrada normalmente.\n");
    } else {
        printf("\nBatalha interrompida.\n");
    }

    screenGotoxy(1, MAXY);
    printf("Enter para sair.");
    getchar();

    screenDestroy();
    keyboardDestroy();

    return 0;
}
