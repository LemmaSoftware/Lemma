cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

# add this options before PROJECT keyword
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

# Bug in CMake on Linux, but Linux, Mac, and Win all support shared libraries, no harm in this, 
# but remove when warning no longer shows.
# Related to calling find_package before a project is created... 
# https://cmake.org/Bug/view.php?id=15999 
SET_PROPERTY(GLOBAL PROPERTY TARGET_SUPPORTS_SHARED_LIBS TRUE)

####################################################################################################
# Lemma versioning, set Major, minor, and patch here                                               #
set(LEMMA_VERSION_MAJOR "0")                                                                       #
set(LEMMA_VERSION_MINOR "3")                                                                       #
set(LEMMA_VERSION_PATCH "3")                                                                       #
set(LEMMA_VERSION "\"${LEMMA_VERSION_MAJOR}.${LEMMA_VERSION_MINOR}.${LEMMA_VERSION_PATCH}\"")      #
set(LEMMA_VERSION_NOQUOTES "${LEMMA_VERSION_MAJOR}.${LEMMA_VERSION_MINOR}.${LEMMA_VERSION_PATCH}") #
####################################################################################################

########################################
#  ##################################  #
#  # Pre-Build checks/ dependencies #  #
#  ##################################  #
########################################

## Options--what do you want to do
option ( BUILD_SHARED_LIBS      "Shared or static libraries"  OFF )

#------------------------------------------------------------------------------
# Set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Debug' as none was specified.")
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS 
    "Debug" 
    "Release"
    "MinSizeRel" 
    "RelWithDebInfo"
  )
endif()

option ( LEMMA_ENABLE_TESTING       "Turn on unit testing" OFF )
option ( LEMMA_BUILD_EXAMPLES       "Compile example Lemma applications" OFF )
option ( LEMMA_USE_OPENMP           "Use OpenMP in Lemma" OFF )
option ( LEMMA_BUILD_DOCUMENTATION  "Build Doxygen man pages" OFF )
option ( LEMMA_VTK8_SUPPORT         "VTK 8.x library for visualisation and grids" OFF )
option ( LEMMA_VTK9_SUPPORT         "VTK 9.x library for visualisation and grids" OFF )
option ( LEMMA_PYTHON3_BINDINGS     "Compile Python 3 bindings" OFF )

# We end up using this for all builds, TODO remove this variable but follow same path
#option (CMAKE_CROSSCOMPILING "Target different arch than you are on" OFF)

SET( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/CMake/ )

SET (CMAKE_CROSSCOMPILING TRUE)

##########################################################################################
## Only look for packages where we build them...this makes finding VTK a problem though! #
########################################################################################## 

if (CMAKE_CROSSCOMPILING)
    message ( STATUS "Performing SuperBuild") 
    project (CHECKSUPERBUILD CXX) # Necessary for VTK query 

#    if ("${CHECKSUPERBUILD_SOURCE_DIR}"  STREQUAL "${CHECKSUPERBUILD_BINARY_DIR}")
#        message(FATAL_ERROR
#            "Lemma (SuperBuild) requires an out of source Build. "
#            "Please create a separate binary directory and run CMake there.")
#    endif()

    find_package (yaml-cpp 0.6    PATHS ${CMAKE_INSTALL_PREFIX}  NO_DEFAULT_PATH) # Serialisation of classes 
    find_package (Eigen3   3.3    PATHS ${CMAKE_INSTALL_PREFIX}  NO_DEFAULT_PATH) # Matrix/Vector & Math

    if (LEMMA_PYTHON3_BINDINGS)
        find_package(pybind11 PATHS ${CMAKE_INSTALL_PREFIX} NO_DEFAULT_PATH)           # Python 3 bindings
    endif()

