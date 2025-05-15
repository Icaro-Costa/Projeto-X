#include "../include/screen.h"
#include "../include/keyboard.h"
#include "../include/inimigo.h"

#include "../include/game_config.h"
#include "../include/barradevida.h"
#include "../include/bonecos.h"

#include "../include/GerenciarTurnoJogador.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

// a tela tem que ta em 162 por 52 px

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
