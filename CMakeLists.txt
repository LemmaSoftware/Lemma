cmake_minimum_required (VERSION 2.8.7)

project (Lemma)

###################
# External Projects
###################
include(ExternalProject)

# Eigen, this header-library is used extensively for linear algebra, matrices, and arrays
# Mercurial (hg) repo pull, Would it be better to just download latest stable?
#ExternalProject_Add(EIGEN
#	HG_REPOSITORY  "https://bitbucket.org/eigen/eigen/"
#	UPDATE_COMMAND ""
#	PATCH_COMMAND ""
#    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/external/eigen
#    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}
#)
# Stable Eigen, requires manual updating when new releases, but lighter weight.  
ExternalProject_Add(EIGEN
	URL "http://bitbucket.org/eigen/eigen/get/3.2.7.tar.gz"
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/external/eigen
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}
)

# Yaml-cpp, this library is used extensively for serialisation of classes (class persistence) 
ExternalProject_Add(YAML-CPP
	GIT_REPOSITORY  "https://github.com/jbeder/yaml-cpp.git"
	GIT_TAG  "master"
	UPDATE_COMMAND ""
	PATCH_COMMAND ""
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/external/yaml-cpp
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}
)
add_compile_options(-DHAVE_YAMLCPP) 

# tinyxml2, this library is used for XML IO 
option(TINYXML_SUPPORT  "MATIO library support for MATLAB .mat files" OFF)
if ( TINYXML_SUPPORT )
	ExternalProject_Add(TINYXML2
		GIT_REPOSITORY "https://github.com/leethomason/tinyxml2.git"
		GIT_TAG  "master"
		UPDATE_COMMAND ""
		PATCH_COMMAND ""
    	PREFIX ${CMAKE_CURRENT_BINARY_DIR}/external/tinyxml2
    	CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}
	)
	add_compile_options(-DTINYXMLSUPPORT) 
endif()

option(MATIO_SUPPORT  "MATIO library support for MATLAB .mat files" OFF)
if ( MATIO_SUPPORT )
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

#####################
# Lemma Configuration
#####################
project(LEMMA_CXX)

# We don't Build VTK, it is too much.
option (VTK_SUPPORT "VTK library for visualisation and grids" OFF)
if (VTK_SUPPORT) 
	find_package(VTK 6 REQUIRED NO_MODULE)
	include(${VTK_USE_FILE})
	add_compile_options(-DLEMMAUSEVTK) 
endif()

add_compile_options(-std=c++11) 

set(LEMMA_VERSION_MAJOR "0")
set(LEMMA_VERSION_MINOR "0")
set(LEMMA_VERSION_PATCH "0")
#set(LEMMA_VERSION "${LEMMA_CPP_VERSION_MAJOR}_${LEMMA_CPP_VERSION_MINOR}_${LEMMA_CPP_VERSION_PATCH}")

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

# TODO add compiler specific instructions


include_directories(${CMAKE_INSTALL_PREFIX}/include)
add_executable(Hello "${PROJECT_SOURCE_DIR}/src/test.cpp")
target_link_libraries(Hello lemmacore)
add_dependencies(Hello YAML-CPP)


