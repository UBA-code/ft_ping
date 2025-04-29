/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   progressInvalidPacket.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybel-hac <ybel-hac@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 11:38:34 by ybel-hac          #+#    #+#             */
/*   Updated: 2025/04/29 07:50:17 by ybel-hac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_ping.h"

void progressInvalidReply(char *recvBuffer, int bytesReceived, char *ip_str, ip_hdr *ipHeader)

{
  //* skip the icmp header
  const ip_hdr *oldPacketIpHeader = (ip_hdr *)&recvBuffer[IP_HEADER_SIZE + ICMP_HEADER_SIZE];
  //* skip the ip header of the reply packet + the 8 bytes of the reply icmp header + the ip header of the packet we sent
  const icmp_hdr *oldPacketIcmpHeader = (icmp_hdr *)&recvBuffer[IP_HEADER_SIZE + ICMP_HEADER_SIZE + IP_HEADER_SIZE];
  char src_ip_str[INET_ADDRSTRLEN];
  char hopIp[INET_ADDRSTRLEN];

  bzero(&src_ip_str, sizeof(src_ip_str));

  if (!inet_ntop(AF_INET, &oldPacketIpHeader->sourceIp, src_ip_str, INET_ADDRSTRLEN))
    ft_error(1, "inet_ntop failed", false);
  if (!inet_ntop(AF_INET, &ipHeader->sourceIp, hopIp, INET_ADDRSTRLEN))
    ft_error(1, "inet_ntop failed", false);

  printf("%d bytes from %s: %s\n",
         bytesReceived - IP_HEADER_SIZE,
         hopIp, "Time to live exceeded"); // 20 bytes of the ip header
  if (ping_struct->options.verboseIsSpecified)
  {
    printf("IP Hdr Dump: \n");
    printf(" %04x ", ntohs(oldPacketIpHeader->verhdrlen));
    printf("%04x ", ntohs(oldPacketIpHeader->length));
    printf("%04x ", ntohs(oldPacketIpHeader->datagramId));
    printf("%04x ", ntohs(oldPacketIpHeader->flagsFragmentOffset));
    printf("%02x%02x ", oldPacketIpHeader->ttl, oldPacketIpHeader->proto);
    printf("%04x ", ntohs(oldPacketIpHeader->checksum));
    printf("%04x %04x", ntohs(((uint16_t *)&oldPacketIpHeader->sourceIp)[0]), ntohs(((uint16_t *)&oldPacketIpHeader->sourceIp)[1]));
    printf(" %04x %04x", ntohs(((uint16_t *)&oldPacketIpHeader->destIp)[0]), ntohs(((uint16_t *)&oldPacketIpHeader->destIp)[1]));
    printf("%s", ping_struct->icmpHeader.data);
    printf("\n");

    printf("Vr\tHL\tTOS\tLen\tID\tFlg\toff\tTTL\tPro\tcks\tSrc\tDst\tData\n");
    printf(" %x\t", (ntohs(oldPacketIpHeader->verhdrlen) & 0xF000) >> 12); // get the first 4 bits ver
    printf("%x\t", (ntohs(oldPacketIpHeader->verhdrlen) & 0x0F00) >> 8);   // get the last 4 bits hdrlen
    printf("%02x\t", ntohs(oldPacketIpHeader->verhdrlen) & 0x00FF);        // get the second byte TOS
    printf("%04x\t", ntohs(oldPacketIpHeader->length));
    printf("%04x\t", ntohs(oldPacketIpHeader->datagramId));
    printf("%x\t", (ntohs(oldPacketIpHeader->flagsFragmentOffset) & 0xE000) >> 13); // get the FLAGS
    printf("%04x\t", ntohs(oldPacketIpHeader->flagsFragmentOffset) & 0x1FFF);         // get the OFFSET
    printf("%02x\t", oldPacketIpHeader->ttl);
    printf("%02x\t", oldPacketIpHeader->proto);
    printf("%04x\t", ntohs(oldPacketIpHeader->checksum));
    printf("%s\t", src_ip_str); // source ip
    printf("%s\t", ip_str);     // destination ip
    printf("%s\t", ping_struct->icmpHeader.data);
    printf("\n");

    printf("ICMP: type=%d, code=%d, size=%d, id=0x%04x, seq=0x%04x\n",
           oldPacketIcmpHeader->type,
           oldPacketIcmpHeader->code,
           ICMP_PACKET_SIZE,
           oldPacketIcmpHeader->id,
           oldPacketIcmpHeader->sequence);
  }
  //* remove the packet from the list
  removePacketFromList(oldPacketIcmpHeader->sequence);
}
