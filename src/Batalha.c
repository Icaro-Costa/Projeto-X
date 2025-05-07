#include "../include/Batalha.h"
#include "../include/screen.h"
#include "../include/timer.h"
#include "../include/Dado.h"

#include <stdio.h>
#include <unistd.h>

// Definições de tamanho e posição da *área total* da box com borda
//Tamanho 80x24
#define OPCOES_BOX_START_X 5
#define OPCOES_BOX_START_Y 16
#define OPCOES_BOX_WIDTH   70
#define OPCOES_BOX_HEIGHT  7

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
