#!/bin/bash

echo "================ Получение исполняемого файла ================"
gcc hello.c -o hello.o

echo "================ Получение списка символов ================"
nm hello.o > symbols.txt
cat symbols.txt | grep -w U

echo "================ Получение списка зависимостей ================"
ldd hello.o > dependencies.txt
cat dependencies.txt

echo "================ Запуск исполняемого файла ================"
./hello.o

echo -e "\n================ Powered by... ================"
cat ./script.sh
