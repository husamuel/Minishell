#include <stdio.h>
#include <../../../../usr/include/readline/readline.h>
#include <../../../../usr/include/readline/history.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <termios.h>
#include <curses.h>
#include <term.h>
#include <errno.h>

typedef enum e_token_type
{
    COMMAND,
    ARGUMENT,
    PIPE,
    REDIRECTION,
    UNKNOWN
} t_token_type;

typedef struct s_token
{
    char *value;
    t_token_type type;
} t_token;


t_token_type get_token_type(char *str);
const char *token_type_to_string(t_token_type type);