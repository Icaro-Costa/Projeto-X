#include "../include/bonecos.h"
#include "../include/screen.h"

void jogadorboneco(int x, int y) {

    screenGotoxy(x, y + 0); printf("      ******                          ");
    screenGotoxy(x, y + 1); printf("    *        *                       ");
    screenGotoxy(x, y + 2); printf("   *_________ *                       ");
    screenGotoxy(x, y + 3); printf("   ||  █  █  ||                       ");
    screenGotoxy(x, y + 4); printf("   |\\_______/ |                       ");
    screenGotoxy(x, y + 5); printf("   |          |                       ");
    screenGotoxy(x, y + 6); printf("   |          |                .     ");
    screenGotoxy(x, y + 7); printf("  ________________            / \\    ");
    screenGotoxy(x, y + 8); printf("                 \\            | |    ");
    screenGotoxy(x, y + 9); printf("  |            |\\ \\           | |    ");
    screenGotoxy(x, y + 10);printf("  |            | \\ \\          | |    ");
    screenGotoxy(x, y + 11);printf("  |            |  \\ \\         | |    ");
    screenGotoxy(x, y + 12);printf("  |            |   \\ \\      __|_|__ ");
    screenGotoxy(x, y + 13);printf("  |            |    \\ \\_____|_____|   ");
    screenGotoxy(x, y + 14);printf("  |            |     \\________|_|    ");

    // O screenGotoxy(x, y + N) posiciona o texto N linhas abaixo da posição inicial (x, y).
}

void inimigoboneco(int x, int y) {

    screenGotoxy(x, y + 0); printf("                         ********");
    screenGotoxy(x, y + 1); printf("                        *        *");
    screenGotoxy(x, y + 2); printf("                       *__________* ");
    screenGotoxy(x, y + 3); printf("                       || █  █   || ");
    screenGotoxy(x, y + 4); printf("                       |\\________/| ");
    screenGotoxy(x, y + 5); printf("        .              |          | ");
    screenGotoxy(x, y + 6); printf("       / \\             |          |  ");
    screenGotoxy(x, y + 7); printf("       | |          ________________");
    screenGotoxy(x, y + 14);printf("       | |          /                 ");
    screenGotoxy(x, y + 8); printf("       | |         /  |            |");
    screenGotoxy(x, y + 9); printf("       | |        /  /|            |");
    screenGotoxy(x, y + 10);printf("       | |       /  / |            |");
    screenGotoxy(x, y + 11);printf("     __|_|__    /  /  |            |");
    screenGotoxy(x, y + 12);printf("     |_____|___/  /   |            |");
    screenGotoxy(x, y + 13);printf("       |_|_______/    |            |");
    screenGotoxy(x, y + 14);printf("                                         ");
}
// --------------------------------------------------------
