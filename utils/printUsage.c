/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printUsage.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybel-hac <ybel-hac@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 17:48:27 by ybel-hac          #+#    #+#             */
/*   Updated: 2025/04/17 14:31:53 by ybel-hac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ping.h"

void printUsage()
{
	printf("Usage: ft_ping [OPTION...] HOST ...\n");
	printf("Send ICMP ECHO_REQUEST packets to network hosts.\n");
	printf("\n");
	printf("Options valid for all request types:\n");
	printf("\n");
	printf("-v \t\tverbose output\n");
	printf("-? \t\tgive this help list\n");
	freeResources();
	exit(0);
}
