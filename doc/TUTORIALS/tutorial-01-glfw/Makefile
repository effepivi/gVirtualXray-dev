DOC=tutorial-01-glfw

all:$(DOC).pdf $(DOC).html

%.pdf : %.tex
	pdflatex $<
	#bibtex $(<:%.tex=%)
	#makeglossaries $(<:%.tex=%)
	pdflatex $<
	pdflatex $<
	$(RM) $(DOC).aux
	$(RM) $(DOC).blg
	$(RM) $(DOC).log
	$(RM) $(DOC).out
	$(RM) $(DOC).bbl
	$(RM) $(DOC).lof
	$(RM) $(DOC).lol
	$(RM) $(DOC).toc

%.html : %.tex
	hevea -fix $<
	$(RM) $(DOC).haux
	$(RM) $(DOC).htoc
	$(RM) $(DOC).image.out
	$(RM) $(DOC).image.tex

clean:
	$(RM) $(DOC).pdf
	$(RM) $(DOC).html
	$(RM) $(DOC).aux
	$(RM) $(DOC).blg
	$(RM) $(DOC).log
	$(RM) $(DOC).out
	$(RM) $(DOC).bbl
	$(RM) $(DOC).lof
	$(RM) $(DOC).lol
	$(RM) $(DOC).toc
	$(RM) $(DOC).haux
	$(RM) $(DOC).htoc
	$(RM) $(DOC).image.out
	$(RM) $(DOC).image.tex