#${CMAKE_INSTALL_PREFIX}  NO_DEFAULT_PATH 
    if (LEMMA_VTK8_SUPPORT)                                                            # Visualisation 
        find_package (VTK  8.2.0      
        COMPONENTS 
        vtkCommonCore 
        vtkRenderingCore 
        vtkFiltersCore 
        vtkFiltersSources 
        vtkCommonDataModel 
        vtkFiltersHyperTree 
        vtkIOXML 
        vtkIOImage 
        vtkIOLegacy 
        vtkIOGeometry 
        vtkInteractionStyle 
        vtkRenderingAnnotation 
        vtkFiltersHybrid 
        vtkFiltersModeling 
        vtkRenderingVolumeOpenGL2
        QUIET
        ) 
    endif()
    if (LEMMA_VTK9_SUPPORT)                                                            # Visualisation 
        find_package (VTK  9.0.1      
        COMPONENTS 
        CommonCore 
        RenderingCore 
        FiltersCore 
        FiltersSources 
        CommonDataModel 
        FiltersHyperTree 
        IOXML 
        IOImage 
        IOLegacy 
        IOGeometry 
        InteractionStyle 
        RenderingAnnotation 
        FiltersHybrid 
        FiltersModeling 
        RenderingVolumeOpenGL2
        QUIET
        ) 
    endif()
else()
    find_package (Eigen3   3.3 QUIET )  # Matrix/Vector & Math
    find_package (yaml-cpp 0.6 QUIET )  # Serialisation of classes 
    if (LEMMA_VTK9_SUPPORT)
        find_package (VTK  9.0.1 COMPONENTS
        vtkCommonCore 
        vtkRenderingCore 
        vtkFiltersCore 
        vtkFiltersSources 
        vtkCommonDataModel 
        vtkFiltersHyperTree 
        vtkIOXML 
        vtkIOImage 
        vtkIOLegacy 
        vtkIOGeometry 
        vtkInteractionStyle 
        vtkRenderingAnnotation 
        vtkFiltersHybrid 
        vtkFiltersModeling 
        vtkRenderingVolumeOpenGL2 
        QUIET )  
    endif()
endif()

INCLUDE_DIRECTORIES(${YAML_CPP_INCLUDE_DIR})

#if (VTK_VERSION VERSION_GREATER "8.2.0")
#    message( STATUS "${VTK_VERSION} is compatible: ${VTK_VERSION_COMPATIBLE} exact? ${VTK_VERSION_EXACT}" )
#    set (VTK_FOUND False) 
#endif()

if (VTK_FOUND)
    set(volumeRenderer volumerenderer.cxx)
	add_definitions(-DLEMMAUSEVTK) 
	if (LEMMA_VTK8_SUPPORT)
    add_definitions(-DLEMMA_VTK8_SUPPORT)
    else (LEMMA_VTK9_SUPPORT) 
    add_definitions(-DLEMMA_VTK9_SUPPORT)
    endif()
endif()

if (LEMMA_BUILD_DOCUMENTATION)
    find_package(Doxygen REQUIRED)
endif()

## Optional Dependencies
if (LEMMA_ENABLE_TESTING)
    if (EXISTS "${CMAKE_CURRENT_BINARY_DIR}/external/CxxTest/src/CxxTest/README")
	    set (CXXTEST_INCLUDE_DIR  "${CMAKE_CURRENT_BINARY_DIR}/external/CxxTest/src/CxxTest/"  CACHE PATH "" FORCE )
    else()
	    set (CXXTEST_INCLUDE_DIR  "CXXTEST_INCLUDE_DIR-NOTFOUND"  CACHE PATH "" FORCE )
    endif()
    if (EXISTS  "${CMAKE_CURRENT_BINARY_DIR}/external/CxxTest/src/CxxTest/bin/cxxtestgen")
	    set (CXXTEST_PYTHON_TESTGEN_EXECUTABLE "${CMAKE_CURRENT_BINARY_DIR}/external/CxxTest/src/CxxTest/bin/cxxtestgen" CACHE PATH "" FORCE )
    else()
	    set (CXXTEST_PYTHON_TESTGEN_EXECUTABLE "CXXTEST_PYTHON_TESTGEN_EXECUTABLE-NOTFOUND" CACHE PATH "" FORCE )
    endif()
    find_package (CxxTest QUIET)   # Unit testing 
