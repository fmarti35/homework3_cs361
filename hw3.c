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
  //char* path= "/bin/";
  //int j=0;
  char fullpath[20];
  int i;
  int size;

/*
  for(i = 0; i < 20; i++)
  {
    argsarray[i] = (char* )malloc(100 * sizeof(char));
  }
*/
    while(1)
    {
      for(i = 0; i < 20; i++)
      {
        argsarray[i] = (char* )malloc(100 * sizeof(char));
      }
        printf("CS361 >");
        fgets(line,500,stdin);

        size_t length = strlen(line);
        if (line[length - 1] == '\n')
           line[length - 1] = '\0';

	      if(strcmp(line,"exit") == 0)
        {
          for(i=0;i<20;i++)
          {
            if(argsarray[i] == NULL)
            {
              free (argsarray[i]);
            }
          }
          break;
        }

        char *word = strtok(line, " ");
        i=0;

        //if()

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
          printf("arg[%d]: %s\n",i,argsarray[i]);
        }

        int pid = fork();
        if (pid == 0)//Child
        {
          int childpid=getpid();
          execvp(argsarray[0], argsarray);
          printf("Couldn't do command\n");
        }

        //This is the parent
        else
        {
          if(childpid!=pid)
          {
            printf("Pid is different");
          }
          printf("pid:%d ",pid);
          int status;
          wait(&status);
          printf("status:%d\n",WEXITSTATUS(status));
        }

      }

}
