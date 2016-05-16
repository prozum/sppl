#!/bin/sh

a=0

until [ $a -ge 10 ]
do
   echo $a
   time ./a.out
   a=`expr $a + 1`
done