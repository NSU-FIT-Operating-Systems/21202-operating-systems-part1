#!/bin/bash

# Компилируем сервер и клиент
gcc server.c -o server.o
gcc client.c -o client.o

# Запускаем сервер в фоновом режиме
./server.o &

# Запускаем клиенты в многопоточном режиме
for i in {1..5}; do
    (sleep $i
    echo $i | ./client.o) &
done

# Завершение процесса сервера
sleep 6
pkill server

# Ожидаем завершения всех клиентов
wait
