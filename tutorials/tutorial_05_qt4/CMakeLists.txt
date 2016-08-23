#
#
# Copyright (c) 2014, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
# http://www.fpvidal.net/
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without modification,
# are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice,
# this list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice,
# this list of conditions and the following disclaimer in the documentation and/or
# other materials provided with the distribution.
#
# 3. Neither the name of the Bangor University nor the names of its contributors
# may be used to endorse or promote products derived from this software without
# specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
# THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
# THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


project(gVirtualXRay-tutorial-04-qt4)

# Initialise some states by default ############################################
INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/version.cmake)

# Headers of the project #######################################################
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/data)

# Qt4 #########################################################################
IF (WIN32)
    # 64 bit version of VC++
    if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
        MESSAGE (WARNING "There is no 64 bit version of Qt4 for MSVC++.")
    else( CMAKE_SIZEOF_VOID_P EQUAL 8 )
        FIND_PACKAGE(Qt4 COMPONENTS QtGui QtOpenGL)
    endif( CMAKE_SIZEOF_VOID_P EQUAL 8 )
else (WIN32)
    FIND_PACKAGE(Qt4 COMPONENTS QtGui QtOpenGL)
endif (WIN32)

IF(Qt4_FOUND)
    SET(CMAKE_AUTOMOC ON)
    SET(CMAKE_INCLUDE_CURRENT_DIR ON)
    SET(optionalLibs ${optionalLibs} Qt4::QtGui Qt4::QtOpenGL)
ELSE(Qt4_FOUND)
    MESSAGE(WARNING "Qt4 not found, the demo (${PROJECT_NAME}) will be disable.")
ENDIF(Qt4_FOUND)


# Create the main program ######################################################
IF(Qt4_FOUND)
add_executable(tutorial_05_qt4
	tutorial_05_qt4.cxx  
	GLWidget.h GLWidget.cxx)

target_link_libraries(tutorial_05_qt4 ${gVirtualXRay_LIBRARY} ${optionalLibs} ${requiredLibs})

ADD_DEPENDENCIES(tutorial_05_qt4 BinaryTutorial04Qt4Files)

INSTALL(TARGETS tutorial_05_qt4 RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/demos/bin COMPONENT demos)
INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../data/arial.ttf DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/demos/data COMPONENT demos)
INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../../LICENSE.txt DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/demos COMPONENT demos)

SET (CPACK_RESOURCE_FILE_LICENSE ${CMAKE_CURRENT_SOURCE_DIR}/../../LICENSE.txt)

include(CPack)
ENDIF(Qt4_FOUND)

