add_subdirectory("src")
add_library( fdem1d ${FDEM1DSOURCE} )  
target_include_directories( fdem1d PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )

set_target_properties(fdem1d PROPERTIES 
	VERSION  "${LEMMA_VERSION_NOQUOTES}"
	SOVERSION "${LEMMA_VERSION_MAJOR}.${LEMMA_VERSION_MINOR}"
	PROJECT_LABEL "FDEM1D ${LABEL_SUFFIX}"
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)

if ( LEMMA_VTK9_SUPPORT ) 
	target_link_libraries(fdem1d ${visibility}VTK::RenderingCore VTK::CommonDataModel)
	vtk_module_autoinit(TARGETS fdem1d MODULES VTK::RenderingCore)
endif()

# Linking
target_link_libraries(fdem1d "lemmacore")
target_link_libraries(fdem1d ${YAML_CPP_LIBRARIES}) 

if ( KIHA_EM1D )
	target_link_libraries(fdem1d "kihaem1d")
	if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  		# using GCC
		target_link_libraries(fdem1d "gfortran")
	elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
  		# using Intel C++
		target_link_libraries(fdem1d "ifcore" "imf" "svml" "intlc")
	endif()
endif()

# Testing
if (LEMMA_ENABLE_TESTING)
	add_subdirectory(testing)
endif()

# Python Bindings
if (LEMMA_PYTHON3_BINDINGS)
	add_subdirectory(python)
endif()

if (LEMMA_MATLAB_MEX)
	add_subdirectory(matlab)
endif()

# Install
install ( TARGETS fdem1d DESTINATION ${CMAKE_INSTALL_PREFIX}/lib )
install ( FILES include/FDEM1D  DESTINATION ${CMAKE_INSTALL_PREFIX}/include/Lemma ) 
install ( DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_PREFIX}/include/Lemma  FILES_MATCHING PATTERN "*.h")

#install ( DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_PREFIX}/include/Lemma/  FILES_MATCHING PATTERN "FDEM1D")
#install ( DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_PREFIX}/include/Lemma/FDEM1D  FILES_MATCHING PATTERN "*.h")

# Examples
if (LEMMA_BUILD_EXAMPLES)
	add_subdirectory(examples)
endif()
	
