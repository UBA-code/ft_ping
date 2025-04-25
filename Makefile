UTILS_FLDR=./utils
PINGER_FLDR=$(UTILS_FLDR)/pinger
SRC=ft_ping.c $(UTILS_FLDR)/initializer.c $(UTILS_FLDR)/checker.c $(UTILS_FLDR)/finisher.c $(UTILS_FLDR)/linked_list/rtt.c $(UTILS_FLDR)/linked_list/hosts.c $(UTILS_FLDR)/linked_list/pendingPackets.c $(UTILS_FLDR)/printUsage.c $(UTILS_FLDR)/freeResources.c $(PINGER_FLDR)/checksum.c $(PINGER_FLDR)/pinger.c $(PINGER_FLDR)/sendPacket.c $(PINGER_FLDR)/progressValidPacket.c $(PINGER_FLDR)/progressInvalidPacket.c 
OBJ=$(SRC:.c=.o)
CC=gcc
CFLAGS=-Wall -Wextra -Werror
RM=rm -f
NAME=ft_ping

GREEN=\033[0;32m
RED=\033[0;31m
YELLOW=\033[0;33m
NC=\033[0m

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) -lm
	@echo "$(GREEN)Compilation successful!$(NC)"
	@echo "$(YELLOW)Executable created: $(NAME)$(NC)"
	@echo "$(YELLOW)To run the program, use: ./$(NAME) <IP_ADDRESS>$(NC)"

clean:
	$(RM) $(OBJ)
	@echo "$(RED)Cleaned up object files!$(NC)"

fclean: clean
	$(RM) $(NAME)
	@echo "$(RED)Cleaned up executable!$(NC)"

re: fclean all
	@echo "$(GREEN)Recompiled everything!$(NC)"
