project (SuperBuild CXX C)
include(ExternalProject)

if (Eigen3_FOUND)
    message( STATUS "Eigen was found ${eigen_DIR}" )
else()
    message( STATUS "Eigen WAS NOT FOUND ${eigen_DIR}" )
        ExternalProject_Add(EIGEN
	    GIT_REPOSITORY "https://gitlab.com/libeigen/eigen.git"
        UPDATE_COMMAND "" 
	    GIT_TAG "3.3.7" 
   	    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/external/eigen
   	    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}
		#CONFIGURE_COMMAND ""
		#BUILD_COMMAND     ""
		#INSTALL_COMMAND     ""
    )
endif()

if (yaml-cpp_FOUND)
    message( STATUS "YAML-CPP was found ${yaml-cpp_FOUND}" )
else()
    message( STATUS "YAML-CPP WAS NOT FOUND, BUILDING" )

    ExternalProject_Add(YAML_CPP
        GIT_REPOSITORY  "https://github.com/jbeder/yaml-cpp.git" 
        GIT_TAG  "yaml-cpp-0.6.3"  # "master" 
        UPDATE_COMMAND ""
        PATCH_COMMAND ""
        PREFIX ${CMAKE_CURRENT_BINARY_DIR}/external/yaml-cpp
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX} 
                   -DYAML_BUILD_SHARED_LIBS=${BUILD_SHARED_LIBS} 
                   -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} 
    	           -DYAML_CPP_BUILD_TESTS=OFF
                   -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
                   -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER} 
                   -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER} 
                   -DCMAKE_CXX_STANDARD=14
                   -DCMAKE_CXX_STANDARD_REQUIRED=ON
                   -DCMAKE_CXX_EXTENSIONS=OFF
    )
endif()

if ( LEMMA_VTK9_SUPPORT )
    if ( NOT VTK_FOUND OR  VTK_VERSION VERSION_LESS "9.0.0" )
        message( STATUS "VTK > 8.20.0 was found! Version found: " ${VTK_VERSION}, ${VTK_USE_FILE} )
        message( STATUS "External build of VTK 8 has been added, this may take some time to build." )
        ExternalProject_Add(VTK9
        GIT_REPOSITORY "https://gitlab.kitware.com/vtk/vtk.git"
        GIT_TAG  "v9.0.1"
        PREFIX ${CMAKE_CURRENT_BINARY_DIR}/external/vtk8
        CMAKE_ARGS
            -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}
            -DBUILD_SHARED_LIBS=${BUILD_SHARED_LIBS} /
            -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER} /
            -DBUILD_EXAMPLES:BOOL=OFF /
            -DBUILD_TESTING:BOOL=OFF /
            -DVTK_Group_Web:BOOL=OFF /
            -DModule_vtkImagingMath:BOOL=ON /
            LOG_DOWNLOAD 0 /
            LOG_UPDATE 0
        )
    endif()
    if (VTK_FOUND AND VTK_VERSION VERSION_LESS_EQUAL "8.20.0")
        #message(STATUS "VTK 8 was found! " ${VTK_VERSION}, ${VTK_USE_FILE} ) 
        message(STATUS "VTK 8.2 was found! " ${VTK_VERSION}, ${VTK_MINOR_VERSION}, ${VTK_USE_FILE} )
        set(volumeRenderer volumerenderer.cxx)
        #include(${VTK_USE_FILE}) 
        add_definitions(-DLEMMAUSEVTK)
    else()
    endif()

endif()

if (LEMMA_PYTHON3_BINDINGS)
    if (pybind11_FOUND)
        message( STATUS "pybind11 was found" )
    else()
        message( STATUS "pybind11 was NOT found, please build or remove LEMMA_PYTHON3_BINDINGS" )
        #find_package(Python COMPONENTS Interpreter Development REQUIRED)
	#find_package(PythonLibs 3.0 REQUIRED)
        ExternalProject_Add(pybind11
		    GIT_REPOSITORY "https://github.com/pybind/pybind11.git"
		    GIT_TAG "v2.5.0" # "master" #"v2.4.3" #"master"
		    UPDATE_COMMAND ""
		    PATCH_COMMAND ""
    	    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/external/pybind11
            CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX} 
                       -DBUILD_SHARED_LIBS=${BUILD_SHARED_LIBS}
                       -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} 
                       -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
                       -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER} 
                       -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER} 
		    #CONFIGURE_COMMAND ""
		    #BUILD_COMMAND     ""
		    #INSTALL_COMMAND   ""
	    )
    endif()
endif()

if (LEMMA_ENABLE_TESTING)
    if (CXXTEST_FOUND)
        message( STATUS "CxxTest was found" )
    else()
	    message( STATUS "CxxTest WAS NOT FOUND")
        ## CxxTest has no installation per se, it uses python  
	    ExternalProject_Add(CxxTest
		    GIT_REPOSITORY "https://github.com/CxxTest/cxxtest.git"
		    GIT_TAG "master"
		    UPDATE_COMMAND ""
		    PATCH_COMMAND ""
    	    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/external/CxxTest
    	    #CMAKE_ARGS -DCXXTEST_INCLUDE_DIR="${CMAKE_CURRENT_BINARY_DIR}/external/CxxTest/src/CxxTest/" 
            #           -DCXXTEST_PYTHON_TESTGEN_EXECUTABLE "${CMAKE_CURRENT_BINARY_DIR}/external/CxxTest/src/CxxTest/bin/cxxtestgen"
            #-DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX} /
            #           -DBUILD_SHARED_LIBS=${BUILD_SHARED_LIBS} /
            #           -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
		    CONFIGURE_COMMAND ""
		    BUILD_COMMAND     ""
		    INSTALL_COMMAND   ""
	    #set (CXXTEST_INCLUDE_DIR  "${CMAKE_CURRENT_BINARY_DIR}/external/CxxTest/src/CxxTest/" CACHE PATH) #  PATH "" FORCE )
	    #set (CXXTEST_PYTHON_TESTGEN_EXECUTABLE "${CMAKE_CURRENT_BINARY_DIR}/external/CxxTest/src/CxxTest/bin/cxxtestgen" CACHE PATH) # PATH "" FORCE )
	    )
        #enable_testing()
	    # The values "CACHE PATH "" FORCE" makes the GUI show the values of the local CxxTest, but this can cause issues 
	    # with users wanting to specify their own path. However, the SuperBuild needs this, in theory you are only here 
        # because a usable CXXTEST was not already found on the machine, so likely this is sane to do 

	    #find_package(CxxTest REQUIRED)
    endif()
endif()

# tinyxml2, this library is used for XML IO 
#option( LEMMA_TINYXML_SUPPORT  "TinyXML library support for .xml files" OFF)
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

#option( LEMMA_MATIO_SUPPORT  "MATIO library support for MATLAB .mat files" OFF)
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

# vim: set tabstop=4 shiftwidth=4 expandtab: 
