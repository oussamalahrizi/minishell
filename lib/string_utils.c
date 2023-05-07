#include "../minishell.h"

char *append_character(char **string, char c)
{
    if (!*string)
        return(NULL);
    if (!c)
        return(*string);
    char *new;
    int i;

    new = malloc(sizeof(char) *(ft_strlen(*string) + 2));
    if(!new)
        return (NULL);
    i = 0;
    while(string[0][i])
    {
        new[i]= string[0][i];
        i++;
    }
    new[i++] = c;
    new[i] = 0;
    free(*string);
    return(new);
}