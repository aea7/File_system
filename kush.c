/**
 * KUSH shell interface program
 */

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>


#define MAX_LINE       80 /* 80 chars per line, per command, should be enough. */

int parseCommand(char inputBuffer[],char buffer[MAX_LINE], char *args[],char *leb[],int *background,int *piping,int *redirection);

int main(void)
{
  char inputBuffer[MAX_LINE]; 	        /* buffer to hold the command entered */
  char buffer[MAX_LINE];
  int background,piping,redirection;             	        /* equals 1 if a command is followed by '&' */
  char *args[MAX_LINE/2 + 1];	        /* command line (of 80) has max of 40 arguments */
  char *leb[MAX_LINE/2 + 1];
  pid_t child;            		/* process id of the child process */
  int status;           		/* result from execv system call*/
  int shouldrun = 1;


  int i, upper;

  while (shouldrun){            		/* Program terminates normally inside setup */
    background = 0;

    shouldrun = parseCommand(inputBuffer,buffer,args,leb,&background,&piping,&redirection);       /* get next command */

    if (strncmp(inputBuffer, "exit", 4) == 0)
      shouldrun = 0;     /* Exiting from kush*/

    if (shouldrun) {

        if(redirection == 1){
          char path[]  = "/usr/bin/";
          char path2[] = "/bin/";
          strcat(path,args[0]);
          strcat(path2,args[0]);

          child=fork();
          if(child == 0){
            //child process
            int out = open(leb[0],O_WRONLY|O_CREAT|O_TRUNC,0666);
            dup2(out,STDOUT_FILENO);
            close(out);

            if( access( path, F_OK ) != -1 ) {
                execv(path,args);
          } if( access( path2, F_OK ) != -1 ) {
              execv(path2,args);
          }
            }
           else if(child > 0){
            //parent process
            if(background != 1){
              wait(NULL);
            }
          }

        } else if (redirection == 2){

          char path[]  = "/usr/bin/";
          char path2[] = "/bin/";
          strcat(path,args[0]);
          strcat(path2,args[0]);

          child=fork();
          if(child == 0){
            //child process
            int out = open(leb[0],O_WRONLY|O_CREAT|O_APPEND,0666);
            dup2(out,STDOUT_FILENO);
            close(out);

            if( access( path, F_OK ) != -1 ) {
                execv(path,args);
          } if( access( path2, F_OK ) != -1 ) {
              execv(path2,args);
          }
            }
           else if(child > 0){
            //parent process
            if(background != 1){
              wait(NULL);
            }
          }

        }
        else if(piping == 1){

          pid_t pid,pid2;
          int fd[2];
          pid = fork();

            if(pid == 0){
              pipe(fd);
              pid2 = fork();

              if(pid2 == 0){

                  close(fd[0]);
                  dup2(fd[1], 1);
                  close(fd[1]);
                  /* exec ls */
                  char path[]  = "/usr/bin/";
                  char path2[] = "/bin/";
                  strcat(path,args[0]);
                  strcat(path2,args[0]);

                    if( access( path, F_OK ) != -1 ) {
                        execv(path,args);
                  } if( access( path2, F_OK ) != -1 ) {
                      execv(path2,args);
                  }
                }
              if(pid2 > 0){
                  close(fd[1]);
                  dup2(fd[0], 0);
                  close(fd[0]);
                  /* exec tr */
                  char path3[]  = "/usr/bin/";
                  char path4[] = "/bin/";
                  strcat(path3,leb[0]);
                  strcat(path4,leb[0]);

                  if( access( path3, F_OK ) != -1 ) {
                      execv(path3,leb);
                } if( access( path4, F_OK ) != -1 ) {
                      execv(path4,leb);
                }
              }
            }
              if(pid > 0){
              if(background != 1){
                wait(NULL);
              }}

}else if (piping != 1){

  if(strcmp(args[0],"trash") == 0){
    pid_t childpid2;
    childpid2=fork();

    if (childpid2)   /*parent process*/
    {
      if(background != 1){
        wait(NULL);
      }
    }

    if (childpid2==0)  //child process
    {
        if(strcmp(args[1],"-l") == 0){
              printf("yepp1");
            char cronPath[] = "/usr/bin/crontab";
	          char *args[40];

            args[0] = cronPath;
	          args[1] = "-l";
	          args[2]=NULL;

	          if( access( cronPath, F_OK ) != -1 ) {
        	execv(cronPath,args);
        }
        }else if(strcmp(args[1],"-r") == 0){
          char sedPath[] = "/bin/sed";
        	char *argb[55];

        	char argv[] = "\\|";
        	char argv2[] = "| d";
        	char yep[45];

        	strcat(argv,args[2]);
        	strcpy(yep,argv);
        	strcat(yep,argv2);
          printf("arg2 : %s\n",yep);
        	argb[0] = sedPath;
         	argb[1]="-i";
        	argb[2]= yep;
        	argb[3] = "/home/james/Desktop/cronos.txt";
        	argb[4] = NULL;

          if( access( sedPath, F_OK ) != -1 ) {

            pid_t childpid3;
            childpid3=fork();

            if (childpid3)   /*parent process*/
            {
                wait(NULL);
                char cronPath[] = "/usr/bin/crontab";
                char textPath[] = "/home/james/Desktop/cronos.txt";
                char *args11[41];

                args11[0]=cronPath;
                args11[1]=textPath;
                args11[2]= NULL;

                if( access( cronPath, F_OK ) != -1 ) {
                        execv(cronPath,args11);
                      }
              }

              if (childpid3==0)  //child process
              {
                  execv(sedPath,argb);
                }
              }
        }else{
      char *h[4];
      char *m[4];
      split(args[1],h,m);
      trash(*m,*h,args[2],args);
      }
    }
  } else if(strcmp(args[0],"put") == 0){

    pid_t childpid4;
    childpid4=fork();

    if (childpid4)   /*parent process*/
    {
      if(background != 1){
        wait(NULL);
      }
    }

    if(childpid4 == 0){
    char commandPath[] = "/bin/sed";
  	char sedCommand[100];
  	char *args22[40];
  	time_t t;
  	struct tm * timeinfo;

  	time (&t);
  	timeinfo = localtime (&t);
   //  put date path
   //  put "text" path
   //  put -e "öncesi" "sonrasi" path

  	//$ sed -i '1s/^/added text/' file
  	strcpy(sedCommand, "1s/^/");
    if(strcmp(args[1],"date") == 0){
  	  strcat(sedCommand, asctime (timeinfo));
      sedCommand[strlen(sedCommand)-1] = '\0'; //get rid of \n at the end of asctime
    }else if(strcmp(args[1],"-e") == 0){
      replace(args[2],args[3],args[4]);
    } else {
      strcat(sedCommand,args[1]);
    }
  	strcat(sedCommand, "\\n/");

  	args22[0] = commandPath;
  	args22[1] ="-i";
  	args22[2] = sedCommand;
  	args22[3] = args[2];
  	args22[4] = NULL;

  	if( access( commandPath, F_OK ) != -1 ) {
          	execv(commandPath,args22);
          }
  }
} else if(strcmp(args[0],"schedInfo") == 0){
//    sudo insmod myfileinfo.ko processID=769 processSPolicy=2 processPrio=50
//    sudo insmod schedInfo.ko processID=769 processSPolicy=2 processPrio=50
    char path[]  = "/usr/bin/sudo"; //sudo path
    char Cpid[] = "processID=";
    char Cpolicy[] = "processSPolicy=";
    char CprocessPrio[] = "processPrio=";

    int pid = atoi(args[1]);
    int policy = atoi(args[2]);
    int prio = atoi(args[3]);
    args[0] = path;

    strcat(CprocessPrio,args[3]);
    strcat(Cpolicy,args[2]);
    strcat(Cpid,args[1]);

    args[5] = CprocessPrio;
    args[4] = Cpolicy;
    args[3] = Cpid;
    args[1] = "insmod";
    args[2] = "schedInfo.ko";
    args[6] = NULL;

    pid_t childsched;
    childsched=fork();

    if (childsched)
    {
      if(background != 1){
        wait(NULL);
      }
    }

    if(childsched == 0){
      execv(path,args);
    }


}
  else{
  child = fork();
  char path[]  = "/usr/bin/";
  char path2[] = "/bin/";
  strcat(path,args[0]);
  strcat(path2,args[0]);

  if(child == 0){
    //child process
    if( access( path, F_OK ) != -1 ) {
        execv(path,args);
  } else   if( access( path2, F_OK ) != -1 ) {
    execv(path2,args);
  } else{
    if(strcmp(args[0],"cd") == 0){
          cd(args[1]);
      }}}
   else if(child > 0){
    //parent process
    if(background != 1){
      wait(NULL);
    }
  }

}
    }
  } }
  return 0;
 }

 int replace(char *old, char *new, char *path){
	char commandPath[] = "/bin/sed";
	char sedCommand[100];
	char *args[40];

	//$ sed 's/Linux/Linux-Unix/' thegeekstuff.txt
	strcpy(sedCommand, "s/");
	strcat(sedCommand, old);
	strcat(sedCommand, "/");
	strcat(sedCommand, new);
	strcat(sedCommand, "/g");

	args[0] = commandPath;
	args[1] = "-i";
	args[2] = sedCommand;
	args[3] = path;
	args[4] = NULL;

	if( access( commandPath, F_OK ) != -1 ) {
        	execv(commandPath,args);
        }

	return 1;
}

 int split(char str[],char *h[],char *m[]){

 	const char s[2] = ".";
 	char *token;
 	int i = 0;
 	/* get the first token */
 	token = strtok(str, s);
 	/* walk through other tokens */

 	while(token != NULL){
 		if(i==0){
 			*h = token;
 			i++;
 		}else{
 			*m = token;
 		}
 		token = strtok(NULL, s);
 	}
 	return 0;
 }

