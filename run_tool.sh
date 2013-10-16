#!/bin/sh

case "$1" in
    getTemp)
	./getTemp $2
	;;
    getAccel)
	./getAccel $2
	;;
    getMagn)
	./getMagn $2
	;;
    getGyro)
	./getGyro $2
	;;
    getHum)
	./getHum $2
	;;
    getBarm)
	./getBarm $2
	;;
    *)
	exit 1
	;;
esac
