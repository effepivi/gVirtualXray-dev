%module swig_example

// Add necessary symbols to generated header
%{
#include "gvxrWrapper.h"
%}

// Process symbols in header
%include "gvxrWrapper.h"
