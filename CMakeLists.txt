cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

set(LEMMA_VERSION_MAJOR "0")
set(LEMMA_VERSION_MINOR "2")
set(LEMMA_VERSION_PATCH "1")
set(LEMMA_VERSION "\"${LEMMA_VERSION_MAJOR}.${LEMMA_VERSION_MINOR}.${LEMMA_VERSION_PATCH}\"")
set(LEMMA_VERSION_NOQUOTES "${LEMMA_VERSION_MAJOR}.${LEMMA_VERSION_MINOR}.${LEMMA_VERSION_PATCH}")

# Mac OSX RPATH is weird 
# enable @rpath in the install name for any shared library being built
# note: it is planned that a future version of CMake will enable this by default
if(POLICY CMP0015)
	cmake_policy(SET CMP0015 NEW)
endif()
set(CMAKE_MACOSX_RPATH 1)

project (Lemma CXX)

##############################
# required external programs # for downloading packages
##############################
FIND_PROGRAM(HAVEGIT git
  PATHS /usr/bin/ /bin ENV PATH NO_DEFAULT_PATH
)
if(NOT HAVEGIT)
  message(FATAL_ERROR "Lemma requires that git is installed and in the path of your machine")
endif(NOT HAVEGIT)

option(BUILD_SHARED_LIBS OFF)
if(BUILD_SHARED_LIBS)
	set(LABEL_SUFFIX "shared")
else()
	set(LABEL_SUFFIX "static")
endif()

if(NOT CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE "Release" CACHE STRING
       "Choose the type of build, options are: Debug Release
        RelWithDebInfo MinSizeRel."
       FORCE)
endif(NOT CMAKE_BUILD_TYPE)

###################
# External Projects
###################

##########################################################################################
# Eigen, this header-library is used extensively for linear algebra, matrices, and arrays
# Eigen now has an official mirror on GitHub, so Mercurial is no longer a requirement! 
#ExternalProject_Add(EIGEN
#	GIT_REPOSITORY "https://github.com/eigenteam/eigen-git-mirror.git"
#	UPDATE_COMMAND ""
#	GIT_TAG "3.3.3" #"default"
#   	PREFIX ${CMAKE_CURRENT_BINARY_DIR}/external/eigen
#   	CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}
#)
#include_directories ("${CMAKE_INSTALL_PREFIX}/include/eigen3")


####################
# Build SubModules #
####################
include(ExternalProject)




#############################################################################################
# Yaml-cpp, this library is used extensively for serialisation of classes (class persistence) 
find_package(yaml-cpp)
if (yaml-cpp_FOUND)
    message( STATUS "YAML-CPP FOUND ${yaml-cpp_FOUND}" )
    message( STATUS "YAML-CPP LIBRARIES ${YAML_CPP_LIBRARIES}" )
    message( STATUS "YAML-CPP LIBRARY ${yaml-cpp_LIBRARY}" )
else()
    message( STATUS "YAML-CPP WAS NOT FOUND" )
	ExternalProject_Add(YAML_CPP
		GIT_REPOSITORY  "https://github.com/jbeder/yaml-cpp.git"
		GIT_TAG   "yaml-cpp-0.6.1" # "master" 
		UPDATE_COMMAND ""
		PATCH_COMMAND ""
    	PREFIX ${CMAKE_CURRENT_BINARY_DIR}/external/yaml-cpp
    	CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX} 
                   -DBUILD_SHARED_LIBS=${BUILD_SHARED_LIBS} 
                   -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE} 
    	           -DYAML_CPP_BUILD_TESTS=OFF
	)	
#	find_package(yaml-cpp)
endif()

# CxxTest 
find_package(CxxTest) 
if (CXXTEST_FOUND)
	# Do nothing
    message( STATUS "CXXTEST FOUND" )
else()
	message( STATUS "CxxTest will be built and installed along with Lemma, override if desired in GUI."  )
	ExternalProject_Add(CxxTest
		GIT_REPOSITORY "https://github.com/CxxTest/cxxtest.git"
		GIT_TAG "master"
		UPDATE_COMMAND ""
		PATCH_COMMAND ""
		CONFIGURE_COMMAND ""
		BUILD_COMMAND ""
		INSTALL_COMMAND ""
    	PREFIX ${CMAKE_CURRENT_BINARY_DIR}/external/CxxTest
    	CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX} /
                   -DBUILD_SHARED_LIBS=${BUILD_SHARED_LIBS} /
                   -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
	)
	# The values "CACHE PATH "" FORCE" makes the GUI show the values of the local CxxTest, but this can cause issues 
	# with users wanting to specify their own path, its not recommended  
	set (CXXTEST_INCLUDE_DIR  "${CMAKE_CURRENT_BINARY_DIR}/external/CxxTest/src/CxxTest/") # CACHE  PATH "" FORCE )
	set (CXXTEST_PYTHON_TESTGEN_EXECUTABLE "${CMAKE_CURRENT_BINARY_DIR}/external/CxxTest/src/CxxTest/bin/cxxtestgen") # CACHE PATH "" FORCE )
	find_package(CxxTest REQUIRED)
