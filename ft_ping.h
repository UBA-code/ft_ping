#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <stdbool.h>
#include <ctype.h>
#include <arpa/inet.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <sys/types.h>
#include <netdb.h>
#include <math.h>
#include <sys/time.h>

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
	int verboseIsSpecified;
	int usageIsSpecified;
} ping_options;

//* program main struct
typedef struct f_ping
{
	int socket;
	icmp_hdr icmpHeader;
	char *host;
	ping_options options;
	//* final results propeties
	rtt_list_head *rttListHead;
	int packetsTransmitted, packetReceived;
	double min_rtt, max_rtt, avg_rtt;
	struct timeval programStartTime;
	struct timeval programEndTime;
} ping;

ping *ping_struct;

#define ECHO_REQUEST_CODE 0

void ft_error(int code, char *msg);
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