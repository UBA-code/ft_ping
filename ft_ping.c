#include "ft_ping.h"

void ft_error(int code, char *msg)
{
	printf("ping failed: %s\n", msg);
	// system("leaks a.out");
	exit(code);
}

int main(int ac, char *av[])
{
	ping_struct = calloc(sizeof(ping), sizeof(ping));

	if (ac <= 1)
		ft_error(1, "usage: ping [ip-address]\n");
	argumentsChecker(av + 1);
	initialize_struct();

	//* handle the ctr + c signal
	signal(SIGINT, finisher);
	pinger();
	return 0;
}
