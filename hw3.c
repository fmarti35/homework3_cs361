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
  char *temp[20];
  int i;
  int size;

    while(1)
    {
      int chk=0;
      for(i = 0; i < 20; i++)
      {
        argsarray[i] = (char* )malloc(100 * sizeof(char));
      }

      for(i = 0; i < 20; i++)
      {
        temp[i] = (char* )malloc(100 * sizeof(char));
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
          int z=0;

          if(strcmp(argsarray[i],";") == 0)
          {
            printf("Detected\n");

            for(int j=0;j<i;j++)
            {
              strcpy(temp[j],argsarray[j]);
              printf("TEMP HAS: %s\n", temp[j]);
              z++;
            }
            temp[z]=NULL;

            /*
            i=i+1;
            z=0;
            for(int j=0;i<size;i++)
            {
              argsarray[j]=argsarray[i];
              printf("Argsarray has: %s\n",argsarray[j]);
              z++;
            }
            size=z;
            argsarray[z]=NULL;
            */
            chk=1;

            int pid = fork();
            if (pid == 0)//Child
            {
              execvp(temp[0], temp);
              printf("Couldn't do command in temp\n");
            }

            //This is the parent
            else
            {
              printf("pid:%d ",pid);
              int status;
              wait(&status);
              printf("status:%d\n",WEXITSTATUS(status));
            }
            break;
          }
        }

        int j=0;
        if(chk == 1)
        {
          for(int i=0;i<size;i++)
          {
            if(strcmp(argsarray[i],";") == 0)
            {
              i=i+1;
              while(1)
              {
                temp[j]=argsarray[i];
                printf("HWIHDS: %s\n",temp[j]);
                i++;
                j++;
                if(argsarray[i]==NULL)
                {
                  break;
                }
              }
              temp[j]=NULL;
              int pid = fork();
              if (pid == 0)//Child
              {
                execvp(temp[0], temp);
                printf("Couldn't do command in temp2\n");
              }

              //This is the parent
              else
              {
                printf("pid:%d ",pid);
                int status;
                wait(&status);
                printf("status:%d\n",WEXITSTATUS(status));
              }
            }
          }

        }

        if(chk ==1)
        {
          continue;
        }

        for(i=0;i<size;i++)
        {
          printf("ARGGGGG[%d]: %s\n",i,argsarray[i]);
        }
        int pid = fork();
        if (pid == 0)//Child
        {
          execvp(argsarray[0], argsarray);
          printf("Couldn't do command\n");
        }

        //This is the parent
        else
        {
          if(pid < 0)
          {
            perror("pid: error");
          }
          else
            printf("pid:%d ",pid);

          int status;
          wait(&status);
          printf("status:%d\n",WEXITSTATUS(status));
        }

      }

}
