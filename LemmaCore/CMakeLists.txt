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
	VERSION ${LEMMA_VERSION_MAJOR}
	SOVERSION ${LEMMA_VERSION})

# Linking
add_dependencies(lemmacore YAML-CPP)
if (TINYXML_SUPPORT)
	add_dependencies(lemmacore TINYXML2)
endif()
if (MATIO_SUPPORT)
	add_dependencies(lemmacore MATIO)
endif()
if (VTK_SUPPORT) 
	target_link_libraries(lemmacore ${VTK_LIBRARIES})
	target_link_libraries(lemmacore "matplot")
endif()
target_link_libraries(lemmacore "yaml-cpp")

# Install
install ( TARGETS lemmacore DESTINATION ${CMAKE_INSTALL_PREFIX}/lib )

option( BUILDEXAMPLES "Compile example Lemma applications" OFF )
if (BUILDEXAMPLES)
	add_subdirectory(examples)
endif()
