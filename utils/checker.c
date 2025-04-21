/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybel-hac <ybel-hac@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 00:31:26 by ybel-hac          #+#    #+#             */
/*   Updated: 2025/04/20 20:47:01 by ybel-hac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ping.h"

int isSupportedArgument(char arg)
{
	return (arg != 'v' && arg != '?' && arg != 'c' && arg != 'q' && arg != 'd' && arg != 'W');
}

long long numValidator(char *num)
{
	long long number = 0;
	int sign = 1;
	int signsCount = 0;

	for (size_t i = 0; i < strlen(num); i++)
	{
		if (num[i] == '+' || num[i] == '-')
			signsCount++;
		if ((!isdigit(num[i]) && num[i] != '+' && num[i] != '-') ||
				signsCount > 1)
		{
			ft_error_printf(1, "ft_ping: invalid value (`%s' near `%s')\n", num, num + i);
		}
		if (num[i] == '-')
			sign = -1;
		else if (num[i] != '+')
			number = number * 10 + num[i] - '0';
	}
	return number * sign;
}

void setSpecifiedOptions(char *arg, char ***arguments)
{
	char **argv = *arguments;

	while (*arg)
	{
		switch (*arg)
		{
		case 'v':
			ping_struct->options.verboseIsSpecified = true;
			break;
		case '?':
			ping_struct->options.usageIsSpecified = true;
			break;
		case 'c':
		case 'W':
			if (*(arg + 1))
			{
				if (!isdigit(*(arg + 1)) && *(arg + 1) != '+' && *(arg + 1) != '-')
					ft_error_printf(1, "ft_ping: invalid value (`%s' near `%s')\n", arg, arg);
				checkAndSetOptionAmount(*arg, arg + 1);
				return;
			}
			else
			{
				if (*(argv + 1) && argv++)
					checkAndSetOptionAmount(*arg, *argv);
				else
				{
					ft_error_printf(1, "ft_ping: option requires an argument -- '%c'\n\
Try 'ft_ping -?' for more information.\n",
													*arg);
				}
				// * skip the packets count argument for the next loop
				(*arguments)++;
			}
			break;
		case 'q':
			ping_struct->options.quitModeIsSpecified = true;
			break;
		case 'd':
			ping_struct->options.debugModeIsSpecified = true;
			break;
		default:
			ft_error_printf(1, "ft_ping: invalid option -- '%c'\n", *arg);
			break;
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
		ft_error_printf(2, "ft_ping: %s: Name or service not known\n", address);
}

void argumentsChecker(char **args)
{
	while (*args)
	{
		//* check if current argument is a flag
		if (**args == '-')
		{
			if (strlen(*args) == 1) //* mean that the argument is "-"
				ft_error_printf(2, "ft_ping: -: Name or service not known\n");
			++(*args);
			//* check that the arguments are valid before check each one
			for (char *currentArg = *args; *currentArg; currentArg++)
			{
				if (isSupportedArgument(**args))
				{
					ft_error_printf(1, "ft_ping: invalid value (`%c' near `%c')\n", *currentArg, *currentArg);
				}
			}
			setSpecifiedOptions(*args, &args);
		}
		else
		{
			addressValidator(*args);
			ping_struct->host = *args;
		}
		args++;
	}
}

void checkAndSetOptionAmount(char arg, char *value)
{
	if (arg == 'c')
		ping_struct->options.countAmount = numValidator(value);
	else
		ping_struct->options.timeOutAmount = numValidator(value);
	if (arg == 'c' && ping_struct->options.countAmount > 0)
		ping_struct->options.countIsSpecified = true;
	else if (arg == 'W' && ping_struct->options.timeOutAmount > 0 &&
					 ping_struct->options.timeOutAmount <= INT_MAX)
		ping_struct->options.timeOutIsSpecified = true;
	if (arg == 'W' && (ping_struct->options.timeOutAmount < 0 || ping_struct->options.timeOutAmount > INT_MAX))
		ft_error_printf(1, "ft_ping: option value too big: %ld\n", ping_struct->options.timeOutAmount);
	else if (arg == 'W' && ping_struct->options.timeOutAmount == 0)
		ft_error_printf(1, "ft_ping: option value too small: %ld\n", ping_struct->options.timeOutAmount);
}
