#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define ASCII_SPACE 32
#define ASCII_NEWLINE 10

#define INPUT_LENGTH 100
#define HISTORY_SIZE 50

// I added the exit command, which will exit the shell and return to the
// upper-level shell.

// I also added a history command, which lists all of the user inputs.

int main(int argc, char *argv[]) {
    int pid, pid2;
    char input[INPUT_LENGTH];
    char history[HISTORY_SIZE];
    int status;
    int numBlank = 0;
    
    while(1) {
        printf("myshell>");
	fflush(stdout);
        
        // receive command from user input
        fgets(input, INPUT_LENGTH, stdin);
        
        #ifdef DEBUG
            printf("Input: %s\n", input);
	    fflush(stdout);
        #endif
        
        strcat(history, input);
        
        if(strcmp(input, "exit\n") == 0) {
	    printf("Goodbye!\n");
	    fflush(stdout);
            exit(0);
        }
        
        if(strcmp(input, "history\n") == 0) {
            printf("%s", history);
	    fflush(stdout);
            continue;
        }
        
        if(strcmp(input, "\n") == 0) {
            numBlank++;
        } else
            numBlank = 0;
        
        if(numBlank == 1)
            continue;
        
        if(numBlank == 2) {
            strcpy(input, "ls\n");
            numBlank = 0;
        }
        
        int counter = 2; // start at 2 because no space after last arguments

        // count how many spaces are in the input to accurately size token array
        for(int i = 0; input[i] != ASCII_NEWLINE; i++) {
            if(input[i] == ASCII_SPACE) { // space separates tokens
                counter++;
            }
        }
        counter++;
        
        #ifdef DEBUG
            printf("Counter: %d\n\n", counter);
            fflush(stdout);
        #endif
        
        if(strchr(input, ';') != NULL) {
            char *firstCommand[counter];
            *firstCommand = calloc(counter, INPUT_LENGTH);
            
            char *secondCommand[counter];
            *secondCommand = calloc(counter, INPUT_LENGTH);
            
            firstCommand[0] = strtok(input, ";");
            secondCommand[0] = strtok(NULL, ";");
            
            #ifdef DEBUG
            	printf("Before ; : %s\n", firstCommand[0]);
            	printf("After ;  : %s\n", secondCommand[0]);
		fflush(stdout);
            #endif
            
            char *firstArguments[counter];
            *firstArguments = calloc(counter, INPUT_LENGTH);
            firstArguments[0] = strtok(firstCommand[0], " ");
            for (int i = 1; i < counter; i++) {
                firstArguments[i] = strtok(NULL, " ");
            }

            #ifdef DEBUG
            	for(int i = 0; firstArguments[i] != NULL; i++) {
            	    printf("Command 1 argument %d before ; : %s\n", i, firstArguments[i]);
            	}
           	fflush(stdout);
            #endif
            
            char *secondArguments[counter];
            *secondArguments = calloc(counter, INPUT_LENGTH);
            secondArguments[0] = strtok(secondCommand[0], " ");
            for (int i = 1; i < counter; i++) {
                secondArguments[i] = strtok(NULL, " ");
            }

            #ifdef DEBUG
            	for(int i = 0; secondArguments[i] != NULL; i++) {
                    printf("Command 2 argument %d after ; : %s\n", i, secondArguments[i]);
            	}
            	fflush(stdout);
            #endif
            
            // null-terminate the arguments
            for(int i = 0; firstArguments[i] != NULL; i++) {
                for(int j = 0; j < counter; j++) {
                    if(firstArguments[i][j] == '\n')
                        firstArguments[i][j] = '\0';
                }
            }
            for(int i = 0; secondArguments[i] != NULL; i++) {
                for(int j = 0; j < counter; j++) {
                    if(secondArguments[i][j] == '\n')
                        secondArguments[i][j] = '\0';
                }
            }

            if((pid = fork()) < 0) {
                perror("Something went wrong in fork");
                exit(-1);
            } else if(pid == 0) { // child
                // start first process
                execvp(firstArguments[0], firstArguments);
                exit(-1);
            } else { // parent
                wait(&status); // wait until child terminates
                if((pid2 = fork()) == 0) {
                    // then start second process
                    execvp(secondArguments[0], secondArguments);
                    exit(-1);
                } else {
                    wait(&status);
                    continue;
                }
            }
        } else if(strchr(input, '|') != NULL) {
            char *firstCommand[counter];
            *firstCommand = calloc(counter, INPUT_LENGTH);
            
            char *secondCommand[counter];
            *secondCommand = calloc(counter, INPUT_LENGTH);
            
            firstCommand[0] = strtok(input, "|");
            secondCommand[0] = strtok(NULL, "|");
            
            int fd[2];
            
            #ifdef DEBUG
            	printf("Before | : %s\n", firstCommand[0]);
            	printf("After |  : %s\n", secondCommand[0]);
		fflush(stdout);
            #endif
            
            char *firstArguments[counter];
            *firstArguments = calloc(counter, INPUT_LENGTH);
            firstArguments[0] = strtok(firstCommand[0], " ");
            for (int i = 1; i < counter; i++) {
                firstArguments[i] = strtok(NULL, " ");
            }

            #ifdef DEBUG
            	for(int i = 0; firstArguments[i] != NULL; i++) {
                    printf("Command 1 argument %d before ; : %s\n", i, firstArguments[i]);
            	}
            	fflush(stdout);
            #endif
            
            char *secondArguments[counter];
            *secondArguments = calloc(counter, INPUT_LENGTH);
            secondArguments[0] = strtok(secondCommand[0], " ");
            for (int i = 1; i < counter; i++) {
                secondArguments[i] = strtok(NULL, " ");
            }

            #ifdef DEBUG
            	for(int i = 0; secondArguments[i] != NULL; i++) {
                    printf("Command 2 argument %d after ; : %s\n", i, secondArguments[i]);
            	}
            	fflush(stdout);
            #endif
            
            // null-terminate the arguments
            for(int i = 0; firstArguments[i] != NULL; i++) {
                for(int j = 0; j < counter; j++) {
                    if(firstArguments[i][j] == '\n')
                        firstArguments[i][j] = '\0';
                }
            }
            for(int i = 0; secondArguments[i] != NULL; i++) {
                for(int j = 0; j < counter; j++) {
                    if(secondArguments[i][j] == '\n')
                        secondArguments[i][j] = '\0';
                }
            }
            
            if((pid = fork()) < 0) {
                perror("Something went wrong in fork");
                exit(-1);
            } else if(pid == 0) { // child
            	pipe(fd);
            	pid2 = fork();
            	if(pid2 < 0) {
            		perror("Something went wrong in fork.");
            		exit(-1);
            	}
            	if(pid2) { // parent
            		wait(&pid2);
            		close(fd[1]);	// close write side of pipe
            		close(0);	// close STDIN
            		dup2(fd[0], 0);
            		execvp(secondArguments[0], secondArguments);
            	}
            	if(pid == 0) {
            		close(fd[0]);	// close read side of pipe
            		close(1);	// close STDOUT
            		dup2(fd[1], 1);
            		execvp(firstArguments[0], firstArguments);
            	}
            } else { // parent
                wait(&status); // make parent wait until child terminates
                continue;
            }

        } else { // one command
            counter = counter - 2;
            char *tokenizedInput[counter];
            tokenizedInput[counter] = NULL;
            
            tokenizedInput[0] = strtok(input, " ");
            
            for(int i = 1; i < counter; i++) {
                tokenizedInput[i] = strtok(NULL, " ");
            }
            
            #ifdef DEBUG
            	for (int i = 0; tokenizedInput[i] != NULL; i++) {
                    printf("Token %d: %s\n", i + 1, tokenizedInput[i]);
            	}
		fflush(stdout);
            #endif
            
            // change the command to a concatenation of "which " and the command
            char str[80];
            strcpy(str, "which ");
            strcat(str, tokenizedInput[0]);
            
            // execute the concatenated command to put the path of the command into
            // the first slot of the tokenizedInput array (the command).
            FILE *pipe;
            pipe = popen(str, "r");
            fgets(str, 80, pipe);
            tokenizedInput[0] = str;
	    pclose(pipe);
            
            #ifdef DEBUG
            	printf("Path to command: %s", tokenizedInput[0]);
		fflush(stdout);
            #endif
            
            // strip newline character
            for(int i = 0; i < counter; i++) {
                for(int j = 0; j < strlen(tokenizedInput[i]); j++) {
                    if(tokenizedInput[i][j] == '\n') {
                        tokenizedInput[i][j] = '\0';
                    }
                }
            }
            
            tokenizedInput[counter] = NULL; // null terminate
            
            if((pid = fork()) < 0) {
                perror("Something went wrong in fork");
                exit(-1);
            } else if (pid == 0) { // child
                if(execv(tokenizedInput[0], tokenizedInput) == -1)
                    perror("execv");
		    exit(-1);
            } else { // parent
                waitpid(pid, NULL, 0);
                continue;
            }
        }
    }
}
