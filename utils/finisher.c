/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   finisher.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybel-hac <ybel-hac@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 00:31:21 by ybel-hac          #+#    #+#             */
/*   Updated: 2025/04/22 08:52:20 by ybel-hac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ping.h"

void signalHandler(int signum)
{
  (void)signum;
  finisher(true, true);
}

//* if cleanResources is true, free the resources and exit
//* if tryHosts is true, send one packet to each host left without waiting for reply (ctrl + c case)
void finisher(bool cleanResources, bool tryHosts)
{
  //* if no packets transmitted nothing must be printed
  if (ping_struct->packetsTransmitted)
  {

    //* when ctrl + c clicked, set the end time
    gettimeofday(&ping_struct->programEndTime, NULL);
    // const size_t totalTimeSpent = ((ping_struct->programEndTime.tv_sec - ping_struct->programStartTime.tv_sec) * 1000) + ((ping_struct->programEndTime.tv_usec - ping_struct->programStartTime.tv_usec) / 1000);

    //* print the final log message
    printf("--- %s ping statistics ---\n", ping_struct->hostsHead->host);
    printf("%d packets transmitted, ", ping_struct->packetsTransmitted);
    printf("%d packets received, ", ping_struct->packetReceived);
    if (ping_struct->packetReceived == 0 && ping_struct->packetsTransmitted > 0)
    {
      printf("%.0f%% packet loss\n", 100.0);
    }
    else
    {
      printf("%.0f%% packet loss\n", (double)((double)(ping_struct->packetsTransmitted - ping_struct->packetReceived) / ping_struct->packetsTransmitted * 100));
    }
    if (ping_struct->packetReceived > 0)
    {
      // printf("time %zums\n", totalTimeSpent);
      printf("round-trip min/avg/max/stddev = ");
      printf("%.3f/%.3f/%.3f/%0.3f ms\n",
             ping_struct->min_rtt == -1 ? 0 : ping_struct->min_rtt,
             getAvg(ping_struct->rttListHead),
             ping_struct->max_rtt,
             getStdDev(ping_struct->rttListHead));
    }
  }
  else
  {
    printf("\n");
  }

  if (tryHosts)
  {
    if (ping_struct->hostsHead)
      ping_struct->hostsHead = ping_struct->hostsHead->next;
    t_host *currentHost = ping_struct->hostsHead;

    while (currentHost)
    {
      ping_struct->packetsTransmitted = 0;
      ping_struct->packetReceived = 0;
      if (ping_struct->results)
      {
        freeaddrinfo(ping_struct->results); //* free the addrinfo struct
        ping_struct->results = NULL;
      }
      pinger(currentHost->host, true);
      finisher(false, false);
      currentHost = currentHost->next;
      ping_struct->hostsHead = ping_struct->hostsHead->next;
    }
  }

  //* free the resources
  if (cleanResources)
  {
    int status = 0;
    if (ping_struct->somePacketsIsExpired || ping_struct->packetReceived == 0)
      status = 1;
    freeResources();
    exit(status);
  }
}
