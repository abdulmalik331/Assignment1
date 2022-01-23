#include<stdio.h> 
#include<string.h> 
#include<stdlib.h> 
#include<sys/wait.h> 
#include<readline/readline.h> 
#include<readline/history.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<MailMsg.Idl>
#define MAX 1000
#define clear() printf("\033[H\033[J") 
void rand(){
    x=1;
}
 
void Input(char* input) 
{ 
	char* temp; 
	temp = readline("\nEnter Command here -> "); 
	add_history(temp); 
	strcpy(input, temp); 

}
void Dir()
{
	char array[300];
	getcwd(array,sizeof(array));
	printf("\nCurrent Directory:%s", array);
	
}
void runsystemcommands(char** collection) 
{ 

	pid_t pid = fork(); 

	if (pid < 0) { 
		printf("\nChild not created"); 
		return; 
	}
	 else if (pid == 0) 
	{ 
		if (execvp(collection[0], collection) < 0) 
		{ 
			printf("\nCommand not found!!"); 
		} 
		exit(0); 
	} 
	else 
	{ 
		wait(NULL); 
		return; 
	} 
} 
int builtincommands(char** parsed) 
{ 
	int NoOfOwnCmds = 2, i, switchOwnArg = 0; 
    char* ListOfOwnCmds[NoOfOwnCmds]; 
    char* username; 
  
    ListOfOwnCmds[0] = "exit"; 
    ListOfOwnCmds[1] = "cd"; 
  
    for (i = 0; i < NoOfOwnCmds; i++) { 
        if (strcmp(parsed[0], ListOfOwnCmds[i]) == 0) { 
            switchOwnArg = i + 1; 
            break; 
        } 
    } 
  int chk = 0;
    switch (switchOwnArg) { 
    case 1: 
        printf("\nExit Successfullly\n"); 
        exit(0); 
    case 2: 
	
        chk=chdir(parsed[1]); 
	if(chk==-1)
	printf("Directory Does not Exist");
        return 1; 
    default:
        break; 
    } 
  
    return 0; 
} 
void parseOnspace(char* s, char** collection) 
{  

	for (int i = 0; i < MAX; i++) 
	{ 
		collection[i] = strsep(&s, " "); 

		if (collection[i] == NULL) 
			break; 
	} 
} 
int parsePipe(char* s, char** c) 
{ 
     
    for (int i = 0; i < 2; i++) { 
        c[i] = strsep(&s, "|"); 
        if (c[i] == NULL) 
            break; 
    } 
    if (c[1] == NULL) 
        return 0;
    else 
    { 
        return 1; 
    } 
} 
void runPipe(char** collection, char** pipes) 
{ 
 
    int fd[2];  
    pid_t p1, p2; 
  
    if (pipe(fd) < 0) { 
        printf("\npipe not initialized"); 
        return; 
    } 
    p1 = fork(); 
    if (p1 < 0) { 
        printf("\nCould not fork"); 
        return; 
    } 
  
    if (p1 == 0) { 
        close(fd[0]); 
        dup2(fd[1], STDOUT_FILENO); 
        close(fd[1]); 
  
        if (execvp(collection[0], collection) < 0) { 
            printf("\nCommand before pipe not executed"); 
            exit(0); 
        } 
    } else { 
        p2 = fork(); 
  
        if (p2 < 0) { 
            printf("\nCould not fork"); 
            return; 
        } 
        if (p2 == 0) { 
            close(fd[1]); 
            dup2(fd[0], STDIN_FILENO); 
            close(fd[0]); 
            if (execvp(pipes[0],pipes) < 0) { 
                printf("\nCommand after pipe not executed"); 
                exit(0); 
            } 
        } else {  
            wait(NULL); 
            wait(NULL); 
        } 
    } 
} 
int sPipe(char* s, char** c,char** p) 
{ 
	char* chkpipe[2]; 
    	int pi = 0; 
  
   	 pi = parsePipe(s,chkpipe); 
  
   	if (pi) 
   	{ 
        parseOnspace(chkpipe[0], c); 
        parseOnspace(chkpipe[1], p); 
        runPipe(c,p);
        return 1;
    	} 
    	return 0;
		
} 
int Builtin(char* s, char** collec)
{
	 parseOnspace(s, collec);
	 if(builtincommands(collec))
	 return 1;
	 else
	 return 0;	
	  
}
void normal(char* s, char** collec)
{
	 runsystemcommands(collec);
		  
}
void start() 
{ 
    clear();  
    char* user = getenv("User"); 
    printf("\n\t\t\tC Shell"); 
    printf("\n\n\nUser is: @%s", user); 
    sleep(1); 
    clear(); 
}
int main() 
{ 
	int check= 0;
	char str[MAX], *collec[MAX],*pipes[MAX]; 
	start();
	while (1) 
	{ 
		Dir();
		Input(str);
	        check=sPipe(str,collec,pipes); 
	        if(check==0)
	        {
	          check=Builtin(str,collec);
	          
	          if(check==0)
	          	{
	          		normal(str,collec);
	          	}
	        }
	        
	       

	} 

} 
