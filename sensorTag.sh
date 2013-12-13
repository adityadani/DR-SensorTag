#!/bin/bash

usage() {
    echo "./sensorTag.sh <MAC Address> > <Output Filename>"
    exit 1
}

[[ $# -ne 1 ]] && usage

./run_gatttool.sh $1 | gawk '{ print system("date +%s%N | cut -b1-13"), $0; }'
