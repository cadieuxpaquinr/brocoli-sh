#include "shell_ui.h"
#include <ncurses.h>
#include <stdlib.h>

// TODO: add structs containing repetitive data
//* like base_x, base_y, maybe struct str, etc.

enum TYPE
{
   integer,
   string,
   character
};
/* usage
int *whatever;
*whatever = (x - base_x);
helper_debug((void *)whatever, integer);
*/
int helper_debug(void *data, enum TYPE type)
{
   int y, x;
   getyx(stdscr, y, x);
   if (type == string)
   {
      char *debug_str = (char *)data;
      mvprintw(y + 1, 0, "        ");
      mvprintw(y + 1, 0, "%s", debug_str);
   }
   else if (type == integer)
   {
      int *debug_str = (int *)data;
      mvprintw(y + 1, 0, "        ");
      mvprintw(y + 1, 0, "%d", *debug_str);
   }
   else if (type == character)
   {
      char *debug_str = (char *)data;
      mvprintw(y + 1, 0, "        ");
      mvprintw(y + 1, 0, "%c", debug_str[0]);
   }

   return 0;
}

int str_insert(char *str, int x, int base_x, int ch)
{
   int max_x = getmaxx(stdscr);
   for (int i = max_x; i > (x - base_x); i--)
   {
      if (i != 0)
         if (str[i] == '\0')
            continue;
      str[i + 1] = str[i];
   }
   str[x - base_x] = ch;
   return 0;
}

int str_remove(char *str, int x, int base_x)
{
   int max_x = getmaxx(stdscr);
   for (int i = (x - base_x - 1); i < max_x; i++)
   {
      str[i] = str[i + 1];
      if (str[i] == '\0')
         break;
   }
   return 0;
}

int str_delete(char *str, int x, int base_x)
{
   int max_x = getmaxx(stdscr);
   for (int i = (x - base_x + 1); i < max_x; i++)
   {
      str[i] = str[i + 1];
      if (str[i] == '\0')
         break;
   }
   return 0;
}

char *the_curse()
{
   int ch, x, y, base_y, base_x, max_x;
   if (!stdscr)
   {
      initscr(); /* Start curses mode 		*/
      scrollok(stdscr, TRUE);
      raw();                /* Line buffering disabled	*/
      cbreak();
      keypad(stdscr, TRUE); /* We get F1, F2 etc..		*/
      noecho();             /* Don't echo() while we do getch */
   }

   getyx(stdscr, y, x);
   mvprintw(y, 0, "Bro > ");
   getyx(stdscr, base_y, base_x);
   max_x = getmaxx(stdscr);
   //! `str` should be a struct or part of a struct
   // TODO: refactor `str` etc.
   char *str = malloc((max_x + 1) * sizeof(char));
   for (int i = 0; i < max_x + 1; i++)
      str[i] = '\0';
   for (;;)
   {
      getyx(stdscr, y, x);
      ch = getch(); /* If raw() hadn't been called
                     * we have to press enter before it
                     * gets to the program 		*/
      switch (ch)
      {
      case KEY_ENTER:
      case '\n':
      case '\r':
         str_insert(str, x, base_x, '\n');
         // mvprintw(y+1, 0, "\n");
         mvaddch(y, x, '\n');
         refresh();
         return str;
      case KEY_LEFT:
         move(y, x - 1);
         break;
      case KEY_RIGHT:
         move(y, x + 1);
         break;
      case KEY_DOWN:
         return 0;
      case KEY_UP:
         return 0;
      case KEY_BACKSPACE: // TODO: write better code
         str_remove(str, x, base_x);
         move(base_y, base_x);
         for (int i = 0; i < max_x - base_x; i++)
            addch(' ');
         mvaddstr(base_y, base_x, str);
         move(y, x - 1);
         refresh();
         break;
      case KEY_DC: // TODO: write better code
         str_delete(str, x, base_x);
         move(base_y, base_x);
         for (int i = 0; i < max_x - base_x; i++)
            addch(' ');
         mvaddstr(base_y, base_x, str);
         move(y, x - 1);
         refresh();
         break;
      default:
         str_insert(str, x, base_x, ch);
         mvaddstr(base_y, base_x, str);
         move(y, x + 1);
         break;
      }
   }
   refresh(); /* Print it on to the real screen */
   return 0;
}

int uncurse()
{
   endwin(); /* End curses mode		  */
   return 0;
}