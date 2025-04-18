/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pinger.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybel-hac <ybel-hac@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 00:31:15 by ybel-hac          #+#    #+#             */
/*   Updated: 2025/04/18 10:43:47 by ybel-hac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ping.h"

void pinger()
{
	struct addrinfo hints;
	struct addrinfo *results;
	char recvBuffer[84];
	char ip_str[INET_ADDRSTRLEN];
	const long long timeToWait = ping_struct->options.timeToWaitResponse == -1
																	 ? 1
																	 : ping_struct->options.timeToWaitResponse;
	struct timeval timeout = {timeToWait, 0}; //* for setting timeout to the socket

	bzero(&ip_str, sizeof(ip_str));
	bzero(&hints, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_RAW;
	hints.ai_protocol = IPPROTO_ICMP;

	//* set timeout for socket if no data come in 1 second, move on instead of hanging forever waiting
	if (setsockopt(ping_struct->socket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)))
		ft_error(1, "Error: setsockopt failed", false);

	if (getaddrinfo(ping_struct->host, NULL, &hints, &results))
		ft_error(1, "unknown host", false);

	//* get the ip address of the host
	if (!inet_ntop(AF_INET, &((struct sockaddr_in *)results->ai_addr)->sin_addr, ip_str, INET_ADDRSTRLEN))
		ft_error(1, "inet_ntop failed", false);

	printf("PING %s (%s): 56 data bytes", ping_struct->host, ip_str);
	if (ping_struct->options.verboseIsSpecified)
		printf(", id 0x%x = %d", ping_struct->icmpHeader.id, ping_struct->icmpHeader.id);
	printf("\n");

	//* if the -c if specefied and it's value more than 0, increment the value to prevent the value from being 0
	if (ping_struct->options.countIsSpecified)
		ping_struct->options.countIsSpecified++;

	//* either -c not specified or it's value is less or equal 0 run the loop infinitely
	//* if the -c value more than 0, run the loop for the amount of specific packets

	while (ping_struct->options.countIsSpecified <= 0 ||
				 ping_struct->options.countIsSpecified-- > 1) //* if we loop untill 0, the first condition will be applied and the loop will repeat infinitely
	{
		struct timeval startTime, endTime;
		float rtt; //* round trip time
		int bytesReceived;
		icmp_hdr *reply = NULL;
		ip_hdr *ipHeader;

		bzero(&recvBuffer, sizeof(recvBuffer));

		//* save the start time before send the packet
		if (gettimeofday(&startTime, NULL))
			ft_error(1, "gettimeofday", true);

		//* send the packet
		if (
				sendto(ping_struct->socket, &ping_struct->icmpHeader, 64, 0, results->ai_addr, results->ai_addrlen) == -1)
		{
			ft_error(1, "ft_ping: sending packet", true);
		}

		//* increment the packet transmitted if the sendTo run successfully
		ping_struct->packetsTransmitted++;

		//* wait for packets
		bytesReceived = recvfrom(ping_struct->socket, &recvBuffer, sizeof(recvBuffer), 0, (struct sockaddr *)results, NULL);

		printf("bytesReceived: %d\n", bytesReceived);

		//* get the end time when the packet came
		if (gettimeofday(&endTime, NULL))
			ft_error(1, "gettimeofday failed", false);

		//* extract the ip header and the icmp reply
		ipHeader = (ip_hdr *)recvBuffer;
		reply = (icmp_hdr *)&recvBuffer[20];

		//* if the icmp type is reply and the id match the pid that's mean the packet is for us
		if (reply->type == ICMP_ECHOREPLY && reply->id == ping_struct->icmpHeader.id && bytesReceived >= 0)
		{
			//* calc the round trip time
			rtt = (endTime.tv_sec - startTime.tv_sec) * 1000.0 + (endTime.tv_usec - startTime.tv_usec) / 1000.0;

			//* set the max and min round-trip-time values
			if (rtt > ping_struct->max_rtt)
				ping_struct->max_rtt = rtt;
			if (rtt < ping_struct->min_rtt || ping_struct->min_rtt == -1)
				ping_struct->min_rtt = rtt;

			//* save the current rtt in the linked list
			pushEnd(ping_struct->rttListHead, rtt);

			//* increment the packet received when the recvfrom response valid
			ping_struct->packetReceived++;

			//* if the quit option is not specified print the log message
			if (!ping_struct->options.quitModeIsSpecified)
				printf("%d bytes from %s: icmp_seq=%d ttl=%d time=%.3f ms\n",
							 bytesReceived - 20,
							 ip_str,
							 reply->sequence,
							 ipHeader->ttl,
							 rtt); // 20 bytes of the ip header
		}
		// else if (reply->type == ICMP_UNREACH_HOST && ping_struct->options.verboseIsSpecified)
		// {
		// 	printf("ICMP Host Unreachable from %s for icmp_seq=%d\n",
		// 				 ip_str, ping_struct->icmpHeader.sequence);
		// }
		// //* if the bytes received less than 0, that's mean the request timedout
		// else if (bytesReceived < 0 && ping_struct->options.verboseIsSpecified)
		// {
		// 	printf("Request timeout for icmp_seq %d\n", ping_struct->icmpHeader.sequence);
		// }

		//* update the sequence and reset checksum and calc it again to send new packet
		ping_struct->icmpHeader.sequence++;
		ping_struct->icmpHeader.checksum = 0;
		ping_struct->icmpHeader.checksum = calcCheckSum(&(ping_struct->icmpHeader), 64);

		//* prevent sleep in last packet
		if (ping_struct->options.countIsSpecified > 1 || ping_struct->options.countIsSpecified <= 0)
			sleep(1);
	}
	finisher(0);
}
