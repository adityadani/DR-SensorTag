#!/bin/bash

usage() {
    echo "./aggregate_data.sh <MAC address> <Hex-Dump Filename> <Audio Filename> <Sampling Rate>"
    exit 1
}

num=1
hexData="hexData_"
audio="audio_"
dat=".dat"

[[ $# -ne 4 ]] && usage

while read -r line
do
    count=$(echo $line | wc -c)
    if [ "$count" -eq 14 ]; then
	prev_line=$line
	#echo $line >> hexData.txt
    fi
    if [[ "${line}" == *Notification* ]]; then
	echo $prev_line >> $hexData$1
	echo $line >> $hexData$1
    elif [[ "${line}" == *descriptor* ]]; then
	echo $prev_line >> $hexData$1
	echo $line >> $hexData$1
    fi
done < $2

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
done < $hexData$1

sox -r $4 $3 $audio$1$dat
