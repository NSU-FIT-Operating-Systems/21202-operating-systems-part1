#!/bin/bash

rm 4_1
gcc 4_1.c -o 4_1
export ENV_VAR="Initial"
./4_1
echo "=======  PROGRAM  ================"
echo "Программа завершилась"
echo 'Должны увидеть Initial:'
echo $ENV_VAR
