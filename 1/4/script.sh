#!/bin/bash

echo "https://pvoid.pro/index.php/articles/41-dl-libraries"

echo "================ Создание динамической библиотеки ================"
gcc -c -fPIC hello_shared.c -o hello_shared.o
gcc -shared -o libhello_shared.so hello_shared.o

echo "================ Получение исполняемого файла ================"
gcc hello.c -o hello.o

echo "================ Получение списка символов ================"
nm hello.o > symbols.txt
cat symbols.txt # | grep hello_from_dynamic_lib
echo -e "\n"

echo "================ Получение списка зависимостей ================"
LD_LIBRARY_PATH="./" ldd hello.o > dependencies.txt
cat dependencies.txt

echo "================ Запуск исполняемого файла ================"

LD_LIBRARY_PATH="./" ./hello.o
