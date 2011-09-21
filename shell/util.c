#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include "util.h"

int find_len(char **cmd_array)
{
	int len = 0;
	while (cmd_array[len] != '\0')
		len++;

	return len;
}

/* Splits the input command based on spaces and returns an array of strings
 * which contains the command and arguments.
 *
 * Note: The max number of arguments allowed for an input command is 200. Input
 * command is truncated if this limit exceeds and an error message is displayed.
 * However, the shell is not exited.
 * */
char **cmd_tokenizer(char *cmd)
{
	int i = 0;
	char delims[] = " ";
	char *result = NULL;
	char **tokenized_cmd = (char **) malloc(CMD_LINE_SIZE * sizeof(char *));

	if (!tokenized_cmd) {
		fprintf(stderr, "ERROR : %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	result = strtok(cmd, delims);
	while (result != NULL) {

		if (i > (CMD_LINE_SIZE - 1)) {
			printf("ERROR : The max number of arguments "
				"allowed is %d\n", CMD_LINE_SIZE);
			printf("Truncating the input command and proceeding "
				"with 200 arguments\n");
			break;

		}
		tokenized_cmd[i] = result;
		i++;
		result = strtok(NULL, delims);
	}

	tokenized_cmd[i] = '\0';
	return tokenized_cmd;
}
