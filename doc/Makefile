DOCS=README.html \
	README.pdf \
	1-overview/1-gVirtualRay-overview-IBSim-4i_2020.html \
	1-overview/1-gVirtualRay-overview-IBSim-4i_2020.pdf \
	2-theory/2-gVirtualRay-theory-IBSim-4i_2020.html \
	2-theory/2-gVirtualRay-theory-IBSim-4i_2020.pdf \
	4-validation/4-gVirtualRay-validation-IBSim-4i_2020.html \
	5-install/5-gVirtualRay-install-IBSim-4i_2020.html \
	5-install/5-gVirtualRay-install-IBSim-4i_2020.pdf

FILES = $(wildcard */.md)

all: ${DOCS}

%.pptx: %.md
	pandoc -s $< -o $@ #--reference-doc theme.pptx

%.html: %.md
	pandoc -t  slidy -s $< -o $@
	# pandoc -t  slideous -s $< -o $@
	#pandoc -t  dzslides -s $< -o $@

%.pdf: %.md
	# pandoc -t  beamer -s $< -o $@
	pandoc -t context -s $< -o $@

clean:
	${RM} ${DOCS}
