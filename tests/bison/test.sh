#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )" 

# prep

echo "" > $DIR/result

cd $DIR
mkdir build
cd build

flex ../../../samples/flexbison/scanner.l
bison -t -d ../../../samples/flexbison/parser.y

c++ lex.yy.c parser.tab.c -o parser

# test

for file in $DIR/../source/*.sppl
do
	$DIR/build/parser $file
	echo $? $file >> $DIR/result
done

cd $DIR
rm -r build

cat result
