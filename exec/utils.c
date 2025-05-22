#include "./../minishell.h"

/* Retrieves the value of an environment variable by key */
char *get_env_value(t_env *env, char *key)
{
    t_env *current = env;

    if (!key || !env)
        return (NULL);

    while (current)
    {
        if (strcmp(current->var, key) == 0)
            return (ft_strdup(current->content));
        current = current->next;
    }
    return (NULL);
}

/* Joins two strings, frees the first, and returns the result */
char *ft_strjoin_free(char *s1, char *s2)
{
    char *result;
    size_t len1, len2;

    if (!s1 || !s2)
    {
        if (s1)
            free(s1);
        return (NULL);
    }

    len1 = strlen(s1);
    len2 = strlen(s2);
    result = malloc(len1 + len2 + 1);
    if (!result)
    {
        free(s1);
        return (NULL);
    }

    memcpy(result, s1, len1);
    memcpy(result + len1, s2, len2 + 1);
    free(s1);
    return (result);
}

/* Frees a split string array */
void ft_free_split(char **split)
{
    int i;

    if (!split)
        return;

    for (i = 0; split[i]; i++)
        free(split[i]);
    free(split);
}

/* Converts environment list to array for execve */
char **get_env_array(t_env *env)
{
    t_env *current = env;
    int count = 0;
    char **env_array;
    char *temp;
    int i;

    /* Count environment variables */
    while (current)
    {
        count++;
        current = current->next;
    }

    /* Allocate array */
    env_array = malloc(sizeof(char *) * (count + 1));
    if (!env_array)
        return (NULL);

    /* Fill array */
    current = env;
    i = 0;
    while (current)
    {
        /* Format: var=content */
        temp = ft_strjoin(current->var, "=");
        if (!temp)
        {
            ft_free_split(env_array);
            return (NULL);
        }
        env_array[i] = ft_strjoin_free(temp, current->content);
        if (!env_array[i])
        {
            ft_free_split(env_array);
            return (NULL);
        }
        i++;
        current = current->next;
    }
    env_array[i] = NULL;

    return (env_array);
}