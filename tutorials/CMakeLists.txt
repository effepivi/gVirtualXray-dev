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


project(gVirtualXRay-tutorials)

INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/version.cmake)

add_subdirectory(tutorial_00_glfw)
add_subdirectory(tutorial_00_glut)
add_subdirectory(tutorial_00_qt4)
add_subdirectory(tutorial_00_qt5)

add_subdirectory(tutorial_01_glfw)
add_subdirectory(tutorial_01_glut)
add_subdirectory(tutorial_01_qt4)
add_subdirectory(tutorial_01_qt5)

add_subdirectory(tutorial_02_glfw)
add_subdirectory(tutorial_02_glut)
add_subdirectory(tutorial_02_qt4)
#add_subdirectory(tutorial_02_qt5)

add_subdirectory(tutorial_03_glfw)
add_subdirectory(tutorial_03_glut)
add_subdirectory(tutorial_03_qt4)
#add_subdirectory(tutorial_03_qt5)

INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../LICENSE.txt DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/tutorials COMPONENT tutorials)

