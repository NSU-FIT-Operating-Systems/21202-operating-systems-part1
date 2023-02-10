#!/bin/bash

echo "================ Получение файла статической библиотеки ================"
gcc -c hello_static.c -o hello_static.o
ar r libhello_static.a hello_static.o

echo "================ Получение исполняемого файла ================"
gcc hello.c -L. -lhello_static -o hello.o

echo "================ Получение списка символов ================"
nm hello.o > symbols.txt
cat symbols.txt | grep hello_from_static_lib
echo -e "Видим, что функция hello_from_static_lib() не unresolved. Это потому, что код статической библиотеки подгружается в исполняемый файл при компиляции.\nСобственно, символ T говорит о том, что код функции лежит в текстовом сегменте данного файла.\n"

echo "================ Получение списка зависимостей ================"
ldd hello.o > dependencies.txt
cat dependencies.txt

echo "================ Запуск исполняемого файла ================"
./hello.o

echo -e "\n================ Powered by... ================"
cat ./script.sh
