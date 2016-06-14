cmake_minimum_required (VERSION 2.8.12)

# Mac OSX RPATH is weird 
# enable @rpath in the install name for any shared library being built
# note: it is planned that a future version of CMake will enable this by default
if(POLICY CMP0015)
	cmake_policy(SET CMP0015 NEW)
endif()
set(CMAKE_MACOSX_RPATH 1)

project (Lemma CXX)

# required external programs (for runtime of nix, not buildtime)
FIND_PROGRAM(HAVEGIT git
  PATHS /usr/bin/ /bin ENV PATH NO_DEFAULT_PATH
)
if(NOT HAVEGIT)
  message(FATAL_ERROR "Lemma requires that git is installed and in the path of your machine")
endif(NOT HAVEGIT)

FIND_PROGRAM(HAVEHG hg
  PATHS /usr/bin/ /bin ENV PATH NO_DEFAULT_PATH
)
if(NOT HAVEHG)
   message(STATUS "Mercurial (hg) was not found.")
endif(NOT HAVEHG)

option(BUILD_SHARED_LIBS OFF)
if(BUILD_SHARED_LIBS)
	set(LABEL_SUFFIX "shared")
else()
	set(LABEL_SUFFIX "static")
endif()

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif()

###################
# External Projects
###################
include(ExternalProject)
# Eigen, this header-library is used extensively for linear algebra, matrices, and arrays
# Mercurial (hg) repo pull, Would it be better to just download latest stable?
if (HAVEHG)
ExternalProject_Add(EIGEN
	HG_REPOSITORY  "https://bitbucket.org/eigen/eigen/"
	UPDATE_COMMAND ""
	HG_TAG "default"
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/external/eigen
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}
)
ELSE() 
# Stable Eigen, requires manual updating when new releases, but lighter weight.  
ExternalProject_Add(EIGEN
    #URL "http://bitbucket.org/eigen/eigen/get/3.2.7.tar.gz"
    URL "http://bitbucket.org/eigen/eigen/get/default.tar.gz" # tip from repo
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/external/eigen
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}
)
ENDIF()

# Yaml-cpp, this library is used extensively for serialisation of classes (class persistence) 
ExternalProject_Add(YAML-CPP
	GIT_REPOSITORY  "https://github.com/jbeder/yaml-cpp.git"
	GIT_TAG  "master"
	UPDATE_COMMAND ""
	PATCH_COMMAND ""
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/external/yaml-cpp
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX} -DBUILD_SHARED_LIBS=ON -DBUILD_SHARED_LIBS=${BUILD_SHARED_LIBS}
)
add_compile_options(-DHAVE_YAMLCPP) 

# tinyxml2, this library is used for XML IO 
option( LEMMA_TINYXML_SUPPORT  "MATIO library support for MATLAB .mat files" OFF)
if ( LEMMA_TINYXML_SUPPORT )
	ExternalProject_Add(TINYXML2
		GIT_REPOSITORY "https://github.com/leethomason/tinyxml2.git"
		GIT_TAG  "master"
		UPDATE_COMMAND ""
		PATCH_COMMAND ""
    	PREFIX ${CMAKE_CURRENT_BINARY_DIR}/external/tinyxml2
    	CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX} -DBUILD_SHARED_LIBS=ON -DBUILD_STATIC_LIBS=ON 
		#-DBUILD_SHARED_LIBS=${BUILD_SHARED_LIBS}
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
	add_subdirectory(Matplot_vtk)	
	include_directories ("${PROJECT_SOURCE_DIR}/Matplot_vtk")
endif()

option (LEMMA_VTK7_SUPPORT "VTK library for visualisation and grids" OFF)
if ( LEMMA_VTK7_SUPPORT ) 
	#find_package(VTK 7 REQUIRED NO_MODULE)
	find_package(VTK 7.0 COMPONENTS vtkCommonCore vtkRenderingCore vtkFiltersCore vtkFiltersSources 
			vtkCommonDataModel vtkFiltersHyperTree vtkIOXML vtkIOImage vtkIOLegacy vtkInteractionStyle 
			vtkRenderingAnnotation vtkFiltersHybrid vtkFiltersModeling vtkRenderingVolumeOpenGL2 NO_MODULE)
	set(volumeRenderer volumerenderer.cxx)
	include(${VTK_USE_FILE}) 
	add_compile_options(-DLEMMAUSEVTK) 
	# Compile Matplot_vtk if VTK is present
	add_subdirectory(Matplot_vtk)	
	include_directories ("${PROJECT_SOURCE_DIR}/Matplot_vtk")
endif()

#####################
# Lemma Configuration
#####################

####################
# Add the c++11 flag 
# TODO add compiler specific instructions
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG(-std=c++14 COMPILER_SUPPORTS_CXX14)
CHECK_CXX_COMPILER_FLAG(-std=c++11 COMPILER_SUPPORTS_CXX11)

if(COMPILER_SUPPORTS_CXX14)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
elseif(COMPILER_SUPPORTS_CXX11)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
else()
	message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 or C++14 support. Please use a different C++ compiler.")
endif()

option( LEMMA_USE_OPENMP  "Use OpenMP in Lemma" OFF)
CHECK_CXX_COMPILER_FLAG(-fopenmp COMPILER_SUPPORTS_OPENMP)
if(COMPILER_SUPPORTS_OPENMP AND LEMMA_USE_OPENMP )
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp -DLEMMAUSEOMP")
endif()

set(LEMMA_VERSION_MAJOR "0")
set(LEMMA_VERSION_MINOR "0")
set(LEMMA_VERSION_PATCH "1")
set(LEMMA_VERSION "${LEMMA_VERSION_MAJOR}.${LEMMA_VERSION_MINOR}.${LEMMA_VERSION_PATCH}")

configure_file (
	"${PROJECT_SOURCE_DIR}/include/LemmaConfig.h.in"
	"${PROJECT_BINARY_DIR}/include/LemmaConfig.h"
)
include_directories("${PROJECT_BINARY_DIR}/include")

include_directories ("${PROJECT_SOURCE_DIR}/LemmaCore/include")
include_directories ("${CMAKE_INSTALL_PREFIX}/include/eigen3")
include_directories ("${CMAKE_INSTALL_PREFIX}/include/")
link_directories ("${CMAKE_INSTALL_PREFIX}/lib/")
add_subdirectory (LemmaCore) 

########################################################################
# Build Modules
add_subdirectory (Modules) 

########################################################################
# 
include_directories(${CMAKE_INSTALL_PREFIX}/include)

#add_executable(Hello "${PROJECT_SOURCE_DIR}/src/test.cpp")
#target_link_libraries(Hello lemmacore)
#add_dependencies(Hello YAML-CPP)


# add a target to generate API documentation with Doxygen
# ALL make documentation build by default if possible
find_package(Doxygen)
	if(DOXYGEN_FOUND)
	option( LEMMA_BUILD_DOCUMENTATION  "Build Doxygen man pages" OFF )
	if (LEMMA_BUILD_DOCUMENTATION)
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Documentation/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Documentation/Doxyfile @ONLY)
		add_custom_target(doc
			ALL
			${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Documentation/Doxyfile
			WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
			COMMENT "Generating API documentation with Doxygen" VERBATIM
		)
	endif (LEMMA_BUILD_DOCUMENTATION)
	endif(DOXYGEN_FOUND)

