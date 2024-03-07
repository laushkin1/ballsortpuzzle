#include <stdio.h>
#include <stdlib.h> 
#include "ballsortpuzzle.h"
#include <ncurses.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

void generator(const int rows, const int columns, char field[rows][columns]){
    char available_symbols[10] = {'@', '#', '$', '%', '&', '*', '+', '/', '^'};
    char symbols[15] = "";
    char symbols_x_rows[150] = "";
    int pointer;
    int d = 0;

    // make a symbols variable
    srand(time(NULL));
    while(d<(columns-2)){
        pointer = (rand() % (columns-2));
        if(strchr(symbols, available_symbols[pointer]) == NULL){
            char available_symbols_pointer[] = {available_symbols[pointer], '\0'};
            strcat(symbols, available_symbols_pointer);
            d++;
        }
    }


    int number_of_first_free_column = rand()%columns, number_of_second_free_column = rand()%columns;
    while(number_of_first_free_column == number_of_second_free_column) number_of_second_free_column = rand()%columns;

    // symbols_x_rows = symbols*rows
    for (int i=0; i<rows; i++) strcat(symbols_x_rows, symbols);

    int indx_of_symbols;

    // make BACKGROUND (field)
    for(int r=0; r<rows; r++){
        for(int c=0; c<columns; c++){
            if(c == number_of_first_free_column || c == number_of_second_free_column){
                field[r][c] = ' ';
            } else{
                indx_of_symbols = ((rand()) % strlen(symbols_x_rows));
                field[r][c] = symbols_x_rows[indx_of_symbols];
                // remove char
                for (int i2 = indx_of_symbols; i2 < strlen(symbols_x_rows); i2++){
                    symbols_x_rows[i2] = symbols_x_rows[i2+1];
                }
            }
        }
    }
}

void down_possible(const int rows, const int columns, char field[rows][columns], int x, int y){
    if(x>columns || x<=0 || y>columns || y<=0 || x==y || field[rows-1][x-1] == ' ' || field[0][y-1] != ' '){
        return;
    }

    int index_x = x-1, index_y = y-1;
    char char_x;
    int indx_of_char;

    // take a char
    for (int i = 0; i<rows; i++){
        if (field[i][index_x] != ' '){
            indx_of_char = i;
            char_x = field[i][index_x];
            field[i][index_x] = ' ';
            break;
        }
    }

    // if column is empty put char to index [column-1](down)
    if(field[rows-1][index_y] == ' '){
        field[rows-1][index_y] = char_x;
        return;
    }

    for (int i = 0; i<rows; i++){
        if (field[i][index_y] != ' ' && field[i][index_y] == char_x){
            field[i-1][index_y] = char_x;
            break;
        } else if (field[i][index_y] != ' ' && field[i][index_y] != char_x){
            field[indx_of_char][index_x] = char_x;
            return;
        }
    }

}

bool check(const int rows, const int columns, char field[rows][columns]){
    bool is_flag = false;
    char one_row[500] = "";
    // Make a one big string of rows and put 0(delim) in the end
    for(int r=0; r<rows; r++){
        for(int c=0; c<columns; c++){
            char filedrc[] = {field[r][c], '\0'};
            strcat(one_row, filedrc);
        }
        strcat(one_row, "0");
    }


    // Make a copy of one_row
    char one_row1[500] = "";
    strcat(one_row1, one_row);


    char *p;
    char zero[2] = "0";
    // all rows
    p = strtok(one_row, zero);

    char *p1;
    // forst row
    p1 = strtok(one_row1, zero);


    while(p != NULL){
        // If first row == other rows this game colums are done
        if (strcmp(p, p1) == 0){
            is_flag = true;
        } else{
            is_flag = false;
            break;
        }
        p = strtok (NULL,zero);
    }

    return is_flag;
}

