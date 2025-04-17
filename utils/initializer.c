/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initializer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybel-hac <ybel-hac@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 00:31:19 by ybel-hac          #+#    #+#             */
/*   Updated: 2025/04/17 14:30:59 by ybel-hac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ping.h"

void initialize_struct()
{
	//* Create socket of type raw and icmp protocol
	ping_struct->socket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

	//* check if socket failed
	if (ping_struct->socket == -1)
		ft_error(1, "failed to create socket", false);

	//* fill the icmp header
	ping_struct->icmpHeader.type = ICMP_ECHO;
	ping_struct->icmpHeader.code = ECHO_REQUEST_CODE;
	ping_struct->icmpHeader.id = getpid();
	ping_struct->icmpHeader.sequence = 0;
	ping_struct->icmpHeader.checksum = calcCheckSum(&ping_struct->icmpHeader, 64);

	//* initialize some propeties
	ping_struct->packetsTransmitted = 0;
	ping_struct->packetReceived = 0;
	ping_struct->min_rtt = -1;
	ping_struct->max_rtt = 0;
	ping_struct->avg_rtt = 0;

	ping_struct->rttListHead = calloc(sizeof(rtt_list_head), sizeof(rtt_list_head));
	ping_struct->rttListHead->node = 0;
	gettimeofday(&ping_struct->programStartTime, NULL);
}