int cd(char *pth){
    char path[MAX_LINE];
    strcpy(path,pth);

    char cwd[MAX_LINE];
    if(pth[0] != '/')
    {// true for the dir in cwd
        getcwd(cwd,sizeof(cwd));
        strcat(cwd,"/");
        strcat(cwd,path);
        chdir(cwd);
    }else{// if at root
        chdir(pth);
    }

    return 0;
}


int trash(char *minute, char *hour, char path[],char *argz[]){
  //char *crString = "* * * * * find /home/kerem/Pictures -type f -delete";
  FILE *f;
  char cronString[100];
  char cronPath[] = "/usr/bin/crontab";
  char textPath[] = "/home/james/Desktop/cronos.txt";
  char *args[41];

  strcpy(cronString,minute);
  strcat(cronString, " ");
  strcat(cronString, hour);
  strcat(cronString, " * * * find ");
  strcat(cronString, path);
  strcat(cronString, " -type f -delete");
  args[0]=cronPath;
  args[1]=textPath;
  args[2]= NULL;


  f = fopen(textPath,"a");
  printf("%s \n",cronString);
  fprintf(f,"%s\n",cronString);
  fclose(f);


    if( access( cronPath, F_OK ) != -1 ) {
            execv(cronPath,args);
          }

        return 1;
}

