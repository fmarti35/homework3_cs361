#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <signal.h>
#include <setjmp.h>
#include <fcntl.h>

jmp_buf lmao;

void sigint_handler(int sig){
  char msg[] = "\ncaught sigint\n";
  write(1, msg, sizeof(msg));
  longjmp(lmao, 4);
}

void sigin_handler(int sig){
  char msg[] = "\ncaught sigstp\n";
  write(1, msg, sizeof(msg));
  longjmp(lmao, 3);
}

void sigin_child(int sig){
  kill(getpid(), SIGTSTP);
}

void sigint_child(int sig){
  kill(getpid(), SIGINT);
}

int main() {
  signal(SIGINT, sigint_handler);
  signal(SIGTSTP, sigin_handler);

  char line[500];
  char *argsarray[20];
  char *temp[20];
  int i;
  int size;
  //jmp_buf env;

    while(1)
    {
      setjmp(lmao);

      int chk=0;
      for(i = 0; i < 20; i++)
      {
        argsarray[i] = (char* )malloc(100 * sizeof(char));
      }

      for(i = 0; i < 20; i++)
      {
        temp[i] = (char* )malloc(100 * sizeof(char));
      }
        printf("CS361 > ");
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
          //printf("arg[%d]: %s\n",i,argsarray[i]);
          int z=0;

          if(strcmp(argsarray[i],";") == 0)
          {
            //printf("Detected\n");

            for(int j=0;j<i;j++)
            {
              strcpy(temp[j],argsarray[j]);
              //printf("TEMP HAS: %s\n", temp[j]);
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
              signal(SIGINT,sigint_child);
              signal(SIGTSTP,sigin_child);
              execvp(temp[0], temp);
              exit(3);
              printf("Couldn't do command in temp\n");
            }

            //This is the parent
            else
            {
              if(pid < 0)
              {
                perror("pid: error ");
              }

              int status;
              wait(&status);
              printf("pid:%d status:%d\n", pid, WEXITSTATUS(status));
            }
            break;
          }

          if(strcmp(argsarray[i],">") == 0)
          {
            //printf("Detected the overwrite in file\n");
            //printf("The last element is: %s\n",argsarray[size-1]);
            for(int j=0;j<i;j++)
            {
              strcpy(temp[j],argsarray[j]);
              //printf("TEMP HAS: %s\n", temp[j]);
              z++;
            }

            temp[z]=NULL;

            int retval = open(argsarray[size-1], O_CREAT | O_WRONLY | O_TRUNC, S_IWUSR | S_IRUSR);

            if(retval < 0)
            {
              perror("We got a problem here!\n");
              //exit(6);
            }

            //fflush(stdout);
            dup2(retval, 1);
            chk=1;

            int pid = fork();
            if (pid == 0)//Child
            {
              signal(SIGINT,sigint_child);
              signal(SIGTSTP,sigin_child);

              execvp(temp[0], temp);
              //exit(4);
              printf("Couldn't do command in temp '<'\n");
            }

            //This is the parent
            else
            {
              close(retval);
              dup2(2,1);
              int status;
              wait(&status);
              printf("pid:%d status:%d\n", pid, WEXITSTATUS(status));
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
                //printf("HWIHDS: %s\n",temp[j]);
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
                exit(2);
                printf("Couldn't do command in temp2\n");
              }

              //This is the parent
              else
              {
                if(pid < 0)
                {
                  perror("pid: error ");
                }

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

        /*
        for(i=0;i<size;i++)
        {
          printf("ARGGGGG[%d]: %s\n",i,argsarray[i]);
        }
        */

        int pid = fork();
        if (pid == 0)//Child
        {
          signal(SIGINT,sigint_child);
          signal(SIGTSTP,sigin_child);

          execvp(argsarray[0], argsarray);
          //exit(1);
          printf("Couldn't do command\n");
        }

        //This is the parent
        else
        {
          if(pid < 0)
          {
            perror("pid: error ");
          }

          else
          {
            int status;
            wait(&status);
            printf("pid:%d status:%d\n",pid,WEXITSTATUS(status));
          }
        }
      }
}
