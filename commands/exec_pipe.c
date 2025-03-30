#include "./../minishell.h"

static void close_pipes(int pipe_fd[2]) {
    if (pipe_fd[0] != -1) close(pipe_fd[0]);
    if (pipe_fd[1] != -1) close(pipe_fd[1]);
}

int exec_pipe(t_mini *ms) {
    int status = 0;
    pid_t pid;
    int prev_pipe[2] = {-1, -1};
    int curr_pipe[2] = {-1, -1};
    t_token *current_token = ms->token;

    if (!current_token || !ms->pipe)  
        return 0;

    while (current_token) {
        if (current_token->type == CMD_PIPE) {
            current_token = current_token->next;
            continue;
        }

        // Criar novo pipe se houver próximo comando
        if (current_token->next && current_token->next->type == CMD_PIPE) {
            if (pipe(curr_pipe) == -1) {
                perror("minishell: pipe");
                return 127;
            }
        } else {
            curr_pipe[0] = -1;
            curr_pipe[1] = -1;
        }

        pid = fork();
        if (pid == -1) {
            perror("minishell: fork");
            return 127;
        }

        if (pid == 0) { // Processo filho
            signal(SIGINT, SIG_DFL);
            signal(SIGQUIT, SIG_DFL);

            if (prev_pipe[0] != -1) {
                if (dup2(prev_pipe[0], STDIN_FILENO) == -1) {
                    perror("dup2 prev_pipe");
                    close_pipes(prev_pipe);
                    exit(1);
                }
            }
            if (curr_pipe[1] != -1) {
                if (dup2(curr_pipe[1], STDOUT_FILENO) == -1) {
                    perror("dup2 curr_pipe");
                    close_pipes(curr_pipe);
                    exit(1);
                }
            }
            
            close_pipes(prev_pipe);
            close_pipes(curr_pipe);

            execvp(current_token->cmd, current_token->args_file);
            perror("minishell");
            exit(127);
        }

        // Processo pai fecha pipes antigos e atualiza
        if (prev_pipe[0] != -1) close(prev_pipe[0]);
        if (prev_pipe[1] != -1) close(prev_pipe[1]);
        prev_pipe[0] = curr_pipe[0];
        prev_pipe[1] = curr_pipe[1];
        
        // Fechar a escrita no pipe corrente no processo pai
        if (curr_pipe[1] != -1) close(curr_pipe[1]);

        current_token = current_token->next;
    }

    // Fechar o último pipe de leitura no pai
    if (prev_pipe[0] != -1) close(prev_pipe[0]);

    // Esperar por todos os processos filhos
    while (wait(&status) > 0) {
        if (WIFEXITED(status))
            ms->exit_status = WEXITSTATUS(status);
        else if (WIFSIGNALED(status))
            ms->exit_status = WTERMSIG(status) + 128;
    }
    return status;
}
