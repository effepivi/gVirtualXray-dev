#/bin/sh

DOC=tutorial-02-glut

pdflatex $DOC
makeglossaries $DOC
bibtex $DOC
pdflatex $DOC
pdflatex $DOC

hevea -fix $DOC

rm -f $DOC.haux
rm -f $DOC.lof
rm -f $DOC.log
rm -f $DOC.tex.backup
rm -f $DOC.htoc
rm -f $DOC.aux  $DOC.lol
rm -f $DOC.bbl  $DOC.image.out  $DOC.out  $DOC.toc
rm -f $DOC.blg  $DOC.image.tex