void game_field(const int rows, const int columns, char field[rows][columns]){
    initscr();

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

    int x1, y1;
    getmaxyx(stdscr, y1, x1);


    for(int r=0; r<rows; r++){
        char str[500] = "";
        sprintf(str, " %d", r+1);
        strcat(one_row, str);
        for(int c=0; c<columns; c++){
            char filedrc[] = {field[r][c], '\0'};
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

    int center_x = (x1/2)-half_of_len_x;
    int center_y = (y1/2)-half_of_len_y;

    while(p != NULL){
        mvprintw(center_y, center_x-1, "%s ", p);
        center_y++;
        p = strtok (NULL,zero);
    }

    center_x = center_x + 2;
    char under_score[150] = "";
    for (int i = 0; i<columns; i++){
        strcat(under_score, " ---");
    }
    mvprintw(center_y, center_x, "%s", under_score);

    center_y++;
    char numbers[150] = "";
    for (int i = 1; i<=columns; i++) {
        char str[150] = "";
        sprintf(str, "  %d ", i);
        strcat(numbers, str);
    }
    mvprintw(center_y, center_x, "%s", numbers);

}

void ball_sort_puzzle(){
    initscr();

    int xmax, ymax;
    getmaxyx(stdscr, ymax, xmax);

    int rows = 4, cols = 6;

    char my_field[rows][cols];
    generator(rows, cols, my_field);
    game_field(rows, cols, my_field);

    int half_of_len = 5;

    do{
        char one_row[300] = "";
        for(int r=0; r<rows; r++){
            for(int c=0; c<cols; c++){
                char filedrc[] = {my_field[r][c], '\0'};
                strcat(one_row, filedrc);
            }
            strcat(one_row, "0");
        }

        echo();
        int x, y;
        mvprintw(20, (xmax/2)-half_of_len, "Enter x: ");
        x = getch();
        x = x - '0';
        getch();


        mvprintw(21, (xmax/2)-half_of_len, "Enter y: ");
        y = getch();
        y = y - '0';
        getch();

        // clear str
        move(20, (xmax/2)-half_of_len);
        clrtoeol();
        mvaddch(20, xmax-1, '#');
        mvaddch(20, 0, '#');

        move(21, (xmax/2)-half_of_len);
        clrtoeol();
        mvaddch(21, xmax-1, '#');
        mvaddch(21, 0, '#');

        if(x==y){
            move(20, (xmax/2)-(strlen("You cannot move a character from %d columns to %d columns")/2));
            printw("You cannot move a character from %d columns to %d columns", x, y);
            getch();
            move(20, (xmax/2)-(strlen("You cannot move a character from %d columns to %d columns")/2));
            clrtoeol();
            mvaddch(20, xmax-1, '#');
            mvaddch(20, 0, '#');
        } else if (x>cols || y>cols){
            move(20, (xmax/2)-(strlen("Invalid parameters, 'x' and 'y' must be less than 'columns'")/2));
            printw("Invalid parameters, 'x' and 'y' must be less than 'columns'");
            getch();
            move(20, (xmax/2)-(strlen("Invalid parameters, 'x' and 'y' must be less than 'columns'")/2));
            clrtoeol();
            mvaddch(20, xmax-1, '#');
            mvaddch(20, 0, '#');
        } else if (my_field[rows-1][x-1] == ' '){
            move(20, (xmax/2)-(strlen("This column does not contain the symbol. Wrong 'x' variable")/2));
            printw("This column does not contain the symbol. Wrong 'x' variable");
            getch();
            move(20, (xmax/2)-(strlen("This column does not contain the symbol. Wrong 'x' variable")/2));
            clrtoeol();
            mvaddch(20, xmax-1, '#');
            mvaddch(20, 0, '#');
        } else if (my_field[0][y-1] != ' '){
            move(20, (xmax/2)-(strlen("There is no space in this column. Wrond 'y' variable")/2));
            printw("There is no space in this column. Wrond 'y' variable");
            getch();
            move(20, (xmax/2)-(strlen("There is no space in this column. Wrond 'y' variable")/2));
            clrtoeol();
            mvaddch(20, xmax-1, '#');
            mvaddch(20, 0, '#');
        } else{
            down_possible(rows, cols, my_field, x, y);

            // check if char matches or not
            char two_row[30] = "";
            for(int r=0; r<rows; r++){
                for(int c=0; c<cols; c++){
                    char filedrc[] = {my_field[r][c], '\0'};
                    strcat(two_row, filedrc);
                }
                strcat(two_row, "0");
            }

            if(strcmp(one_row, two_row) == 0){
                move(20, (xmax/2)-(strlen("The characters in the two columns must be the same")/2));
                printw("The characters in the two columns must be the same");
                getch();
                move(20, (xmax/2)-(strlen("The characters in the two columns must be the same")/2));
                clrtoeol();
                mvaddch(20, xmax-1, '#');
                mvaddch(20, 0, '#');
            }
        }

        game_field(rows, cols, my_field);


    } while (!check(rows, cols, my_field));
    noecho();
    curs_set(0);

    getch();
    clear();

    mvprintw(ymax/2, (xmax/2)-(strlen("YOU WON!")/2), "YOU WON!");

    getch();
    endwin();
}