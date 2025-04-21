/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pinger.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybel-hac <ybel-hac@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 00:31:15 by ybel-hac          #+#    #+#             */
/*   Updated: 2025/04/20 19:51:06 by ybel-hac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ping.h"

void pinger()
{
	struct addrinfo hints;
	struct addrinfo *results;
	char recvBuffer[84];
	char ip_str[INET_ADDRSTRLEN];
	struct timeval timeout = {1, 0}; //* for setting timeout to the socket
	struct timeval sendingTime = {0, 0};

	bzero(&ip_str, sizeof(ip_str));
	bzero(&hints, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_RAW;
	hints.ai_protocol = IPPROTO_ICMP;

	if (ping_struct->options.debugModeIsSpecified &&
			setsockopt(ping_struct->socket, SOL_SOCKET, SO_DEBUG, &timeout, sizeof(timeout)))
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

	//* main loop
	while (1)
	{
		removeExpiredPackets();
		struct timeval endTime;
		float rtt; //* round trip time
		int bytesReceived = -1;
		icmp_hdr *reply = NULL;
		ip_hdr *ipHeader;

		bzero(&recvBuffer, sizeof(recvBuffer));

		//* send the packet after checking the sending time and add it to the list
		sendPacket(results, &sendingTime);

		//* wait for packets
		FD_ZERO(&ping_struct->readFds);
		FD_SET(ping_struct->socket, &ping_struct->readFds);
		if (select(ping_struct->socket + 1, &ping_struct->readFds, NULL, NULL, &timeout) == -1)
			ft_error(1, "select()", true);
		else if (FD_ISSET(ping_struct->socket, &ping_struct->readFds))
		{
			//* get the end time when the packet came
			if (gettimeofday(&endTime, NULL))
				ft_error(1, "gettimeofday failed", false);

			bytesReceived = recvfrom(ping_struct->socket, &recvBuffer, sizeof(recvBuffer), 0, (struct sockaddr *)results, NULL);

			//* extract the ip header and the icmp reply
			ipHeader = (ip_hdr *)recvBuffer;
			reply = (icmp_hdr *)&recvBuffer[IP_HEADER_SIZE];

			const ushort tempChecksum = reply->checksum;
			//* reset the checksum to 0 and calc it again
			reply->checksum = 0;

			//* if the icmp type is reply and the id match the pid that's mean the packet is for us
			if (reply->type == ICMP_ECHOREPLY && reply->id == ping_struct->icmpHeader.id && bytesReceived >= 0 && tempChecksum == calcCheckSum(reply, bytesReceived - sizeof(ip_hdr)))
			{
				if (isValidPacket(&ping_struct->pendingPacketsHead, reply->sequence))
					progressValidReply(
							&rtt, endTime, ip_str, reply, ipHeader, bytesReceived);
			}
		}
	}
	finisher(0);
}

void sendPacket(struct addrinfo *results, struct timeval *sendingTime)
{
	struct timeval currentTime;
	time_t timeElpasedSinceLastSend;

	gettimeofday(&currentTime, NULL);
	timeElpasedSinceLastSend = (currentTime.tv_sec - sendingTime->tv_sec) * 1000 +
														 (currentTime.tv_usec - sendingTime->tv_usec) / 1000;

	if (timeElpasedSinceLastSend >= 1000.0 &&
			(!ping_struct->options.countIsSpecified || ping_struct->options.countAmount > 0)) //* 1000ms = 1 second, send request each second
	{
		addPacketToList(&ping_struct->pendingPacketsHead, ping_struct->icmpHeader.sequence);
		//* send the packet
		if (
				sendto(ping_struct->socket, &ping_struct->icmpHeader, ICMP_PACKET_SIZE, 0, results->ai_addr, results->ai_addrlen) == -1)
		{
			ft_error(1, "ft_ping: sending packet", true);
		}

		gettimeofday(sendingTime, NULL);
		//* increment the packet transmitted if the sendTo run successfully
		ping_struct->packetsTransmitted++;

		//* update the sequence and reset checksum and calc it again to send new packet
		ping_struct->icmpHeader.sequence++;
		ping_struct->icmpHeader.checksum = 0;
		ping_struct->icmpHeader.checksum = calcCheckSum(&(ping_struct->icmpHeader), ICMP_PACKET_SIZE);

		//* if the count option is specified, decrement the count amount
		if (ping_struct->options.countIsSpecified)
			ping_struct->options.countAmount--;
	}
}

void progressValidReply(
		float *rtt, struct timeval endTime,
		char *ip_str, icmp_hdr *reply, ip_hdr *ipHeader, int bytesReceived)
{
	PendingPacket *packet = getPacketFromList(reply->sequence);
	//* calc the round trip time
	*rtt = (endTime.tv_sec - (packet->sent_time.tv_sec - 1)) * 1000.0 + (endTime.tv_usec - packet->sent_time.tv_usec) / 1000.0; //* we remove the 1 second added to the sent time

	//* set the max and min round-trip-time values
	if (*rtt > ping_struct->max_rtt)
		ping_struct->max_rtt = *rtt;
	if (*rtt < ping_struct->min_rtt || ping_struct->min_rtt == -1)
		ping_struct->min_rtt = *rtt;

	//* save the current rtt in the linked list
	pushEnd(ping_struct->rttListHead, *rtt);

	//* increment the packet received when the recvfrom response valid
	ping_struct->packetReceived++;

	//* if the quit option is not specified print the log message
	if (!ping_struct->options.quitModeIsSpecified)
		printf("%d bytes from %s: icmp_seq=%d ttl=%d time=%.3f ms\n",
					 bytesReceived - IP_HEADER_SIZE,
					 ip_str,
					 reply->sequence,
					 ipHeader->ttl,
					 *rtt); // 20 bytes of the ip header

	//* remove the packet from the list
	removePacketFromList(reply->sequence);
}
