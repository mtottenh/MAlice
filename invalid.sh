#!/bin/bash


echo "##### TESTING TEST $1 #####"
echo "##### Reference Compiler #####"
MAlice --exe "../malice_examples/invalid/test$1.alice"
echo
echo
echo '##### Derpy Compiler #####'
./parser "../malice_examples/invalid/test$1.alice"
