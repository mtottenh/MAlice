#!/bin/bash
echo "/* Auto-Generated File  */" > NodeIncludes.hpp
echo "#ifndef HEADERS" >> NodeIncludes.hpp
echo "#define HEADERS" >> NodeIncludes.hpp
for f in *.hpp
do
	echo " #include \"$f\" " >> NodeIncludes.hpp
done
echo "#endif" >> NodeIncludes.hpp
