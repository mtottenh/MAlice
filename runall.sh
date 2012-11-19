for f in ../malice_examples/archaeology/*.alice
do
	echo "***** $f *****"
	./parser < "$f"	
done

for f in ../malice_examples/valid/*.alice
do
	echo "***** $f *****"
	./parser < "$f"
done
