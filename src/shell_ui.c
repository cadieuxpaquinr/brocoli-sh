#include "shell_ui.h"
#include <ncurses.h>
#include <stdlib.h>

WINDOW *winshell;

// int str_insert(char *str, int x, int base_x, int ch)
// {
//    for (int i = 0; getmaxx(winshell) > i; i--)
//    {
//       if (str[i]=='\0')
//          continue;
//       str[i+1] = str[i];
//    }
//    return 0;
// }

int test_curse()
{
   int ch, x, y, base_y, base_x, max_x;
   winshell = initscr(); /* Start curses mode 		*/
   raw();                /* Line buffering disabled	*/
   cbreak();

   keypad(stdscr, TRUE); /* We get F1, F2 etc..		*/
   noecho();             /* Don't echo() while we do getch */
   printw("Bro > ");
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
         return 0;
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
         // str_insert(str, x, base_x, ch);
         str[x - base_x] = ch;
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