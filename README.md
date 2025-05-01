# ft_ping

A custom implementation of the `ping` utility in C, developed as part of the 42 school curriculum. This project aims to replicate the core functionality of the standard `ping` command, including sending ICMP echo requests, receiving replies, and calculating network statistics.

## Features

- Sends ICMP echo request packets to a specified host.
- Receives and processes ICMP echo replies.
- Displays round-trip time (RTT) for each packet.
- Calculates and shows statistics: packet loss, min/avg/max RTT.
- Supports common `ping` options:
  - `-c <count>`  : Number of packets to send.
  - `-d`          :	set the SO_DEBUG option
  - `-v`          :	verbose output
  - `-w <seconds>`:	stop after N seconds
  - `-W <seconds>`:	number of seconds to wait for response
  - `-q`          :	quiet output
  - `-?`          : Display help message.
- Handles errors gracefully (e.g., invalid host, permissions).

## Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/UBA-code/ft_ping.git
   cd ft_ping
   ```

2. Build the project:
   ```bash
   make
   ```

3. Run the program (requires root privileges for ICMP):
   ```bash
   sudo ./ft_ping <hostname>
   ```

## Usage

```bash
sudo ./ft_ping [options] <hostname>
```

### Options
- `-c <count>`  : Stop after sending `<count>` packets.
- `-d`          : Set the SO_DEBUG option to the socket.
- `-w <seconds>`: Stop after `<seconds>` seconds.
- `-W <seconds>`: How much time to wait for a reply before marking it as lost.
- `-q`          : hide the ping reply and show only the statistics.
- `-v`          : Enable verbose mode to display detailed packet info.
- `-h`          : Show help and exit.

### Example
```bash
sudo ./ft_ping -c 4 google.com
```
Output:
```
PING google.com (172.217.167.78): 56 data bytes
64 bytes from 172.217.167.78: icmp_seq=0 ttl=117 time=23.456 ms
64 bytes from 172.217.167.78: icmp_seq=1 ttl=117 time=24.123 ms
64 bytes from 172.217.167.78: icmp_seq=2 ttl=117 time=22.987 ms
64 bytes from 172.217.167.78: icmp_seq=3 ttl=117 time=23.789 ms

--- google.com ping statistics ---
4 packets transmitted, 4 packets received, 0% packet loss
round-trip min/avg/max = 22.987/23.589/24.123 ms
```

## Project Structure

- `utils/`: Source code for the ping implementation.
- `includes/`: Header files.
- `tests/`: some testing scripts
- `resources/`: some useful resources for learning and deepening knowledge 
- `Makefile`: Build configuration.


## Dependencies

- Root privileges for raw socket access (ICMP).

## Contributing

Contributions are welcome! Please:
1. Fork the repository.
2. Create a feature branch (`git checkout -b feature-name`).
3. Commit changes (`git commit -m 'Add feature'`).
4. Push to the branch (`git push origin feature-name`).
5. Open a pull request.

## Acknowledgments

Inspired by the standard `ping` utility and the 42 school curriculum. Thanks to the open-source community for references and documentation.
