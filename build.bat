@REM Uncomment the line below and replace "c:\whatever\else" with the path to cmake.exe
@REM SET PATH=%PATH%;c:\whatever\else

cmake ^
	-DUNIT_TESTS:BOOL=OFF ^
	-DBUILD_CSHARP:BOOL=ON ^
	-DBUILD_JAVA:BOOL=OFF ^
	-DBUILD_OCTAVE:BOOL=OFF ^
	-DBUILD_PERL:BOOL=OFF ^
	-DBUILD_PYTHON3:BOOL=ON ^
	-DBUILD_PYTHON2:BOOL=OFF ^
	-DBUILD_R:BOOL=OFF ^
	-DBUILD_RUBY:BOOL=OFF ^
	-DBUILD_TCL:BOOL=OFF ^
	-DWRAPPER_TEST:BOOL=OFF ^
	-P build.cmake
