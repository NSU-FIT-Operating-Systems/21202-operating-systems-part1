#!/bin/bash

gcc main.c -o main.o
./main.o &
(sleep 1s
PID=$!
cat /proc/$PID/maps > maps.txt
ps -p $PID > ps.txt)
sleep 30s
cat maps.txt
cat ps.txt
rm *.txt