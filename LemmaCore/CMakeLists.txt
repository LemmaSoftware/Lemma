include_directories(${CMAKE_INSTALL_PREFIX}/include)

add_subdirectory("src")

add_library( lemmacore ${SOURCE} )  

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

option( BUILDEXAMPLES "Compile example Lemma applications" OFF )
if (BUILDEXAMPLES)
	add_subdirectory(examples)
endif()
