#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include "shell.h"
#include "util.h"
#include "linkedlist.h"
#include "stack.h"

/* Global variables */
struct pathnode *PATH;

/* Read the user input from the console and passes it to other functions
 * to execute the commands.
 *
 * Note: The input length of the command can be anything but the fgets function
 * truncates it at the BUFSIZ length which is 8192 characters. Any command
 * lengthier than BUFSIZ is obviously invalid hence truncating the input.
 * */
int main(int argc, char *argv[])
{
	char buffer[BUFSIZ];
	char *cmd;
	char **cmd_array;
	int cmd_len;
	int word_count;
	int status = 0;
	char *currdir;

	/* Initialize global variable */
	PATH = init_path();
	initstack();
	currdir = get_curr_dir();
	push(currdir);
	free(currdir);

	printf("\nNote: Please set path variable using \"path\" command before"
		" issuing commands. Path is empty by default\n");

	while (1) {
		printf("\nMyShell>>");
		fgets(buffer, BUFSIZ, stdin);
		cmd_len = strlen(buffer);
		cmd = strndup(buffer, cmd_len - 1);
		cmd_array = cmd_tokenizer(cmd);
		word_count = find_len(cmd_array);

		if (word_count == 0)
			continue;

		else if (word_count > 0)
			status = handle_shell_cmds(cmd_array, word_count);

		if (status == -1 || status == 1)
			continue;

		execute_command(cmd_array);
		free(cmd);
		free(cmd_array);
	}
	return 0;
}

void execute_command(char *cmd_args[])
{
	pid_t pid;
	int status;
	char *cmd_abs_path;

	pid = fork();
	if (pid == 0) {
		/* Child process*/
		cmd_abs_path = find_absolute_path(PATH, cmd_args[0]);
		execute_exec(cmd_abs_path, cmd_args);
		free(cmd_abs_path);

	} else if (pid == -1) {
		/* Error in the fork() system call*/
		fprintf(stderr, "ERROR : %s\n", strerror(errno));
		exit(EXIT_FAILURE);

	} else {
		/* Parent process */
		if (wait(&status) == -1) {
			fprintf(stderr, "ERROR : %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
	}
}

void execute_exec(const char *cmd, char * const argv[])
{
	int exec_status = execv(cmd, argv);
	if (exec_status == -1) {
		fprintf(stderr, "ERROR : %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
}

/* Handles shell commands such as exit, cd, pushd, popd, dirs, path.
 *
 * Returns:
 * -1 when anything fails or the input is empty string containing only '\0'.
 *  1 on successfully executing the command.
 *  0 when the input command is non among the above stated commands.
 * */
int handle_shell_cmds(char **cmd_array, int word_count)
{
	if (strcmp(cmd_array[0], "exit") == 0) {
		free(cmd_array);
		exit(0);

	} else if (strcmp(cmd_array[0], "cd") == 0) {
		return handle_cd_cmd(cmd_array, word_count);

	} else if (strcmp(cmd_array[0], "pushd") == 0) {
		return handle_pushd_cmd(cmd_array, word_count);

	} else if (strcmp(cmd_array[0], "dirs") == 0) {
		return handle_dirs_cmd(cmd_array, word_count);

	} else if (strcmp(cmd_array[0], "popd") == 0) {
		return handle_popd_cmd(cmd_array, word_count);

	} else if (strcmp(cmd_array[0], "path") == 0) {
		return handle_path_cmd(cmd_array, word_count);

	} else if (strcmp(cmd_array[0], "\0") == 0) {
		return -1;

	}
	return 0;
}

/*
 * Adds the given command line arguments to the path variable.
 *
 * Returns:
 *  1 on success.
 * -1 when there is an error in the path command syntax.
 *
 * Note: path command with more than 1 arguments is not accepted.
 * */
int handle_path_cmd(char **cmd_array, int word_count)
{
	if (word_count == 1) {
		printlist(PATH);
		return 1;

	} else if (word_count == 3 && !strcmp(cmd_array[1], "+")) {
		PATH = add_to_path(PATH, cmd_array[2]);
		printlist(PATH);
		return 1;

	} else if (word_count == 3 && !strcmp(cmd_array[1], "-")) {
		PATH = del_from_path(PATH, cmd_array[2]);
		printlist(PATH);
		return 1;

	} else {
		printf("Error in path command syntax. It should be path [+/-] "
			"/some/dir\n");
		return -1;
	}
}

/*
 * Changes the current working directory.
 *
 * Returns:
 * -1 when chdir fails or fetching the HOME environmental variable fails.
 *  1 on success
 * */
int handle_cd_cmd(char **cmd_array, int word_count)
{
	const char *home;
	char *currdir;

	if (word_count == 1) {
		home = getenv("HOME");
		if (home == NULL) {
			fprintf(stderr, "ERROR : %s\n", strerror(errno));
			return -1;
		}

		if (chdir(home) == -1) {
			fprintf(stderr, "ERROR : %s\n", strerror(errno));
			return -1;
		}

	} else if (word_count > 1) {
		if (chdir(cmd_array[1]) == -1) {
			fprintf(stderr, "ERROR : %s\n", strerror(errno));
			return -1;
		}
	}

	pop();
	currdir = get_curr_dir();
	push(currdir);
	free(currdir);

	return 1;
}

/*
 * Handles pushd command.
 * Returns:
 *  1 on success.
 * -1 when chdir fails.
 * */
int handle_pushd_cmd(char **cmd_array, int word_count)
{
	char *newdir;
	int stacksize = sizeofstack();
	char *tmp1, *tmp2;

	if (word_count == 1) {
		if (stacksize == 0 || stacksize == 1) {
			printf("no other directory\n");
			return -1;

		} else {
			tmp1 = pop();
			tmp2 = pop();
			push(tmp1);
			push(tmp2);

			if (chdir(tmp2) == -1) {
				fprintf(stderr, "ERROR: %s\n", strerror(errno));
				return -1;
			}

			print_dir_stack();
			return 1;
		}

	} else {
		if (chdir(cmd_array[1]) == -1) {
			fprintf(stderr, "ERROR : %s\n", strerror(errno));
			return -1;

		} else {
			newdir = get_curr_dir();
			push(newdir);
			free(newdir);
			print_dir_stack();
			return 1;
		}
	}
}

/* Displays directory stack.
 * Note: supports only dirs command with no flags
 * */
int handle_dirs_cmd(char **cmd_array, int word_count)
{
	if (word_count > 1) {
		printf("invalid arguments! \"dirs\" takes no arguments in this"
			" shell");
		return -1;
	} else {
		print_dir_stack();
		return 1;
	}
}

int handle_popd_cmd(char **cmd_array, int word_count)
{
	const char *dirname;
	int stacksize = sizeofstack();

	if (stacksize == 1) {
		printf("directory stack is empty\n");
		return -1;
	}

	pop();
	dirname = peek();
	if (chdir(dirname) == -1) {
		fprintf(stderr, "ERROR : %s\n", strerror(errno));
		return -1;
	}

	print_dir_stack();
	return 1;
}

/* Note: The calling function should free the returned char pointer */
char *get_curr_dir()
{
	char *dirname = get_current_dir_name();
	if (dirname == NULL) {
		fprintf(stderr, "ERROR : %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	return dirname;
}
