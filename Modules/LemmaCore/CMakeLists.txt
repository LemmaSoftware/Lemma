add_subdirectory("src")
add_library( lemmacore ${SOURCE} )  
target_include_directories( lemmacore PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )

set_target_properties(lemmacore PROPERTIES 
	VERSION  "${LEMMA_VERSION}"
	SOVERSION "${LEMMA_VERSION_MAJOR}.${LEMMA_VERSION_MINOR}"
	PROJECT_LABEL "LemmaCore ${LABEL_SUFFIX}"
)

# External project dependencies 
add_dependencies(lemmacore YAML-CPP)
if (LEMMA_TINYXML_SUPPORT)
	add_dependencies(lemmacore TINYXML2)
endif()
if (LEMMA_MATIO_SUPPORT)
	add_dependencies(lemmacore MATIO)
endif()

# Linking
if ( LEMMA_VTK6_SUPPORT OR LEMMA_VTK7_SUPPORT ) 
	target_link_libraries(lemmacore ${VTK_LIBRARIES})
#	target_link_libraries(lemmacore "matplot")
endif()

target_link_libraries(lemmacore "yaml-cpp")

# Testing
if (LEMMA_ENABLE_TESTING)
	add_subdirectory(testing)
endif()

# Install
install ( TARGETS lemmacore DESTINATION ${CMAKE_INSTALL_PREFIX}/lib )

# Examples
if (LEMMA_BUILD_EXAMPLES)
	add_subdirectory(examples)
endif()
