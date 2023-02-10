#!/bin/bash

echo "================ Создание динамической библиотеки ================"
gcc -c -fPIC hello_dynamic.c -o hello_dynamic.o
gcc -shared -o libhello_dynamic.so hello_dynamic.o

echo "================ Получение исполняемого файла ================"
gcc hello.c -L. -lhello_dynamic -o hello.o

echo "================ Получение списка символов ================"
nm hello.o > symbols.txt
cat symbols.txt | grep hello_from_dynamic_lib
echo -e "Теперь наша функия Unresolved, она подгружается в оперативную память при запуске программы.\n"

echo "================ Получение списка зависимостей ================"
LD_LIBRARY_PATH="./" ldd hello.o > dependencies.txt
cat dependencies.txt

echo "================ Запуск исполняемого файла ================"

LD_LIBRARY_PATH="./" ./hello.o
