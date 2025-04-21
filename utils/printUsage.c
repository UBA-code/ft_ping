/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printUsage.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybel-hac <ybel-hac@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 17:48:27 by ybel-hac          #+#    #+#             */
/*   Updated: 2025/04/20 19:54:25 by ybel-hac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ping.h"

void printUsage()
{
	printf("Usage: ft_ping [OPTION...] HOST ...\n");
	printf("Send ICMP ECHO_REQUEST packets to network hosts.\n");
	printf("\n");
	printf("-c\tstop after sending <count> packets\n");
	printf("-d\tset the SO_DEBUG option\n");
	printf("-v\tverbose output\n");
	printf("-w\tstop after N seconds (not implemented yet)\n");
	printf("-W\tnumber of seconds to wait for response\n");
	printf("-q\tquiet output\n");
	printf("\n");
	printf("-?\tgive this help list\n");
	freeResources();
	exit(0);
}
