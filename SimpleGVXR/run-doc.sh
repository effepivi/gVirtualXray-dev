#!/bin/sh

doxygen Doxyfile && doxy2swig/doxy2swig.py doc/xml/index.xml SimpleGVXR-doc.i
