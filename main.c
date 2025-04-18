#include "minishell.h"

int main(int argc, char *argv[], char **envp)
{
    t_mini ms;

    (void)(argc);
    (void)(argv);
    ms = init(envp);
    disable_ctrl_backslash();
	setup_signals();
    while(1)
    {
        ms.input = get_input(&ms, ms.prompt);
        if (ms.input)
        {
            ms.token = lexer(ms.input);
            if (ms.token)
            {
				parser(&ms);
				exec(&ms);
				free_tokens(ms.token);
            }
        }
		ms.pipe = 0;
        ms.exit_status_count = 0;
        ms.count++;
        ms.none = 0;
    }
	free(&ms);
}