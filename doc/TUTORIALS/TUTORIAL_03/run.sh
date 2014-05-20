FILE=tutorial_01

fig2pdf --bbox=gs --nogv scene.fig

pdflatex $FILE
#biber $FILE
makeglossaries $FILE
pdflatex $FILE
pdflatex $FILE

rm -f $FILE.aux  $FILE-blx.bib  $FILE.ist  $FILE.log  $FILE.out  $FILE.run.xml  $FILE.toc	$FILE.acn   $FILE.bbl  $FILE.blg  $FILE.glo	$FILE.lof  $FILE.lot
rm -f $FILE.acr  $FILE.alg  $FILE.glg  $FILE.gls $FILE.bcf
rm scene.pdf

