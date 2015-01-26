/* Alan Hsueh, MengJie Li (athsueh, mli55)*/
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

#define maxLen 20

extern char **getline();

int main() {
  int i, j;
  int pid = -1;
  int status, opt;
  int outFD, inFD;
  int fd[2];
  char **args;
  char * outF, * inF;
  char *cmd1[maxLen], *cmd2[maxLen];


  while(1) {
    args = getline();
    opt = 0;
    for(i = 0; args[i] != NULL; i++) {
      printf("Argument %d: %s\n", i, args[i]);
      if(!strcmp(args[i], ">")) {
        opt = 1;
        outF = args[i+1];
        /*printf("output file name is %s\n", outF);*/
        break;
       }
     else if(!strcmp(args[i],"<")) {
       opt = 2;
       inF = args[i+1];
     /*  printf("input file name is %s\n", inF);*/
       break;
     }
     else if(!strcmp(args[i],"|")) {
       opt = 3;
       cmd2[0] = args[i+1];
       cmd2[1] = NULL;
       /*printf("The 2nd cmd is %s\n", cmd2);*/
       break;
     }
     else
       cmd1[i] = args[i];
    }
    cmd1[i] = NULL;

/*    printf("sub_arguments\n");
    for(j = 0; cmd1[j] != NULL; j++)
         printf("%s ", cmd1[j]);
    printf("\n"); */

    if(!strcmp(args[0], "exit"))
      exit(0);

    pid = fork();
    /*printf("pid: %d, status: %d\n", pid, status);*/

    if (pid == 0) {
        switch (opt) {
                case 1:
                        outFD = open(outF, O_RDWR|O_CREAT|O_TRUNC, 777);
                       /* printf("filename %s descriptor %d",outF, outFD);*/
                        close(1);
                        dup(outFD);
                        close(outFD);
                        execvp(cmd1[0],cmd1);
                        break;
                case 2:
                        inFD = open(inF, O_RDWR);
                        close(0);
                        dup(inFD);
                        close(inFD);
                        execvp(cmd1[0],cmd1);
                        break;
                case 3:
                        pipe(fd);
                        /*printf("This is a pipe.\n"); */
                        {
                          int cpid;
                          switch (cpid = fork()) {
                            case 0:
                                    dup2(fd[0], 0);
                                    close(fd[1]);
                                    execvp(cmd2[0], cmd2);
                            default:
                                     dup2(fd[1], 1);
                                     close(fd[0]);
                                     execvp(cmd1[0], cmd1);
                          }
                        }
                        break;
                default:
                        wait(&status);
                        execvp(cmd1[0],cmd1);
        } /*end of switch*/
     } /*end of else if */
     else
       wait(&status);
  } /* end of while */
  return 0;
}