endif()

    
message ( STATUS "VTK Found? " ${VTK_FOUND} )
## Optional Dependencies
if ( NOT Eigen3_FOUND OR 
     NOT yaml-cpp_FOUND OR 
     (LEMMA_PYTHON3_BINDINGS AND NOT pybind11_FOUND) OR 
     (LEMMA_VTK9_SUPPORT AND NOT VTK_FOUND) OR
     (LEMMA_ENABLE_TESTING AND NOT CxxTest_FOUND) )
    message ( STATUS "Missing hard dependencies have been found, these will be downloaded any compiled." )
    message ( STATUS "This necessitates a two step build." )
    message ( STATUS "Build these packages first...THEN RERUN CMAKE!" )
    project (SUPERBUILD NONE) 
  	# execute the superbuild (this script will be invoked again without the
  	# USE_SUPERBUILD option this time) 
    include (CMake/SuperBuild.cmake)
  	return()                      # stop processing this file further
else()
	#project (Blah)               # <-- YOUR PROJECT NAME HERE
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

# Disallow in-source build
if ("${Lemma_SOURCE_DIR}"  STREQUAL "${Lemma_BINARY_DIR}")
  message(FATAL_ERROR
    "Lemma requires an out of source Build. "
    "Please create a separate binary directory and run CMake there.")
endif()

if(CXXTEST_FOUND)
	if (LEMMA_ENABLE_TESTING)
        include (CTest)
    	enable_testing()
	endif()
endif()

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
  		1.64.0                         # Minimum or EXACT version e.g. 1.36.0
#  		COMPONENTS math                # Boost libraries by their canonical name
	)  
	if(Boost_FOUND)
  		include_directories(${Boost_INCLUDE_DIRS})
		find_path( HAVE_BOOST_SPECIAL_FUNCTIONS "boost/math/special_functions.hpp" ) 
		if(HAVE_BOOST_SPECIAL_FUNCTIONS)
			add_definitions(-DHAVE_BOOST_SPECIAL_FUNCTIONS)
		endif()
	endif()
endif()

if (LEMMA_PYTHON3_BINDINGS)
    #find_package(PythonLibs 3.0 REQUIRED)
    #find_package(pythoninterp REQUIRED)
    find_package(pybind11 REQUIRED)
    #    find_package(Boost 1.64.0   REQUIRED COMPONENTS system python3 numpy3)
    # Is this OK for portability of non-python libraries
    #include_directories(${PYTHON_INCLUDE_DIRS})
    #include_directories(${Boost_INCLUDE_DIRS})
    install ( FILES python/setup.py DESTINATION ${CMAKE_INSTALL_PREFIX}/pyLemma/ ) 
    install ( DIRECTORY python/pyLemma DESTINATION ${CMAKE_INSTALL_PREFIX}/pyLemma/ ) 
endif()

####################
# MSVC error fix 
####################
option( MSVC_EXTENDED_ALIGNMENT "Turn on MSVC compiler definition _ENABLE_EXTENDED_ALIGNED_STORAGE " OFF  )
if (MSVC_EXTENDED_ALIGNMENT)
    add_definitions(-D_ENABLE_EXTENDED_ALIGNED_STORAGE)
    SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} /LTCG")
    #add_definitions(-DCMAKE_WINDOWS_EXPORT_ALL_SYMBOLS=TRUE)
    #set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
    #include(GenerateExportHeader)
endif()

####################
# Magic rpath flag
####################
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

