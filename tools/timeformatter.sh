#!/bin/bash
OUTPUT=$((time $1) 2>&1 >/dev/null)

#OUT1=$(echo $OUTPUT | cut -d 'l' -f 2)
#real=$(echo $OUT1 | cut -d 'u' -f 1)

#OUT2=$(echo $OUT1 | cut -d 'r' -f 2)
#user=$(echo $OUT2 | cut -d 's' -f 1)

real=$(echo $OUTPUT | sed "s/^real //" | sed "s/user.*$//")
user=$(echo $OUTPUT | sed "s/^.*user //" | sed "s/sys.*$//")
sys=$(echo $OUTPUT | sed "s/^.*sys //")

echo $real $user $sys
