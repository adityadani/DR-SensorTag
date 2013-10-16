#!/bin/sh

gcc -o getTemp testTemp.c -lm
gcc -o getHum testHum.c -lm
gcc -o getBarm testBarm.c -lm
gcc -o getAccel testAccel.c -lm
gcc -o getMagn testMagn.c -lm
gcc -o getGyro testGyro.c -lm
