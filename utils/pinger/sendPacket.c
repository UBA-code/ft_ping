/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sendPacket.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybel-hac <ybel-hac@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 11:36:27 by ybel-hac          #+#    #+#             */
/*   Updated: 2025/04/27 19:56:00 by ybel-hac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_ping.h"

//* if instant is true, send the packet immediately without waiting 1 second between each packet
void sendPacket(struct addrinfo *results, struct timeval *sendingTime, bool instant)
{
  struct timeval currentTime;
  time_t timeElpasedSinceLastSend;

  gettimeofday(&currentTime, NULL);
  timeElpasedSinceLastSend = (currentTime.tv_sec - sendingTime->tv_sec) * 1000 +
                             (currentTime.tv_usec - sendingTime->tv_usec) / 1000;

  if ((timeElpasedSinceLastSend >= 1000.0 &&
       (!ping_struct->options.countIsSpecified || ping_struct->options.countAmount > 0)) ||
      instant) //* 1000ms = 1 second, send request each second
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
