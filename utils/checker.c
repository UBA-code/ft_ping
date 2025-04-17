/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybel-hac <ybel-hac@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 00:31:26 by ybel-hac          #+#    #+#             */
/*   Updated: 2025/04/17 16:46:04 by ybel-hac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ping.h"

int numValidator(char *num)
{
	int number = 0;
	int sign = 1;
	int signsCount = 0;

	for (size_t i = 0; i < strlen(num); i++)
	{
		if (num[i] == '+' || num[i] == '-')
			signsCount++;
		if ((!isnumber(num[i]) && num[i] != '+' && num[i] != '-') ||
				signsCount > 1)
		{
			printf("ping: invalid value (`%s' near `%s')\n", num, num + i);
			freeResources();
			exit(1);
		}
		if (num[i] == '-')
			sign = -1;
		else if (num[i] != '+')
			number = number * 10 + num[i] - '0';
	}
	return number * sign;
}

void is_valid_argument(char *arg, char ***arguments)
{
	char **argv = *arguments;

	while (*arg)
	{
		if (*arg == 'v')
			ping_struct->options.verboseIsSpecified = true;
		else if (*arg == '?')
			ping_struct->options.usageIsSpecified = true;
		else if (*arg == 'c')
		{
			if (*(arg + 1))
			{
				printf("ping: invalid value (`%c' near `%s')\n", *arg, arg);
				freeResources();
				exit(1);
			}
			if (*(argv + 1) && argv++)
				ping_struct->options.countIsSpecified = numValidator(*argv);
			else
			{
				ft_error(1, "option requires an argument -- 'c'\n\
Try 'ft_ping -?' for more information.",
								 false);
			}
			// * skip the packets count argument for the next loop
			(*arguments)++;
		}
		else
		{
			printf("ft_ping: invalid option -- '%c'\n", *arg);
			freeResources();
			exit(1);
		}
		arg++;
	}
}

void ipValidator(char *address)
{
	(void)address;
}

// * 10.103.13.4
// * localhost
// * http://localhost
// * https://10.2.0.0

void addressValidator(char *address)
{
	if (isdigit(*address))
		ipValidator(address);
	else if (isalpha(*address))
		;
	else
	{
		printf("ft_ping: %s: Name or service not known\n", address);
		freeResources();
		exit(2);
	}
}

void argumentsChecker(char **args)
{
	while (*args)
	{
		//* check if current argument is a flag
		if (**args == '-')
		{
			if (strlen(*args) == 1) //* mean that the argument is "-"
			{
				printf("ft_ping: -: Name or service not known\n");
				freeResources();
				exit(2);
			}
			++(*args);
			//* check that the arguments are valid before check each one
			for (char *currentArg = *args; *currentArg; currentArg++)
			{
				if (*currentArg != 'v' && *currentArg != '?' && *currentArg != 'c')
				{
					printf("invalid value (`%c' near `%c')\n", *currentArg, *currentArg);
					freeResources();
					exit(1);
				}
			}
			is_valid_argument(*args, &args);
		}
		else
		{
			addressValidator(*args);
			ping_struct->host = *args;
		}
		args++;
	}
}
