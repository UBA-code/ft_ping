#!/bin/bash

# Script to run standard ping and custom ping for 5 packets targeting google.com

# Target address and packet count
TARGET="google.com"
COUNT=5

# Output files for each program
STANDARD_OUT="standard_ping.txt"
MY_OUT="my_ping.txt"

./ping -c "$COUNT" "$TARGET" > "$STANDARD_OUT"

echo "First ping done!"

./ft_ping -c "$COUNT" "$TARGET" > "$MY_OUT"

# Notify completion
echo "Both ping commands completed."
echo "Standard ping output saved to: $STANDARD_OUT"
echo "Custom ping output saved to: $MY_OUT"

python3 compare.py
