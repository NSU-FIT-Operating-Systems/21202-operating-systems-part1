#!/bin/bash

# Компилируем сервер и клиент
gcc server.c -o server.o
gcc client.c -o client.o

# Запускаем сервер в фоновом режиме и сохраняем его PID
./server.o &
server_pid=$!

# Запускаем клиенты в многопоточном режиме
for i in {1..5}; do
    (sleep $i
    echo $i | ./client.o) &
done

# Завершение процесса сервера с использованием сохраненного PID
sleep 6
kill $server_pid

# Ожидаем завершения всех клиентов
wait
