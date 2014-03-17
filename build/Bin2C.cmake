################################################################################
#
#	@file		Bin2C.cmake
#
#	@brief		Configuration file to generate a C header file from any file.
#			 using the CMake tool. (see http://www.cmake.org/)
#
#	@version	1.0
#
#	@date		02/03/2014
#
#	@author		Dr Franck P. Vidal
#
#	@section	License
#				BSD 3-Clause License.
#
#				For details on use and redistribution please refer
#				to http://opensource.org/licenses/BSD-3-Clause
#
#	@section	Copyright
#				(c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
#				http://www.fpvidal.net/, Dec 2013, 2013, version 1.0,
#				BSD 3-Clause License
#
################################################################################


# Minimum CMake version ########################################################
IF(WIN32)
	cmake_minimum_required(VERSION 2.6.0)
ENDIF(WIN32)


# portably convert binary file to header
FUNCTION(FILE2C INFILE VARNAME OUTFILE)
	FILE(READ ${INFILE} HEXFILE HEX)
	STRING(LENGTH ${HEXFILE} XRSLEN)
	SET(HEXPOS 0)
	FILE(WRITE ${OUTFILE} "/* generated from ${INFILE}.tmp; do not edit */\n" "const unsigned char ${VARNAME}[] = {")
	
	WHILE(${HEXPOS} LESS ${XRSLEN})
		MATH(EXPR LPOS "${HEXPOS} % 32")
	
		IF(NOT ${LPOS})
			FILE(APPEND ${OUTFILE} "\n")
		ENDIF(NOT ${LPOS})
	
		STRING(SUBSTRING ${HEXFILE} ${HEXPOS} 2 HEXBYTE)
		FILE(APPEND ${OUTFILE} "0x${HEXBYTE}")
		MATH(EXPR HEXPOS "${HEXPOS} + 2")
	
		IF(${HEXPOS} LESS ${XRSLEN})
			FILE(APPEND ${OUTFILE} ",")
		ENDIF(${HEXPOS} LESS ${XRSLEN})
	ENDWHILE(${HEXPOS} LESS ${XRSLEN})
	FILE(APPEND ${OUTFILE} "};\n")
ENDFUNCTION(FILE2C)
