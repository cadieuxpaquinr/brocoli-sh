#include "shell_data_structures.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
int BRO_split_line(char *line, queue *q)
{
   char *delims = " ";
   char *str;
   char *ret = NULL;
   int offset = 0;
   while (1)
   {
      str = line;
      if (ret)
         offset += strlen(ret) + 1;
      str = str + offset;
      ret = strtok(str, delims);
      if (strstr(str, "\n"))
      {
         ret = strtok(str, "\n");
         queue_push(q, str); // saves token
         break;
      }
      queue_push(q, str); // saves token
   }
   return 0;
}
int BRO_execute(queue *args)
{
   char *command = malloc(1024*sizeof(char));
   strcpy(command,""); //init mallocd value to suppress warning
   while (!queue_is_empty(args))
   {
      char *temp_str = queue_pop(args);
      strcat(command, temp_str);
      strcat(command, " ");
      free(temp_str);
   }
   // fprintf(stderr, "Command: %s\n", command); //! keep for debug output
   int err = system(command);
   free(command);
   return err;
}

int BRO_loop()
{
   int status;
   char line[SH_BUFZISE];
   int err;
   queue args;
   queue_initialize(&args);
   do
   {
      fprintf(stdout, "Bro > ");
      err = BRO_read_line(line);
      err = BRO_split_line(line, &args);
      status = BRO_execute(&args);
   } while (status);
   return 0;
}

int main(int argc, char **argv)
{
   // for (;;)
      BRO_loop();
   fprintf(stdout, "\nBRO > Bye!\n");
   return 0;
}
