macro(build_GLFW install_prefix staging_prefix)

# make a custom GLFW configuration file

if(NOT TARGET GLFW)
	
	SET (GLFW_VERSION_STRING 3.2)
	SET (GLFW_VERSION_MAJOR  3.2)
	SET (GLFW_VERSION_MINOR  0)

	set(CMAKE_OSX_EXTERNAL_PROJECT_ARGS)
	if(APPLE)
		list(APPEND CMAKE_OSX_EXTERNAL_PROJECT_ARGS
			-DCMAKE_OSX_ARCHITECTURES=${CMAKE_OSX_ARCHITECTURES}
			-DCMAKE_OSX_SYSROOT=${CMAKE_OSX_SYSROOT}
			-DMACOSX_RPATH:BOOL=ON
			-DCMAKE_OSX_DEPLOYMENT_TARGET=${CMAKE_OSX_DEPLOYMENT_TARGET}
		)
	endif()


	ExternalProject_Add(GLFW
		URL  "https://github.com/GLFW/GLFW/releases/download/3.2/GLFW-3.2.zip"
		UPDATE_COMMAND ""
		SOURCE_DIR  ${staging_prefix}/${install_prefix}-source
		BINARY_DIR  ${staging_prefix}/${install_prefix}-build
		INSTALL_DIR ${staging_prefix}/${install_prefix}-install
		LIST_SEPARATOR :::  
		CMAKE_GENERATOR ${CMAKE_GEN}
		CMAKE_ARGS
		-DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
		-DBUILD_SHARED_LIBS:BOOL=OFF
		#-DCMAKE_SKIP_RPATH:BOOL=ON
		-DCMAKE_INSTALL_PREFIX:PATH=${staging_prefix}/${install_prefix}-install
		-DCMAKE_C_FLAGS:STRING=${CMAKE_C_FLAGS}
		-DCMAKE_C_COMPILER:FILEPATH=${CMAKE_C_COMPILER}
		#-DCMAKE_SKIP_RPATH:BOOL=OFF
		#-DCMAKE_SKIP_INSTALL_RPATH:BOOL=OFF
		#-DCMAKE_INSTALL_RPATH:PATH=${install_prefix}/lib${LIB_SUFFIX}
	#INSTALL_COMMAND $(MAKE) install DESTDIR=${staging_prefix}/${install_prefix}-install
	)

	SET(GLFW_INCLUDE_DIR ${staging_prefix}/${install_prefix}-install/include )
	SET(GLFW_LIBRARY     ${staging_prefix}/${install_prefix}-install/lib/glfw3.lib )
	SET(GLFW_DIR         ${staging_prefix}/${install_prefix}-install)
	SET(GLFW_FOUND ON)

	#configure_file(${CMAKE_SOURCE_DIR}/GLFW3-config.cmake.install.in ${staging_prefix}/${install_prefix}/share/cmake/GLFW/GLFWConfig.cmake @ONLY )

endif(NOT TARGET GLFW)

endmacro(build_GLFW)

