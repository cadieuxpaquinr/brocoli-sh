#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SH_BUFZISE 1024

int BRO_read_line(char *line)
{

   if (!fgets(line, SH_BUFZISE, stdin))
      // error
      return 1;
   // all good
   return 0;
}
char **BRO_split_line(char *line)
{
   fprintf(stderr, "BRO_split_line()\n");
   return 0;
}
int BRO_execute(char **args)
{
   fprintf(stderr, "BRO_execute()\n");
   return 0;
}

int BRO_loop()
{
   char **args;
   int status;
   char line[SH_BUFZISE];
   int err;
   do
   {
      fprintf(stderr, "Bro > ");
      err = BRO_read_line(line);
      fprintf(stdout, "%s\n", line);
      //   args = BRO_split_line(line);
      //   status = BRO_execute(args);
      free(args);
      usleep(100000);
   } while (status);
   return 0;
}

int main(int argc, char **argv)
{
   for (;;)
      BRO_loop();
   fprintf(stdout, "\nBRO > Bye!\n");
   return 0;
}
