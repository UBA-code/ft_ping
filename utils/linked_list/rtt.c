/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rtt.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybel-hac <ybel-hac@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 00:31:17 by ybel-hac          #+#    #+#             */
/*   Updated: 2025/04/22 06:14:58 by ybel-hac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_ping.h"

void pushEnd(rtt_list_head *head, double rtt)
{
	rtt_node *node = calloc(sizeof(rtt_node), sizeof(rtt_node));
	rtt_node *current = head->node;

	if (!node)
		ft_error(1, "rtt_node Memory allocation failed", false);
	//* create a node with the specefied rtt
	node->rtt = rtt;
	node->next = 0;
	if (!current)
		head->node = node;
	else
	{
		while (current->next)
			current = current->next;
		current->next = node;
	}
}

int getListLen(rtt_list_head *head)
{
	rtt_node *current = head->node;
	int len = 0;

	while (current)
	{
		current = current->next;
		len++;
	}
	return len;
}

double calcRttSum(rtt_list_head *head)
{
	rtt_node *current = head->node;
	double sum = 0;

	while (current)
	{
		sum += current->rtt;
		current = current->next;
	}

	return sum;
}

void cleanList(rtt_list_head *head)
{
	if (!head)
		return;
	rtt_node *current = head->node;
	rtt_node *tmp = current;

	while (current)
	{
		tmp = current;
		current = current->next;
		free(tmp);
	}
	if (head)
	{
		free(head);
		head = NULL;
	}
}

double getAvg(rtt_list_head *head)
{
	if (!head->node)
		return 0;
	return (calcRttSum(head) / getListLen(head));
}

double getStdDev(rtt_list_head *head)
{
	double squaresSum = 0;
	rtt_node *current = head->node;
	double avg = getAvg(head);

	if (getListLen(head) <= 1)
		return 0;
	while (current)
	{
		squaresSum += pow(current->rtt - avg, 2);
		current = current->next;
	}
	return sqrt(squaresSum / getListLen(head));
}
