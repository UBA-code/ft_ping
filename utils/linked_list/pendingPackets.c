/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pendingPackets.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybel-hac <ybel-hac@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 15:37:47 by ybel-hac          #+#    #+#             */
/*   Updated: 2025/04/27 20:02:32 by ybel-hac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_ping.h"

void addPacketToList(PendingPacket **head, uint16_t seq)
{
  PendingPacket *new;
  PendingPacket *current = *head;

  new = malloc(sizeof(PendingPacket));
  if (!new)
    ft_error(1, "malloc failed", false);
  new->seq = seq;
  gettimeofday(&new->sent_time, NULL);
  new->next = NULL;
  if (*head == NULL)
  {
    *head = new;
    return;
  }

  while (current->next != NULL)
    current = current->next;
  current->next = new;
}

//* return true if packet still valid, else false (remove the packet from the list if expired)
int isValidPacket(PendingPacket **head, uint16_t seq)
{
  PendingPacket *currentPacket = *head;
  struct timeval currentTime;

  while (currentPacket != NULL)
  {
    if (currentPacket->seq == seq)
    {
      gettimeofday(&currentTime, NULL);
      if ((currentTime.tv_sec - currentPacket->sent_time.tv_sec) * 1000.0 +
              (currentTime.tv_usec - currentPacket->sent_time.tv_usec) / 1000.0 <=
          ping_struct->options.timeOutAmount * 1000.0)
        return (1);
      else
      {
        removePacketFromList(seq);
        return (0);
      }
    }
    currentPacket = currentPacket->next;
  }
  return (0);
}

PendingPacket *getPacketFromList(uint16_t seq)
{
  PendingPacket *current = ping_struct->pendingPacketsHead;

  while (current != NULL)
  {
    if (current->seq == seq)
      return current;
    current = current->next;
  }
  return NULL;
}

PendingPacket *removePacketFromList(uint16_t seq)
{
  PendingPacket *current = ping_struct->pendingPacketsHead;
  PendingPacket *prev = NULL;

  while (current != NULL)
  {
    if (current->seq == seq)
    {
      if (prev == NULL)
        ping_struct->pendingPacketsHead = current->next;
      else
        prev->next = current->next;
      free(current);
      return prev;
    }
    prev = current;
    current = current->next;
  }
  return current;
}

void freePendingPackets(PendingPacket **head)
{
  PendingPacket *current = *head;
  PendingPacket *next;

  while (current != NULL)
  {
    next = current->next;
    free(current);
    current = next;
  }
  *head = NULL;
}

//* remove all expired packets from the list, return 1 if count is specified and all packets are transmitted
//* return 0 if there are still packets in the list
int removeExpiredPackets()
{
  PendingPacket *current = ping_struct->pendingPacketsHead;
  struct timeval currentTime = {0, 0};
  time_t timeElpasedSinceLastRequest = 0;
  PendingPacket *prev = NULL;
  PendingPacket *temp = NULL;
  int len = 0;

  while (current != NULL)
  {
    gettimeofday(&currentTime, NULL);
    timeElpasedSinceLastRequest = (currentTime.tv_sec - current->sent_time.tv_sec) * 1000.0 + (currentTime.tv_usec - current->sent_time.tv_usec) / 1000.0;

    //* if the packet exceed the time limit, remove it from the list
    if (timeElpasedSinceLastRequest >= ping_struct->options.timeOutAmount * 1000)
    {
      //* if the packet is the first one, point the head on the next one
      if (!prev)
        ping_struct->pendingPacketsHead = current->next;
      else
        prev->next = current->next;
      temp = current;
      current = current->next;
      free(temp);
      ping_struct->somePacketsIsExpired = true;
    }
    else
    {
      prev = current;
      current = current->next;
      len++;
    }
  }
  if (len == 0 && ping_struct->options.countIsSpecified && ping_struct->options.countAmount == 0)
  {
    if (ping_struct->pendingPacketsHead)
      freePendingPackets(&ping_struct->pendingPacketsHead);
    return 1;
  }
  return 0;
}

void printList()
{
  PendingPacket *current = ping_struct->pendingPacketsHead;

  printf("################ Print Pending requests ################\n");
  while (current != NULL)
  {
    printf("Pending packet seq: %d\n", current->seq);
    current = current->next;
  }
  printf("########################################################\n");
}
