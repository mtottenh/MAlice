for f in ../malice_examples/archaeology/*.alice
do
	echo "***** $f *****"
	./parser "$f"	> output
done

for f in ../malice_examples/valid/*.alice
do
	echo "***** $f *****"
	./parser "$f" > output
done

for f in ../malice_examples/invalid/*.alice
do
	echo "***** $f *****"
	./parser "$f" > output
done
