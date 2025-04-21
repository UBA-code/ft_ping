/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybel-hac <ybel-hac@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 00:30:52 by ybel-hac          #+#    #+#             */
/*   Updated: 2025/04/20 21:06:24 by ybel-hac         ###   ########.fr       */
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

	// printSpecifiedOptions();

	if (ping_struct->options.usageIsSpecified)
		printUsage();

	//* handle the ctr + c signal
	signal(SIGINT, finisher);
	pinger();
	return 0;
}

void printSpecifiedOptions()
{
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
	printf("-d: %d\n", ping_struct->options.debugModeIsSpecified);
	printf("-q: %d\n", ping_struct->options.quitModeIsSpecified);
	exit(0);
}
