include_directories(${CMAKE_INSTALL_PREFIX}/include)

include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/include" )
add_subdirectory("src")

add_library( fdem1d ${FEM1DSOURCE} )  
target_link_libraries(fdem1d "lemmacore")
set_target_properties(fdem1d PROPERTIES 
	VERSION  "${LEMMA_VERSION}"
	SOVERSION "${LEMMA_VERSION_MAJOR}.${LEMMA_VERSION_MINOR}"
	PROJECT_LABEL "Lemma ${LABEL_SUFFIX}"
)

install ( TARGETS fdem1d DESTINATION ${CMAKE_INSTALL_PREFIX}/lib )

if (LEMMA_BUILD_EXAMPLES)
	add_subdirectory(examples)
endif()
	
if (LEMMA_ENABLE_TESTING)
	add_subdirectory(testing)
endif()

install ( TARGETS fdem1d DESTINATION ${CMAKE_INSTALL_PREFIX}/lib )
