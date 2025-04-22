/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hosts.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybel-hac <ybel-hac@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 11:05:36 by ybel-hac          #+#    #+#             */
/*   Updated: 2025/04/22 06:11:25 by ybel-hac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_ping.h"

void addHost(t_host *head, char *host)
{
	t_host *newHost = malloc(sizeof(t_host));
	if (!newHost)
		ft_error(1, "malloc() failed", false);
	newHost->host = strdup(host);
	if (!newHost->host)
	{
		free(newHost);
		perror("Failed to duplicate host string");
		freeResources();
		exit(EXIT_FAILURE);
	}
	if (!head)
		ping_struct->hostsHead = newHost;
	else
	{
		t_host *current = head;
		while (current->next)
			current = current->next;
		current->next = newHost;
	}
	newHost->next = NULL;
}

void freeHosts(t_host *head)
{
	t_host *current = head;
	t_host *next;

	while (current)
	{
		next = current->next;
		free(current->host);
		current->host = NULL;
		free(current);
		current = next;
	}
	if (ping_struct->hostsHead)
		ping_struct->hostsHead = NULL;
}
