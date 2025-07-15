#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define SH_BUFSIZE 512
#define SH_MAX_TOKENS 64

int BRO_read_line(char *line)
{

   if (!fgets(line, SH_BUFSIZE, stdin))
      // error
      return 1;
   // all good
   return 0;
}
char **BRO_split_line(char *line)
{
   char *delims = " ";
   char *str = line;
   char *ret = NULL;
   int offset = 0;
   char **args = malloc(SH_MAX_TOKENS * sizeof(char *));
   for (int i = 0; i < SH_MAX_TOKENS; i++)
   {
      str = line;
      if (ret)
         offset += strlen(ret) + 1;
      str += offset;
      ret = strtok(str, delims);
      args[i] = str;
      if (strstr(args[i], "\n"))
      {
         args[i][strlen(args[i]) - 1] = '\0';
         break;
      }
   }

   return args;
}

int BRO_execute(char **args)
{
   pid_t pid, wpid;
   int status;

   pid = fork();
   if (pid == 0)
   {
      // Child process
      if (execvp(args[0], args) == -1)
      {
         perror("BRO");
      }
      exit(EXIT_FAILURE);
   }
   else if (pid < 0)
   {
      // Error forking
      perror("BRO");
   }
   else
   {
      // Parent process
      do
      {
         wpid = waitpid(pid, &status, WUNTRACED);
      } while (!WIFEXITED(status) && !WIFSIGNALED(status));
   }

   return 1;
}

int BRO_loop()
{
   int status;
   char line[SH_BUFSIZE];
   int err;
   char **args;
   fprintf(stdout, "Bro > ");
   err = BRO_read_line(line);
   args = BRO_split_line(line);
   status = BRO_execute(args);
   return 0;
}

int main(int argc, char **argv)
{
   // for (;;)
   BRO_loop();
   fprintf(stdout, "\nBRO > Bye!\n");
   return 0;
}
