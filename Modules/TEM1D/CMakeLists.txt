add_subdirectory("src")
add_library( tem1d ${TEM1DSOURCE} )  
target_include_directories( tem1d PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )

set_target_properties(tem1d PROPERTIES 
	VERSION  "${LEMMA_VERSION_NOQUOTES}"
	SOVERSION "${LEMMA_VERSION_MAJOR}.${LEMMA_VERSION_MINOR}"
	PROJECT_LABEL "TEM1D ${LABEL_SUFFIX}"
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)

# Linking
target_link_libraries(tem1d "lemmacore")
target_link_libraries(tem1d "fdem1d")

# Testing
if (LEMMA_ENABLE_TESTING)
	add_subdirectory(testing)
endif()

# Install
install ( TARGETS tem1d DESTINATION ${CMAKE_INSTALL_PREFIX}/lib )
install ( FILES include/TEM1D  DESTINATION ${CMAKE_INSTALL_PREFIX}/include/Lemma ) 
install ( DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_PREFIX}/include/Lemma  FILES_MATCHING PATTERN "*.h")

#install ( DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_PREFIX}/include/Lemma/  FILES_MATCHING PATTERN "FDEM1D")
#install ( DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_PREFIX}/include/Lemma/FDEM1D  FILES_MATCHING PATTERN "*.h")

# Examples
if (LEMMA_BUILD_EXAMPLES)
	add_subdirectory(examples)
endif()
