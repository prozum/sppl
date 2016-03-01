#!/bin/bash

# Prep work that creates and prepares the bnfc parser for cpp

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

echo "" > $DIR/cpptest.result

cd $DIR
mkdir build
cd build

bnfc -m -cpp ../../../tools/bnf/bnf.cf
make

# Test Files

for file in $DIR/../source/*.sppl
do
	$DIR/build/Testbnf $file
	echo $?    $file >> $DIR/cpptest.result
done

echo CPP RESULT
echo
 
cat $DIR/cpptest.result

# Clean-up

cd $DIR
rm -r build