if (LEMMA_USE_OPENMP)

    if(APPLE)
        #if(CMAKE_C_COMPILER_ID MATCHES "Clang")
        #    set(OpenMP_C "${CMAKE_C_COMPILER}")
        #    set(OpenMP_C_FLAGS "-fopenmp=libomp -Wno-unused-command-line-argument")
        #    set(OpenMP_C_LIB_NAMES "libomp" "libgomp" "libiomp5")
        #    set(OpenMP_libomp_LIBRARY ${OpenMP_C_LIB_NAMES})
        #    set(OpenMP_libgomp_LIBRARY ${OpenMP_C_LIB_NAMES})
        #    set(OpenMP_libiomp5_LIBRARY ${OpenMP_C_LIB_NAMES})
        #endif()
        if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
            find_library( libiomp5 NAMES  libiomp5.dylib PATHS "/anaconda3/lib/" REQUIRED )
            set(OpenMP_CXX "${CMAKE_CXX_COMPILER}")
            set(OpenMP_CXX_FLAGS "-fopenmp=libiomp5 -Wno-unused-command-line-argument")
            set(OpenMP_CXX_LIB_NAMES "libiomp5") #"libomp" "libgomp" "libiomp5")
            #set(OpenMP_libomp_LIBRARY ${OpenMP_CXX_LIB_NAMES})
            #set(OpenMP_libgomp_LIBRARY ${OpenMP_CXX_LIB_NAMES})
            set(OpenMP_libiomp5_LIBRARY libiomp5 )# ${OpenMP_CXX_LIB_NAMES})
            set(OpenMP_LIBRARIES  libiomp5)
            link_directories("/anaconda3/lib")
    #        find_package(OpenMP REQUIRED)
    #         set(OpenMP_CXX_FLAGS "-Xpreprocessor -fopenmp -I/usr/local/opt/libomp/include")
    #         " -DOpenMP_CXX_LIB_NAMES=\"omp\" -DOpenMP_omp_LIBRARY=/usr/local/opt/libomp/lib/libomp.dylib ")
    #         set(OpenMP_CXX_LIB_NAMES "omp") # "libgomp" "libiomp5")
    #         set(OpenMP_libomp_LIBRARY "/usr/local/opt/libomp/lib/libomp.dylib") #${OpenMP_CXX_LIB_NAMES})
    #         set(OpenMP_libgomp_LIBRARY ${OpenMP_CXX_LIB_NAMES})
    #         set(OpenMP_libiomp5_LIBRARY ${OpenMP_CXX_LIB_NAMES})
        endif()
    endif()

    if (CMAKE_CROSSCOMPILING)
        set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp")
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
        set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fopenmp")
    else()
        find_package(OpenMP REQUIRED)
        if (OPENMP_FOUND)
            # include_directories("${OPENMP_INCLUDES}")
            link_directories("${OPENMP_LIBRARIES}")
            set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
            set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
            set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
        endif()
    endif()
    add_definitions(-DLEMMAUSEOMP) # -DEIGEN_DONT_VECTORIZE)
endif()

configure_file (
	"${PROJECT_SOURCE_DIR}/config/LemmaConfig.h.in"
	"${PROJECT_BINARY_DIR}/include/LemmaConfig.h"
)
install ( FILES  ${PROJECT_BINARY_DIR}/include/LemmaConfig.h   DESTINATION ${CMAKE_INSTALL_PREFIX}/include/Lemma/ )

include_directories("${PROJECT_BINARY_DIR}/include")
include_directories ("${CMAKE_INSTALL_PREFIX}/include/")
link_directories ("${CMAKE_INSTALL_PREFIX}/lib/")

##############
# gprof hack #
##############
#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")

########################################################################
# Build Modules
add_subdirectory (Modules) 

#######################################################################
# CXXTEST Framework
if(CXXTEST_FOUND)
	if (LEMMA_ENABLE_TESTING)
        include (CTest)
    	enable_testing()
	endif()
endif()

########################################################################
# add a target to generate API documentation with Doxyfile.in 
# ALL make documentation build by default if possible
if (LEMMA_BUILD_DOCUMENTATION)
    find_package(Doxygen)
	if(DOXYGEN_FOUND)
    message( STATUS "LEMMA_BUILD_DOCUMENTATION must be positive" )
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
	endif(DOXYGEN_FOUND)
endif (LEMMA_BUILD_DOCUMENTATION)

# vim: set tabstop=4 shiftwidth=4 expandtab: 
