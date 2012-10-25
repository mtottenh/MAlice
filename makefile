

.PHONY: clean

doc: malice_spec.tex
	pdflatex malice_spec


clean:
	rm -f *.log
	rm -f *.pdf
	rm -f *.aux
