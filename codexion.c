#include <stdio.h>
#include "codexion.h"

int main(int ac,char **av)
{
    t_config *config;

    config = malloc(sizeof(t_config));
    if (parsing(ac, av, config) == 1)
        return (free(config), 1);
    free(config);
    return 0;
}