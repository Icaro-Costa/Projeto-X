#include "../include/Batalha.h"
#include "../include/screen.h"
#include "../include/Dado.h"
#include "../include/game_config.h"

#include <stdio.h>
#include <unistd.h>

int realizarDefesa() {
    int resultadoDado = rolarDado20();

    screenGotoxy(OPCOES_BOX_START_X, OPCOES_BOX_START_Y + OPCOES_BOX_HEIGHT + 1);
    screenSetColor(WHITE, BLACK);

    screenUpdate();
    usleep(50000);

    return resultadoDado;
}

int realizarAtaque() {

    int resultado = 0;
    int dado = rolarDado20();


    screenGotoxy(OPCOES_BOX_START_X, OPCOES_BOX_START_Y + OPCOES_BOX_HEIGHT + 1);
    screenSetColor(WHITE, BLACK);

    resultado = dado;


    printf("Atacou!           ");
    screenUpdate();
    usleep(50000);
    return resultado;
}


int realizarCura() {
    int resultado = 0;
    int dado = rolarDado20();

    screenGotoxy(OPCOES_BOX_START_X, OPCOES_BOX_START_Y + OPCOES_BOX_HEIGHT + 1);
    screenSetColor(WHITE, BLACK);

    resultado = dado;

    printf("Se curou!          ");
    screenUpdate();
    usleep(50000);
    return resultado;
}

//--------------------------------------------------

int realizarIrritar() {
    int resultadoDado = rolarDado20();

    screenGotoxy(OPCOES_BOX_START_X, OPCOES_BOX_START_Y + OPCOES_BOX_HEIGHT + 1);
    screenSetColor(WHITE, BLACK);
    printf("Você tenta provocar o inimigo... [%d]", resultadoDado);
    screenUpdate();
    usleep(50000);

    return resultadoDado;
}
