#!/bin/bash
echo "/* Auto-Generated File  */" > NodeIncludes.hpp
echo "#ifndef HEADERS" >> NodeIncludes.hpp
echo "#define HEADERS" >> NodeIncludes.hpp
for f in *.hpp
do
	if [ "$f" != "TypeDefs.hpp" ] && [ "$f" != "NodeIncludes.hpp" ]
	then
		echo " #include \"$f\" " >> NodeIncludes.hpp
	fi
done
echo " #include \"../y.tab.h\"" >> NodeIncludes.hpp
echo "#endif" >> NodeIncludes.hpp
