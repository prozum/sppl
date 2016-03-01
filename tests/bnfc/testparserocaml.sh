#!/bin/bash

# Prep work that creates and prepares the bnfc parser for ocaml

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

echo "" > $DIR/ocamltest.result

cd $DIR
mkdir build
cd build

bnfc -m -ocaml ../../../tools/bnf/bnf.cf
make

# Test Files

for file in $DIR/../source/*.sppl
do
	$DIR/build/Testbnf $file
	echo $?    $file >> $DIR/ocamltest.result
done

echo OCAML RESULT
echo

cat $DIR/ocamltest.result

# Clean-up

cd $DIR
rm -r build
