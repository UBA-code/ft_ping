/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybel-hac <ybel-hac@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 00:30:52 by ybel-hac          #+#    #+#             */
/*   Updated: 2025/04/17 19:50:12 by ybel-hac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/ft_ping.h"

void ft_error(int code, char *msg, bool readErrno)
{
	if (readErrno)
	{
		perror(msg);
	}
	else
	{
		printf("ft_ping: %s\n", msg);
	}
	freeResources();
	exit(code);
}

int main(int ac, char *av[])
{
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
	initialize_struct();

	argumentsChecker(av + 1);

	if (ping_struct->options.usageIsSpecified)
		printUsage();

	//* handle the ctr + c signal
	signal(SIGINT, finisher);
	pinger();
	return 0;
}
