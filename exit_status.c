#include "minishell.h"

void sigint_handler(int sig)
{
    (void)sig;
    write(1, "\n", 1);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

void sigquit_handler(int sig)
{
    (void)sig;
}

void setup_signals(void)
{
    signal(SIGINT, sigint_handler);
    signal(SIGQUIT, sigquit_handler);
}

void reset_signals(void)
{
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
}

int execute_command(t_token *cmd)
{
    pid_t pid;
    int status;

    if (!cmd || !cmd->cmd || !cmd->args_file)
        return -1;

    void (*old_sigint)(int) = signal(SIGINT, sigint_handler);
    void (*old_sigquit)(int) = signal(SIGQUIT, SIG_IGN);

    pid = fork();
    if (pid == 0)
    {
        reset_signals();
        
        execvp(cmd->cmd, cmd->args_file);

        perror("minishell");
        exit(127);
    }
    else if (pid > 0)
    {
        waitpid(pid, &status, 0);

        signal(SIGINT, old_sigint);
        signal(SIGQUIT, old_sigquit);

        if (WIFEXITED(status))
            return WEXITSTATUS(status);
        else if (WIFSIGNALED(status))
        {
            write(1, "\n", 1);
            return WTERMSIG(status) + 128;
        }
        return 2;
    }
    else
    {
        perror("minishell: fork");
        return 2;
    }
}