endif()

# tinyxml2, this library is used for XML IO 
option( LEMMA_TINYXML_SUPPORT  "TinyXML library support for .xml files" OFF)
if ( LEMMA_TINYXML_SUPPORT )
	ExternalProject_Add(TINYXML2
		GIT_REPOSITORY "https://github.com/leethomason/tinyxml2.git"
		GIT_TAG  "master"
		UPDATE_COMMAND ""
		PATCH_COMMAND ""
    	PREFIX ${CMAKE_CURRENT_BINARY_DIR}/external/tinyxml2
    	CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX} -DBUILD_SHARED_LIBS=${BUILD_SHARED_LIBS} /
			-DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
	)
	add_compile_options(-DTINYXMLSUPPORT) 
endif()

option( LEMMA_MATIO_SUPPORT  "MATIO library support for MATLAB .mat files" OFF)
if ( LEMMA_MATIO_SUPPORT )
	add_compile_options(-DHAVE_MATIO) 
	# matio, this library is used for MATLAB file IO
	ExternalProject_ADD(MATIO
		GIT_REPOSITORY  "git://git.code.sf.net/p/matio/matio"
		GIT_TAG  "master"
		UPDATE_COMMAND ""
		PATCH_COMMAND ""
    	PREFIX ${CMAKE_CURRENT_BINARY_DIR}/external/matio
    	#CONFIGURE_COMMAND ${CMAKE_CURRENT_BINARY_DIR}/external/matio/src/MATIO/autogen.sh && configure
    	CONFIGURE_COMMAND ./autogen.sh && ./configure --prefix=${CMAKE_INSTALL_PREFIX}
		BUILD_IN_SOURCE 1 
    	BUILD_COMMAND ${MAKE}
	)
endif()

# We don't build VTK, it is too heavy.
option (LEMMA_VTK6_SUPPORT "VTK library for visualisation and grids" OFF)
if ( LEMMA_VTK6_SUPPORT ) 
	#find_package(VTK 6 REQUIRED NO_MODULE)
	find_package(VTK 6.0 COMPONENTS vtkCommonCore vtkRenderingCore vtkFiltersCore vtkFiltersSources 
			vtkCommonDataModel vtkFiltersHyperTree vtkIOXML vtkIOImage vtkIOLegacy vtkInteractionStyle 
			vtkRenderingAnnotation vtkFiltersHybrid vtkFiltersModeling vtkRenderingVolumeOpenGL NO_MODULE)
	set(volumeRenderer volumerenderer.cxx)
	include(${VTK_USE_FILE}) 
	add_compile_options(-DLEMMAUSEVTK) 
	# Compile Matplot_vtk if VTK is present
	#add_subdirectory(Matplot_vtk)	
	#include_directories ("${PROJECT_SOURCE_DIR}/Matplot_vtk")
endif()

option (LEMMA_VTK7_SUPPORT "VTK library for visualisation and grids" OFF)
if ( LEMMA_VTK7_SUPPORT ) 
	#find_package(VTK 7 REQUIRED NO_MODULE)
	find_package(VTK 7.0 COMPONENTS vtkCommonCore vtkRenderingCore vtkFiltersCore vtkFiltersSources 
			vtkCommonDataModel vtkFiltersHyperTree vtkIOXML vtkIOImage vtkIOLegacy vtkInteractionStyle 
			vtkRenderingAnnotation vtkFiltersHybrid vtkFiltersModeling vtkRenderingVolumeOpenGL2 NO_MODULE)
	set(volumeRenderer volumerenderer.cxx)
	if (VTK_FOUND)
		message(STATUS "VTK 7 was found! ") 
	        include(${VTK_USE_FILE}) 
		#add_compile_options(-DLEMMAUSEVTK)
		#target_compile_definitions( lemmacore PRIVATE LEMMAUSEVTK=1 )
		add_definitions(-DLEMMAUSEVTK) 
	else()
		ExternalProject_Add(VTK
  		GIT_REPOSITORY "https://gitlab.kitware.com/vtk/vtk.git"
		GIT_TAG  "v7.1.1"
    	PREFIX ${CMAKE_CURRENT_BINARY_DIR}/external/vtk7
  		CMAKE_ARGS
   	        -DBUILD_SHARED_LIBS:BOOL=ON
	        -DBUILD_EXAMPLES:BOOL=OFF
	        -DBUILD_TESTING:BOOL=OFF
		    -DVTK_Group_Web:BOOL=OFF
	        -DModule_vtkImagingMath:BOOL=ON
	  	    LOG_DOWNLOAD 0
	  	    LOG_UPDATE 0
		)
	endif()
endif()

