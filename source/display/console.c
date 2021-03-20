#include<stdio.h>
#include "console.h"
#include "../board/board.h"

void displayBoard(){
    int size = getBoardSize();

    printf(" +");
    for(int i = 0; i < size; i++)
        printf("-");
    printf("+\n");

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
        printf("|\n");
    }

    printf(" +");
    for (int i = 0; i < size; i++)
        printf("-");
    printf("+\n");

    printf("  ");
    for(int i = 0; i < size; i++)
        printf("%d", i);
    printf("\n");
}