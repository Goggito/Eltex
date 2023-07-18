#include <curses.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <termios.h>

#define FILE_NAME_LEN 150
#define MAX_STRING_LEN 255

struct winsize size;


void sig_winch(int signo) {
  ioctl(fileno(stdout), TIOCGWINSZ, (char *)&size);
  resizeterm(size.ws_row, size.ws_col);
}

void symbol_restor(WINDOW *win, int x, int y, char ch) {
  wmove(win, y, x);
  wprintw(win, "%c", ch);
}

int char_calc(int x, int y, char *buff, char *ch) {
  int index = 0;
  int string_len = 0;
  int string_index = 0;

  // for (int i = 0; i < y + scrollOffset; i++) {
  for (int i = 0; i < y; i++) {
    string_len = 0;
    while (buff[index] != '\n' && buff[index] != '\0') {
      index++;
    }
    index++;
  }

  string_index = index;
  string_len = 0;
  while (buff[string_index] != '\n' && buff[string_index] != '\0') {
    string_index++;
    string_len++;
  }

  if (x < string_len) {
    index += x;
    *ch = buff[index];
    return index;
  } else {
    *ch = ' ';
    return -1;
  }
}

void symbol_delet(WINDOW *win, int x, int y, char *buff) {
  char plug;
  int index = char_calc(x, y, buff, &plug);

  if (x > 0) {
    wmove(win, y, x - 1);
    wprintw(win, " ");
    buff[index - 1] = ' ';
    wmove(win, y, x);
  }
}

void symbol_enter(WINDOW *win, int x, int y, char ch, char *buff) {
  char plug;
  int index = char_calc(x, y, buff, &plug);
  if (x >= 0) {
    wmove(win, y, x);
    wprintw(win, "%c", ch);
    buff[index] = ch;
    wmove(win, y, x);
  }
}

int main(int argc, char **argv) {
  WINDOW *main_window;
  WINDOW *text_window;
  WINDOW *menu_window;
  
  FILE *fp;
  char filename[FILE_NAME_LEN];
  char file_buffer[1000000];
  
  int curs_y = 0;
  int curs_x = 0;


  printf("ENTER FILE NAME: ");
  scanf("%s", filename);
  fp = fopen(filename, "r");
  if (fp == NULL) {
    printf("Ошибка открытия файла.");
    return 1;
  }

  signal(SIGWINCH, sig_winch);

  initscr();
  cbreak();
  noecho();
  curs_set(FALSE);
  start_color();
  refresh();

  init_pair(1, COLOR_WHITE, COLOR_BLACK);
  init_pair(2, COLOR_YELLOW, COLOR_BLUE);

  ioctl(fileno(stdout), TIOCGWINSZ, (char *)&size);
  main_window = newwin(size.ws_row, size.ws_col, 0, 0);

  text_window = derwin(main_window, size.ws_row - 3, size.ws_col, 0, 0);
  wbkgd(text_window, COLOR_PAIR(1));
  wattron(text_window, A_BOLD);

  menu_window = derwin(main_window, 3, size.ws_col, size.ws_row - 3, 0);
  wbkgd(menu_window, COLOR_PAIR(2));
  wattron(menu_window, A_BOLD);

  
  fread(file_buffer, sizeof(char), sizeof(file_buffer) - 1, fp);
  file_buffer[sizeof(file_buffer) - 1] = '\0';
  wprintw(text_window, "%s", file_buffer);
  fclose(fp);


  keypad(stdscr, TRUE);
  while (1) {
    char char_save;
    char_calc(curs_x, curs_y, file_buffer, &char_save);

    wmove(menu_window, 0, 0);
    wprintw(menu_window, "%d %d ch - %c (%d)\n", curs_x, curs_y, char_save,
            char_save);
    wattron(text_window, A_REVERSE);
    wmove(text_window, curs_y, curs_x);
    wprintw(text_window, "%c", char_save);
    wattroff(text_window, A_REVERSE);

    wrefresh(text_window);
    wrefresh(menu_window);
    wrefresh(main_window);

    int ch = getch();
    switch (ch) {
      case 27:
        endwin();
        return 0;
        break;


      case KEY_F(5):
        fp = fopen(filename, "w");
        if (fp == NULL) {
          wprintw(menu_window, "Ошибка открытия файла.");
          return 1;
        }
        fprintf(fp, "%s", file_buffer);
        fclose(fp);
        break;

      case KEY_BACKSPACE:
        symbol_delet(text_window, curs_x, curs_y, file_buffer);
        if (curs_x > 0) {
          symbol_restor(text_window, curs_x, curs_y, char_save);
          curs_x--;
        }
        break;

      case KEY_UP:
        if (curs_y > 0) {
          symbol_restor(text_window, curs_x, curs_y, char_save);
          curs_y--;
        }
        break;

      case KEY_DOWN:
        if (curs_y < size.ws_row - 4) {
          symbol_restor(text_window, curs_x, curs_y, char_save);
          curs_y++;
        }
        break;

      case KEY_LEFT:
        if (curs_x > 0) {
          symbol_restor(text_window, curs_x, curs_y, char_save);
          curs_x--;
        }
        break;

      case KEY_RIGHT:
        if (curs_x < size.ws_col - 1) {
          symbol_restor(text_window, curs_x, curs_y, char_save);
          curs_x++;
        }
        break;

      default:
        if (ch >= 32 && ch <= 127){
          symbol_enter(text_window, curs_x, curs_y, ch, file_buffer);
          curs_x++;
        }
        break;
    }

  }

  delwin(text_window);
  delwin(menu_window);
  delwin(main_window);

  refresh();
  gets();

  endwin();
  exit(EXIT_SUCCESS);
}
