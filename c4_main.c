#include <ncurses.h>
#include "c4.c"
// task_12
int main(){
    initscr();
    curs_set(0);

    int xmax, ymax;
    getmaxyx(stdscr, ymax, xmax);

    int rows = 4, cols = 7;
    char board[rows][cols];
    initialize_board(rows, cols, board);
    // task_7
    print_board(rows, cols, board);

    do{
        int pl1, pl2;
        while (1) {
            mvprintw(20, (xmax/2)-(strlen("Player 1, enter your move (1-%d): ")/2), "Player 1, enter your move (1-%d): ", cols);
            pl1 = getch();
            pl1 = pl1 - '0';

            move(20, (xmax/2)-(strlen("Player 1, enter your move (1-%d): ")/2));
            clrtoeol();
            mvaddch(20, xmax-1, '#');
            mvaddch(20, 0, '#');

            if (is_valid_move(rows, cols, board, pl1) && pl1 <= cols && pl1 > 0) {
                drop_piece(rows, cols, board, pl1, 'X');
                print_board(rows, cols, board);
                break;
            } else {
                mvprintw(20, (xmax / 2) - (strlen("Sorry, this move is invalid") / 2), "Sorry, this move is invalid");
                getch();
                move(20, (xmax/2)-(strlen("Sorry, this move is invalid")/2));
                clrtoeol();
                mvaddch(20, xmax-1, '#');
                mvaddch(20, 0, '#');
            }
        }


        if(check_win(rows, cols, board, 100, 100, 'X')){
            getch();
            clear();
            mvprintw(ymax/2, xmax/2-(strlen("Player 1 wins!")/2), "Player 1 wins!");
            getch();
            endwin();
            return 0;
        }

        while (1) {
            mvprintw(20, (xmax/2)-(strlen("Player 2, enter your move (1-%d): ")/2), "Player 2, enter your move (1-%d): ", cols);
            pl2 = getch();
            pl2 = pl2 - '0';

            move(20, (xmax/2)-(strlen("Player 2, enter your move (1-%d): ")/2));
            clrtoeol();
            mvaddch(20, xmax-1, '#');
            mvaddch(20, 0, '#');

            if (is_valid_move(rows, cols, board, pl2) && pl2 <= cols && pl2 > 0) {
                drop_piece(rows, cols, board, pl2, 'O');
                print_board(rows, cols, board);
                break;
            } else {
                mvprintw(20, (xmax / 2) - (strlen("Sorry, this move is invalid") / 2), "Sorry, this move is invalid");
                getch();
                move(20, (xmax/2)-(strlen("Sorry, this move is invalid")/2));
                clrtoeol();
                mvaddch(20, xmax-1, '#');
                mvaddch(20, 0, '#');
            }
        }

        if(check_win(rows, cols, board, 100, 100, 'O')){
            getch();
            clear();
            mvprintw(ymax/2, xmax/2-(strlen("Player 2 wins!")/2), "Player 2 wins!");
            getch();
            endwin();
            return 0;
        }

    } while (!is_board_full(rows, cols, board));

    clear();
    mvprintw(ymax/2, xmax/2-(strlen("It's a draw!")/2), "It's a draw!");

    getch();
    endwin();

    return 0;
}
