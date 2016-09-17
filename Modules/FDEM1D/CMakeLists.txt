add_subdirectory("src")
add_library( fdem1d ${FEM1DSOURCE} )  
target_include_directories( fdem1d PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )

set_target_properties(fdem1d PROPERTIES 
	VERSION  "${LEMMA_VERSION}"
	SOVERSION "${LEMMA_VERSION_MAJOR}.${LEMMA_VERSION_MINOR}"
	PROJECT_LABEL "FDEM1D ${LABEL_SUFFIX}"
)

# Linking
target_link_libraries(fdem1d "lemmacore")

# Testing
if (LEMMA_ENABLE_TESTING)
	add_subdirectory(testing)
endif()

# Install
install ( TARGETS fdem1d DESTINATION ${CMAKE_INSTALL_PREFIX}/lib )

# Examples
if (LEMMA_BUILD_EXAMPLES)
	add_subdirectory(examples)
endif()
	