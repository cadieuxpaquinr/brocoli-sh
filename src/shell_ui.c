#include "shell_ui.h"
#include <ncurses.h>
#include <stdlib.h>

WINDOW *winshell;

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
   getyx(winshell, y, x);
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
   char *debug_str;
   int max_x = getmaxx(winshell);
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

char *the_curse()
{
   int ch, x, y, base_y, base_x, max_x;
   if (!winshell)
   {
      winshell = initscr(); /* Start curses mode 		*/
      raw();                /* Line buffering disabled	*/
      cbreak();
      keypad(stdscr, TRUE); /* We get F1, F2 etc..		*/
      noecho();             /* Don't echo() while we do getch */
   }

   getyx(winshell, y, x);
   mvprintw(y, 0, "Bro > ");
   getyx(winshell, base_y, base_x);
   max_x = getmaxx(winshell);
   char *str = malloc(max_x * sizeof(char));
   for (int i = 0; i < max_x; i++)
      str[i] = '\0';
   for (;;)
   {
      getyx(winshell, y, x);
      ch = getch(); /* If raw() hadn't been called
                     * we have to press enter before it
                     * gets to the program 		*/
      if (KEY_ENTER == ch || '\n' == ch || '\r' == ch)
      {
         str_insert(str, x, base_x, '\n');
         // mvprintw(y+1, 0, "\n");
         mvaddch(y, x, '\n');
         refresh();
         return str;
      }
      else if (KEY_LEFT == ch)
         move(y, x - 1);
      else if (KEY_RIGHT == ch)
         move(y, x + 1);
      else if (KEY_DOWN == ch)
         return 0;
      else if (KEY_UP == ch)
         return 0;
      else
      {
         str_insert(str, x, base_x, ch);
         mvaddstr(base_y, base_x, str);
         move(y, x + 1);
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