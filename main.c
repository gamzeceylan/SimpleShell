// C Program to design a shell in Linux
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include <sys/stat.h>
#include<sys/wait.h>
#include <errno.h>
#include <assert.h>
#include <dirent.h>
#include <fcntl.h>
#include <ctype.h>
#include <time.h>

#define FORMAT "%5s %s\t%8s %s\n"

#define MAX_COMMAND 80 // girilecek komutun max karakter sayısı
#define MAXLIST 10 // girilecek komut max 10 kelime içerebilir

void init_shell();
int built_in_commands(char** parsed);
void execution(char** parsed);
void parseSpace(char* str, char** parsed);
int processString(char* str, char** parsed);
void showpid();

int main()
{
	while (1) {
		init_shell();
	}
	return 0;
}

// Kabuğumuzu başlatıyoruz
void init_shell()
{
	printf("\e[1;1H\e[2J"); // terminali temizler
	int flag;
	pid_t process_id;
	char s[100];


	while (1) {

		printf("\33[0;34m"); // renk
			// bulundugumuz dizini gösterir
		printf("%s/", getcwd(s, 100));
		printf(": sau> ");
		printf("\33[0m"); // renk

		char input[MAX_COMMAND], * parsedArgs[MAXLIST];
		scanf(" %[^\n]", input);

		if (strlen(input) > MAX_COMMAND) { // 80 karakterden fazla komut girilirse
			fprintf(stderr, "Too long command. Max command character size is 80.\n");

		}

		else if (feof(stdin)) { // dosya sonu ve okunacak bir şey kalmadı
			perror("Exiting shell ");
			exit(0);
		}

		else {
			flag = processString(input, parsedArgs);
			if (flag == 1) {	//eğer flag 1 ise gelen komut builtin komut değildir bu yüzden executiona göndeririz
				execution(parsedArgs);
			}
		}
	}
	printf("\n");
}

void showpid() //Arkaplanda çalışan proseslerin PID'lerini gösteriyor
{
	DIR *dir;
	  struct dirent *ent;
	  int i, fd_self, fd;
	  unsigned long time, stime;
	  char flag, *tty;
	  char cmd[256], tty_self[256], path[256], time_s[256];
	  FILE* file;

	  dir = opendir("/proc");
	  fd_self = open("/proc/self/fd/0", O_RDONLY);
	  sprintf(tty_self, "%s", ttyname(fd_self));
	  printf(FORMAT, "PID", "TTY", "TIME", "CMD");

	  while ((ent = readdir(dir)) != NULL)
	  {
	  flag = 1;
	  for (i = 0; ent->d_name[i]; i++)
	  if (!isdigit(ent->d_name[i]))
	  {
	   flag = 0;
	   break;
	  }

	  if (flag)
	  {
	  sprintf(path, "/proc/%s/fd/0", ent->d_name);
	  fd = open(path, O_RDONLY);
	  tty = ttyname(fd);

	  if (tty && strcmp(tty, tty_self) == 0)
	  {

	   sprintf(path, "/proc/%s/stat", ent->d_name);
	   file = fopen(path, "r");
	   fscanf(file, "%d%s%c%c%c", &i, cmd, &flag, &flag, &flag);
	   cmd[strlen(cmd) - 1] = '\0';

	  for (i = 0; i < 11; i++)
	  fscanf(file, "%lu", &time);
	  fscanf(file, "%lu", &stime);
	  time = (int)((double)(time + stime) / sysconf(_SC_CLK_TCK));
	  sprintf(time_s, "%02lu:%02lu:%02lu",
	  (time / 3600) % 3600, (time / 60) % 60, time % 60);

	  printf(FORMAT, ent->d_name, tty + 5, time_s, cmd + 1);
	  fclose(file);
	  }
	 close(fd);
	}
	}
	close(fd_self);
}

void execution(char** parsed){	//Fork ile yeni process oluşturuyor 
				//execvp ile yavru processin üzerine gelen komut yazılıyor

	pid_t pid = fork();
	int status;
	
	if (pid == -1) {
		return;
	} else if (pid == 0) {

		if (execvp(parsed[0], parsed) < 0) {
			printf("Could not execute command..\n");
		}
		exit(0);
	} else {
    		do {
      			waitpid(pid, &status, WUNTRACED);
    		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
        
    	return;
	}
}

int built_in_commands(char** parsed) //gelen builtin komutları çağırır
{
	int numOfCmds = 3, i, switchOwnArg = 0;
	char* builtin_str[numOfCmds];
	char* username;

	builtin_str[0] = "exit";
	builtin_str[1] = "cd";
	builtin_str[2] = "showpid";

	for (i = 0; i < numOfCmds ; i++) {
		if (strcmp(parsed[0], builtin_str[i]) == 0) {
			switchOwnArg = i + 1;
			break;
		}
	}

	switch (switchOwnArg) {
	case 1:
		printf("Exit\n");
		exit(0);
	case 2:
		chdir(parsed[1]);
		return 1;
	case 3:
		showpid();
		return 1;
	default:
		break;
	}

	return 0;
}

// gelen stringi boşluk karakterine göre parçalara ayıran fonksiyon
void parseSpace(char* str, char** parsed)
{
	int i;

	for (i = 0; i < MAXLIST; i++) {
		parsed[i] = strsep(&str, " ");

		if (parsed[i] == NULL)
			break;
		if (strlen(parsed[i]) == 0)
			i--;
	}
}
int processString(char* str, char** parsed) //parçalanış stringleri builtin komutsa işler ve 0 return eder
{					  // builtin komut değilse 1 return eder..
        parseSpace(str, parsed);
   	if(built_in_commands(parsed)){
   		return 0;
   	}
   	return 1;
}
