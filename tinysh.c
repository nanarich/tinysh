/******************************************************************/
// Author: Richard Adusei
// Email: nanaregi86@gmail.com
/******************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MAX_COMMAND 100
#define MAX_NUMBER_PARAMS 10


void tiny_parser(char *cmd, char **params)
{
	for(int i = 0; i < MAX_NUMBER_PARAMS; ++i){
		params[i] = strsep(&cmd, " ");
		if(params[i] == NULL)
			break;
	}
}

int tiny_execute(char** params)
{
	/* Fork process */
	pid_t pid = fork();

        if (pid == 0) {
		/* Execute command with child pprocess */
		execvp(params[0], params);

		char* error = strerror(errno);
		printf("shell: %s: %s\n", params[0], error);
		return 0;
	}

	else {
		// Wait for child process to finish
		int childstatus;
		waitpid(pid, &childstatus, 0);
		return 1;
	}
}


int main()
{
        char cmd[MAX_COMMAND +1];
        char *params[MAX_NUMBER_PARAMS +1];

        while(1){
                char *username = getenv("USER");
                printf("%s@shell ~> ", username);

                if(fgets(cmd, sizeof(cmd), stdin) == NULL)
                        break;
                if(cmd[strlen(cmd)-1] == '\n'){
                        cmd[strlen(cmd)-1] = '\0';
                }

                tiny_parser(cmd, params);

                if(strcmp(params[0], "exit") == 0)
                        break;
                if(tiny_execute(params) == 0)
                        break;
        }

        exit(EXIT_SUCCESS);
}

