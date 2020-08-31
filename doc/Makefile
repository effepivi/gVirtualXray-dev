DOCS=0-gVirtualRay-content-IBSim-4i_2020.html \
	0-gVirtualRay-content-IBSim-4i_2020.pdf \
	1-gVirtualRay-overview-IBSim-4i_2020.html \
	1-gVirtualRay-overview-IBSim-4i_2020.pdf \
	4-gVirtualRay-validation-IBSim-4i_2020.html \
	5-gVirtualRay-install-IBSim-4i_2020.html \
	5-gVirtualRay-install-IBSim-4i_2020.pdf


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