option (LEMMA_VTK8_SUPPORT "VTK library for visualisation and grids" OFF)
if ( LEMMA_VTK8_SUPPORT ) 
	find_package(VTK 8.0 COMPONENTS vtkCommonCore vtkRenderingCore vtkFiltersCore vtkFiltersSources 
			vtkCommonDataModel vtkFiltersHyperTree vtkIOXML vtkIOImage vtkIOLegacy vtkInteractionStyle 
			vtkRenderingAnnotation vtkFiltersHybrid vtkFiltersModeling vtkRenderingVolumeOpenGL2 NO_MODULE)
	if (VTK_FOUND)
		message(STATUS "VTK 8 was found! ") 
		set(volumeRenderer volumerenderer.cxx)
	    include(${VTK_USE_FILE}) 
	else()
		message( STATUS "External build of VTK 8 has been added, this may take some time to build." )
		ExternalProject_Add(VTK8
  		GIT_REPOSITORY "https://gitlab.kitware.com/vtk/vtk.git"
		GIT_TAG  "v8.1.0"
    	PREFIX ${CMAKE_CURRENT_BINARY_DIR}/external/vtk8
  		CMAKE_ARGS 
    	    -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX} 
            -DBUILD_SHARED_LIBS=${BUILD_SHARED_LIBS} /
            -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER} /
   	        -DBUILD_SHARED_LIBS:BOOL=ON /
	        -DBUILD_EXAMPLES:BOOL=OFF /
	        -DBUILD_TESTING:BOOL=OFF /
		    -DVTK_Group_Web:BOOL=OFF /
	        -DModule_vtkImagingMath:BOOL=ON /
	  	    LOG_DOWNLOAD 0 /
	  	    LOG_UPDATE 0
		)
	endif()
	add_definitions(-DLEMMAUSEVTK) 
endif()

#######################
# Lemma Configuration #
#######################

#############################
# Look for Ki Ha Lee (EM1D) #
#############################
FIND_LIBRARY( KIHA_EM1D kihaem1d ) 
if ( KIHA_EM1D )
	add_definitions(-DKIHALEE_EM1D) 
endif()

####################
# Look for Boost
####################
option (LEMMA_USE_BOOST "Boost is required by gaussian quadrature classes" OFF)
if (LEMMA_USE_BOOST)
	find_package(Boost
  		1.58.0                         # Minimum or EXACT version e.g. 1.36.0
#  		COMPONENTS math                # Boost libraries by their canonical name
	)  
	if(Boost_FOUND)
  		include_directories(${Boost_INCLUDE_DIRS})
	endif()
	find_path( HAVE_BOOST_SPECIAL_FUNCTIONS "boost/math/special_functions.hpp" ) 
	if(HAVE_BOOST_SPECIAL_FUNCTIONS)
		add_compile_options(-DHAVE_BOOST_SPECIAL_FUNCTIONS)
	endif()
endif()

####################
# Magic rpath flag
####################
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

option( LEMMA_BUILD_EXAMPLES "Compile example Lemma applications" OFF )

option( LEMMA_USE_OPENMP  "Use OpenMP in Lemma" OFF)
if (LEMMA_USE_OPENMP)
    find_package(OpenMP)
    if (OPENMP_FOUND)
        set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
        set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
    else()
	message (FATAL ERROR "OpenMP cannot be found, but was specified!")
    endif()
endif()

configure_file (
	"${PROJECT_SOURCE_DIR}/config/LemmaConfig.h.in"
	"${PROJECT_BINARY_DIR}/include/LemmaConfig.h"
)
install ( FILES  ${PROJECT_BINARY_DIR}/include/LemmaConfig.h   DESTINATION ${CMAKE_INSTALL_PREFIX}/include/Lemma/ )

include_directories("${PROJECT_BINARY_DIR}/include")
include_directories ("${CMAKE_INSTALL_PREFIX}/include/")
link_directories ("${CMAKE_INSTALL_PREFIX}/lib/")

#######################################################################
# CXXTEST Framework
if(CXXTEST_FOUND)
	option (LEMMA_ENABLE_TESTING  "Turn on unit testing" ON)
	if (LEMMA_ENABLE_TESTING)
    	#include_directories(${CXXTEST_INCLUDE_DIR}) # Add this in testing CMakeLists.txt instead, cleaner
    	enable_testing()
	endif()
endif()

########################################################################
# Build Modules
add_subdirectory (Modules) 

########################################################################
# add a target to generate API documentation with Doxyfile.in 
# ALL make documentation build by default if possible
find_package(Doxygen)
	if(DOXYGEN_FOUND)
	option( LEMMA_BUILD_DOCUMENTATION  "Build Doxygen man pages" OFF )
	if (LEMMA_BUILD_DOCUMENTATION)

# Custom header and footer option, enable in Doxygen 
#	file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Documentation/header.html
#        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Documentation/header.html
#	)
#	file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Documentation/footer.html
#        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Documentation/footer.html
#	)

	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Documentation/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Documentation/Doxyfile @ONLY)
		add_custom_target(doc
			ALL
			${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Documentation/Doxyfile
			WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
			COMMENT "Generating API documentation with Doxygen" VERBATIM
		)

	endif (LEMMA_BUILD_DOCUMENTATION)
	endif(DOXYGEN_FOUND)

