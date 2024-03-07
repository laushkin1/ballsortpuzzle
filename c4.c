#include <stdio.h>
#include <ncurses.h>
#include <string.h>
#include "c4.h"

void initialize_board(int rows, int cols, char board[rows][cols]){

    if (rows > 10){
        printf("Sorry, param 'rows' is too big\n");
    } else if(rows < 4){
        printf("Sorry, param 'rows' is too little\n");
    }

    if (cols > 10){
        printf("Sorry, param 'cols' is too big\n");
    } else if(cols < 4){
        printf("Sorry, param 'cols' is too little\n");
    }

    for(int r=0; r<rows; r++){
        for(int c=0; c<cols; c++){
            board[r][c] = '.';
        }
    }
}

void print_board(int rows, int cols, const char board[rows][cols]){

    int y, x;
    getmaxyx(stdscr, y, x);
    x--;
    y--;

    for (int yy = 0; yy<=y; yy++){
        mvaddch(yy, 0, '#');
    }
    for (int xx = 0; xx<=x; xx++){
        mvaddch(0, xx, '#');
    }
    for (int xx = 0; xx<=x; xx++){
        mvaddch(y, xx, '#');
    }
    for (int yy = 0; yy<=y; yy++){
        mvaddch(yy, x, '#');
    }


    char one_row[500] = "";

    int xmax, ymax;
    getmaxyx(stdscr, ymax, xmax);


    for(int r=0; r<rows; r++){
        for(int c=0; c<cols; c++){
            char filedrc[] = {board[r][c], '\0'};
            strcat(one_row, " | ");
            strcat(one_row, filedrc);
        }
        strcat(one_row, " |");
        strcat(one_row, "0");
    }

    char *p;
    char zero[2] = "0";
    // all rows
    p = strtok(one_row, zero);

    int half_of_len_x = strlen(p)/2;
    int half_of_len_y = rows/2;

    int center_x = (xmax/2)-half_of_len_x;
    int center_y = (ymax/2)-half_of_len_y;

    while(p != NULL){
        mvprintw(center_y, center_x-1, "%s ", p);
        center_y++;
        p = strtok (NULL,zero);
    }

    char under_score[150] = "";
    for (int i = 0; i<cols; i++){
        strcat(under_score, "----");
    }
    mvprintw(center_y, center_x, "%s", under_score);
    printw("-");

    center_y++;
    char numbers[150] = "";
    for (int i = 1; i<=cols; i++) {
        char str[150] = "";
        sprintf(str, "  %d ", i);
        strcat(numbers, str);
    }
    mvprintw(center_y, center_x, "%s", numbers);

}

int is_valid_move(int rows, int cols, const char board[rows][cols], int col) {
    int indx_col = col-1;
    int is_flag = 0;
    for (int r = rows-1; r >= 0; r--) {
        if (board[r][indx_col] == '.'){
            is_flag = 1;
            break;
        }
        is_flag = 0;
    }

    return is_flag;
}

int drop_piece(int rows, int cols, char board[rows][cols], int col, char player_piece){
    int indx_col = col-1;
    for (int r = rows-1; r >= 0; r--) {
        if (board[r][indx_col] == '.'){
            board[r][indx_col] = player_piece;
            return r;
        }
    }
    return 0;
}

int check_win(int rows, int cols, const char board[rows][cols], int row, int col, char player_piece) {

    // trash param, i will check all board
    row = 100;
    col = 100;


    // check horizontal
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if((cols-c)>=4) {
                if (board[r][c] == player_piece && board[r][c + 1] == player_piece &&
                    board[r][c + 2] == player_piece && board[r][c + 3] == player_piece) {
                    return 1;
                }
            }
        }
    }

    // check vertical
    for(int c=0; c<cols; c++){
        for(int r=0; r<rows; r++){
            if(board[r][c] == player_piece) {
                if(row-r>=4){
                    if (board[r][c] == player_piece && board[r+1][c] == player_piece &&
                        board[r+2][c] == player_piece && board[r+3][c] == player_piece) {
                        return 1;
                    }
                }
            }
        }
    }

    // check diagonal
    for (int r=rows-1; r>=0; r--){
        for(int c=cols-1; c>=0; c--){
            if(board[r][c] == player_piece){
                // right up
                if((cols-c)>=4 && r+1>=4){
                    if(board[r][c] == player_piece && board[r-1][c+1] == player_piece &&
                        board[r-2][c+2] == player_piece && board[r-3][c+3] == player_piece){
                        return 1;
                    }
                }

                // left up
                if((c-4)>=-1 && r+1>=4){
                    if(board[r][c] == player_piece && board[r-1][c-1] == player_piece &&
                        board[r-2][c-2] == player_piece && board[r-3][c-3] == player_piece){
                        return 1;
                    }
                }
            }
        }
    }


    return 0;

}

int is_board_full(int rows, int cols, const char board[rows][cols]){

    for(int r=0; r<rows; r++){
        for(int c=0; c<cols; c++){
            if(board[r][c] == '.'){
                return 0;
            }
        }
    }

    return 1;
}