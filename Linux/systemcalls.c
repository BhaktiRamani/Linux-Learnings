#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdint.h>
#include <sys/types.h>
#include <syslog.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>




bool do_system(const char *cmd);
bool do_exec(int count, ...);
bool do_exec_redirect(const char *outputfile, int count, ...);

int main()
{

	// bool result = do_system("echo hello");
	// printf("result is %B\n", result);

	// bool result2 = do_exec(3,"/home/bakri/Work/Learnings/Bash/writer", NULL);
	// printf("\n");
	// printf("result2 is %B\n", result2);

	printf("------------------------------------------------------------------\n");
	const char *outputfile = "/home/bakri/Work/Learnings/Linux/output.txt";
	bool result3 = do_exec_redirect(outputfile, 3, "/bin/ls", "-l", "/home/bakri/Work");
	printf("\n");
	printf("result 3 is %B\n", result3);




}


/**
 * @param cmd the command to execute with system()
 * @return true if the command in @param cmd was executed
 *   successfully using the system() call, false if an error occurred,
 *   either in invocation of the system() call, or if a non-zero return
 *   value was returned by the command issued in @param cmd.
*/
bool do_system(const char *cmd)
{

/*
 * TODO  add your code here
 *  Call the system() function with the command set in the cmd
 *   and return a boolean true if the system() call completed with success
 *   or false() if it returned a failure
*/
	int return_code = system(cmd);
	if(return_code == 0)
	{
		printf("success\n");
		return true;
	}
    else
    {	
		printf("failed\n");
    	return false;
    }
}

/**
* @param count -The numbers of variables passed to the function. The variables are command to execute.
*   followed by arguments to pass to the command
*   Since exec() does not perform path expansion, the command to execute needs
*   to be an absolute path.
* @param ... - A list of 1 or more arguments after the @param count argument.
*   The first is always the full path to the command to execute with execv()
*   The remaining arguments are a list of arguments to pass to the command in execv()
* @return true if the command @param ... with arguments @param arguments were executed successfully
*   using the execv() call, false if an error occurred, either in invocation of the
*   fork, waitpid, or execv() command, or if a non-zero return value was returned
*   by the command issued in @param arguments with the specified arguments.
*/

bool do_exec(int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;
    // this line is to avoid a compile warning before your implementation is complete
    // and may be removed
    command[count] = command[count];

/*
 * TODO:
 *   Execute a system command by calling fork, execv(),
 *   and wait instead of system (see LSP page 161).
 *   Use the command[0] as the full path to the command to execute
 *   (first argument to execv), and use the remaining arguments
 *   as second argument to the execv() command.
 *
*/
	char *path = command[0];
	for(int i = 0; i<(count-1); i++)
	{
		char *temp = command [count + 1];
		command[count] = temp;
	}
	pid_t pid = fork();
	if(pid == 0)
	{
		// runs by child process
		execv(path, command);
		//if execv returns - error occured
		perror("execv");
		return true;
	}
	else if(pid > 0)
	{
		//Child successfully created
		printf("---------- waiting for child to complete its process\n");
		wait(NULL);
		printf("---------- Child finished, Parent process complete\n");
	}
	else 
	{
		printf("---------- Fork failed");
		perror("fork");
		return false;
	}

    va_end(args);

    return true;
}

// /**
// * @param outputfile - The full path to the file to write with command output.
// *   This file will be closed at completion of the function call.
// * All other parameters, see do_exec above
// */
bool do_exec_redirect(const char *outputfile, int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;
    // this line is to avoid a compile warning before your implementation is complete
    // and may be removed
    command[count] = command[count];


/*
 * TODO
 *   Call execv, but first using https://stackoverflow.com/a/13784315/1446624 as a refernce,
 *   redirect standard out to a file specified by outputfile.
 *   The rest of the behaviour is same as do_exec()
 *
*/
	char *path = command[2];
	char *new_cmd;
    printf("*****************************************\n");
	for (int i = 0; i < count; i++) {
    printf("command[%d] = %s\n", i, command[i]);
	}


	// Opening a file where to redirect output of command
    int fd = open(outputfile, O_WRONLY | O_TRUNC | O_CREAT, 0644);
    if (fd < 0){ perror("open"); return false;}				//returning with error if given files or path is wrong

	// printf("------- %s\n cmd[0]\n", outputfile);
	int pid = fork();
	if(pid == 0)
	{
		//Successful child generation

		//Making STDOUT file point to opened file descriptor
		if(dup2(fd, STDOUT_FILENO) < 0)
		{
			//duplication failed
			close(fd);
			abort();
		}
		close(fd);			//Closing fd to avoid leakage of file descriptor

		// runs by child process - replaces current process with a new program
		// inherits the file discriptors from the parent
		// so its STDOUT_FILENO is still pointing to fd
		// Whatever output child gives - gets redirected to fd
		if(execv(command[0], command) == -1)
		{
			//if execv returns - error occured
			perror("execv");
			exit(1);
		}

		return true;
	}
	
	else if(pid > 0)
	{
		//Child successfully created
		printf("---------- waiting for child to complete its process\n");
		wait(NULL);
		printf("---------- Child finished, Parent process complete\n");
	}
	else
	{
		printf("---------- Fork failed");
		perror("fork");
		close(fd);
		return false;
	}
    va_end(args);

    return true;
}


bool popen_implementation(const char *outputfile)
{
	FILE *fp;
	int status;
	int PATH_MAX = 20;
	char path[PATH_MAX];

	fp = popen("echo hello world", "r");
	if(fp == NULL)
	{
		printf("error in fp\n");
	}

	int fd = open("/home/bakri/Work/Learnings/Linux/output2.txt", O_WRONLY | O_TRUNC | O_CREAT, 0644);
	while(fgets(path, PATH_MAX, fp) != NULL)
	{
		printf("%s", path);
		write(fd, path, strlen(path));

	}
	pclose(fp);
}

