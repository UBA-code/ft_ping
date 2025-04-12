/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybel-hac <ybel-hac@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 00:31:26 by ybel-hac          #+#    #+#             */
/*   Updated: 2025/04/13 00:31:27 by ybel-hac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_ping.h"

void is_valid_argument(char *arg)
{
	while (*arg)
	{
		if (*arg != 'v' && *arg != '?')
		{
			printf("ft_ping: invalid option -- '%c'\n", *arg);
			exit(1);
		}
		if (*arg == 'v')
		{
			ping_struct->options.verboseIsSpecified = true;
		}
		if (*arg == '?')
		{
			ping_struct->options.usageIsSpecified = true;
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
				exit(2);
			}
			is_valid_argument(&(*args)[1]);
		}
		else
		{
			// addressValidator(*args);
			ping_struct->host = *args;
		}
		args++;
	}
}
