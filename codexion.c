/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imansar <imansar@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/14 15:14:15 by imansar           #+#    #+#             */
/*   Updated: 2026/06/14 15:14:16 by imansar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "codexion.h"

int main(int ac,char **av)
{
    t_config config;
    t_sim sim;

    if (parsing(ac, av, &config) == 1)
        return (1);
    
    if (init_sim(&config, &sim) != 0)
        return 1;
        
    if (start_simulation(&sim) != 0)
    {
        clear_sim(&sim);
        return 1;
    }
    clear_sim(&sim);
    return 0;
}
