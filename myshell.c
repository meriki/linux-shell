#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void shell();
char ** parseArg(char *);
char *readArg();
void executeArg(char **);
void new_cd(char **);
void new_exit();

int main(int argc, char **argv){
	shell();
	return EXIT_SUCCESS;
}

char *builtin_string[]={"cd","exit"};

char * readArg(){
	char *line=malloc(sizeof(char)*1000);
	int c,i=0;
	c=getchar();
	while(c!=EOF && c!='\n'){
		line[i++]=c;
		c=getchar();
	}

	line[i]='\0';
	//printf("%s9\n",line );
	return line;
}

void shell(){
	
	while(1){
		char *line,**arguments;
		printf("newShell > ");
		line=readArg();
		arguments=parseArg(line);
		executeArg(arguments);

		free(arguments);
		free(line);
	}
}

char ** parseArg(char *line){
	char c,**arguments=malloc(sizeof(char *)*1000);
	int i=0,j=0,k=0;
	arguments[j]=malloc(sizeof(char)*100);
	
	c=line[i++];
	while(c!='\0')
	{		
		if(c==' '){
		//	arguments[j][k]='\0';
			j=j+1;
			arguments[j]=malloc(sizeof(char)*100);
			k=0;
			c=line[i++];
		}		
		else{	
			arguments[j][k++]=c;
			c=line[i++];
		}
		
	}
	arguments[j+1]='\0'; //******very important to incude this otherwise the command execvp doesnt work properly

	// for(i=0;i<=j;i++)
	// 	printf("%s ",arguments[i] );
	// printf("\n");
	return arguments;
}

void executeArg(char **arguments){
	int status;
	
	if(arguments[0]==NULL){
		return ;
	}
	
	if(!strcmp(arguments[0],"cd")){
		
		return new_cd(arguments);
	}
	else if(!strcmp(arguments[0],"exit")){
		new_exit();
		return;
	}

	else{
		pid_t process_pid,wait_for_pid;
		
		process_pid=fork();
		if(process_pid==0){
			//child process
			//printf("gh\n");
			if(execvp(*arguments,arguments)<0){
				perror("ERROR : Problem in executing the given command");
				exit(1);
			}
			
		}
		else if(process_pid<0){
			perror("ERROR : Wasn't successful in creating a child process");
		}
		else{
			//printf("fd\n");
			while(wait(&status)!=process_pid){;};
    		//printf("whats wrong\n");
		}
	}
	return;
}

void new_cd(char **arguments){
	if(arguments[1]==NULL){
		printf("An argument is expected to cd\n" );
	}else
	{
		if(chdir(arguments[1])){
			perror("ERROR");
		}
	}
}

void new_exit(){
	printf("Successfully exited\n");
	exit(EXIT_SUCCESS);
}