#!/bin/sh

case "$1" in
    getTemp)
	./getTemp $2 $3
	;;
    getAccel)
	./getAccel $2 $3
	;;
    getMagn)
	./getMagn $2 $3
	;;
    getGyro)
	./getGyro $2 $3
	;;
    getHum)
	./getHum $2 $3
	;;
    getBarm)
	./getBarm $2 $3
	;;
    *)
	exit 1
	;;
esac
