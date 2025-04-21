/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freeResources.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybel-hac <ybel-hac@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 14:47:55 by ybel-hac          #+#    #+#             */
/*   Updated: 2025/04/21 17:55:47 by ybel-hac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ping.h"

void freeResources()
{
	if (ping_struct)
	{
		if (ping_struct->results)
		{
			freeaddrinfo(ping_struct->results); //* free the addrinfo struct
			ping_struct->results = NULL;
		}
		if (ping_struct->pendingPacketsHead)
			freePendingPackets(&ping_struct->pendingPacketsHead);
		freeHosts(ping_struct->hostsHead);
		cleanList(ping_struct->rttListHead);
		close(ping_struct->socket);
		free(ping_struct);
		ping_struct = NULL;
	}
	system("leaks ft_ping");
}
