#!/bin/bash
echo "#ifndef HEADERS"
echo "#define HEADERS"
for f in *.hpp
do
	echo " #include \"$f\" "
done
echo "#endif"
