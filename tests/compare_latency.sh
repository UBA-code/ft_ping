#!/bin/bash

../ft_ping -c 1000 $1 > ft_ping.log &

ping -c 1000 $1 > ping.log &

wait

echo "#### ft_ping ####"
cat ft_ping.log

echo

echo "#### ping ####"
cat ping.log
