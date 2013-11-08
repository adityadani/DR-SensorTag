#!/bin/bash

num=1
while read -r line
do
    count=$(echo $line | wc -c)
    if [ "$count" -eq 14 ]; then
	prev_line=$line
	#echo $line >> hexData.txt
    fi
    if [[ "${line}" == *Notification* ]]; then
	echo $prev_line >> hexData.txt
	echo $line >> hexData.txt
    elif [[ "${line}" == *descriptor* ]]; then
	echo $prev_line >> hexData.txt
	echo $line >> hexData.txt
    fi
done < $1

while read -r line
do
    count=$(echo $line | wc -c)
    if [ "$count" -eq 14 ]; then
	prev_line=$line
	#echo $line >> hexData.txt
    else
	case $line in
	    *0x0025* )
		echo "Temp"
	    	./getTemp $prev_line ${line:38} $1
		;;
	    *0x002d* )
		echo "Accel"
	    	./getAccel $prev_line ${line:38} $1
		;;
	    *0x0038* )
		echo "Hum"
	    	./getHum $prev_line ${line:38} $1
		;;
	    *0x0040* )
		echo "Magn"
	    	./getMagn $prev_line ${line:38} $1
		;;
	    *0x0057* )
		echo "Gyro"
	    	./getGyro $prev_line ${line:38} $1
		;;
	esac
    fi
done < hexData.txt
