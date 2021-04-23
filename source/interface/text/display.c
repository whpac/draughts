#include<stdio.h>
#include "display.h"
#include "../../board/board.h"

void displayBoard(){
    int size = getBoardSize();

    printf(" +");
    for(int i = 0; i < size; i++)
        printf("-");
    printf("+\n");

    int white_cnt = countPawnsOfColor(white);
    int black_cnt = countPawnsOfColor(black);

    for(int row = 0; row < size; row++){
        printf("%d|", row);
        for(int col = 0; col < size; col++){
            Pawn* p = getPawnAt(row, col);

            if(p == NULL){
                // Game takes place on some fields only (black ones)
                printf("%c", isPlayableField(row, col) ? '.' : ' ');
                continue;
            }

            if(getPawnColor(p) == black){
                printf("%c", isPawnKing(p) ? 'B' : 'b');
            }else{
                printf("%c", isPawnKing(p) ? 'W' : 'w');
            }
        }
        printf("|   ");

        if(row == 0) printf("WHITE has %2d pawns", white_cnt);
        if(row == 1) printf("BLACK has %2d pawns", black_cnt);
        printf("\n");
    }

    printf(" +");
    for (int i = 0; i < size; i++)
        printf("-");
    printf("+\n");

    printf("  ");
    for(int i = 0; i < size; i++)
        printf("%d", i);
    printf("\n");

    if(white_cnt > 0 && black_cnt > 0)
        printf("It's %s's move.\n", getNextMoveColor() == white ? "WHITE" : "BLACK");
}