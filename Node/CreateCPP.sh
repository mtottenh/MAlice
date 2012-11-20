#!/bin/bash

# Looks in directory for .hpp files that don't have associated .cpp files
# Generates the .cpp file, including #ifndef, #define, #endif and an #include
# of the .hpp file.

for f in *.hpp
do
	y=${f^^}
	if [ ! -e ${f%.*}.cpp ]
	then
		echo ${f%.*}.cpp >> TODO.txt
		touch ${f%.*}.cpp
		echo -ne "#ifndef ___" >> 	${f%.*}.cpp
		echo -ne "${y%.*}" >> ${f%.*}.cpp
		echo  "__" >>  ${f%.*}.cpp
		
		echo -ne "#define ___" >> 	${f%.*}.cpp
		echo -ne "${y%.*}" >> ${f%.*}.cpp
		echo  "__" >>  ${f%.*}.cpp
		
		echo  "#include \"$f\"" >> ${f%.*}.cpp
		echo  "#endif" >> 	${f%.*}.cpp

	fi
done
