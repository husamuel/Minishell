#include "minishell.h"

void disable_ctrl_backslash(void)
{
    struct termios term;

    if (tcgetattr(STDIN_FILENO, &term) == -1)
        return;

    term.c_cc[VQUIT] = _POSIX_VDISABLE;

    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void sigint_handler(int sig)
{
    (void)sig;
    write(1, "\n", 1);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

void sigint_cmd_handler(int sig)
{
    (void)sig;
    write(1, "\n", 1);
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

void setup_exec_signals(void)
{
    signal(SIGINT, sigint_cmd_handler);
    signal(SIGQUIT, SIG_IGN);
}

void reset_signals(void)
{
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
}

int execute_command(t_token *cmd, t_mini *ms)
{
    pid_t pid;
    (void)ms;
    int status;
    
    if (!cmd || !cmd->cmd || !cmd->args)
        return -1;
    
    setup_exec_signals();
    pid = fork();
    if (pid == 0)
    {
        reset_signals();
    
        execvp(cmd->cmd, cmd->args);
        exit(127);
    }
    else if (pid > 0)
    {
        waitpid(pid, &status, 0);
        
        setup_signals();
        
        if (WIFEXITED(status))
            return WEXITSTATUS(status);
        else if (WIFSIGNALED(status))
            return WTERMSIG(status) + 128;
        
        exit(1);
    }
    else
    {
        perror("minishell: fork");
        
        setup_signals();
        
        exit(1);
    }
}
