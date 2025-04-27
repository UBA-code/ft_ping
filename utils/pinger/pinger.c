/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pinger.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybel-hac <ybel-hac@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 00:31:15 by ybel-hac          #+#    #+#             */
/*   Updated: 2025/04/27 20:18:28 by ybel-hac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_ping.h"

// take start time and measure the time between the start and the end
// and print the time
void printTime(struct timeval *start)
{
  struct timeval endTime;
  double rtt;

  if (gettimeofday(&endTime, NULL))
    ft_error(1, "gettimeofday failed", false);
  rtt = (endTime.tv_sec - start->tv_sec) * 1000.0 + (endTime.tv_usec - start->tv_usec) / 1000.0;
  printf("=== takes %.3f ms ===\n", rtt);
}

//* is once true, send one packet and exit without waiting for reply
void pinger(char *host, bool once)
{
  struct addrinfo hints;
  char recvBuffer[84];
  char ip_str[INET_ADDRSTRLEN];
  struct timeval timeout = {1, 0};
  struct timeval sendingTime = {0, 0};
  int ttl = TTL_VALUE;
  struct timeval measuringTime;

  bzero(&ip_str, sizeof(ip_str));
  bzero(&hints, sizeof(struct addrinfo));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_RAW;
  hints.ai_protocol = IPPROTO_ICMP;

  if (ping_struct->options.debugModeIsSpecified &&
      setsockopt(ping_struct->socket, SOL_SOCKET, SO_DEBUG, &timeout, sizeof(timeout)) < 0)
    ft_error(1, "ft_ping: setsockopt()", true);

  if (setsockopt(ping_struct->socket, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl)) < 0)
    ft_error(1, "ft_ping: setsockopt()", true);

  if (getaddrinfo(host, NULL, &hints, &ping_struct->results))
    ft_error(1, "unknown host", false);

  //* get the ip address of the host
  if (!inet_ntop(AF_INET, &((struct sockaddr_in *)ping_struct->results->ai_addr)->sin_addr, ip_str, INET_ADDRSTRLEN))
    ft_error(1, "inet_ntop failed", false);

  printf("PING %s (%s): 56 data bytes", host, ip_str);
  if (ping_struct->options.verboseIsSpecified)
    printf(", id 0x%04x = %d", ping_struct->icmpHeader.id, ping_struct->icmpHeader.id);
  printf("\n");

  //* main loop
  while (once || 1)
  {
    if (ping_struct->options.stopAfterIsSpecified)
    {
      struct timeval currentTime;
      time_t timeElpasedSinceProgramStart;

      gettimeofday(&currentTime, NULL);
      timeElpasedSinceProgramStart = (currentTime.tv_sec - ping_struct->programStartTime.tv_sec) * 1000 +
                                     (currentTime.tv_usec - ping_struct->programStartTime.tv_usec) / 1000;
      if (timeElpasedSinceProgramStart >= ping_struct->options.stopAfterAmount * 1000.0)
        finisher(true, false);
    }

    if (removeExpiredPackets())
      break;
    struct timeval endTime;
    float rtt; //* round trip time
    int bytesReceived = -1;
    icmp_hdr *reply = NULL;
    ip_hdr *ipHeader;

    bzero(&recvBuffer, sizeof(recvBuffer));

    //* send the packet after checking the sending time and add it to the list
    sendPacket(ping_struct->results, &sendingTime, once);

    //* send the packet and exit (ctr + c case)
    if (once == 1)
      break;

    FD_CLR(ping_struct->socket, &ping_struct->readFds);
    FD_SET(ping_struct->socket, &ping_struct->readFds);
    int result = select(ping_struct->socket + 1, &ping_struct->readFds, NULL, NULL, &timeout);
    //* wait for packets
    if (result == -1 && errno != EINTR) //* eintr is for the signal handler, if the error because of the signal handler it's okey
      ft_error(1, "select()", true);
    else if (result && FD_ISSET(ping_struct->socket, &ping_struct->readFds))
    {
      //* get the end time when the packet came
      if (gettimeofday(&endTime, NULL))
        ft_error(1, "gettimeofday failed", false);

      bytesReceived = recvfrom(ping_struct->socket, &recvBuffer, sizeof(recvBuffer), 0, (struct sockaddr *)ping_struct->results, &ping_struct->results->ai_addrlen);

      //* extract the ip header and the icmp reply
      ipHeader = (ip_hdr *)recvBuffer;
      reply = (icmp_hdr *)&recvBuffer[IP_HEADER_SIZE];

      const ushort tempChecksum = reply->checksum;
      //* reset the checksum to 0 and calc it again
      reply->checksum = 0;

      //* if the icmp type is reply and the id match the pid that's mean the packet is for us
      if (reply->code == ICMP_ECHOREPLY && reply->type == ICMP_ECHOREPLY && reply->id == ping_struct->icmpHeader.id && bytesReceived > 0 && tempChecksum == calcCheckSum(reply, bytesReceived - sizeof(ip_hdr)))
      {
        if (isValidPacket(&ping_struct->pendingPacketsHead, reply->sequence))
          progressValidReply(
              &rtt, endTime, ip_str, reply, ipHeader, bytesReceived);
      }
      else if (reply->code == ICMP_ECHOREPLY && reply->type == ICMP_TIMXCEED && bytesReceived >= 56)
        progressInvalidReply(
            recvBuffer, bytesReceived, ip_str, ipHeader);
    }
    gettimeofday(&measuringTime, NULL);
    usleep(100);
    printTime(&measuringTime);
  }
  freeaddrinfo(ping_struct->results);
  ping_struct->results = NULL;
}
