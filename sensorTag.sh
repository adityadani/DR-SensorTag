#!/bin/sh
./run_gatttool.sh $1 | gawk '{ print system("date +%s%N | cut -b1-13"), $0; }'
