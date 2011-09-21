/* Structure to store the directories in the path variable */
struct pathnode {
	char *value;
	struct pathnode *next;
};

struct pathnode *init_path();
struct pathnode *add_to_path(struct pathnode *, const char *);
struct pathnode *del_from_path(struct pathnode *, const char *);
void printlist(struct pathnode *);

/* Handles failures and displays error message
 *
 * Arguments: pointer, error msg to display.
 * */
void handle_failure(void *);
char *find_absolute_path(struct pathnode *, const char *);
int check_file_exists(const char *);
