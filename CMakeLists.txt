cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

####################################################################################################
# Lemma versioning, set Major, minor, and patch here                                               #
set(LEMMA_VERSION_MAJOR "0")                                                                       #
set(LEMMA_VERSION_MINOR "2")                                                                       #
set(LEMMA_VERSION_PATCH "1")                                                                       #
set(LEMMA_VERSION "\"${LEMMA_VERSION_MAJOR}.${LEMMA_VERSION_MINOR}.${LEMMA_VERSION_PATCH}\"")      #
set(LEMMA_VERSION_NOQUOTES "${LEMMA_VERSION_MAJOR}.${LEMMA_VERSION_MINOR}.${LEMMA_VERSION_PATCH}") #
####################################################################################################

########################################
#  ##################################  #
#  # Pre-Build checks/ dependencies #  #
#  ##################################  #
########################################

## Options--what do you want to do

option ( BUILD_SHARED_LIBS          "Shared or static libraries"  OFF ) 
option ( LEMMA_ENABLE_TESTING       "Turn on unit testing" OFF )
option ( LEMMA_BUILD_EXAMPLES       "Compile example Lemma applications" OFF )
option ( LEMMA_USE_OPENMP           "Use OpenMP in Lemma" OFF )
option ( LEMMA_BUILD_DOCUMENTATION  "Build Doxygen man pages" OFF )

## Hard Dependencies
find_package (Eigen3 3.3  NO_MODULE QUIET)   # Matrix/Vector & Math
find_package (yaml-cpp 0.6 QUIET)            # Serialisation of classes 

INCLUDE_DIRECTORIES(${YAML_CPP_INCLUDE_DIR})

if (LEMMA_BUILD_DOCUMENTATION)
    find_package(Doxygen REQUIRED)
endif()

## Optional Dependencies
if (LEMMA_ENABLE_TESTING)
    message (STATUS "LEMMA_ENABLE_TESTING is set" )
    #find_package (CxxTest QUIET)            # Unit testing
    if (EXISTS  "${CMAKE_CURRENT_BINARY_DIR}/external/CxxTest/src/CxxTest/cxxtest")
	    set (CXXTEST_INCLUDE_DIR  "${CMAKE_CURRENT_BINARY_DIR}/external/CxxTest/src/CxxTest/" CACHE PATH "" FORCE)
    else()
	    set (CXXTEST_INCLUDE_DIR  "CXXTEST_INCLUDE_DIR-NOTFOUND" CACHE PATH "" FORCE)
    endif()
    if (EXISTS "${CMAKE_CURRENT_BINARY_DIR}/external/CxxTest/src/CxxTest/bin/cxxtestgen")
	    set (CXXTEST_PYTHON_TESTGEN_EXECUTABLE "${CMAKE_CURRENT_BINARY_DIR}/external/CxxTest/src/CxxTest/bin/cxxtestgen" CACHE PATH "" FORCE) 
    else()
	    set (CXXTEST_PYTHON_TESTGEN_EXECUTABLE "CXXTEST_PYTHON_TESTGEN_EXECUTABLE-NOTFOUND" CACHE PATH "" FORCE) 
    endif ()
    find_package (CxxTest)            # Unit testing 
    message (STATUS "CXXTEST_FOUND ${CxxTest_FOUND}" )
endif()

#######################################################################
# CXXTEST Framework
if(CXXTEST_FOUND)
	if (LEMMA_ENABLE_TESTING)
    	#include_directories(${CXXTEST_INCLUDE_DIR}) # Add this in testing CMakeLists.txt instead, cleaner
    	enable_testing()
	endif()
endif()

## Optional Dependencies
if ( NOT Eigen3_FOUND OR 
     NOT yaml-cpp_FOUND OR 
     (LEMMA_ENABLE_TESTING AND NOT CxxTest_FOUND) )
    message ( STATUS "Missing hard dependencies have been found, these will be downloaded any compiled." )
    message ( STATUS "This necessitates a two step build." )
    message ( STATUS "Build these packages first...THEN RERUN CMAKE!" )
    project (SUPERBUILD NONE)
  	# execute the superbuild (this script will be invoked again without the
  	# USE_SUPERBUILD option this time)
  	include (CMake/SuperBuild.cmake)

    #add_dependencies(SUPERBUILD CxxTest) 

  	return()                      # stop processing this file further
else()
	#project (Blah)                # <-- YOUR PROJECT NAME HERE
	message( STATUS "CXX test Dir${CXXTEST_DIR}" )
	message( STATUS "Everything was found, happy Lemma-ing" )
endif()

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



########################################################################
# Build Modules
add_subdirectory (Modules) 

########################################################################
# add a target to generate API documentation with Doxyfile.in 
# ALL make documentation build by default if possible
find_package(Doxygen)
	if(DOXYGEN_FOUND)
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

# vim: set tabstop=4 shiftwidth=4 expandtab: 
