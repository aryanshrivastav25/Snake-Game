#include <stdio.h>
#include <ncurses.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int play_game(void);
void game_over(WINDOW *win, int max_y, int max_x, int score);
void set_ball(WINDOW* win, int max_y, int max_x, int curr_y, int curr_x, int *rand_y, int *rand_x);
void instructions(void);
void record_scores(char *name, int score);
void display_score(void);

int main(void)
{
    initscr();
    int col, rows;
    getmaxyx(stdscr, col, rows);
    endwin();

    char *welcome = "Welcome to snake game";
    int length = strlen(welcome);
    for (int i = 0; i < rows; i++)
    {
        printf("#");
    }
    printf("\n");

    for (int i = 0; i < rows / 3; i++)
    {
        printf("#");
    }
    printf(" %s ", welcome);

    for (int i = 0; i < (rows - (rows / 3) - length - 2); i++)
    {
        printf("#");
    }
    printf("\n");

    for (int i = 0; i < rows; i++)
    {
        printf("#");
    }
    printf("\n\n\n");

    char name[50];
    printf("Enter your name: ");

    // to prevent buffer between printf and scanf
    fflush(stdout);

    scanf("%s", name);
    printf("Hello, %s\n", name);
    
    int choice;
    printf("Press 1 to play.\nPress 2 to view high scores.\nPress 3 to see instructions.\n");
    fflush(stdout);
    scanf("%i", &choice); 
    
    int score;

    switch (choice)
    {
        case 1: score = play_game();
                record_scores(name, score);
                break;
        
        case 2: display_score();
                break;

        case 3: instructions();
                break;
            
        default: printf("Invalid input.\n");
                 break;
    }
}

int play_game(void)
{
    initscr();
    noecho();

    WINDOW *win = newwin(25, 120, 0, 0);
    nodelay(win, true);
    refresh();
    box(win, 0, 0);
    wrefresh(win);

    wmove(win, 1, 1);
    wrefresh(win);

    keypad(win, true);
    int curr_y, curr_x, beg_y, beg_x, max_y, max_x, curr_ball_y, curr_ball_x;

    getbegyx(win, beg_y, beg_x);
    getmaxyx(win, max_y, max_x);
    getyx(win, curr_y, curr_x);
    
    int score = 0;
    int i = 1, j = 0;

    set_ball(win, max_y, max_x, curr_y, curr_x, &curr_ball_y, &curr_ball_x);

    while(true)
    {   
        getyx(win, curr_y, curr_x);
        wmove(win, curr_y + j, curr_x + i);
        int check = wgetch(win);
        
        wrefresh(win);
        if (curr_y == 0 || curr_x == 0 || curr_y == max_y - 1 || curr_x == max_x - 1)
        {   
            refresh();
            game_over(win, max_y, max_x, score);
            refresh();
            break;
        }

        if (curr_ball_y == curr_y && curr_ball_x == curr_x)
        {   
            mvwaddch(win, curr_ball_y, curr_ball_x, ' ');
            set_ball(win, max_y, max_x, curr_y, curr_x, &curr_ball_y, &curr_ball_x);
            score++;
            wrefresh(win);
        }

        switch (check)
        {
            case KEY_UP: 
                j = -1, i = 0;
                break;

            case KEY_DOWN:
                j = 1, i = 0;
                break;
            
            case KEY_LEFT:
                j = 0; i = -1;
                break;

            case KEY_RIGHT:
                j = 0; i = 1;
                break;
        }
        usleep(100000);
    }

    getch();
    endwin();

    return score;
}

void instructions(void)
{
    printf("1. Use the UP/DOWN/LEFT/RIGHT key to move the cursor up/down/left right respectively.\n"
        "2. Do not touch the border.\n"
        "3. Grab the balls and increase your score.\n"
        "4. Your score will be recorded.\n");
}

void game_over(WINDOW *win, int max_y, int max_x, int score)
{
    clear();
    move(max_y / 2, max_x / 2);

    start_color();
    init_pair(1, COLOR_WHITE, COLOR_MAGENTA);

    attron(COLOR_PAIR(1));
    printw("Game over");
    move((max_y / 2) + 1, max_x / 2);
    printw("Your score is %i", score);
    attroff(COLOR_PAIR(1));
    return;
}

void set_ball(WINDOW *win, int max_y, int max_x, int curr_y, int curr_x, int *rand_y, int *rand_x)
{
    *rand_y = rand() % (max_y - 1);
    *rand_x = rand() % (max_x - 1);

    if (*rand_y == 0)
    {
        *rand_y = 1;
    }
    if (*rand_x == 0)
    {
        *rand_x = 1;
    }

    mvwprintw(win, *rand_y, *rand_x, "B");
    wmove(win, curr_y, curr_x);
    return;
}

void record_scores(char *name, int score)
{
    FILE *f = fopen("scores.csv", "a");
    
    char str_score[5];
    sprintf(str_score, "%i", score);
    fprintf(f, "%s, %s\n", name, str_score);
    fclose(f);

    return;
}

void display_score(void)
{
    FILE *f = fopen("scores.csv", "r");

    char row[100];
    while(!feof(f))
    {
        fread(&row, sizeof(row), 1, f);
        printf("%s", row);
    }

    fclose(f);
    return;
}
