#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>

int main() {
  char line[500];
  char *argsarray[20];
  char* path= "/bin/";
  //int j=0;
  char fullpath[20];
  int i;
  int size;

  for(i = 0; i < 20; i++)
    argsarray[i] = (char* )malloc(100 * sizeof(char));

    while(1)
    {
        printf("CS361 >");
        fgets(line,500,stdin);

        size_t length = strlen(line);
        if (line[length - 1] == '\n')
           line[length - 1] = '\0';

	      if(strcmp(line,"exit") == 0)
        {
          break;
        }

        char *word = strtok(line, " ");
        i=0;

        while(word)
        {
          strcpy(argsarray[i],word);
          word= strtok(NULL, " ");
          i = i + 1;
        }

        argsarray[i]=NULL;
        size=i;

        for(i=0;i<size;i++)
        {
          printf("arg[%d]: %s",i,argsarray[i]);
        }
        /*
        for(i=0;i<strlen(progpath);i++) //Deletes "\n"
        {
          if(propath[i]=="\n")
            propath[i]='/0';
        }
        */

        int pid = fork();
        if (pid == 0)//Child
        {
          execvp(argsarray[0], argsarray);
          printf("Couldn't do command\n");
        }

        //This is the parent
        else
        {
          int status;
          wait(&status);
          printf("pid: %d status: %d\n", pid,WEXITSTATUS(status));
        }

      }

}
