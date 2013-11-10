#!/bin/bash
rm archoutputs

for f in ../malice_examples/src/archaeology/*.alice
do
	echo "---- Running $f ----" >> archoutputs
	./compile $f >/dev/null
	name=`basename "$f" .alice`
	../malice_examples/bin/archaeology/$name >> archoutputs
	echo "" >> archoutputs 
done

echo "##### Finished running tests! #####"
echo "'cat archoutputs' for results."
