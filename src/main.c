#include "./../includes/minishell.h"

int	main(void)
{
	char *input;

	while(1)
	{
 input = readline("\033[31mm\033[0m"
                         "\033[32mi\033[0m"
                         "\033[33mn\033[0m"
                         "\033[34mi\033[0m"
                         "\033[35ms\033[0m"
                         "\033[36mh\033[0m"
                         "\033[35me\033[0m"
                         "\033[31ml\033[0m"
                         "\033[32ml\033[0m"
                         "\033[33m>\033[0m"
						 " ");
		if (*input)
            add_history(input);

		if (strcmp(input, "exit") == 0)
        {
            free(input);
            break;
        }
		t_token_type type = get_token_type(input);
        printf("Token: '%s', Type: %s\n", input, token_type_to_string(type));
	}
}