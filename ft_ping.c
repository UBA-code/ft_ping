/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybel-hac <ybel-hac@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 00:30:52 by ybel-hac          #+#    #+#             */
/*   Updated: 2025/04/29 10:13:33 by ybel-hac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/ft_ping.h"

ping *ping_struct;

void ft_error(int code, char *msg, bool readErrno)
{
  if (readErrno)
  {
    perror(msg);
  }
  else
  {
    fprintf(stderr, "ft_ping: %s\n", msg);
  }
  freeResources();
  exit(code);
}

void ft_error_printf(int errCode, char *format, ...)
{
  va_list args;
  va_start(args, format);
  vfprintf(stderr, format, args);
  va_end(args);
  freeResources();
  exit(errCode);
}

int main(int ac, char *av[])
{
  t_host *currentHost;
  ping const_ping_struct;

  if (geteuid())
    ft_error(1, "Please run the executable with root permessions", false);
  if (ac <= 1)
  {
    ft_error(64, "\
missing host operand\n\
Try 'ft_ping -?' for more information.",
             false);
  }

  ping_struct = calloc(sizeof(ping), sizeof(ping));
  if (!ping_struct)
    ft_error(1, "ping_struct Memory allocation failed", false);
  initialize_struct();

  argumentsChecker(av + 1);

  const_ping_struct = *ping_struct;
  // printSpecifiedOptions();

  if (ping_struct->options.usageIsSpecified)
    printUsage();

  //* handle the ctr + c signal
  signal(SIGINT, signalHandler);
  currentHost = ping_struct->hostsHead;
  while (currentHost)
  {
    pinger(currentHost->host, false);
    finisher(false, false);
    if (currentHost->next)
      resetStruct(const_ping_struct);
    currentHost = currentHost->next;
  }
  if (ping_struct->somePacketsIsExpired || ping_struct->packetReceived == 0)
  {
    freeResources();
    return 1;
  }
  freeResources();
  return 0;
}

void resetStruct(const ping const_ping_struct)
{
  ping_struct->packetsTransmitted = 0;
  ping_struct->packetReceived = 0;
  ping_struct->min_rtt = -1;
  ping_struct->max_rtt = 0;
  ping_struct->avg_rtt = 0;
  ping_struct->options.countIsSpecified = const_ping_struct.options.countIsSpecified;
  ping_struct->options.countAmount = const_ping_struct.options.countAmount;
  ping_struct->options.debugModeIsSpecified = const_ping_struct.options.debugModeIsSpecified;
  ping_struct->options.timeOutIsSpecified = const_ping_struct.options.timeOutIsSpecified;
  ping_struct->options.timeOutAmount = const_ping_struct.options.timeOutAmount;
  freePendingPackets(&ping_struct->pendingPacketsHead);
  cleanList(ping_struct->rttListHead);
  ping_struct->rttListHead = calloc(sizeof(rtt_list_head), sizeof(rtt_list_head));
  if (!ping_struct->rttListHead)
    ft_error(1, "rttListHead Memory allocation failed", false);
  ping_struct->rttListHead->node = 0;
}

void printSpecifiedOptions()
{
  t_host *current = ping_struct->hostsHead;
  printf("### hosts ###\n");
  while (current)
  {
    printf("Host: %s\n", current->host);
    current = current->next;
  }
  printf("### options ###\n");
  printf("-?: %d\n", ping_struct->options.usageIsSpecified);
  printf("-v: %d\n", ping_struct->options.verboseIsSpecified);
  printf("### bonus ###\n");
  if (ping_struct->options.countIsSpecified)
    printf("-c: %lld\n", ping_struct->options.countAmount);
  else
    printf("-c: %d\n", ping_struct->options.countIsSpecified);
  if (ping_struct->options.timeOutIsSpecified)
    printf("-W: %lld\n", ping_struct->options.timeOutAmount);
  else
    printf("-W: %d\n", ping_struct->options.timeOutIsSpecified);
  if (ping_struct->options.stopAfterIsSpecified)
    printf("-w: %lld\n", ping_struct->options.stopAfterAmount);
  else
    printf("-w: %d\n", ping_struct->options.stopAfterIsSpecified);

  printf("-d: %d\n", ping_struct->options.debugModeIsSpecified);
  printf("-q: %d\n", ping_struct->options.quitModeIsSpecified);
  exit(0);
}