/**
 * The parseCommand function below will not return any value, but it will just: read
 * in the next command line; separate it into distinct arguments (using blanks as
 * delimiters), and set the args array entries to point to the beginning of what
 * will become null-terminated, C-style strings.
 */

int parseCommand(char inputBuffer[],char buffer[], char *args[],char *leb[],int *background,int *piping,int *redirection)
{
    int length,		/* # of characters in the command line */
      i,		/* loop index for accessing inputBuffer array */
      start,		/* index where beginning of next command parameter is */
      ct,lt,	        /* index of where to place the next parameter into args[] */
      command_number;	/* index of requested command number */

    ct =0,lt = 0,*piping = 0,*redirection = 0;

    /* read what the user enters on the command line */
    do {
	  printf("kush>");
	  fflush(stdout);
	  length = read(STDIN_FILENO,inputBuffer,MAX_LINE);
    strcpy(buffer,inputBuffer);
    }
    while (inputBuffer[0] == '\n'); /* swallow newline characters */

    /**
     *  0 is the system predefined file descriptor for stdin (standard input),
     *  which is the user's screen in this case. inputBuffer by itself is the
     *  same as &inputBuffer[0], i.e. the starting address of where to store
     *  the command that is read, and length holds the number of characters
     *  read in. inputBuffer is not a null terminated C-string.
     */
    start = -1;
    if (length == 0)
      exit(0);            /* ^d was entered, end of user command stream */

    /**
     * the <control><d> signal interrupted the read system call
     * if the process is in the read() system call, read returns -1
     * However, if this occurs, errno is set to EINTR. We can check this  value
     * and disregard the -1 value
     */

    if ( (length < 0) && (errno != EINTR) ) {
      perror("error reading the command");
      exit(-1);           /* terminate with error code of -1 */
    }
    /**
     * Parse the contents of inputBuffer
     */
    for (i=0;i<length;i++) {
      /* examine every character in the inputBuffer */
      switch (inputBuffer[i]){
      case ' ':
      case '\t' :               /* argument separators */
	if(start != -1){
    if(*redirection == 1 || *redirection == 2){
      leb[lt] = &inputBuffer[start];
      lt++;
      args[ct] = NULL;
    }
    else if(*piping == 1){
      leb[lt] = &inputBuffer[start];
      lt++;
      args[ct] = NULL;
    } else{
        args[ct] = &inputBuffer[start];    /* set up pointer */
    }
	  ct++;
	}
	inputBuffer[i] = '\0'; /* add a null char; make a C string */
	start = -1;
	break;

      case '\n':                 /* should be the final char examined */
	if (start != -1){
    if(*redirection == 1 || *redirection == 2){
      leb[lt] = &inputBuffer[start];
      lt++;
      args[ct] = NULL;
    }
    else if(*piping == 1){
      leb[lt] = &inputBuffer[start];
      lt++;
      args[ct] = NULL;
    } else{
        args[ct] = &inputBuffer[start];    /* set up pointer */
    }
	  ct++;
	}
	inputBuffer[i] = '\0';
	args[ct] = NULL; /* no more arguments to this command */
  leb[lt] = NULL;
	break;

      default :             /* some other character */

      if(inputBuffer[i] == '|'){
        *piping = 1;
        break;
      }

      if(inputBuffer[i] == '>' && *redirection != 1){
        *redirection = 1;
        break;
      }

      if(inputBuffer[i] == '>' && *redirection == 1){
        *redirection = 2;
        break;
      }

	if (start == -1)
	  start = i;
	if (inputBuffer[i] == '&') {
	  *background  = 1;
	  inputBuffer[i-1] = '\0';
	}

      } /* end of switch */
    }    /* end of for */

    /**
     * If we get &, don't enter it in the args array
     */

    if (*background)
      args[--ct] = NULL;

    args[ct] = NULL; /* just in case the input line was > 80 */

    return 1;

} /* end of parseCommand routine */
