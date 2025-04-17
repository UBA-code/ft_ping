/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freeResources.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybel-hac <ybel-hac@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 14:47:55 by ybel-hac          #+#    #+#             */
/*   Updated: 2025/04/17 20:55:11 by ybel-hac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ping.h"

void freeResources()
{
	if (ping_struct)
	{
		cleanList(ping_struct->rttListHead);
		close(ping_struct->socket);
		free(ping_struct);
	}
	// system("leaks a.out");
}
