#include "./../minishell.h"

int setup_redirections(t_token *token)
{
    t_token *current = token->next;
    int fd;

    while (current)
    {
        if (current->type == CMD_REDIRECT_OUT && current->next)
        {
            fd = open(current->next->cmd, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0 || dup2(fd, STDOUT_FILENO) == -1)
            {
                perror(current->next->cmd);
                return (-1);
            }
            close(fd);
        }
        else if (current->type == CMD_APPEND && current->next)
        {
            fd = open(current->next->cmd, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd < 0 || dup2(fd, STDOUT_FILENO) == -1)
            {
                perror(current->next->cmd);
                return (-1);
            }
            close(fd);
        }
        else if (current->type == CMD_REDIRECT_IN && current->next)
        {
            fd = open(current->next->cmd, O_RDONLY);
            if (fd < 0 || dup2(fd, STDIN_FILENO) == -1)
            {
                perror(current->next->cmd);
                return (-1);
            }
            close(fd);
        }
        else if (current->type == CMD_HEREDOC && current->next)
        {
            fd = setup_heredoc(current->next->cmd);
            if (fd < 0 || dup2(fd, STDIN_FILENO) == -1)
            {
                perror("heredoc");
                return (-1);
            }
            close(fd);
        }
        current = current->next;
    }

    return (0);
}


int setup_heredoc(char *delimiter)
{
    int fd[2];
    char *line;
    char *prompt = "> ";

    if (!delimiter)
        return (-1);

    if (pipe(fd) == -1)
    {
        perror("pipe");
        return (-1);
    }

    while (1)
    {
        write(STDOUT_FILENO, prompt, strlen(prompt));
        line = NULL;
        size_t len = 0;
        if (getline(&line, &len, stdin) == -1)
        {
            free(line);
            close(fd[1]);
            return (fd[0]);
        }

        if (strcmp(line, delimiter) == 0 || (strncmp(line, delimiter, strlen(delimiter)) == 0 && line[strlen(delimiter)] == '\n'))
        {
            free(line);
            close(fd[1]);
            return (fd[0]);
        }

        write(fd[1], line, strlen(line));
        free(line);
    }
}