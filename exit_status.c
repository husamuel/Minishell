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


int execute_command(t_token *cmd, t_mini *ms)
{
    pid_t pid;
    int status;
    int pipe_fd[2];

    if (!cmd || !cmd->cmd)
        return -1;

    // Criação do pipe
    if (pipe(pipe_fd) == -1) {
        perror("minishell: pipe");
        return -1;
    }

    // Salvando os manipuladores antigos para SIGINT e SIGQUIT
    void (*old_sigint)(int) = signal(SIGINT, sigint_handler);  // Usando sigint_handler para capturar SIGINT
    void (*old_sigquit)(int) = signal(SIGQUIT, SIG_IGN);  // Ignorando SIGQUIT no processo pai

    pid = fork();
    if (pid == 0)
    {
        // No processo filho, redirecionamos a saída para o pipe
        close(pipe_fd[0]);
        dup2(pipe_fd[1], STDOUT_FILENO);

        reset_signals();  // Reseta sinais para o processo filho
        execvp(cmd->cmd, cmd->args_file);
        perror("minishell");
        exit(127);
    }
    else if (pid > 0)
    {
        // No processo pai, fechamos a escrita do pipe
        close(pipe_fd[1]);

        // Lendo o output do pipe
        char buffer[1024];
        ssize_t bytes_read;
        size_t total_size = 0;
        ms->output = malloc(1);  // Inicializa o buffer para armazenar o output
        ms->output[0] = '\0';

        while ((bytes_read = read(pipe_fd[0], buffer, sizeof(buffer) - 1)) > 0) {
            buffer[bytes_read] = '\0';  // Garantir que a string terminada por '\0'
            ms->output = realloc(ms->output, total_size + bytes_read + 1);
            if (!ms->output) {
                perror("minishell: realloc");
                return -1;
            }
            strcat(ms->output, buffer);  // Adiciona ao buffer
            total_size += bytes_read;
        }

        waitpid(pid, &status, 0);  // Espera o processo filho terminar

        // Restaurando os sinais para o processo pai
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


void handle_exit_status(t_mini *ms, int status)
{
    int signal = WTERMSIG(status);

    if(ms->exit_status == 2)
        return;
    else if (WIFEXITED(status))
        ms->exit_status = WEXITSTATUS(status);
    else if (WIFSIGNALED(status))
    {
        ms->exit_status = 127;
        if (signal == SIGINT || signal == SIGQUIT)
            ms->exit_status = 130;
    }
    else if (signal == SIGINT || signal == SIGQUIT)
        ms->exit_status = 130;
    else
        ms->exit_status = 0;
}
