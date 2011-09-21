/* Function prototypes */

void execute_command(char **);

int find_len(char **cmd_array);

int handle_shell_cmds(char **, int);

int handle_cd_cmd(char **, int);

int handle_pushd_cmd(char **, int);

int handle_path_cmd(char **, int);

int handle_dirs_cmd(char **, int);

int handle_popd_cmd(char **, int);

void execute_exec(const char *, char *const argv[]);

char *get_curr_dir();
