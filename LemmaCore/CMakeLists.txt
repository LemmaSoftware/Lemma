include_directories(${CMAKE_INSTALL_PREFIX}/include)

add_subdirectory("src")

#option (SHARED_LEMMA_LIB off)
#
#if (SHARED_LEMMA_LIB)
#	add_library( lemmacore SHARED ${SOURCE} )  
#else()
#	add_library( lemmacore STATIC ${SOURCE} )  
#endif()
add_library( lemmacore ${SOURCE} )  

set_target_properties(lemmacore PROPERTIES 
	VERSION  "${LEMMA_VERSION}"
	SOVERSION "${LEMMA_VERSION_MAJOR}.${LEMMA_VERSION_MINOR}"
	PROJECT_LABEL "Lemma ${LABEL_SUFFIX}"
)

# Linking
add_dependencies(lemmacore YAML-CPP)
if (LEMMA_TINYXML_SUPPORT)
	add_dependencies(lemmacore TINYXML2)
endif()
if (LEMMA_MATIO_SUPPORT)
	add_dependencies(lemmacore MATIO)
endif()

if ( LEMMA_VTK6_SUPPORT OR LEMMA_VTK7_SUPPORT ) 
	target_link_libraries(lemmacore ${VTK_LIBRARIES})
	target_link_libraries(lemmacore "matplot")
endif()

target_link_libraries(lemmacore "yaml-cpp")

# Install
install ( TARGETS lemmacore DESTINATION ${CMAKE_INSTALL_PREFIX}/lib )

option( LEMMA_BUILD_EXAMPLES "Compile example Lemma applications" OFF )
if (LEMMA_BUILD_EXAMPLES)
	add_subdirectory(examples)
endif()
