for f in ../malice_examples/archaeology/*.alice
do
	echo "***** $f *****" 1>&2
	./parser "$f"	
done

for f in ../malice_examples/valid/*.alice
do
	echo "***** $f *****" 1>&2
	./parser "$f"
done

for f in ../malice_examples/invalid/*.alice
do
	echo "***** $f *****" 1>&2
	./parser "$f"
done
