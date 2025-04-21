/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybel-hac <ybel-hac@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 00:31:07 by ybel-hac          #+#    #+#             */
/*   Updated: 2025/04/20 21:03:37 by ybel-hac         ###   ########.fr       */
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
#include <signal.h>

#define RESPONSE_WAIT_TIME 10
#define TIME_BETWEEN_PACKETS 1
#define IP_HEADER_SIZE 20
#define ICMP_PACKET_SIZE 64
#define ECHO_REQUEST_CODE 0

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
	uint8_t ver : 4, hel : 4, type;
	uint16_t length;
	uint16_t datagramId;
	uint8_t flags : 3;
	uint16_t fragmentOffset : 13;
	uint8_t ttl, proto;
	uint16_t checksum;
	uint32_t sourceIp, destIp;
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
} ping_options;

typedef struct s_pendingPacket PendingPacket;

typedef struct s_pendingPacket
{
	uint16_t seq;
	struct timeval sent_time;
	PendingPacket *next;
} PendingPacket;

//* program main struct
typedef struct f_ping
{
	PendingPacket *pendingPacketsHead;
	//* socket properties
	int socket;
	fd_set readFds;
	//*icmp header properties
	icmp_hdr icmpHeader;
	char *host;
	ping_options options;
	//* final results properties
	rtt_list_head *rttListHead;
	int packetsTransmitted, packetReceived;
	double min_rtt, max_rtt, avg_rtt;
	struct timeval programStartTime;
	struct timeval programEndTime;
} ping;

#ifndef PING_STRUCT
extern ping *ping_struct;
#endif

void ft_error(int code, char *msg, bool readErrno);
void argumentsChecker(char **args);
u_short calcCheckSum(void *packet, int length);
void finisher(int signum);
void pushEnd(rtt_list_head *head, double rtt);
int getListLen(rtt_list_head *head);
double calcRttSum(rtt_list_head *head);
void cleanList(rtt_list_head *head);
double getAvg(rtt_list_head *head);
double getStdDev(rtt_list_head *head);
void initialize_struct();
void pinger();
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
void sendPacket(struct addrinfo *results, struct timeval *sendingTime);
PendingPacket *getPacketFromList(uint16_t seq);
int removeExpiredPackets();
void printList();
void printSpecifiedOptions();
void checkAndSetOptionAmount(char arg, char *value);

#endif
