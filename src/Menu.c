#include <stdio.h>

#include "../include/screen.h"
#include "../include/Menu.h"

void ProjetoX(int x, int y){

   screenGotoxy(x, y + 0); printf("######  ######   #####      ####  ####### ######    #####       ##  ##\n");
   screenGotoxy(x, y + 1); printf("##  ##  ##  ##  ##   ##      ##   ##      # ## #   ##   ##      ##  ##\n");
   screenGotoxy(x, y + 2); printf("##  ##  ##  ##  ##   ##      ##   ##        ##     ##   ##       #### \n");
   screenGotoxy(x, y + 3); printf("#####   #####   ##   ##      ##   ######    ##     ##   ##        ##  \n");
   screenGotoxy(x, y + 4); printf("##      ## ##   ##   ##  ##  ##   ##        ##     ##   ##       #### \n");
   screenGotoxy(x, y + 5); printf("##      ##  ##  ##   ##  ##  ##   ##        ##     ##   ##      ##  ##\n");
   screenGotoxy(x, y + 6); printf("##      ##   ##  #####    ####    #######   ##      #####       ##  ##\n");

}

void Start(int x, int y){

  screenGotoxy(x, y + 0); printf(" #####   ######     ##      ######  ######                    ");
  screenGotoxy(x, y + 1); printf("##   ##  # ## #    ####     ##  ##  # ## #              #     ");
  screenGotoxy(x, y + 2); printf("#          ##     ##  ##    ##  ##    ##                 #    ");
  screenGotoxy(x, y + 3); printf(" #####     ##     ##  ##    #####     ##     ##############   ");
  screenGotoxy(x, y + 4); printf("     ##    ##     ######    ## ##     ##                 #    ");
  screenGotoxy(x, y + 5); printf("##   ##    ##     ##  ##    ##  ##    ##                #     ");
  screenGotoxy(x, y + 6); printf("#####      ##     ##  ##    ##  ##    ##                      ");

}

void perda(int x, int y) {

screenGotoxy(x, y + 0); printf("#####     #######  ######   ######   #####   ######     ##     ");
screenGotoxy(x, y + 1); printf("##  ##    ##       ##  ##   ##  ##  ##   ##  # ## #    ####    ");
screenGotoxy(x, y + 2); printf("##   ##   ##       ##  ##   ##  ##  ##   ##    ##     ##  ##   ");
screenGotoxy(x, y + 3); printf("##   ##   ######   #####    #####   ##   ##    ##     ##  ##   ");
screenGotoxy(x, y + 4); printf("##   ##   ##       ## ##    ## ##   ##   ##    ##     ######   ");
screenGotoxy(x, y + 5); printf("##  ##    ##       ##  ##   ##  ##  ##   ##    ##     ##  ##   ");
screenGotoxy(x, y + 6); printf("#####     ######   ###  ##  ### ##   #####    ####    ##  ##   ");

}
