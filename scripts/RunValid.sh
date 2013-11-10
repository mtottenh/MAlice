#!/bin/bash
rm valid_outputs

for f in ../malice_examples/src/valid/*.alice
do
	echo "---- Compiling $f ----" >> valid_outputs
	./compile  $f  #>/dev/null
	name=`basename "$f" .alice`
#	../malice_examples/bin/valid/$name >> valid_outputs
	echo "" >> valid_outputs
done

for f in ../malice_examples/bin/valid/*.asm
do
    echo "---- Running $f ----"
    name=`basename "$f" .asm`
    ../malice_examples/bin/valid/$name
    echo ""
done


echo "##### Finished running tests! #####"
echo "'cat valid_outputs' for results."
