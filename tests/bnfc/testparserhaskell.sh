#!/bin/bash

# Prep work that creates and prepares the bnfc parser for cpp

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

echo "" > $DIR/haskelltest.result

cd $DIR
mkdir build
cd build

bnfc -m -haskell ../../../tools/bnf/bnf.cf
make

# Test Files

for file in $DIR/../source/*.sppl
do
	$DIR/build/TestBnf $file
	echo $?    $file >> $DIR/haskelltest.result
done

echo HASKELL RESULT
echo
 
cat $DIR/haskelltest.result

# Clean-up

cd $DIR
rm -r build
