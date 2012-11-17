for f in malice_examples/archaeology/*.alice
do
	echo "****** $f *****"
	./parser < "$f"
	
done
