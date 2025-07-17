#include <fcntl.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <unistd.h>

#include "shell_ui.h"

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

#define READ_END 0
#define WRITE_END 1

int BRO_execute(char **args)
{
   pid_t pid, wpid;
   int status;
   int fd[2];
   pipe(fd);
   pid = fork();
   if (pid == 0)
   {

      close(STDOUT_FILENO);
      dup2(fd[WRITE_END], STDOUT_FILENO);
      // close(fd[READ_END]);
      // close(fd[WRITE_END]);
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
         // close(fd[READ_END]);
         // close(fd[WRITE_END]);
         wpid = waitpid(pid, &status, WUNTRACED);
      } while (!WIFEXITED(status) && !WIFSIGNALED(status));
   }
   char *buf = calloc(4096, sizeof(char));
   read(fd[READ_END], buf, 4096);
   int x, y;
   getyx(winshell, y, x);
   mvaddstr(y, x, buf);
   free(buf);
   close(fd[READ_END]);
   close(fd[WRITE_END]);
   return 1;
}

int BRO_cd(char **args);
int BRO_help(char **args);
int BRO_exit(char **args);

char *builtin_str[] = {"cd", "help", "exit"};

int (*builtin_func[])(char **) = {&BRO_cd, &BRO_help, &BRO_exit};

int BRO_num_builtins() { return sizeof(builtin_str) / sizeof(char *); }

int BRO_cd(char **args)
{
   char *p;
   if (args[1] == NULL)
   {
      args[1] = getenv("HOME");
      if (chdir(args[1]) != 0)
      {
         perror("BRO");
      }
   }
   else if ((p = strstr(args[1], "~")))
   {
      // int pos = p - args[1];
      char *temp = args[1] + strlen("~") + 1;
      char *home = getenv("HOME");
      strcat(home, "/");
      strcat(home, temp);
      args[1] = home;
   }
   if (chdir(args[1]) != 0)
   {
      perror("BRO");
   }
   return 1;
}

int BRO_exit(char **args)
{
   fprintf(stdout, "\nBro > Bye!\n");
   exit(0);
}

int BRO_help(char **args)
{
   fprintf(stderr, "\e[1;1H\e[2J");
   struct winsize w;
   ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
   for (int i = 0; i < w.ws_row; i++)
      fprintf(stderr, "\n");
   fprintf(stderr, "HEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEELP!");
   for (int i = 0; i < w.ws_row; i++)
   {
      usleep(25000);
      fprintf(stderr, "\n");
   }

   return 0;
}

int BRO_prexec(char **args)
{
   int i;

   if (args[0] == NULL)
   {
      return 1;
   }

   for (i = 0; i < BRO_num_builtins(); i++)
   {
      if (strcmp(args[0], builtin_str[i]) == 0)
      {
         return (*builtin_func[i])(args);
      }
   }
   return BRO_execute(args);
}

int BRO_loop()
{
   int status;
   // char line[SH_BUFSIZE];
   char *line;
   int err;
   char **args;
   line = the_curse();
   // err = BRO_read_line(line);
   args = BRO_split_line(line);
   BRO_prexec(args);
   // status = BRO_execute(args);
   free(line);
   free(args);
   return 0;
}

int main(int argc, char **argv)
{

   for (;;)
   BRO_loop();
      uncurse();
   fprintf(stdout, "\nBRO > Bye!\n");
   return 0;
}
