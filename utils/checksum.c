/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checksum.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybel-hac <ybel-hac@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 00:31:23 by ybel-hac          #+#    #+#             */
/*   Updated: 2025/04/13 00:31:24 by ybel-hac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_ping.h"

u_short calcCheckSum(void *packet, int length)
{
  uint16_t *buf = packet;
  uint32_t sum = 0;

  //* add every two word: (type + code) + (id + sequence) as hex
  while (length > 1)
  {
    sum += *buf++;
    length -= 2;
  }

  // // Handle odd byte if present
  // if (length == 1)
  // {
  //   sum += *(uint8_t *)buf; // Add last byte (padded with 0)
  // }

  // Fold 32-bit sum into 16 bits
  while (sum >> 16)
    sum = (sum & 0xFFFF) + (sum >> 16);

  // Return one's complement
  return (uint16_t)~sum; //? ICMP uses one’s complement checksum (RFC 792).
}
