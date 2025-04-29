/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybel-hac <ybel-hac@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 00:31:26 by ybel-hac          #+#    #+#             */
/*   Updated: 2025/04/29 10:17:50 by ybel-hac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ping.h"

int isSupportedArgument(char arg)
{
  return (arg != 'v' && arg != '?' && arg != 'c' && arg != 'q' && arg != 'd' && arg != 'W' && arg != 'w');
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
      ft_error_printf(1, "ft_ping: invalid value (`%s' near `%s')\n", num, num);
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
    case 'w':
    case 'c':
    case 'W':
      if (*(arg + 1))
      {
        if (!isdigit(*(arg + 1)) && *(arg + 1) != '+' && *(arg + 1) != '-')
          ft_error_printf(1, "ft_ping: invalid value (`%s' near `%s')\n", arg + 1, arg + 1);
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
      ft_error_printf(64, "ft_ping: invalid option -- '%c'\n\
Try 'ping -?' for more information.\n",
                      *arg);
      break;
    }
    arg++;
  }
}

void argumentsChecker(char **args)
{
  bool hostFound = false;
  while (*args)
  {
    //* check if current argument is a flag
    if (**args == '-')
    {
      ++(*args);
      //* -- is valid
      if (**args == '-')
        ++(*args);

      setSpecifiedOptions(*args, &args);
    }
    else
    {
      addHost(ping_struct->hostsHead, *args);
      hostFound = true;
    }
    args++;
  }
  if (!hostFound)
  {
    ft_error(64, "\
missing host operand\n\
Try 'ft_ping -?' for more information.",
             false);
  }
}

void checkAndSetOptionAmount(char arg, char *value)
{
  if (arg == 'c')
    ping_struct->options.countAmount = numValidator(value);
  else if (arg == 'W')
    ping_struct->options.timeOutAmount = numValidator(value);
  else
    ping_struct->options.stopAfterAmount = numValidator(value);
  if (arg == 'c' && ping_struct->options.countAmount > 0)
    ping_struct->options.countIsSpecified = true;
  else if (arg == 'W' && IS_VALID_RANGE(ping_struct->options.timeOutAmount))
    ping_struct->options.timeOutIsSpecified = true;
  else if (arg == 'w' && IS_VALID_RANGE(ping_struct->options.stopAfterAmount))
    ping_struct->options.stopAfterIsSpecified = true;

  if (arg == 'W' && (!IS_VALID_RANGE(ping_struct->options.timeOutAmount)))
  {
    if (ping_struct->options.timeOutAmount == 0)
      ft_error_printf(1, "ft_ping: option value too small: %s\n", value);
    ft_error_printf(1, "ft_ping: option value too big: %s\n", value);
  }

  if (arg == 'w' && (!IS_VALID_RANGE(ping_struct->options.stopAfterAmount)))
  {
    if (ping_struct->options.stopAfterAmount == 0)
      ft_error_printf(1, "ft_ping: option value too small: %s\n", value);
    ft_error_printf(1, "ft_ping: option value too big: %s\n", value);
  }
}
