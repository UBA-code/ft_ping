/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   progressValidPacket.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybel-hac <ybel-hac@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 11:37:51 by ybel-hac          #+#    #+#             */
/*   Updated: 2025/04/25 11:37:55 by ybel-hac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_ping.h"

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
