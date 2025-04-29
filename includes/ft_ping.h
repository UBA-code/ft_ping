/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybel-hac <ybel-hac@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 00:31:07 by ybel-hac          #+#    #+#             */
/*   Updated: 2025/04/29 07:51:04 by ybel-hac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PING_H
#define FT_PING_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <stdbool.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdarg.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <sys/types.h>
#include <netdb.h>
#include <math.h>
#include <sys/time.h>
#include <time.h>
#include <signal.h>

#define TTL_VALUE 64
#define RESPONSE_WAIT_TIME 10
#define TIME_BETWEEN_PACKETS 1
#define IP_HEADER_SIZE 20
#define ICMP_PACKET_SIZE 64
#define ECHO_REQUEST_CODE 0
#define ICMP_HEADER_SIZE 8

#define IS_VALID_RANGE(value) ((value) >= 1 && (value) <= INT_MAX)

typedef struct s_rtt_node rtt_node;

//* rtt linked list head
typedef struct s_rtt_list_head
{
  rtt_node *node;
} rtt_list_head;

//* rtt linked list node
typedef struct s_rtt_node
{
  double rtt;
  rtt_node *next;
} rtt_node;

//* define a icmp header (64 bytes)
typedef struct s_icmp_hdr
{
  uint8_t type, code;
  uint16_t checksum, id, sequence;
  char data[56];
} icmp_hdr;

//* define a ip header (problem: different os implementation)
typedef struct s_ip_hdr
{
  uint16_t verhdrlen;
  uint16_t length;
  uint16_t datagramId;
  uint16_t flagsFragmentOffset;
  uint8_t ttl, proto;
  uint16_t checksum;
  uint32_t sourceIp, destIp;
  char *data;
} ip_hdr;

typedef struct s_ping_options
{
  bool verboseIsSpecified;
  bool usageIsSpecified;
  bool countIsSpecified;
  long long countAmount;
  bool quitModeIsSpecified;
  bool debugModeIsSpecified;
  bool timeOutIsSpecified;
  long long timeOutAmount;
  bool stopAfterIsSpecified;
  long long stopAfterAmount;
} ping_options;

typedef struct s_pendingPacket PendingPacket;

typedef struct s_pendingPacket
{
  uint16_t seq;
  struct timeval sent_time;
  PendingPacket *next;
} PendingPacket;

typedef struct s_host t_host;
typedef struct s_host
{
  char *host;
  t_host *next;
} t_host;

//* program main struct
typedef struct f_ping
{
  PendingPacket *pendingPacketsHead;
  //* socket properties
  int socket;
  fd_set readFds;
  //*icmp header properties
  icmp_hdr icmpHeader;
  t_host *hostsHead; //* incremented for each host
  ping_options options;
  struct addrinfo *results;
  //* final results properties
  rtt_list_head *rttListHead;
  int packetsTransmitted, packetReceived;
  double min_rtt, max_rtt, avg_rtt;
  struct timeval programStartTime;
  struct timeval programEndTime;
  bool somePacketsIsExpired;
} ping;

#ifndef PING_STRUCT
extern ping *ping_struct;
#endif

void ft_error(int code, char *msg, bool readErrno);
void argumentsChecker(char **args);
u_short calcCheckSum(void *packet, int length);
void finisher(bool cleanResources, bool tryHosts);
void signalHandler(int signum);
void pushEnd(rtt_list_head *head, double rtt);
int getListLen(rtt_list_head *head);
double calcRttSum(rtt_list_head *head);
void cleanList(rtt_list_head *head);
double getAvg(rtt_list_head *head);
double getStdDev(rtt_list_head *head);
void initialize_struct();
void pinger(char *host, bool once);
void printUsage();
void freeResources();
void ft_error_printf(int errCode, char *format, ...);
void progressValidReply(
    float *rtt, struct timeval endTime,
    char *ip_str, icmp_hdr *reply, ip_hdr *ipHeader, int bytesReceived);
void addPacketToList(PendingPacket **head, uint16_t seq);
PendingPacket *removePacketFromList(uint16_t seq);
void freePendingPackets(PendingPacket **head);
int isValidPacket(PendingPacket **head, uint16_t seq);
void sendPacket(struct addrinfo *results, struct timeval *sendingTime, bool instant);
PendingPacket *getPacketFromList(uint16_t seq);
int removeExpiredPackets();
void printList();
void printSpecifiedOptions();
void checkAndSetOptionAmount(char arg, char *value);
void addHost(t_host *head, char *host);
void freeHosts(t_host *head);
void resetStruct(const ping const_ping_struct);
void progressInvalidReply(char *recvBuffer, int bytesReceived, char *ip_str, ip_hdr *ipHeader);

#endif
