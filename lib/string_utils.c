#include "../minishell.h"

void append_character(char **string, char c)
{
    if (!*string)
        return ;
    if (!c)
        return ;
    char *new;
    int i;

    new = malloc(sizeof(char) *(ft_strlen(*string) + 2));
    if(!new)
        return;
    i = 0;
    while(string[0][i])
    {
        new[i]= string[0][i];
        i++;
    }
    new[i++] = c;
    new[i] = 0;
    free(*string);
    *string = new;
}